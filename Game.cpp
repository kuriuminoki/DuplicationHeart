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

#include <sstream>
#include <direct.h>

using namespace std;


// どこまで
const int FINISH_STORY = 10;
// エリア0でデバッグするときはtrueにする
const bool TEST_MODE = false;
// スキルが発動可能になるストーリー番号
const int SKILL_USEABLE_STORY = 1;


/*
* キャラのデータ
*/
CharacterData::CharacterData(const char* name) {
	m_version = 1;
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
	fwrite(&m_version, sizeof(m_version), 1, intFp);
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
	fread(&m_version, sizeof(m_version), 1, intFp);
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
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_name = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_brainName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_targetName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_followName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_actionName = str;
	fgets(str, N, strFp);
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
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
	if ((find = strchr(str, '\n')) != nullptr) { *find = '\0'; }
	m_fileName = str;
}


/*
* ゲームのセーブデータ
*/
// 初期状態のデータを作成
GameData::GameData() {

	m_exist = false;

	m_soundVolume = 50;

	loadCommon(&m_soundVolume, &GAME_WIDE, &GAME_HEIGHT);

	m_saveFilePath = "";

	m_areaNum = 1;
	m_storyNum = 1;
	m_latestStoryNum = 1;

	if (TEST_MODE) {
		m_areaNum = 0;
		m_storyNum = 0;
	}

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
GameData::GameData(const char* saveFilePath) :
	GameData()
{
	// セーブ場所
	m_saveFilePath = saveFilePath;
	// セーブデータを読み込んで初期状態のデータを上書き
	m_exist = load();
}

// ファイルとチャプターを指定してデータを復元
GameData::GameData(const char* saveFilePath, int storyNum) :
	GameData()
{
	// セーブ場所
	m_saveFilePath = saveFilePath;
	// いったん最新のデータを読み込む
	load();
	
	// 古いチャプターのデータを読み込んで上書き latestStoryNumだけは変わらない
	int latestStoryNum = m_latestStoryNum;
	ostringstream oss;
	oss << m_saveFilePath << "chapter/" << storyNum << "/";
	m_saveFilePath = oss.str();
	load();
	m_saveFilePath = saveFilePath;
	m_latestStoryNum = latestStoryNum;
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

	// 今やっているチャプターが最新ならセーブ
	if (m_storyNum == m_latestStoryNum) {
		FILE* intFp = nullptr, * strFp = nullptr;

		// 全セーブデータ共通
		if (!saveCommon(m_soundVolume, GAME_WIDE, GAME_HEIGHT)) { return false; }

		// セーブデータ固有
		string fileName = m_saveFilePath;
		if (fopen_s(&intFp, (fileName + "intData.dat").c_str(), "wb") != 0 || fopen_s(&strFp, (fileName + "strData.dat").c_str(), "wb") != 0) {
			return false;
		}
		// Write
		fwrite(&m_areaNum, sizeof(m_areaNum), 1, intFp);
		fwrite(&m_storyNum, sizeof(m_storyNum), 1, intFp);
		fwrite(&m_latestStoryNum, sizeof(m_latestStoryNum), 1, intFp);
		for (unsigned int i = 0; i < m_characterData.size(); i++) {
			m_characterData[i]->save(intFp, strFp);
		}
		unsigned int doorSum = (unsigned int)m_doorData.size();
		fwrite(&doorSum, sizeof(doorSum), 1, intFp);
		for (unsigned int i = 0; i < m_doorData.size(); i++) {
			m_doorData[i]->save(intFp, strFp);
		}
		// ファイルを閉じる
		fclose(intFp);
		fclose(strFp);
	}
	return true;
}

// ロード
bool GameData::load() {
	FILE* intFp = nullptr, * strFp = nullptr, * commonFp = nullptr;

	// 全セーブデータ共通
	if (!loadCommon(&m_soundVolume, &GAME_WIDE, &GAME_HEIGHT)) { return false; }

	// セーブデータ固有
	string fileName = m_saveFilePath;
	if (fopen_s(&intFp, (fileName + "intData.dat").c_str(), "rb") != 0 || fopen_s(&strFp, (fileName + "strData.dat").c_str(), "rb") != 0) {
		return false;
	}
	// Read
	fread(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fread(&m_storyNum, sizeof(m_storyNum), 1, intFp);
	fread(&m_latestStoryNum, sizeof(m_latestStoryNum), 1, intFp);
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		m_characterData[i]->load(intFp, strFp);
	}
	int doorSum = 0;
	m_doorData.clear();
	fread(&doorSum, sizeof(doorSum), 1, intFp);
	for (int i = 0; i < doorSum; i++) {
		m_doorData.push_back(new DoorData(intFp, strFp));
	}
	// ファイルを閉じる
	fclose(intFp);
	fclose(strFp);
	return true;
}

// バックアップを取る（チャプター巻き戻し機能用）
bool GameData::saveChapter() {

	// savedata/<セーブデータ番号>/chapter/<storyNum>/
	ostringstream oss;
	oss << m_saveFilePath << "chapter/" << m_storyNum;
	_mkdir(oss.str().c_str());
	string filePath = m_saveFilePath;
	m_saveFilePath = oss.str() + "/";
	save();
	m_saveFilePath = filePath;

	return true;
}

// 全セーブデータ共通
bool GameData::saveCommon(int soundVolume, int gameWide, int gameHeight) {

	FILE* commonFp = nullptr;
	if (fopen_s(&commonFp, "savedata/commonData.dat", "wb") != 0) {
		return false;
	}
	fwrite(&soundVolume, sizeof(soundVolume), 1, commonFp);
	fwrite(&gameWide, sizeof(gameWide), 1, commonFp);
	fwrite(&gameHeight, sizeof(gameHeight), 1, commonFp);
	fclose(commonFp);
	return true;
}
// 全セーブデータ共通の項目だけをロード
bool GameData::loadCommon(int* soundVolume, int* gameWide, int* gameHeight) {

	FILE* commonFp = nullptr;
	if (fopen_s(&commonFp, "savedata/commonData.dat", "rb") != 0) {
		return false;
	}
	fread(soundVolume, sizeof(*soundVolume), 1, commonFp);
	fread(gameWide, sizeof(*gameWide), 1, commonFp);
	fread(gameHeight, sizeof(*gameHeight), 1, commonFp);
	fclose(commonFp);
	return true;
}

// セーブデータ削除
void GameData::removeSaveData() {
	string fileName = m_saveFilePath;
	remove((fileName + "intData.dat").c_str());
	remove((fileName + "strData.dat").c_str());
}

// 自身のデータをWorldにデータ反映させる
void GameData::asignWorld(World* world, bool playerHpReset) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedCharacterData(m_characterData[i]->name(), m_characterData[i]);
	}
	size = m_doorData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedDoorData(m_doorData[i]);
	}
	if (playerHpReset) {
		world->playerHpReset();
	}
}

