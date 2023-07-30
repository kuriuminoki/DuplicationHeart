#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "DxLib.h"

/*
* �L�����̃f�[�^
*/
CharacterData::CharacterData(const char* name) {
	m_name = name;
	m_hp = -1;
}


/*
* �Q�[���̃Z�[�u�f�[�^
*/
// ������Ԃ̃f�[�^���쐬
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	// ��v�L������ݒ�
	m_characterData.push_back("�n�[�g");

}

// �t�@�C�����w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// �Z�[�u�f�[�^��ǂݍ���ŏ�����Ԃ̃f�[�^���㏑��

}

void GameData::asignWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		int hp = m_characterData[i].hp();
		if (hp == -1) { continue; }
		world->asignedCharacterData(m_characterData[i].name(), hp);
	}
}

void GameData::asignedWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		int hp = 0;
		world->asignCharacterData(m_characterData[i].name(), hp);
		m_characterData[i].setHp(hp);
	}
}


/*
* �Q�[���{��
*/
Game::Game() {
	// �f�[�^
	m_gameData = new GameData();

	// �T�E���h�v���C���[
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(50);

	// ���E
	int startAreaNum = 0;
	m_world = new World(startAreaNum, m_soundPlayer);

	// �f�[�^�𐢊E�ɔ��f
	m_gameData->asignWorld(m_world);
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

	// �G���A�ړ�
	if (m_world->getBrightValue() == 0) {
		int nextAreaNum = m_world->getAreaNum();
		m_gameData->asignedWorld(m_world);
		delete m_world;
		m_world = new World(nextAreaNum, m_soundPlayer);
		m_gameData->asignWorld(m_world);
	}
}