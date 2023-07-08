#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


class World;


class Game {
private:
	World* m_world;

public:
	Game();
	~Game();

	// デバッグ
	void debug(int x, int y, int color);

	// ゲームをプレイする
	void play();
};

#endif