// Worldのデータを自身に反映させる
void GameData::asignedWorld(const World* world, bool notCharacterPoint) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i]->name(), m_characterData[i], m_areaNum, notCharacterPoint);
	}
	world->asignDoorData(m_doorData, m_areaNum);
}

// ストーリーが進んだ時にセーブデータを更新する エリア外（World以外）も考慮する
void GameData::updateStory(Story* story) {
	m_areaNum = story->getWorld()->getAreaNum();
	m_storyNum = story->getStoryNum();
	m_latestStoryNum = max(m_latestStoryNum, m_storyNum);
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
Game::Game(const char* saveFilePath, int storyNum) {
	// データ
	if (storyNum == -1) { // チャプター指定なし、最新のチャプター
		m_gameData = new GameData(saveFilePath);
	}
	else { // チャプター指定あり
		m_gameData = new GameData(saveFilePath, storyNum);
	}

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
	m_gameData->asignWorld(m_world, true);

	// スキル
	m_skill = nullptr;

	// 一時停止画面
	m_battleOption = nullptr;

	// 一時停止音
	m_pauseSound = LoadSoundMem("sound/system/pause.wav");

	// ゲームの再起動（タイトルへ戻る）を要求
	m_rebootFlag = false;

	// 初期データをセーブ
	if (!m_gameData->getExist()) {
		m_gameData->save();
	}
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
	DeleteSoundMem(m_pauseSound);
}

bool Game::play() {

	// 一時停止
	if (controlQ() == 1) {
		if (m_battleOption == nullptr) {
			m_battleOption = new BattleOption(m_soundPlayer);
			// ここで音楽も止める
			m_soundPlayer->stopBGM();
		}
		else {
			delete m_battleOption;
			m_battleOption = nullptr;
			m_soundPlayer->playBGM();
		}
		m_soundPlayer->pushSoundQueue(m_pauseSound);
	}
	if (m_battleOption != nullptr) {
		m_battleOption->play();
		if (m_battleOption->getTitleFlag()) {
			// タイトルへ戻る
			m_rebootFlag = true;
		}
		// 音
		m_soundPlayer->play();
		return false;
	}

	// これ以上ストーリーを進ませない（テスト用）
	if (m_gameData->getStoryNum() == FINISH_STORY || m_gameData->getStoryNum() == 0) {
		m_world->battle();
		m_soundPlayer->play();
		return false;
	}

	// スキル発動 Fキーかつスキル未発動状態かつ発動可能なイベント中（もしくはイベント中でない）かつエリア移動中でない
	if (m_gameData->getStoryNum() >= SKILL_USEABLE_STORY) { // ストーリーの最初は発動できない
		if (controlF() == 1 && m_skill == nullptr) { // Fキーで発動、ただしスキル身発動時
			if (m_story->skillAble() && m_world->getBrightValue() == 255) { // 特定のイベント時やエリア移動中はダメ
				if (m_world->getCharacterWithName("ハート")->getHp() > 0) {
					m_world->setSkillFlag(true);
					m_skill = new HeartSkill(3, m_world, m_soundPlayer);
				}
			}
		}
	}
	
	// スキル発動中
	if (m_skill != nullptr) {
		if (m_skill->play()) {
			// スキル終了
			delete m_skill;
			m_skill = nullptr;
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
		m_gameData->asignedWorld(m_world, false);
		// Worldに反映
		m_world->addCharacter(m_story->getCharacterLoader());
		m_world->addObject(m_story->getObjectLoader());
		// セーブ
		m_gameData->save();
		// チャプターのバックアップ
		m_gameData->saveChapter();
	}

	// 音
	m_soundPlayer->play();

	// 前のセーブポイントへ戻ることが要求された
	if (m_story->getBackPrevSaveFlag()) {
		backPrevSave();
		m_story->doneBackPrevSave();
		return true;
	}
	// ゲームオーバー
	else if (m_world->playerDead() && m_world->getBrightValue() == 0) {
		// storyからハートがやられたことを伝えられたらタイトルへ戻る
		// やられるのがイベントの成功条件なら前のif文(m_story->getBackPrevSaveFlag())にひっかかるはず
		m_rebootFlag = true;
	}

	// エリア移動
	if (m_world->getBrightValue() == 0) {
		int fromAreaNum = m_world->getAreaNum();
		int toAreaNum = m_world->getNextAreaNum();
		m_gameData->asignedWorld(m_world, false);
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

// セーブデータをロード（前のセーブポイントへ戻る）
void Game::backPrevSave() {
	m_gameData->asignedWorld(m_world, true);
	// これまでのWorldを削除
	delete m_world;
	// 前のセーブデータをロード
	GameData prevData(m_gameData->getSaveFilePath(), m_gameData->getStoryNum() - 1);
	// 以前のAreaNumでロード
	m_world = new World(-1, prevData.getAreaNum(), m_soundPlayer);
	m_gameData->asignWorld(m_world, true);
	m_world->setPlayerOnDoor(m_gameData->getAreaNum());
	m_story->setWorld(m_world);
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
				m_world_p->popCharacterController(m_duplicationId[i]);
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