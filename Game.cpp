#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "DxLib.h"

/*
* キャラのデータ
*/
CharacterData::CharacterData(const char* name) {
	m_name = name;
	m_hp = -1;
}


/*
* ゲームのセーブデータ
*/
// 初期状態のデータを作成
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	// 主要キャラを設定
	m_characterData.push_back("ハート");

}

// ファイルを指定してデータを復元
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// セーブデータを読み込んで初期状態のデータを上書き

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
* ゲーム本体
*/
Game::Game() {
	// データ
	m_gameData = new GameData();

	// サウンドプレイヤー
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(50);

	// 世界
	int startAreaNum = 0;
	m_world = new World(startAreaNum, m_soundPlayer);

	// データを世界に反映
	m_gameData->asignWorld(m_world);
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

	// エリア移動
	if (m_world->getBrightValue() == 0) {
		int nextAreaNum = m_world->getAreaNum();
		m_gameData->asignedWorld(m_world);
		delete m_world;
		m_world = new World(nextAreaNum, m_soundPlayer);
		m_gameData->asignWorld(m_world);
	}
}