#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	WorldDrawer* m_worldDrawer;

	// スキルの情報のフォント
	const int SKILL_SIZE = 100;
	int m_skillHandle;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw();
};


#endif