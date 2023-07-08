#include "Game.h"
#include "World.h"
#include "Define.h"
#include "DxLib.h"


Game::Game() {
	int startAreaNum = 0;
	m_world = new World(startAreaNum);
}

Game::~Game() {
	delete m_world;
}

void Game::play() {
	m_world->battle();
}