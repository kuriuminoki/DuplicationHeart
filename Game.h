#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

class SoundPlayer;
class World;


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

public:
	GameData();
	GameData(const char* saveFilePath);

	// ゲッタ
	inline int getAreaNum() const { return m_areaNum; }

	// セッタ
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }

	// 自身のデータをWorldにデータ反映させる
	void asignWorld(World* world);

	// Worldのデータを自身に反映させる
	void asignedWorld(World* world);
};


class Game {
private:
	GameData* m_gameData;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

	// 世界
	World* m_world;

public:
	Game();
	~Game();

	// ゲッタ
	World* getWorld() const { return m_world; }

	// デバッグ
	void debug(int x, int y, int color) const;

	// ゲームをプレイする
	void play();
};

#endif