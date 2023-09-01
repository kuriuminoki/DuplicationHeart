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
* �L�����̃f�[�^
*/
CharacterData::CharacterData(const char* name) {
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
* �Q�[���̃Z�[�u�f�[�^
*/
// ������Ԃ̃f�[�^���쐬
GameData::GameData() {
	m_saveFilePath = "data/save/savedata1.dat";

	m_areaNum = 1;
	m_storyNum = 1;
	m_soundVolume = 10;

	// ��v�L������ݒ�
	m_characterData.push_back("�n�[�g");
	m_characterData.push_back("�V�G�X�^");

}

// �t�@�C�����w�肵�ăf�[�^�𕜌�
GameData::GameData(const char* saveFilePath):
	GameData()
{
	m_saveFilePath = saveFilePath;
	// �Z�[�u�f�[�^��ǂݍ���ŏ�����Ԃ̃f�[�^���㏑��

}

// ���g�̃f�[�^��World�Ƀf�[�^���f������
void GameData::asignWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignedCharacterData(m_characterData[i].name(), m_characterData[i]);
	}
}

// World�̃f�[�^�����g�ɔ��f������
void GameData::asignedWorld(World* world) {
	size_t size = m_characterData.size();
	for (unsigned int i = 0; i < size; i++) {
		world->asignCharacterData(m_characterData[i].name(), m_characterData[i], m_areaNum);
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

	// �f�[�^�𐢊E�ɔ��f
	m_gameData->asignWorld(m_world);

	// �X�g�[���[
	m_story = new Story(m_gameData->getStoryNum(), m_world, m_soundPlayer);

	// �X�L��
	m_skill = NULL;
}

Game::~Game() {
	delete m_gameData;
	delete m_soundPlayer;
	delete m_world;
}

bool Game::play() {

	// ����ȏ�X�g�[���[��i�܂��Ȃ��i�e�X�g�p�j
	if (m_gameData->getStoryNum() == 5) {
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
		m_world->addObject(m_story->getObjectLoader());
	}

	// ��
	m_soundPlayer->play();

	// �G���A�ړ�
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