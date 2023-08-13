#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

class SoundPlayer;
class World;
class Story;
class Character;


// キャラのセーブデータ
class CharacterData {
private:
	const char* m_name;
	int m_hp;
public:
	CharacterData(const char* name);

	// ゲッタ
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }

	// セッタ
	inline void setHp(int hp) { m_hp = hp; }
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

public:
	GameData();
	GameData(const char* saveFilePath);

	// ゲッタ
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }

	// セッタ
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }

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

public:
	HeartSkill(int loopNum, World* world);

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