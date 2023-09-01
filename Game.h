#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <string>

class SoundPlayer;
class World;
class Story;
class Character;


// キャラのセーブデータ
class CharacterData {
private:

	// 名前
	const char* m_name;

	// HP
	int m_hp;

	// ID
	int m_id;

	// GroupID
	int m_groupId;

	// どこのエリアにいるか
	int m_areaNum;

	// 座標
	int m_x, m_y;

	// Brainのクラス名
	std::string m_brainName;

	// 攻撃対象の名前 いないなら空文字
	std::string m_targetName;

	// 追跡対象の名前 いないなら空文字
	std::string m_followName;

	// CharacterActionのクラス名
	std::string m_actionName;

	// 効果音ありか
	bool m_soundFlag;

	// CharacterControllerのクラス名
	std::string m_controllerName;

public:
	CharacterData(const char* name);

	// ゲッタ
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }
	inline int id() const { return m_id; }
	inline int groupId() const { return m_groupId; }
	inline int areaNum() const { return m_areaNum; }
	inline int x() const { return m_x; }
	inline int y() const { return m_y; }
	inline std::string brainName() const { return m_brainName; }
	inline std::string targetName() const { return m_targetName; }
	inline std::string followName() const { return m_followName; }
	inline std::string actionName() const { return m_actionName; }
	inline bool soundFlag() const { return m_soundFlag; }
	inline std::string controllerName() const { return m_controllerName; }

	// セッタ
	inline void setHp(int hp) { m_hp = hp; }
	inline void setId(int id) { m_id = id; }
	inline void setGroupId(int groupId) { m_groupId = groupId; }
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setBrainName(const char* brainName) { m_brainName = brainName; }
	inline void setTargetName(const char* targetName) { m_targetName = targetName; }
	inline void setFollowName(const char* followName) { m_followName = followName; }
	inline void setActionName(const char* actionName) { m_actionName = actionName; }
	inline void setSoundFlag(bool soundFlag) { m_soundFlag = soundFlag; }
	inline void setControllerName(const char* controllerName) { m_controllerName = controllerName; }
};


// セーブデータ
class GameData {
private:
	// セーブする場所
	const char* m_saveFilePath;

	// キャラのデータ
	std::vector<CharacterData> m_characterData;

	// 今いるエリア
	int m_areaNum;

	// 今やっているストーリー
	int m_storyNum;

	// 音量
	int m_soundVolume;

public:
	GameData();
	GameData(const char* saveFilePath);

	// ゲッタ
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }
	inline int getSoundVolume() const { return m_soundVolume; }

	// セッタ
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }
	inline void setSoundVolume(int soundVolume) { m_soundVolume = soundVolume; }

	// 自身のデータをWorldにデータ反映させる
	void asignWorld(World* world);

	// Worldのデータを自身に反映させる
	void asignedWorld(World* world);
};


// ハートのスキル
class HeartSkill {
private:
	// 複製の数
	int m_loopNum;

	// 今何ループ目か
	int m_loopNow;

	// 元の世界
	World* m_world_p;

	// 複製
	World* m_duplicationWorld;

	// 何秒間か
	const int DUPLICATION_TIME = 600;

	// DUPLICATION_TIMEまでカウントする
	int m_cnt;

	// 複製のキャラID スキル終了時に消すため
	std::vector<int> m_duplicationId;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// 効果音
	int m_sound;

public:
	HeartSkill(int loopNum, World* world, SoundPlayer* soundPlayer);
	~HeartSkill();

	// ゲッタ
	inline int getLoopNum() const { return m_loopNum; }
	inline int getLoopNow() const { return m_loopNow; }
	inline World* getWorld() const { return m_loopNow < m_loopNum ? m_duplicationWorld : m_world_p; }
	inline double getCnt() const { return ((double)DUPLICATION_TIME / 60.0) - ((double)m_cnt / 60.0); }

	// スキル進行中
	bool play();

private:
	// 世界のコピーを作る コピーの変更はオリジナルに影響しない
	World* createDuplicationWorld(const World* world);

	// 操作記録をコピーする
	void copyRecord(const World* from, World* to);

	void createDuplicationHeart();
};


class Game {
private:
	GameData* m_gameData;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

	// 世界
	World* m_world;

	// ストーリー
	Story* m_story;

	// スキル
	HeartSkill* m_skill;

public:
	Game();
	~Game();

	// ゲッタ
	World* getWorld() const { return m_world; }
	HeartSkill* getSkill() const { return m_skill; }

	// デバッグ
	void debug(int x, int y, int color) const;

	// ゲームをプレイする
	bool play();
};

#endif