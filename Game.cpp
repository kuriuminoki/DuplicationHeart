#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "Story.h"
#include "Control.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "PausePage.h"
#include "DxLib.h"

using namespace std;

/*
* キャラのデータ
*/
CharacterData::CharacterData(const char* name) {
	m_initFlag = false;
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

// セーブ
void CharacterData::save(FILE* intFp, FILE* strFp) {
	fwrite(&m_initFlag, sizeof(m_initFlag), 1, intFp);
	fwrite(&m_hp, sizeof(m_hp), 1, intFp);
	fwrite(&m_id, sizeof(m_id), 1, intFp);
	fwrite(&m_groupId, sizeof(m_groupId), 1, intFp);
	fwrite(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fwrite(&m_x, sizeof(m_x), 1, intFp);
	fwrite(&m_y, sizeof(m_y), 1, intFp);
	fwrite(&m_soundFlag, sizeof(m_soundFlag), 1, intFp);

	fprintf(strFp, "%s\n", m_name.c_str());
	fprintf(strFp, "%s\n", m_brainName.c_str());
	fprintf(strFp, "%s\n", m_targetName.c_str());
	fprintf(strFp, "%s\n", m_followName.c_str());
	fprintf(strFp, "%s\n", m_actionName.c_str());
	fprintf(strFp, "%s\n", m_controllerName.c_str());
}

// ロード
void CharacterData::load(FILE* intFp, FILE* strFp) {
	fread(&m_initFlag, sizeof(m_initFlag), 1, intFp);
	fread(&m_hp, sizeof(m_hp), 1, intFp);
	fread(&m_id, sizeof(m_id), 1, intFp);
	fread(&m_groupId, sizeof(m_groupId), 1, intFp);
	fread(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fread(&m_x, sizeof(m_x), 1, intFp);
	fread(&m_y, sizeof(m_y), 1, intFp);
	fread(&m_soundFlag, sizeof(m_soundFlag), 1, intFp);

	const int N = 256;
	char* find;
	char str[N];
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_name = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_brainName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_targetName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_followName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_actionName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_controllerName = str;
}


/*
* ドアのデータ
*/
DoorData::DoorData(FILE* intFp, FILE* strFp) {
	load(intFp, strFp);
}
DoorData::DoorData(int x1, int y1, int x2, int y2, int from, int to, const char* fileName) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	m_from = from;
	m_to = to;
	m_fileName = fileName;
}

// セーブ
void DoorData::save(FILE* intFp, FILE* strFp) {
	fwrite(&m_x1, sizeof(m_x1), 1, intFp);
	fwrite(&m_y1, sizeof(m_y1), 1, intFp);
	fwrite(&m_x2, sizeof(m_x2), 1, intFp);
	fwrite(&m_y2, sizeof(m_y2), 1, intFp);
	fwrite(&m_from, sizeof(m_from), 1, intFp);
	fwrite(&m_to, sizeof(m_to), 1, intFp);

	fprintf(strFp, "%s\n", m_fileName.c_str());
}

// ロード
void DoorData::load(FILE* intFp, FILE* strFp) {
	fread(&m_x1, sizeof(m_x1), 1, intFp);
	fread(&m_y1, sizeof(m_y1), 1, intFp);
	fread(&m_x2, sizeof(m_x2), 1, intFp);
	fread(&m_y2, sizeof(m_y2), 1, intFp);
	fread(&m_from, sizeof(m_from), 1, intFp);
	fread(&m_to, sizeof(m_to), 1, intFp);

	const int N = 256;
	char* find;
	char str[N];
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != NULL) { *find = '\0'; }
	m_fileName = str;
}


/*
* ゲームのセーブデータ
*/
// 初期状態のデータを作成
GameData::GameData() {
	m_saveFilePath = "";

	const bool test = false;

	m_areaNum = 1;
	m_storyNum = 1;

	if (test) {
		m_areaNum = 0;
		m_storyNum = 0;
	}

	m_soundVolume = 50;

	// 主要キャラを設定
	const int mainSum = 6;
	const char* mainCharacters[mainSum] = {
		"ハート",
		"シエスタ",
		"ヒエラルキー",
		"ヴァルキリア",
		"トロイ",
		"エム・サディ"
	};
	for (int i = 0; i < mainSum; i++) {
		m_characterData.push_back(new CharacterData(mainCharacters[i]));
	}
}

// ファイルを指定してデータを復元
GameData::GameData(const char* saveFilePath):
	GameData()
{
	// セーブ場所
	m_saveFilePath = saveFilePath;
	// セーブデータを読み込んで初期状態のデータを上書き
	load();
}

GameData::~GameData() {
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		delete m_characterData[i];
	}
	for (unsigned int i = 0; i < m_doorData.size(); i++) {
		delete m_doorData[i];
	}
}

