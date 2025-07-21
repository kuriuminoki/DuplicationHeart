#include "Game.h"
#include "World.h"
#include "Define.h"
#include "Sound.h"
#include "Story.h"
#include "Timer.h"
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


// �G���A0�Ńf�o�b�O����Ƃ���true�ɂ���
const bool TEST_MODE = false;


/*
* �L�����̃f�[�^
*/
CharacterData::CharacterData(const char* name) {
	m_version = 1;
	m_name = name;
	m_hp = -1;
	m_skillGage = 0;
	m_invincible = false;
	// id=-1�̓f�[�^�Ȃ����Ӗ�����
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


void CharacterData::save(FILE* intFp, FILE* strFp) {
	fwrite(&m_version, sizeof(m_version), 1, intFp);
	fwrite(&m_hp, sizeof(m_hp), 1, intFp);
	fwrite(&m_skillGage, sizeof(m_skillGage), 1, intFp);
	fwrite(&m_invincible, sizeof(m_invincible), 1, intFp);
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


void CharacterData::load(FILE* intFp, FILE* strFp) {
	fread(&m_version, sizeof(m_version), 1, intFp);
	fread(&m_hp, sizeof(m_hp), 1, intFp);
	fread(&m_skillGage, sizeof(m_skillGage), 1, intFp);
	fread(&m_invincible, sizeof(m_invincible), 1, intFp);
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
* �h�A�̃f�[�^
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


void DoorData::save(FILE* intFp, FILE* strFp) {
	fwrite(&m_x1, sizeof(m_x1), 1, intFp);
	fwrite(&m_y1, sizeof(m_y1), 1, intFp);
	fwrite(&m_x2, sizeof(m_x2), 1, intFp);
	fwrite(&m_y2, sizeof(m_y2), 1, intFp);
	fwrite(&m_from, sizeof(m_from), 1, intFp);
	fwrite(&m_to, sizeof(m_to), 1, intFp);

	fprintf(strFp, "%s\n", m_fileName.c_str());
}


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
* �N���A�����C�x���g�̃��X�g
*/
EventData::EventData() {

}
EventData::EventData(FILE* eventFp) {
	load(eventFp);
}


void EventData::save(FILE* eventFp) {
	if (m_clearEvent.size() == 0) {
		// ��̃t�@�C�����ƃ��[�h���ɂ��܂������Ȃ�����0�����ċ�ɂȂ�Ȃ��悤�ɂ���B
		m_clearEvent.push_back(0);
		m_clearLoop.push_back(0);
	}
	for (unsigned int i = 0; i < m_clearEvent.size(); i++) {
		fwrite(&m_clearEvent[i], sizeof(int), 1, eventFp);
		fwrite(&m_clearLoop[i], sizeof(int), 1, eventFp);
	}
}


void EventData::load(FILE* eventFp) {
	while (feof(eventFp) == 0) {
		int num;
		fread(&num, sizeof(int), 1, eventFp);
		int loop;
		fread(&loop, sizeof(int), 1, eventFp);
		setClearEvent(num, loop);
	}
}


// ����̃C�x���g���N���A���Ă邩
bool EventData::checkClearEvent(int eventNum, int loop) {
	for (unsigned int i = 0; i < m_clearEvent.size(); i++) {
		if (m_clearEvent[i] == eventNum && m_clearLoop[i] <= loop) {
			return true;
		}
	}
	return false;
}


//����̃C�x���g���N���A��Ԃɂ���
void EventData::setClearEvent(int eventNum, int loop) {
	if (!checkClearEvent(eventNum)) {
		m_clearEvent.push_back(eventNum);
		m_clearLoop.push_back(loop);
	}
}


/*
* �Q�[���̃Z�[�u�f�[�^
*/
// ������Ԃ̃f�[�^���쐬
GameData::GameData() {

	m_noticeSaveDone = 0;

	m_exist = false;

	m_soundVolume = 50;

	m_money = 0;

	loadCommon(&m_soundVolume, &GAME_WIDE, &GAME_HEIGHT);

	m_saveFilePath = "";

	m_areaNum = 1;
	m_time = 0;
	m_loop = 1;
	m_latestLoop = 0;

	if (TEST_MODE) {
		m_areaNum = 0;
		m_time = 0;
		m_loop = 1;
	}

	// ��v�L������ݒ�
	const int mainSum = 15;
	const char* mainCharacters[mainSum] = {
		"�n�[�g",
		"�V�G�X�^",
		"�q�G�����L�[",
		"���@���L���A",
		"�g���C",
		"�������[",
		"���[��",
		"�G���E�T�f�B",
		"�t�����`",
		"�A�[�J�C�u",
		"�A�C�M�X",
		"�R�n��",
		"�}�X�J�[��",
		"���F�������A",
		"�T��"
	};
	for (int i = 0; i < mainSum; i++) {
		m_characterData.push_back(new CharacterData(mainCharacters[i]));
	}

	m_eventData = new EventData();

}


// �t�@�C�����w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath) :
	GameData()
{
	m_saveFilePath = saveFilePath;
	m_exist = load();
}


// �t�@�C���ƃ��[�v���w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath, int loop) :
	GameData()
{
	m_saveFilePath = saveFilePath;
	// ��������ŐV�̃f�[�^��ǂݍ���
	m_exist = load();

	// �Â����[�v�̃f�[�^��ǂݍ���ŏ㏑��
	loadLoop(loop);
}


GameData::~GameData() {
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		delete m_characterData[i];
	}
	for (unsigned int i = 0; i < m_doorData.size(); i++) {
		delete m_doorData[i];
	}
	delete m_eventData;
}


CharacterData* GameData::getCharacterData(string characterName) {
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		if (m_characterData[i]->name()) {
			return m_characterData[i];
		}
	}
	return nullptr;
}


// �Z�[�u force��false�Ȃ�ŐV�̃��[�v�ȊO�̃Z�[�u�����ۂ���
bool GameData::save(bool force) {

	// ������Ă��郋�[�v���ŐV�Ȃ�Z�[�u
	if (m_loop == m_latestLoop || force) {
		FILE* intFp = nullptr, * strFp = nullptr, * eventFp = nullptr;

		if (!saveCommon(m_soundVolume, GAME_WIDE, GAME_HEIGHT)) { return false; }

		string fileName = m_saveFilePath;
		if (fopen_s(&intFp, (fileName + INT_DATA_PATH).c_str(), "wb") != 0 ||
			fopen_s(&strFp, (fileName + STR_DATA_PATH).c_str(), "wb") != 0 ||
			fopen_s(&eventFp, (fileName + EVENT_DATA_PATH).c_str(), "wb") != 0) {
			return false;
		}

		fwrite(&m_areaNum, sizeof(m_areaNum), 1, intFp);
		fwrite(&m_time, sizeof(m_time), 1, intFp);
		fwrite(&m_loop, sizeof(m_loop), 1, intFp);
		fwrite(&m_latestLoop, sizeof(m_latestLoop), 1, intFp);
		fwrite(&m_money, sizeof(m_money), 1, intFp);
		for (unsigned int i = 0; i < m_characterData.size(); i++) {
			m_characterData[i]->save(intFp, strFp);
		}
		unsigned int doorSum = (unsigned int)m_doorData.size();
		fwrite(&doorSum, sizeof(doorSum), 1, intFp);
		for (unsigned int i = 0; i < m_doorData.size(); i++) {
			m_doorData[i]->save(intFp, strFp);
		}
		m_eventData->save(eventFp);

		fclose(intFp);
		fclose(strFp);
		fclose(eventFp);

		// �Z�[�u�����ʒm�̊J�n �ŏ������͂��Ȃ�
		if (m_time > 1) {
			m_noticeSaveDone = NOTICE_SAVE_DONE_TIME;
		}
	}
	return true;
}


bool GameData::load() {
	FILE* intFp = nullptr, * strFp = nullptr, * eventFp = nullptr;

	if (!loadCommon(&m_soundVolume, &GAME_WIDE, &GAME_HEIGHT)) { return false; }

	string fileName = m_saveFilePath;
	if (fopen_s(&intFp, (fileName + INT_DATA_PATH).c_str(), "rb") != 0 ||
		fopen_s(&strFp, (fileName + STR_DATA_PATH).c_str(), "rb") != 0 ||
		fopen_s(&eventFp, (fileName + EVENT_DATA_PATH).c_str(), "rb") != 0) {
		return false;
	}

	fread(&m_areaNum, sizeof(m_areaNum), 1, intFp);
	fread(&m_time, sizeof(m_time), 1, intFp);
	fread(&m_loop, sizeof(m_loop), 1, intFp);
	fread(&m_latestLoop, sizeof(m_latestLoop), 1, intFp);
	fread(&m_money, sizeof(m_money), 1, intFp);
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		if (feof(intFp) != 0 || feof(strFp) != 0) { break; }
		m_characterData[i]->load(intFp, strFp);
	}
	int doorSum = 0;
	m_doorData.clear();
	fread(&doorSum, sizeof(doorSum), 1, intFp);
	for (int i = 0; i < doorSum; i++) {
		m_doorData.push_back(new DoorData(intFp, strFp));
	}
	m_eventData->init();
	m_eventData->load(eventFp);

	fclose(intFp);
	fclose(strFp);
	fclose(eventFp);
	return true;
}

// �o�b�N�A�b�v�����i���[�v�����߂��@�\�p�j
bool GameData::saveLoop() {

	// �ꎞ�I�Ƀp�X�� savedata/<�Z�[�u�f�[�^�ԍ�>/loop/<m_loop>/ �ɂ���
	string filePath = m_saveFilePath;
	ostringstream oss;
	oss << m_saveFilePath << "loop/" << m_loop;
	_mkdir(oss.str().c_str());
	m_saveFilePath = oss.str() + "/";
	save(true);
	// �p�X�����Ƃɖ߂�
	m_saveFilePath = filePath;

	return true;
}


// ���[�v���w�肵�ă��[�h�AlatestLoop�����͕ς��Ȃ�
bool GameData::loadLoop(int loop) {
	int latestLoop = m_latestLoop;
	string filePath = m_saveFilePath;
	ostringstream oss;
	oss << m_saveFilePath << "loop/" << loop << "/";
	m_saveFilePath = oss.str();
	bool flag = load();
	m_saveFilePath = filePath;
	m_latestLoop = latestLoop;
	return flag;
}


// �S�Z�[�u�f�[�^���ʂ�1�����L�^����f�[�^
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

// �S�Z�[�u�f�[�^���ʂ̍��ڂ��������[�h
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


// �Z�[�u�f�[�^�폜
void GameData::removeSaveData() {
	string fileName = m_saveFilePath;
	remove((fileName + INT_DATA_PATH).c_str());
	remove((fileName + STR_DATA_PATH).c_str());
	remove((fileName + EVENT_DATA_PATH).c_str());
}


// ���g�̃f�[�^��World�Ƀf�[�^���f������
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
	world->setMoney(m_money);
}


