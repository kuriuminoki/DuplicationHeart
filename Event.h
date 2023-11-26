#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>
#include <string>


class World;
class SoundPlayer;
class CharacterController;
class Character;
class Conversation;
class Movie;


enum class EVENT_RESULT {
	NOW,
	SUCCESS,
	FAILURE
};


/*
* イベントの発火条件 Worldに変化を加えない
*/
class EventFire {
protected:

	// イベントが起きるWorld
	const World* m_world_p;

public:
	EventFire(World* world);

	virtual bool fire() = 0;

	virtual void setWorld(World* world) { m_world_p = world; }
};


/*
* イベントを構成する要素
*/
class EventElement {
protected:

	// イベントが起きているWorld
	World* m_world_p;

public:
	EventElement(World* world);
	virtual ~EventElement();

	// 初期化
	virtual void init(){}

	// プレイ
	virtual EVENT_RESULT play() = 0;

	// ハートのスキル発動が可能かどうか
	virtual bool skillAble() = 0;

	// セッタ
	virtual void setWorld(World* world) { m_world_p = world; }
};


/*
* イベント
* EventElementを管理する
*/
class Event {
private:

	// 発火後にこれを使ってElement生成
	World* m_world_p;
	SoundPlayer* m_soundPlayer;

	// イベント番号
	int m_eventNum;

	// イベントの発火条件
	std::vector<EventFire*> m_eventFire;

	// イベントの要素
	std::vector<EventElement*> m_eventElement;

	// イベントの進捗(EventElementのインデックス)
	int m_nowElement;

public:
	Event(int eventNum, World* world, SoundPlayer* soundPlayer);
	~Event();

	// 発火
	bool fire();

	// イベント進行
	EVENT_RESULT play();

	// 今ハートのスキル発動可能かどうか
	bool skillAble();

	// Worldを設定しなおす
	void setWorld(World* world);

private:
	void createFire(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
	void createElement(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
};


/*
* EventFireの派生クラス
*/
// 特定のキャラが指定した座標にいる
class CharacterPointFire :
	public	EventFire
{
private:
	// パラメータ
	std::vector<std::string> m_param;

	// キャラ
	Character* m_character_p;

	// エリア番号
	int m_areaNum;

	// 目標座標
	int m_x, m_y;

	// 座標のずれの許容
	int m_dx, m_dy;

public:
	CharacterPointFire(World* world, std::vector<std::string> param);

	bool fire();

	// 世界を設定しなおす
	void setWorld(World* world);
};

// 勝手に発火
class AutoFire :
	public EventFire
{
public:

	AutoFire(World* world_p) :
		EventFire(world_p)
	{

	}

	bool fire() { return true; }

};


/*
* EventElementの派生クラス
*/
// キャラのAIを変える
class ChangeBrainEvent :
	public EventElement
{
private:

	// パラメータ
	std::vector<std::string> m_param;

	// Brainのクラス名
	std::string m_brainName;

	// 対象のキャラ
	CharacterController* m_controller_p;

public:
	ChangeBrainEvent(World* world, std::vector<std::string> param);

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return false; }

	// 世界を設定しなおす
	void setWorld(World* world);
};

// キャラのGroupIDを変える
class ChangeGroupEvent :
	public EventElement
{
private:

	// パラメータ
	std::vector<std::string> m_param;

	int m_groupId;

	// 対象のキャラ
	Character* m_character_p;

public:
	ChangeGroupEvent(World* world, std::vector<std::string> param);

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return false; }

	// 世界を設定しなおす
	void setWorld(World* world);
};

// 特定のキャラのHPが0になるまで戦う
class DeadCharacterEvent :
	public EventElement
{
private:

	// パラメータ
	std::vector<std::string> m_param;

	// 対象のキャラ
	Character* m_character_p;

public:
	DeadCharacterEvent(World* world, std::vector<std::string> param);

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return true; }

	// 世界を設定しなおす
	void setWorld(World* world);
};

// 特定のグループが全滅するまで戦う
class DeadGroupEvent :
	public EventElement
{
private:

	// 対象のグループ
	int m_groupId;

	// エリア番号
	int m_areaNum;

public:
	DeadGroupEvent(World* world, std::vector<std::string> param);

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return true; }
};

// 会話イベント
class TalkEvent :
	public EventElement
{
private:

	Conversation* m_conversation;

public:
	TalkEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param);
	~TalkEvent();

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return false; }
};

// ムービーイベント
class MovieEvent :
	public EventElement
{
private:

	Movie* m_movie;

public:
	MovieEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param);
	~MovieEvent();

	// 初期化
	void init();

	// プレイ
	EVENT_RESULT play();

	// ハートのスキル発動が可能かどうか
	bool skillAble() { return false; }
};

#endif