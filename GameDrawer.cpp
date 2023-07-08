#include "GameDrawer.h"
#include "Game.h"
#include "World.h"
#include "WorldDrawer.h"


GameDrawer::GameDrawer(const Game* game) {
	m_game = game;

	m_worldDrawer = new WorldDrawer(NULL);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
}

void GameDrawer::draw() {
	// ¢ŠE‚ğ•`‰æ
	m_worldDrawer->setWorld(m_game->getWorld());
	m_worldDrawer->draw();
}