// World�̃f�[�^�����g�ɔ��f������
void GameData::asignedWorld(const World* world, bool notCharacterPoint) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i]->name(), m_characterData[i], m_areaNum, notCharacterPoint);
	}
	world->asignDoorData(m_doorData, m_areaNum);
	m_money = world->getMoney();
}


// �X�g�[���[���i�񂾎��ɃZ�[�u�f�[�^���X�V���� �G���A�O�iWorld�ȊO�j���l������
void GameData::updateStory(Story* story) {
	m_areaNum = story->getWorld()->getAreaNum();
	m_time = story->getTimer()->getTime();
	m_loop = story->getLoop();
	m_latestLoop = max(m_latestLoop, m_loop);
	m_soundVolume = story->getWorld()->getSoundPlayer()->getVolume();
	m_money = story->getWorld()->getMoney();
}


void GameData::updateWorldVersion(Story* story) {
	// Story�ɂ���ĕύX�E�V�o�ꂳ�ꂽ�L���������擾
	CharacterLoader* characterLoader = story->getCharacterLoader();
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		characterLoader->saveCharacterData(m_characterData[i]);
	}
	// �h�A�̏����擾
	ObjectLoader* objectLoader = story->getObjectLoader();
	objectLoader->saveDoorData(m_doorData);
	story->doneWorldUpdate();
}