// セーブ
bool GameData::save() {
	FILE *intFp = nullptr, *strFp = nullptr;
	string fileName = m_saveFilePath;
	if (fopen_s(&intFp, (fileName + "intData.txt").c_str(), "w") != 0 || fopen_s(&strFp, (fileName + "strData.txt").c_str(), "w") != 0) {
		return false;
	}
	// Write
	fwrite(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fwrite(&m_storyNum, sizeof(m_storyNum), 1, intFp);
	fwrite(&m_soundVolume, sizeof(m_soundVolume), 1, intFp);
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		m_characterData[i]->save(intFp, strFp);
	}
	unsigned int doorSum = m_doorData.size();
	fwrite(&doorSum, sizeof(doorSum), 1, intFp);
	for (unsigned int i = 0; i < m_doorData.size(); i++) {
		m_doorData[i]->save(intFp, strFp);
	}
	// ファイルを閉じる
	fclose(intFp); 
	fclose(strFp);
	return true;
}

// ロード
bool GameData::load() {
	FILE* intFp = nullptr, * strFp = nullptr;
	string fileName = m_saveFilePath;
	if (fopen_s(&intFp, (fileName + "intData.txt").c_str(), "r") != 0 || fopen_s(&strFp, (fileName + "strData.txt").c_str(), "r") != 0) {
		return false;
	}
	// Read
	fread(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fread(&m_storyNum, sizeof(m_storyNum), 1, intFp);
	fread(&m_soundVolume, sizeof(m_soundVolume), 1, intFp);
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		m_characterData[i]->load(intFp, strFp);
	}
	int doorSum = 0;
	fread(&doorSum, sizeof(doorSum), 1, intFp);
	for (unsigned int i = 0; i < doorSum; i++) {
		m_doorData.push_back(new DoorData(intFp, strFp));
	}
	// ファイルを閉じる
	fclose(intFp);
	fclose(strFp);
	return true;
}

// 自身のデータをWorldにデータ反映させる
void GameData::asignWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedCharacterData(m_characterData[i]->name(), m_characterData[i]);
	}
	size = m_doorData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedDoorData(m_doorData[i]);
	}
}

// Worldのデータを自身に反映させる
void GameData::asignedWorld(const World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i]->name(), m_characterData[i], m_areaNum);
	}
	world->asignDoorData(m_doorData, m_areaNum);
}

// ストーリーが進んだ時にセーブデータを更新する エリア外（World以外）も考慮する
void GameData::updateStory(Story* story) {
	m_areaNum = story->getWorld()->getAreaNum();
	m_storyNum = story->getStoryNum();
	m_soundVolume = story->getWorld()->getSoundPlayer()->getVolume();
	// Storyによって変更・新登場されたキャラ情報を取得
	CharacterLoader* characterLoader = story->getCharacterLoader();
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		characterLoader->saveCharacterData(m_characterData[i]);
	}
	// ドアの情報も取得
	ObjectLoader* objectLoader = story->getObjectLoader();
	objectLoader->saveDoorData(m_doorData);
}


/*
* ゲーム本体
*/
Game::Game() {
	// データ
	//m_gameData = new GameData();
	m_gameData = new GameData("savedata/test/");

	// サウンドプレイヤー
	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	// 世界
	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);

	// ストーリー
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);

	// セーブデータに上書き
	m_gameData->updateStory(m_story);

	// データを世界に反映
	m_gameData->asignWorld(m_world);

	// スキル
	m_skill = NULL;

	// 一時停止画面
	m_gamePause = NULL;
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// 一時停止
	if (controlQ() == 1) {
		if (m_gamePause == NULL) {
			m_gamePause = new GamePause(m_soundPlayer);
			// ここで音楽も止める
			m_soundPlayer->stopBGM();
		}
		else {
			delete m_gamePause;
			m_gamePause = NULL;
			m_soundPlayer->playBGM();
		}
	}
	if (m_gamePause != NULL) {
		m_gamePause->play();
		return false;
	}

	// これ以上ストーリーを進ませない（テスト用）
	if (m_gameData->getStoryNum() == 5 || m_gameData->getStoryNum() == 0) {
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
		int nextStoryNum = m_gameData->getStoryNum() + 1;
		delete m_story;
		m_story = new Story(nextStoryNum, m_world, m_soundPlayer);
		// ストーリーの影響でオブジェクトが追加される（一度追加されると今後GameDataでデータは保持され続ける）
		// セーブデータに上書き
		m_gameData->updateStory(m_story);
		m_gameData->asignedWorld(m_world);
		// Worldに反映
		m_world->addCharacter(m_story->getCharacterLoader());
		m_world->addObject(m_story->getObjectLoader());
		// セーブ
		m_gameData->save();
	}

	// 音
	m_soundPlayer->play();

	// エリア移動
	if (m_world->getBrightValue() == 0) {
		int fromAreaNum = m_gameData->getAreaNum();
		int toAreaNum = m_world->getAreaNum();
		m_gameData->asignedWorld(m_world);
		delete m_world;
		InitGraph();
		m_world = new World(fromAreaNum, toAreaNum, m_soundPlayer);
		m_gameData->asignWorld(m_world);
		m_world->setPlayerOnDoor(fromAreaNum);
		m_story->setWorld(m_world);
		m_gameData->setAreaNum(toAreaNum);
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