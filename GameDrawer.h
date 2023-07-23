#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	WorldDrawer* m_worldDrawer;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw();
};


#endif