// ���E�̂�蒼��
void GameData::resetWorld() {
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		m_characterData[i]->setId(-1);
		m_characterData[i]->setAreaNum(-1);
	}
	m_doorData.clear();
	m_money = 0;
}



/*
* �Q�[���{��
*/
Game::Game(const char* saveFilePath, int loop) {

	m_timeSpeed = DEFAULT_TIME_SPEED;

	if (loop == -1) { // ���[�v�w��Ȃ��A�ŐV�̃��[�v
		m_gameData = new GameData(saveFilePath);
	}
	else { // ���[�v�w�肠��
		m_gameData = new GameData(saveFilePath, loop);
	}

	m_soundPlayer = new SoundPlayer();
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);
	m_world->setMoney(m_gameData->getMoney());
	m_soundPlayer->stopBGM();

	m_story = new Story(m_gameData->getLoop(), m_gameData->getTime(), m_world, m_soundPlayer, m_gameData->getEventData(), WORLD_LIFESPAN, MAX_VERSION);
	m_world->changeCharacterVersion(m_story->getVersion());
	m_world->setDate(m_story->getDate());

	// �Z�[�u�f�[�^�ɏ㏑��
	m_gameData->updateStory(m_story);

	// �f�[�^�𐢊E�ɔ��f
	m_gameData->asignWorld(m_world, false);

	m_world->cameraPointInit();

	m_skill = nullptr;

	// �ꎞ��~�֘A
	m_battleOption = nullptr;
	m_pauseSound = LoadSoundMem("sound/system/pause.wav");

	// �Q�[���̍ċN���i�^�C�g���֖߂�j��v������Flag
	m_rebootFlag = false;

	// �͂��߂���v���C����ꍇ�����f�[�^���Z�[�u
	if (!m_gameData->getExist()) {
		m_gameData->save();
		m_gameData->saveLoop();
	}

	m_gameoverCnt = 0;
}


