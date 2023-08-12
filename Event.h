#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>
#include <string>


class World;
class SoundPlayer;
class CharacterController;
class Character;
class Conversation;


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
	virtual EVENT_RESULT play() = 0;
};


/*
* イベント
* EventElementを管理する
*/
class Event {
private:

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
	// キャラの名前
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
};


/*
* EventElementの派生クラス
*/
// キャラのAIを変える
class ChangeBrainEvent :
	public EventElement
{
private:

	// Brainのクラス名
	std::string m_brainName;

	// 対象のキャラ
	CharacterController* m_controller_p;

public:
	ChangeBrainEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();
};

// 特定のキャラのHPが0になるまで戦う
class DeadCharacterEvent :
	public EventElement
{
private:

	// 対象のキャラ
	Character* m_character_p;

public:
	DeadCharacterEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();
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

	EVENT_RESULT play();
};

#endif