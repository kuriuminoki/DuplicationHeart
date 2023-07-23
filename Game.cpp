#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "DxLib.h"


Game::Game() {
	// サウンドプレイヤー
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(50);

	// 世界
	int startAreaNum = 0;
	m_world = new World(startAreaNum, m_soundPlayer);
}

Game::~Game() {
	delete m_soundPlayer;
	delete m_world;
}

void Game::play() {
	// 戦わせる
	m_world->battle();

	// 音
	m_soundPlayer->play();
}