Game::~Game() {
	delete m_gameData;
	delete m_world;
	delete m_story;
	if (m_battleOption != nullptr) {
		delete m_battleOption;
	}
	if (m_skill != nullptr) {
		delete m_skill;
	}
	DeleteSoundMem(m_pauseSound);
	delete m_soundPlayer;
}


bool Game::play() {

	// �Q�[���I�[�o�[
	if (m_gameoverCnt > 0) {
		m_gameoverCnt++;
		if (m_gameoverCnt == 120) {
			m_rebootFlag = true;
		}
		return false;
	}

	// �ꎞ��~
	if (controlQ() == 1) {
		if (m_battleOption == nullptr) {
			if (m_timeSpeed == DEFAULT_TIME_SPEED) {
				m_battleOption = new BattleOption(m_soundPlayer);
				m_soundPlayer->stopBGM();
			}
			else {
				// ���x�A�b�v���[�h���Ȃ�ꎞ��~�������[�h��������
				m_timeSpeed = DEFAULT_TIME_SPEED;
			}
		}
		else {
			// ���ʂ̕ύX�����邩������Ȃ��̂ŃZ�[�u
			m_gameData->saveCommon(m_battleOption->getNewSoundVolume(), GAME_WIDE, GAME_HEIGHT);
			delete m_battleOption;
			m_battleOption = nullptr;
			m_soundPlayer->playBGM();
		}
		m_soundPlayer->pushSoundQueue(m_pauseSound);
	}
	if (m_battleOption != nullptr) {
		m_battleOption->play();
		if (m_battleOption->getTitleFlag()) {
			// ���ʂ̕ύX�����邩������Ȃ��̂ŃZ�[�u���^�C�g���֖߂�
			m_gameData->saveCommon(m_battleOption->getNewSoundVolume(), GAME_WIDE, GAME_HEIGHT);
			m_rebootFlag = true;
		}
		if (m_battleOption->getQuickFlag()) {
			if (m_skill == nullptr && !m_story->eventNow()) {
				// ���x�A�b�v���[�h���J�n
				m_timeSpeed = QUICK_TIME_SPEED;
			}
			delete m_battleOption;
			m_battleOption = nullptr;
			m_soundPlayer->playBGM();
		}
		m_soundPlayer->play();
		return false;
	}

	// �X�L������
	if (controlF() == 1 && skillUsable()) {
		m_timeSpeed = DEFAULT_TIME_SPEED;
		m_world->setSkillFlag(true);
		m_skill = new HeartSkill(min(m_story->getLoop() - 1, MAX_SKILL), m_world, m_soundPlayer);
	}

	// �X�L���������ŁA����L�^��
	if (m_skill != nullptr && !m_skill->finishRecordFlag()) {
		m_skill->battle();
		if (m_world->getWorldFreezeTime() == 0) {
			m_skill->play();
		}
	}
	// �X�g�[���[�i�s
	else if (m_story->play(WORLD_LIFESPAN, MAX_VERSION, m_timeSpeed)) {
		m_timeSpeed = DEFAULT_TIME_SPEED;
		// ���̃��[�v�ֈڍs�̏ꍇStory����蒼��
		if (m_story->getLoop() > m_gameData->getLoop()) {
			int nextLoopNum = m_story->getLoop();
			delete m_story;
			m_story = new Story(nextLoopNum, 0, m_world, m_soundPlayer, m_gameData->getEventData(), WORLD_LIFESPAN, MAX_VERSION);
			m_gameData->updateStory(m_story);
			// ���[�v����̏�Ԃ��o�b�N�A�b�v
			m_gameData->saveLoop();
		}
		else { // �C�x���g�N���A
			m_gameData->updateStory(m_story);
			m_gameData->asignedWorld(m_world, false);
			if (m_gameData->getLoop() < m_gameData->getLatestLoop()) {
				m_gameData->saveLoop();// �ߋ��̃f�[�^(�o�b�N�A�b�v)���v���C���Ȃ炻�̃f�[�^���X�V
			}
		}
		// �Z�[�u (�o�b�N�A�b�v�͍X�V����Ȃ�)
		m_gameData->save();
	}
	else if (m_skill != nullptr) { // �X�L���������ŁA�Ō�̃��[�v��
		if (m_world->getWorldFreezeTime() == 0 && m_skill->play()) {
			endSkill();
		}
	}

	if (m_story->eventNow()) {
		m_timeSpeed = DEFAULT_TIME_SPEED;
	}

	if (m_story->getNeedWorldUpdate()) {
		m_gameData->updateWorldVersion(m_story);
	}

	// �Z�[�u�����ʒm�̏���
	m_gameData->setNoticeSaveDone(max(0, m_gameData->getNoticeSaveDone() - 1));

	if (m_world->getWorldFreezeTime() == 0) {
		m_soundPlayer->play();
	}

	// �e�X�g�͈ȍ~�����s���Ȃ�
	if (TEST_MODE) { return false; }

	// �O�̃Z�[�u�|�C���g�֖߂邱�Ƃ��v�����ꂽ TODO: �폜
	int prevLoop = m_story->getBackPrevSave();
	if (prevLoop > 0) {
		endSkill();
		backPrevSave();
		m_story->doneBackPrevSave();
		return true;
	}
	// �Q�[���I�[�o�[
	else if (m_world->playerDead() && m_world->getBrightValue() == 0) {
		// story����n�[�g�����ꂽ���Ƃ�`����ꂽ��^�C�g���֖߂�
		// �����̂��C�x���g�̐��������Ȃ�O��if��(m_story->getBackPrevSaveFlag())�ɂЂ�������͂�
		m_gameoverCnt++;
	}
	// �G���A�ړ�
	else if (m_world->getBrightValue() == 0 && CheckSoundMem(m_world->getDoorSound()) == 0) {
		endSkill();
		m_world->changePlayer(m_world->getCharacterWithName("�n�[�g"));
		int fromAreaNum = m_world->getAreaNum();
		int toAreaNum = m_world->getNextAreaNum();
		m_gameData->asignedWorld(m_world, false);
		bool resetBgmFlag = m_world->getResetBgmFlag();
		bool blindFlag = m_world->getBlindFlag();
		delete m_world;
		m_world = new World(fromAreaNum, toAreaNum, m_soundPlayer);
		m_world->setBlindFlag(blindFlag);
		if (resetBgmFlag) { m_soundPlayer->stopBGM(); }
		m_gameData->asignWorld(m_world);
		if (m_story->getLoop() == m_gameData->getLoop()) {
			m_world->setPlayerOnDoor(fromAreaNum);
		}
		else { // ���[�v���N�������Ƃɂ��G���A�ړ��̎��͏������Ȃǂ��K�v
			m_world->cameraPointInit();
			m_world->setBlindFlag(false);
			m_world->clearCharacter();
			m_story->loopInit();
			m_gameData->resetWorld();
			m_gameData->asignWorld(m_world, true);
			m_soundPlayer->stopBGM();
		}
		m_story->setWorld(m_world);
		m_gameData->setAreaNum(toAreaNum);
		m_story->checkFire(); // ���ꂪ�Ȃ��ƃG���A�ړ������u�ԂɎn�܂�C�x���g��Fire��1F�x���
		m_world->playBGM(); // �G���A�ړ������u�ԂɃC�x���g���n�܂�Ɩ����ɂȂ�̂�h��
		return true;
	}

	return false;
}


