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
#include "Brain.h"
#include "ControllerRecorder.h"
#include "PausePage.h"
#include "DxLib.h"

/*
* �L�����̃f�[�^
*/
CharacterData::CharacterData(const char* name) {
	m_initFlag = false;
	m_name = name;
	m_hp = -1;
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


/*
* �h�A�̃f�[�^
*/
DoorData::DoorData(int x1, int y1, int x2, int y2, int from, int to, const char* fileName) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	m_from = from;
	m_to = to;
	m_fileName = fileName;
}


/*
* �Q�[���̃Z�[�u�f�[�^
*/
// ������Ԃ̃f�[�^���쐬
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	const bool test = false;

	m_areaNum = 1;
	m_storyNum = 1;

	if (test) {
		m_areaNum = 0;
		m_storyNum = 0;
	}

	m_soundVolume = 50;

	// ��v�L������ݒ�
	const int mainSum = 5;
	const char* mainCharacters[mainSum] = {
		"�n�[�g",
		"�V�G�X�^",
		"�q�G�����L�[",
		"���@���L���A",
		"�G���E�T�f�B"
	};
	for (int i = 0; i < mainSum; i++) {
		m_characterData.push_back(new CharacterData(mainCharacters[i]));
	}
}

// �t�@�C�����w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// �Z�[�u�f�[�^��ǂݍ���ŏ�����Ԃ̃f�[�^���㏑��

}

GameData::~GameData() {
	for (unsigned int i = 0; i < m_characterData.size(); i++) {
		delete m_characterData[i];
	}
	for (unsigned int i = 0; i < m_doorData.size(); i++) {
		delete m_doorData[i];
	}
}

// ���g�̃f�[�^��World�Ƀf�[�^���f������
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

// World�̃f�[�^�����g�ɔ��f������
void GameData::asignedWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i]->name(), m_characterData[i], m_areaNum);
	}
	world->asignDoorData(m_doorData, m_areaNum);
}

// �X�g�[���[���i�񂾎��ɃZ�[�u�f�[�^���X�V����
void GameData::updateStory(Story* story) {
	m_storyNum = story->getStoryNum();
	// Story�ɂ���ĕύX�E�V�o�ꂳ�ꂽ�L���������擾
	CharacterLoader* characterLoader = story->getCharacterLoader();
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		characterLoader->saveCharacterData(m_characterData[i]);
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
	m_soundPlayer->setVolume(m_gameData->getSoundVolume());

	// ���E
	m_world = new World(-1, m_gameData->getAreaNum(), m_soundPlayer);

	// �X�g�[���[
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
	// �Z�[�u�f�[�^�ɏ㏑��
	m_gameData->updateStory(m_story);

	// �f�[�^�𐢊E�ɔ��f
	m_gameData->asignWorld(m_world);

	// �X�L��
	m_skill = NULL;

	// �ꎞ��~���
	m_gamePause = NULL;
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// �ꎞ��~
	if (controlQ() == 1) {
		if (m_gamePause == NULL) {
			m_gamePause = new GamePause(m_soundPlayer);
			// �����ŉ��y���~�߂�
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

	// ����ȏ�X�g�[���[��i�܂��Ȃ��i�e�X�g�p�j
	if (m_gameData->getStoryNum() == 5 || m_gameData->getStoryNum() == 0) {
		m_world->battle();
		m_soundPlayer->play();
		return false;
	}

	// �X�L������ F�L�[���X�L����������Ԃ������\�ȃC�x���g���i�������̓C�x���g���łȂ��j���G���A�ړ����łȂ�
	if (m_gameData->getStoryNum() >= 4) { // �X�g�[���[�̍ŏ��͔����ł��Ȃ�
		if (controlF() == 1 && m_skill == NULL) { // F�L�[�Ŕ����A�������X�L���g������
			if (m_story->skillAble() && m_world->getBrightValue() == 255) { // ����̃C�x���g����G���A�ړ����̓_��
				if (m_world->getCharacterWithName("�n�[�g")->getHp() > 0) {
					m_world->setSkillFlag(true);
					m_skill = new HeartSkill(3, m_world, m_soundPlayer);
				}
			}
		}
	}
	
	// �X�L��������
	if (m_skill != NULL) {
		if (m_skill->play()) {
			// �X�L���I��
			delete m_skill;
			m_skill = NULL;
			m_world->setSkillFlag(false);
		}
	}
	// �X�g�[���[�i�s
	else if (m_story->play()) {
		// ���̃X�g�[���[��
		m_gameData->setStoryNum(m_gameData->getStoryNum() + 1);
		delete m_story;
		m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);
		// �X�g�[���[�̉e���ŃI�u�W�F�N�g���ǉ������i��x�ǉ������ƍ���GameData�Ńf�[�^�͕ێ����ꑱ����j
		// �Z�[�u�f�[�^�ɏ㏑��
		m_gameData->updateStory(m_story);
		// World�ɔ��f
		m_world->addCharacter(m_story->getCharacterLoader());
		m_world->addObject(m_story->getObjectLoader());
	}

	// ��
	m_soundPlayer->play();

	// �G���A�ړ�
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

	// ���ʉ�
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
		// ���̃��[�v��
		m_cnt = 0;
		m_loopNow++;
		m_world_p->initRecorder();
		m_soundPlayer_p->pushSoundQueue(m_sound);

		if (m_loopNow < m_loopNum) {
			// duplicationWorld��V���ɍ��Aworld�ƈȑO��duplicationWorld�̑���L�^���R�s�[����
			World* nextWorld = createDuplicationWorld(m_world_p);
			copyRecord(m_duplicationWorld, nextWorld);
			delete m_duplicationWorld;
			m_duplicationWorld = nextWorld;
		}
		else if (m_loopNow == m_loopNum) {
			// �I���W�i���̃n�[�g�����ɖ߂�
			Character* original = m_world_p->getCharacterWithId(m_world_p->getPlayerId());
			original->setGroupId(0);
			m_world_p->setBrainWithId(m_world_p->getPlayerId(), new KeyboardBrain(m_world_p->getCamera()));
			m_world_p->setFocusId(m_world_p->getPlayerId());
			// �Ō�̃��[�v�͂��Ƃ�World�ɑ���L�^���R�s�[���āA����World��battle
			copyRecord(m_duplicationWorld, m_world_p);
			delete m_duplicationWorld;
		}
		else {
			// �X�L���I��
			for (unsigned int i = 0; i < m_duplicationId.size(); i++) {
				m_world_p->popCharacter(m_duplicationId[i]);
				m_world_p->eraseRecorder();
			}
			return true;
		}
	}

	// ��킹��i�Ō�̃��[�v�ȊO�Ȃ�A����L�^������Ƃ������������������j
	if (m_loopNow < m_loopNum) {
		m_duplicationWorld->battle();
	}
	else {
		m_world_p->battle();
	}

	return false;
}

// ���E�̃R�s�[����� �R�s�[�̕ύX�̓I���W�i���ɉe�����Ȃ�
World* HeartSkill::createDuplicationWorld(const World* world) {
	createDuplicationHeart();
	World* res = new World(world);
	res->setSkillFlag(true);
	return res;
}

// ����L�^���R�s�[����
void HeartSkill::copyRecord(const World* from, World* to) {

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