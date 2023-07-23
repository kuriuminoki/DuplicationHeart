#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


class SoundPlayer;
class World;


class Game {
private:
	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

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