// �Z�[�u�f�[�^�����[�h�i�O�̃Z�[�u�|�C���g�֖߂�j TODO: �폜
void Game::backPrevSave() {
	m_gameData->asignedWorld(m_world, true);
	// ����܂ł�World���폜
	delete m_world;
	// �O�̃Z�[�u�f�[�^�����[�h
	GameData prevData(m_gameData->getSaveFilePath(), m_gameData->getLoop());
	// �ȑO��AreaNum�Ń��[�h
	m_world = new World(-1, prevData.getAreaNum(), m_soundPlayer);
	m_gameData->asignWorld(m_world, true);
	m_world->setPlayerPoint(prevData.getCharacterData("�n�[�g"));
	m_world->setPlayerFollowerPoint();
	m_story->setWorld(m_world);
}


// �`�悵�Ă����Ȃ�true
bool Game::ableDraw() {
	return !m_story->getInitDark();
}


// �X�L�������ł���Ƃ���܂ŃX�g�[���[���i��ł��邩
bool Game::afterSkillUsableLoop() const {
	return m_gameData->getLoop() > 1;
}


void Game::endSkill() {
	if (m_skill != nullptr) {
		delete m_skill;
		m_skill = nullptr;
		m_world->setSkillFlag(false);
	}
}


// �X�L�������\���`�F�b�N
bool Game::skillUsable() {

	// �X�g�[���[�̍ŏ��͔����ł��Ȃ�
	if (afterSkillUsableLoop() || TEST_MODE) {
		// �X�L���������A�d�����Ĕ����̓_��
		if (m_skill == nullptr) {
			// ����̃C�x���g����G���A�ړ����̓_��
			if (m_story->skillAble() &&
				m_world->getBrightValue() == 255 &&
				m_world->getControlCharacterName() == "�n�[�g" &&
				m_world->getConversation() == nullptr &&
				m_world->getObjectConversation() == nullptr &&
				m_world->getWorldFreezeTime() == 0)
			{
				// �n�[�g���g���X�L�������\�ȏ�Ԃ�
				Character* character = m_world->getCharacterWithName("�n�[�g");
				if (character->getHp() > 0 && character->getSkillGage() == character->getMaxSkillGage()) {
					character->setSkillGage(0);
					return true;
				}
			}
		}
	}

	return false;
}


