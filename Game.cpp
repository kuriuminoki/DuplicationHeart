#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "Story.h"
#include "Control.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
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

	m_areaNum = 0;
	m_storyNum = 0;

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
	m_world = new World(-1, startAreaNum, m_soundPlayer);

	// データを世界に反映
	m_gameData->asignWorld(m_world);

	// ストーリー
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);

	// スキル
	m_skill = NULL;
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// これ以上ストーリーを進ませない（テスト用）
	if (m_gameData->getStoryNum() == 1) {
		return false;
	}

	// スキル発動 Fキーかつスキル未発動状態かつ発動可能なイベント中（もしくはイベント中でない）かつエリア移動中でない
	if (controlF() == 1 && m_skill == NULL && m_story->skillAble() && m_world->getBrightValue() == 255) {
		m_world->setSkillFlag(true);
		m_skill = new HeartSkill(3, m_world);
	}
	
	// スキル発動中
	if (m_skill != NULL) {
		if (m_skill->play()) {
			// スキル終了
			delete m_skill;
			m_skill = NULL;
			m_world->setSkillFlag(false);
		}
	}
	// ストーリー進行
	else if (m_story->play()) {
		// 次のストーリーへ
		m_gameData->setStoryNum(m_gameData->getStoryNum() + 1);
		delete m_story;
		m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
	}

	// 音
	m_soundPlayer->play();

	// エリア移動
	if (m_world->getBrightValue() == 0) {
		int nextAreaNum = m_world->getAreaNum();
		m_gameData->asignedWorld(m_world);
		delete m_world;
		InitGraph();
		m_world = new World(m_gameData->getAreaNum(), nextAreaNum, m_soundPlayer);
		m_gameData->asignWorld(m_world);
		m_gameData->setAreaNum(nextAreaNum);
		return true;
	}
	return false;
}


/*
* ハートのスキル
*/
HeartSkill::HeartSkill(int loopNum, World* world) {
	m_loopNum = loopNum;
	m_loopNow = 0;
	m_world_p = world;
	m_duplicationWorld = createDuplicationWorld(m_world_p);
	m_cnt = 0;
}

bool HeartSkill::play() {
	m_cnt++;
	if (m_cnt == DUPLICATION_TIME) {
		// 次のループへ
		m_cnt = 0;
		m_loopNow++;

		if (m_loopNow < m_loopNum) {
			// duplicationWorldを新たに作り、worldと以前のduplicationWorldの操作記録をコピーする
			Character* heart = new Heart("ハート", 100, 500 + GetRand(500), 0, 0);
			m_duplicationId.push_back(heart->getId());
			CharacterAction* action = new StickAction(heart, m_world_p->getSoundPlayer());
			Brain* brain = new KeyboardBrain(m_duplicationWorld->getCamera());
			m_world_p->pushCharacter(heart, new NormalController(brain, action));
			World* nextWorld = createDuplicationWorld(m_world_p);
			copyRecord(m_duplicationWorld, nextWorld);
			delete m_duplicationWorld;
			m_duplicationWorld = nextWorld;
		}
		else if (m_loopNow == m_loopNum) {
			// 最後のループはもとのWorldに操作記録をコピーして、そのWorldでbattle
			copyRecord(m_duplicationWorld, m_world_p);
			delete m_duplicationWorld;
		}
		else {
			// スキル終了
			for (unsigned int i = 0; i < m_duplicationId.size(); i++) {
				m_world_p->popCharacter(m_duplicationId[i]);
			}
			return true;
		}
	}

	// 戦わせる（最後のループ以外なら、操作記録をするという言い方が正しい）
	if (m_loopNow < m_loopNum) {
		m_duplicationWorld->battle();
	}
	else {
		m_world_p->battle();
	}

	return false;
}

// 世界のコピーを作る コピーの変更はオリジナルに影響しない
World* HeartSkill::createDuplicationWorld(const World* world) {
	World* res = new World(world);
	res->setSkillFlag(true);
	return res;
}

// 操作記録をコピーする
void HeartSkill::copyRecord(const World* from, World* to) {

}