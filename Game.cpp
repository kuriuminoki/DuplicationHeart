#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "Story.h"
#include "Control.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "DxLib.h"

/*
* キャラのデータ
*/
CharacterData::CharacterData(const char* name) {
	m_name = name;
	m_hp = -1;
	// id=-1はデータなしを意味する
	m_id = -1;
	m_groupId = -1;
	m_areaNum = -1;
	m_x, m_y = -1;
	m_brainName = "";
	m_targetName = "";
	m_followName = "";
	m_actionName = "";
	m_soundFlag = false;
	m_controllerName = "";
}


/*
* ゲームのセーブデータ
*/
// 初期状態のデータを作成
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	m_areaNum = 1;
	m_storyNum = 1;
	m_soundVolume = 10;

	// 主要キャラを設定
	m_characterData.push_back("ハート");
	m_characterData.push_back("シエスタ");

}

// ファイルを指定してデータを復元
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// セーブデータを読み込んで初期状態のデータを上書き

}

// 自身のデータをWorldにデータ反映させる
void GameData::asignWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedCharacterData(m_characterData[i].name(), m_characterData[i]);
	}
}

// Worldのデータを自身に反映させる
void GameData::asignedWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i].name(), m_characterData[i], m_areaNum);
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
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	// 世界
	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);

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
	if (m_gameData->getStoryNum() == 5) {
		m_world->battle();
		m_soundPlayer->play();
		return false;
	}

	// スキル発動 Fキーかつスキル未発動状態かつ発動可能なイベント中（もしくはイベント中でない）かつエリア移動中でない
	if (m_gameData->getStoryNum() >= 4) { // ストーリーの最初は発動できない
		if (controlF() == 1 && m_skill == NULL) { // Fキーで発動、ただしスキル身発動時
			if (m_story->skillAble() && m_world->getBrightValue() == 255) { // 特定のイベント時やエリア移動中はダメ
				if (m_world->getCharacterWithName("ハート")->getHp() > 0) {
					m_world->setSkillFlag(true);
					m_skill = new HeartSkill(3, m_world, m_soundPlayer);
				}
			}
		}
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
		m_world->addObject(m_story->getObjectLoader());
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
		m_world->addObject(m_story->getObjectLoader());
		m_story->setWorld(m_world);
		return true;
	}
	return false;
}


/*
* ハートのスキル
*/
HeartSkill::HeartSkill(int loopNum, World* world, SoundPlayer* soundPlayer) {
	m_loopNum = loopNum;
	m_loopNow = 0;
	m_world_p = world;
	m_cnt = 0;

	// オリジナルのハートを動けなくさせ、無敵
	Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
	original->setGroupId(-1);
	m_world_p->setBrainWithId(m_world_p->getPlayerId(), new Freeze());
	m_world_p->createRecorder();

	// 最初の複製
	m_duplicationWorld = createDuplicationWorld(m_world_p);

	// 効果音
	m_soundPlayer_p = soundPlayer;
	m_sound = LoadSoundMem("sound/battle/skill.wav");
	m_soundPlayer_p->pushSoundQueue(m_sound);
}

HeartSkill::~HeartSkill() {
	DeleteSoundMem(m_sound);
}

bool HeartSkill::play() {
	m_cnt++;
	if (m_cnt == DUPLICATION_TIME) {
		// 次のループへ
		m_cnt = 0;
		m_loopNow++;
		m_world_p->initRecorder();
		m_soundPlayer_p->pushSoundQueue(m_sound);

		if (m_loopNow < m_loopNum) {
			// duplicationWorldを新たに作り、worldと以前のduplicationWorldの操作記録をコピーする
			World* nextWorld = createDuplicationWorld(m_world_p);
			copyRecord(m_duplicationWorld, nextWorld);
			delete m_duplicationWorld;
			m_duplicationWorld = nextWorld;
		}
		else if (m_loopNow == m_loopNum) {
			// オリジナルのハートを元に戻す
			Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
			original->setGroupId(0);
			m_world_p->setBrainWithId(m_world_p->getPlayerId(), new KeyboardBrain(m_world_p->getCamera()));
			m_world_p->setFocusId(m_world_p->getPlayerId());
			// 最後のループはもとのWorldに操作記録をコピーして、そのWorldでbattle
			copyRecord(m_duplicationWorld, m_world_p);
			delete m_duplicationWorld;
		}
		else {
			// スキル終了
			for (unsigned int i = 0; i < m_duplicationId.size(); i++) {
				m_world_p->popCharacter(m_duplicationId[i]);
				m_world_p->eraseRecorder();
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
	createDuplicationHeart();
	World* res = new World(world);
	res->setSkillFlag(true);
	return res;
}

// 操作記録をコピーする
void HeartSkill::copyRecord(const World* from, World* to) {

}

// m_world_pに複製をpush
void HeartSkill::createDuplicationHeart() {
	// ハートの複製
	Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
	Character* duplicationHeart = new Heart("複製のハート", original->getHp(), original->getX(), original->getY(), 0, original->getAttackInfo());
	// Character* duplicationHeart = original->createCopy();
	duplicationHeart->setX(duplicationHeart->getX() + GetRand(200));
	duplicationHeart->setHp(original->getHp());
	duplicationHeart->setLeftDirection(original->getLeftDirection());

	// push
	m_duplicationId.push_back(duplicationHeart->getId());
	CharacterAction* action = new StickAction(duplicationHeart, m_world_p->getSoundPlayer());
	Brain* brain = new KeyboardBrain(m_world_p->getCamera());
	NormalController* controller = new NormalController(brain, action);
	controller->setStickRecorder(new ControllerRecorder(0));
	controller->setJumpRecorder(new ControllerRecorder(0));
	controller->setSquatRecorder(new ControllerRecorder(0));
	controller->setSlashRecorder(new ControllerRecorder(0));
	controller->setBulletRecorder(new ControllerRecorder(0));
	//controller->setDamageRecorder(new ControllerRecorder(0));
	m_world_p->pushCharacter(duplicationHeart, controller);
	m_world_p->setFocusId(duplicationHeart->getId());
}