/*
* �n�[�g�̃X�L��
*/
HeartSkill::HeartSkill(int loopNum, World* world, SoundPlayer* soundPlayer) {
	m_loopNum = loopNum;
	m_loopNow = 0;
	m_world_p = world;
	m_cnt = 0;

	// �I���W�i���̃n�[�g�𓮂��Ȃ������A���G
	Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
	original->setGroupId(-1);
	m_world_p->setBrainWithId(m_world_p->getPlayerId(), new Freeze());
	m_world_p->createRecorder();

	// �ŏ��̕���
	m_duplicationWorld = createDuplicationWorld(m_world_p);

	m_soundPlayer_p = soundPlayer;
	m_sound = LoadSoundMem("sound/battle/skill.wav");
	m_soundPlayer_p->pushSoundQueue(m_sound);
}


HeartSkill::~HeartSkill() {
	for (unsigned int i = 0; i < m_duplicationId.size(); i++) {
		m_world_p->popCharacterController(m_duplicationId[i]);
		m_world_p->eraseRecorder();
	}
	DeleteSoundMem(m_sound);
	if (m_duplicationWorld != nullptr) {
		delete m_duplicationWorld;
	}
}

// �X�L���I������true
bool HeartSkill::play() {
	m_cnt++;
	if (m_cnt == DUPLICATION_TIME) {
		// ���̃��[�v��
		m_cnt = 0;
		m_loopNow++;
		m_world_p->initRecorder();
		m_soundPlayer_p->pushSoundQueue(m_sound);

		if (m_loopNow < m_loopNum) {
			// duplicationWorld��V���ɍ��Aworld�ƈȑO��duplicationWorld�̑���L�^���R�s�[����
			World* nextWorld = createDuplicationWorld(m_world_p);
			delete m_duplicationWorld;
			m_duplicationWorld = nextWorld;
		}
		else if (m_loopNow == m_loopNum) {
			// �I���W�i���̃n�[�g�����ɖ߂�
			Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
			original->setGroupId(0);
			m_world_p->setBrainWithId(m_world_p->getPlayerId(), new KeyboardBrain(m_world_p->getCamera()));
			m_world_p->setFocusId(m_world_p->getPlayerId());
			delete m_duplicationWorld;
			m_duplicationWorld = nullptr;
		}
		else {
			return true;
		}
	}
	return false;
}


// ��킹��i����L�^������Ƃ������������������j
void HeartSkill::battle() {
	m_duplicationWorld->battle();
}


// �I��������ǂ����̔���
bool HeartSkill::finishRecordFlag() {
	return m_loopNow >= m_loopNum;
}


// ���E�̃R�s�[����� �R�s�[�̕ύX�̓I���W�i���ɉe�����Ȃ�
World* HeartSkill::createDuplicationWorld(const World* world) {
	createDuplicationHeart();
	World* res = new World(world);
	res->setSkillFlag(true);
	return res;
}


// m_world_p�ɕ�����push
void HeartSkill::createDuplicationHeart() {
	// �n�[�g�̕���
	Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
	Character* duplicationHeart = new Heart("�����̃n�[�g", original->getHp(), original->getX(), original->getY(), 0, original->getAttackInfo());
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