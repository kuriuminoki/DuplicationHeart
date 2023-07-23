#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "DxLib.h"


Game::Game() {
	// �T�E���h�v���C���[
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(50);

	// ���E
	int startAreaNum = 0;
	m_world = new World(startAreaNum, m_soundPlayer);
}

Game::~Game() {
	delete m_soundPlayer;
	delete m_world;
}

void Game::play() {
	// ��킹��
	m_world->battle();

	// ��
	m_soundPlayer->play();
}