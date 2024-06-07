#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Animation.h"
#include "Sound.h"
#include "CsvReader.h"
#include "Control.h"
#include "Define.h"
#include "Item.h"
#include "Text.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include "Game.h"
#include "GraphHandle.h"
#include "DxLib.h"
#include <algorithm>


using namespace std;


/*
* �v���C���[�؂�ւ�����
*/
PlayerChanger::PlayerChanger(std::vector<CharacterController*> controllers_p, const Character* player_p) {
	m_nowCharacter_p = player_p;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		if (controllers_p[i]->getAction()->getCharacter()->getId() == m_nowCharacter_p->getId()) {
			m_prevBrainName = controllers_p[i]->getBrain()->getBrainName();
			if (m_prevBrainName == "KeyboardBrain") {
				m_prevBrainName = "FollowNormalAI";
			}
			break;
		}
	}
}

const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p) {
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		int groupId = target->getGroupId();
		int id = target->getId();
		if (id == m_nowCharacter_p->getId()) { continue; }
		// ���Ԃ��ǂ�������
		if (groupId == m_nowCharacter_p->getGroupId()) {
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			if (id > m_nowCharacter_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}

void PlayerChanger::changeCharacter(string prevBrainName, const Character* nextCharacter_p) {
	m_prevBrainName = prevBrainName;
	m_nowCharacter_p = nextCharacter_p;
}


// vector�ɓ������S�I�u�W�F�N�g���폜����
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector�ɓ�����deleteFlag��tarue�̃I�u�W�F�N�g���폜����
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector�ɓ������S�I�u�W�F�N�g�𓮂���
void actionObject(vector<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �I�u�W�F�N�g�̓���
		objects[i]->action();
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̊ђʎ�����
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		objects[i]->penetration(controller);
	}
}


/*
* �R���X�g���N�^
*/
World::World() {
	m_duplicationFlag = false;

	m_brightValue = 255;

	m_resetBgmFlag = false;

	m_blindFlag = false;

	// ��b�C�x���g
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// ���[�r�[
	m_movie_p = nullptr;

	// �X�L��������
	m_skillFlag = false;

	// �J�����̔{���̍ő�E�ŏ��l���𑜓x���猈��
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;

	m_date = 0;

	m_bossDeadEffectCnt = 0;

	m_money = 0;

}

/*
* �I�u�W�F�N�g�̃��[�h�Ȃ�
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// �T�E���h�v���C���[
	m_soundPlayer_p = soundPlayer;

	// ��l���̃X�^�[�g�n�_
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// �G���A�����[�h
	const AreaReader data(fromAreaNum, toAreaNum, m_soundPlayer_p);
	m_camera = data.getCamera();
	m_focusId = data.getFocusId();
	m_playerId = data.getPlayerId();
	m_soundPlayer_p->setBGM(data.getBgm());
	m_characters = data.getCharacters();
	m_characterControllers = data.getCharacterControllers();
	m_stageObjects = data.getObjects();
	m_doorObjects = data.getDoorObjects();
	data.getBackGround(m_backGroundGraph, m_backGroundColor);

	// �v���C���[���Z�b�g
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}
	// �v���C���[�̕����֌�������
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		m_characterControllers[i]->setPlayerDirection(m_player_p, true);
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	m_camera->setEx(m_cameraMaxEx);

	m_characterDeadGraph = new GraphHandles("picture/effect/dead", 5, 1.0, 0, true);
	m_characterDamageGraph = new GraphHandles("picture/effect/damage", 1, 0.2, 0, true);
	m_bombGraph = new GraphHandles("picture/effect/bomb", 9, 1.0, 0, true);
	m_characterDeadSound = LoadSoundMem("sound/battle/dead.wav");
	m_bombSound = LoadSoundMem("sound/battle/bomb.wav");
	m_doorSound = LoadSoundMem("sound/battle/door.wav");
	m_cameraInSound = LoadSoundMem("sound/battle/cameraIn.mp3");
	m_cameraOutSound = LoadSoundMem("sound/battle/cameraOut.mp3");

}

World::World(const World* original) :
	World()
{
	m_duplicationFlag = true;

	// �G���A���R�s�[ (�v���~�e�B�u�^)
	m_areaNum = original->getAreaNum();
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_date = original->getDate();
	m_money = original->getMoney();

	// �G���A���R�s�[ (�R�s�[���Ƌ��L�������)
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_characterDamageGraph = original->getCharacterDamageGraph();
	m_bombGraph = original->getBombGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_bombSound = original->getBombSound();
	m_doorSound = original->getDoorSound();
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();
	m_bossDeadEffectCnt = original->getBossDeadEffextCnt();

	// �V�K�쐬������� (�|�C���^���ς��)
	m_camera = new Camera(original->getCamera());
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player_p = copy; }
	}
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// Brain��Action�R�s�[�p��Character�ƃJ������n��
		copy = original->getCharacterControllers()[i]->createCopy(m_characters, m_camera);
		m_characterControllers.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getStageObjects().size(); i++) {
		Object* copy;
		copy = original->getStageObjects()[i]->createCopy();
		m_stageObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getDoorObjects().size(); i++) {
		Object* copy;
		copy = original->getDoorObjects()[i]->createCopy();
		m_doorObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAttackObjects().size(); i++) {
		Object* copy;
		copy = original->getAttackObjects()[i]->createCopy();
		m_attackObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAnimations().size(); i++) {
		Animation* copy;
		copy = original->getAnimations()[i]->createCopy();
		m_animations.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getItemVector().size(); i++) {
		Item* copy;
		copy = original->getItemVector()[i]->createCopy();
		m_itemVector.push_back(copy);
	}

	// �����ݒ�
	m_camera->setEx(m_cameraMaxEx);

}

World::~World() {
	// �J�������폜����
	delete m_camera;

	// �S�I�u�W�F�N�g���폜����B
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// �U���G�t�F�N�g�폜
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// �A�C�e���폜
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// �S�R���g���[�����폜����B
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// �S�L�����N�^�[���폜����B
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// �w�i
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
		delete m_characterDeadGraph;
		delete m_characterDamageGraph;
		delete m_bombGraph;
		DeleteSoundMem(m_characterDeadSound);
		DeleteSoundMem(m_bombSound);
		DeleteSoundMem(m_doorSound);
		DeleteSoundMem(m_cameraInSound);
		DeleteSoundMem(m_cameraOutSound);
	}

	if (m_objectConversation != nullptr) {
		delete m_objectConversation;
	}
}

// Drawer�p�FCharacterAction��vector��Ԃ�
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// HP���O����DeadGraph���Ȃ��Ȃ�\�����Ȃ�
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() > 0 || m_characterControllers[i]->getAction()->getCharacter()->haveDeadGraph()) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
		else if (m_characterControllers[i]->getAction()->getCharacter()->getBossFlag() && m_bossDeadEffectCnt > 0) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
	}
	return actions;
}

// Drawer�p�FObject��vector��Ԃ�
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer�p�F�L���������ɕ`�悷��Object��vector��Ԃ�
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer�p�FAnimation��vector��Ԃ�
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// �G�t�F�N�g
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// �A�C�e��
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			// ���ł������Ȃ�_��
			if (m_itemVector[i]->getCnt() < m_itemVector[i]->ERASE_CNT * 2 / 3 || m_itemVector[i]->getCnt() / 3 % 2 == 0) {
				allAnimations.push_back(m_itemVector[i]->getAnimation());
			}
		}
	}

	return allAnimations;
}

// ���O�ŃL��������
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ���O�ŃR���g���[������
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// ID�ŃL��������
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID�w���Brain�ύX
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// �X�g�[���[�ɂ��ǉ��L�����N�^�[
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// �L�����N�^�[
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// �R���g���[��
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// �X�g�[���[�ɂ��L�����̐��\�ω�
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

// �X�g�[���[�ɂ��ǉ��I�u�W�F�N�g
void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// �ǂ⏰
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// �h�A
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// �v���C���[��HP��0�Ȃ�true
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// �����삵�Ă���L�����̖��O
string World::getControlCharacterName() const {
	return m_playerChanger->getNowPlayer()->getCharacterInfo()->name();
}

// �v���C���[��HP��MAX�ɂ���
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
}


// �X�L�������F�n�[�g��Freeze�ɂ���
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// �X�L�������F�����̃n�[�g�ǉ��p
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// �X�L�������F�����̃n�[�g�폜�p
void World::popCharacterController(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	// Target�ɂ��Ă���AI�͊O���Ă��炢�����̂�HP=0�ɂ���B
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "�n�[�g") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// �X�L�������F���R�[�_���쐬���g�p���J�n
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// �X�L�������F���R�[�_�̎��Ԃ��ŏ��ɖ߂�
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// �X�L�������F���R�[�_�̎g�p����߂č폜����
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "�n�[�g") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// �f�[�^�Ǘ��F�L�����̏�Ԃ�ύX���� ���Ȃ��Ȃ�쐬����
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// �L�����̐ݒ�
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// �L������V�K�쐬����ꍇ�i���̃G���A�ɂ���͂��̃L���������܂����Ȃ��j
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "�n�[�g")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// �R���g���[���A�A�N�V�����ABrain�̐ݒ�
	size_t controllerSize = m_characterControllers.size();
	for (unsigned int i = 0; i < controllerSize; i++) {
		const Character* character = m_characterControllers[i]->getAction()->getCharacter();
		if (name == character->getName()) {
			CharacterController* controller = createControllerWithData(character, data);
			controller->setPlayerDirection(m_player_p, true);
			delete m_characterControllers[i];
			m_characterControllers[i] = controller;
		}
	}
}

// �f�[�^�Ǘ��F�L�����̏�Ԃ�������
void World::asignCharacterData(const char* name, CharacterData* data, int fromAreaNum, bool notCharacterPoint) const {
	size_t size = m_characterControllers.size();
	for (unsigned i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == name) {
			const Character* c = m_characterControllers[i]->getAction()->getCharacter();
			data->setVersion(c->getVersion());
			data->setHp(c->getHp());
			data->getSkillGage(c->getSkillGage());
			data->setInvincible(c->getInvincible());
			data->setId(c->getId());
			data->setGroupId(c->getGroupId());
			data->setAreaNum(fromAreaNum);
			if (!notCharacterPoint) {
				data->setX(c->getX());
				data->setY(c->getY() + c->getHeight()); // Y2���W��ۑ� ���[�h���͐g���ŕ␳
			}
			data->setBrainName(m_characterControllers[i]->getBrain()->getBrainName());
			data->setTargetName(m_characterControllers[i]->getBrain()->getTargetName());
			if (m_characterControllers[i]->getBrain()->getFollow() != nullptr) {
				data->setFollowName(m_characterControllers[i]->getBrain()->getFollow()->getName().c_str());
			}
			data->setActionName(m_characterControllers[i]->getAction()->getActionName());
			data->setSoundFlag(m_characterControllers[i]->getAction()->getSoundPlayer() != nullptr);
			data->setControllerName(m_characterControllers[i]->getControllerName());
			break;
		}
	}
}

// �f�[�^�Ǘ��FDoor�̏�Ԃ�ύX���� ���Ȃ��Ȃ�쐬����
void World::asignedDoorData(DoorData* data) {
	if (data->from() != m_areaNum) { return; }
	bool flag = false;
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		if (data->to() == m_doorObjects[i]->getAreaNum()) {
			m_doorObjects[i]->setX1(data->x1());
			m_doorObjects[i]->setY1(data->y1());
			m_doorObjects[i]->setX2(data->x2());
			m_doorObjects[i]->setY2(data->y2());
			flag = true;
			break;
		}
	}
	if (!flag) {
		m_doorObjects.push_back(new DoorObject(data->x1(), data->y1(), data->x2(), data->y2(), data->fileName(), data->to()));
	}
}

// �f�[�^�Ǘ��FDoor�̏�Ԃ�������
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// �h�A����Ȃ�
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
		// �Z�[�u�f�[�^�Ƀh�A�����݂��邩
		bool flag = false;
		for (unsigned j = 0; j < size; j++) {
			if (data[j]->to() == m_doorObjects[i]->getAreaNum() && data[j]->from() == fromAreaNum) {
				data[j]->setX1(m_doorObjects[i]->getX1());
				data[j]->setY1(m_doorObjects[i]->getY1());
				data[j]->setX2(m_doorObjects[i]->getX2());
				data[j]->setY2(m_doorObjects[i]->getY2());
				data[j]->setFrom(fromAreaNum);
				data[j]->setTo(m_doorObjects[i]->getAreaNum());
				data[j]->setFileName(m_doorObjects[i]->getFileName());
				flag = true;
				break;
			}
		}
		if (!flag) { // �V���ȃh�A�Ȃ�Z�[�u�f�[�^�ɒǉ�
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// �f�[�^�Ǘ��F�v���C���[�Ƃ��̒��Ԃ��h�A�̑O�Ɉړ�
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// �v���C���[
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// ���Ԃ��ړ�
	setPlayerFollowerPoint();

	// �J�������Z�b�g
	cameraPointInit();
}

// �v���C���[�����̍��W�ֈړ�
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// �J�������Z�b�g
	cameraPointInit();
}

// ���Ԃ��v���C���[�̈ʒu�ֈړ�
void World::setPlayerFollowerPoint() {
	// �v���C���[�̒���
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// �ǐՑΏۂ��v���C���[�Ȃ�
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controller�ɑΉ�����Character�ɕύX��������
			for (unsigned int j = 0; j < m_characters.size(); j++) {
				if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_characters[j]->getId()) {
					m_characters[j]->setX(m_player_p->getX());
					m_characters[j]->setY(m_player_p->getY() + m_player_p->getHeight() - m_characters[j]->getHeight());
					break;
				}
			}
		}
	}
}

// �f�[�^�Ǘ��F�J�����̈ʒu�����Z�b�g
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// �f�[�^�Ǘ��F�L����1�̂̏��𐢊E�ɔ��f
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// ���̃Q�[���ŏ��o�ꂶ��Ȃ�
		character->setHp(data->hp());
		character->setSkillGage(data->skillGage());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y���W�͐g���ɍ��킹�Ē���
		character->setY(data->y() - character->getHeight());
	}
}

// �f�[�^�Ǘ��F�R���g���[��1�̏��𐢊E�ɔ��f
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Action���쐬
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brain���쐬
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controller���쐬
	return createController(data->controllerName(), brain, action);
}

/*
*  ��킹��
*/
void World::battle() {
	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	
	// ��ʈÓ]����
	if (dealBrightValue()) { return; }

	// �I�u�W�F�N�g�𒲂ׂ����̃e�L�X�g
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// ��b�I��
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

	// deleteFlag��true�̃I�u�W�F�N�g���폜����B
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	// ��ŃL�������폜��������X�V��������K�v
	updateCharacter();

	// �L�����N�^�[�̓���
	controlCharacter();

	// �I�u�W�F�N�g�̓���
	controlObject();

	// �A�C�e���̓���
	controlItem();

	// �{�X���j�̃G�t�F�N�g
	createBossDeadEffect();

	// �J�����̍X�V
	updateCamera();

	// �T�E���h�v���C���[�̃p���ݒ�p
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// �A�j���[�V�����̍X�V
	updateAnimation();

	// �L�����ύX
	if (!m_duplicationFlag && !m_skillFlag) {
		changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers));
	}

}

void World::changePlayer(const Character* nextPlayer) {
	// �ύX�ł���L���������Ȃ�
	if (nextPlayer == nullptr) { return; }

	// �����쒆�̃L������NPC�ɕύX(Brain��߂�)
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_playerChanger->getNowPlayer()->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// �����삷��L�������C��(Brain��Keyboard�ɂ���)
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_playerChanger->changeCharacter(brainName, nextPlayer);
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// �ǐՑΏۂ𑀍�L�����ɂ���
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getGroupId() == m_characterControllers[i]->getAction()->getCharacter()->getGroupId()) {
			m_characterControllers[i]->setBrainFollow(nextPlayer);
		}
	}
	// �J���������ڂ���L�������ύX
	m_focusId = nextPlayer->getId();
}

//  Battle�F�J�����̍X�V
void World::updateCamera() {

	// �J������h�炷
	m_camera->shaking();

	// �L�����ƃJ�����̋����̍ő�l�𒲂ׂ�
	int max_dx = 0, max_dy = 0;
	// ��ʓ��ɓ���悤�Ƃ��鋗���̍ő�@�����藣�ꂽ�L�����͖���
	const int MAX_DISABLE = 2500;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// ���t�H�[�J�X���Ă���L�����̍��W�ɍ��킹��
		if (m_focusId == m_characters[i]->getId()) {
			m_camera->setGPoint(m_characters[i]->getAtariCenterX(), m_characters[i]->getAtariCenterY());
		}
		// �t�H�[�J�X���Ă���L�����ȊO�Ȃ狗���𒲂ׂ�
		else if (m_characters[i]->getHp() > 0) {
			int x = m_characters[i]->getX();
			if (m_camera->getX() < x) { x += m_characters[i]->getWide() * 2; }
			else { x -= m_characters[i]->getWide(); }
			int dx = abs(m_camera->getX() - x);
			if (dx < MAX_DISABLE) {
				max_dx = max(max_dx, dx);
				int y = m_characters[i]->getY();
				if (m_camera->getY() < y) { y += m_characters[i]->getHeight() * 3 / 2; }
				else { y -= m_characters[i]->getHeight() / 2; }
				max_dy = max(max_dy, abs(m_camera->getY() - y));
			}
		}
	}

	// �J������ڕW�ʒu�֋߂Â���
	m_camera->move();

	// �J�����̊g��E�k��
	// �傫���ύX����K�v������ꍇ�قǁA�傫���g�嗦��ύX����B
	double nowEx = m_camera->getEx();
	int shift = controlLeftShift() + controlRightShift();
	if (shift == 1) {
		bool zoomOutMode = m_camera->getZoomOutMode();
		m_camera->setZoomOutMode(!zoomOutMode);
		if (zoomOutMode) {
			m_soundPlayer_p->pushSoundQueue(m_cameraInSound);
		}
		else {
			m_soundPlayer_p->pushSoundQueue(m_cameraOutSound);
		}
	}
	if (m_camera->getZoomOutMode()) {
		if (nowEx > m_cameraMinEx) {
			m_camera->setEx(max(nowEx - 0.02 * m_exX, 0.1));
		}
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		if (nowEx > m_cameraMinEx && (max_dx > nowWide || max_dy > nowHeight)) {
			// �k��
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.1, d / 100000) * m_exX);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// �g��
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.005, d / 100000) * m_exX);
		}
	}
}

//  Battle�F�A�j���[�V�����̍X�V
void World::updateAnimation() {
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->setVy(m_animations[i]->getVy() + 1);
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}
}

//  Battle�F�L�����̍X�V�i�U���Ώۂ̕ύX�j
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain�̗v���ōU���ΏەύX
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) {
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

//  Battle�F�L�����N�^�[�̓���
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP��0�Ȃ�X�L�b�v
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) {
			continue;
		}

		// �s���O�̏���
		controller->init();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		atariCharacterAndObject(controller, m_stageObjects, true);
		atariCharacterAndObject(controller, m_stageObjects, false); // 1��ڂŎΖʂɂ��邩���킩��A����ɂ���ď������ς�邽��2��ڂ��K�v
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects, false);
		}
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// ���� original�̃n�[�g�̓t���[�Y
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// �ˌ��U��
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// �a���U��
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// ���f original�̃n�[�g�̓t���[�Y
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// �I�u�W�F�N�g�Ƃ̊ђʔ���
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle�F�I�u�W�F�N�g�̓���
void World::controlObject() {
	// �ǂ⏰�̓���
	actionObject(m_stageObjects);

	// �U�������蔻��̓���
	actionObject(m_attackObjects);

	// �ǂ⏰<->�U���̓����蔻��
	atariStageAndAttack();

	// �U��<->�U���̓����蔻��
	atariAttackAndAttack();
}

// Battle�F�A�C�e���̓���
void World::controlItem() {
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// �擾�ς�
		if (m_itemVector[i]->getDeleteFlag()) {
			// ���ʉ����Đ����łȂ��Ȃ�폜
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// ������
		m_itemVector[i]->init();
		// �Ǐ��Ƃ̓����蔻��
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			int vx = m_itemVector[i]->getVx();
			int vy = m_itemVector[i]->getVy();
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, vx, vy)) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
			m_itemVector[i]->setVx(vx);
			m_itemVector[i]->setVy(vy);
		}
		// �L�����Ƃ̓����蔻��
		if (m_itemVector[i]->atariCharacter(m_player_p)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
			// �����ł�����World�ɔ��f
			m_money = min(m_money + m_player_p->getMoney(), MAX_MONEY);
			m_player_p->setMoney(0);
		}
		// ����
		m_itemVector[i]->action();
	}
}

//  Battle�F�L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects, bool slope) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->slopeFlag() != slope) { continue; }
		// �����蔻��������ōs��
		if (objects[i]->atari(controller)) {
			const Character* character = controller->getAction()->getCharacter();
			int targetX1 = 0, targetY1 = 0, targetX2 = 0, targetY2 = 0;
			character->getAtariArea(&targetX1, &targetY1, &targetX2, &targetY2);
			// �G�t�F�N�g�쐬
			int x = (targetX1 + targetX2) / 2;
			int y = (targetY1 + targetY2) / 2;
			Animation* atariAnimation = objects[i]->createAnimation(x, y, 3);
			if (atariAnimation != nullptr) {
				m_animations.push_back(atariAnimation);
			}
			createDamageEffect(x, y, GetRand(3) + 1);
			// ���ʉ�
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			// HP = 0�ɂȂ����Ƃ��i���ꂽ�Ƃ��j
			if (!character->haveDeadGraph() && character->getHp() == 0) {
				if (character->getBossFlag()) {
					m_bossDeadEffectCnt = 300;
				}
				else {
					m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
					m_camera->shakingStart(20, 20);
					m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
					if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && !character->getBossFlag()) {
						int r = GetRand(100);
						// �X�L���������łȂ���ΎG���L�����͊m���ŃA�C�e����������
						if (r < 20) {
							m_itemVector.push_back(new CureItem("cure", x, y, 50));
						}
						else {
							for (int i = 0; i < r % 4; i++) {
								MoneyItem* money = new MoneyItem("money", x, y, 1);
								money->setVx(GetRand(30) - 15);
								money->setVy(GetRand(30) - 31);
								m_itemVector.push_back(money);
							}
						}
					}
				}
			}
		}
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

//  Battle�F�L�����N�^�[�Ɣ��I�u�W�F�N�g�̓����蔻��
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// �X�L���������͔��ɓ���Ȃ�
	if (m_skillFlag) { return; }

	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// �����蔻��������ōs��
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// �h�A����Ȃ�
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// ���������ꍇ �G���A�ړ�������
				m_nextAreaNum = objects[i]->getAreaNum();
				// ��ʂ��Ó]
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle�F�ǂ⏰<->�U���̓����蔻��
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// �U�����Ǐ��ɓ������Ă��邩����
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// �G�t�F�N�g�쐬
				Animation* atariAnimation = m_attackObjects[i]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// �Ǐ���deleteFlag��true�Ȃ�폜����
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// �U����deleteFlag��true�Ȃ�폜����
		if (m_attackObjects[i]->getDeleteFlag()) {
			createBomb(x, y, m_attackObjects[i]);
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle�F�U��<->�U���̓����蔻��
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// �U�����Ǐ��ɓ������Ă��邩����
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// �G�t�F�N�g�쐬
				Animation* atariAnimation = m_attackObjects[j]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				createBomb(x, y, m_attackObjects[i]);
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
		}
	}
}

// Battle: �������N����
void World::createBomb(int x, int y, Object* attackObject) {
	if (attackObject == nullptr) {
		m_animations.push_back(new Animation(x, y, 3, m_bombGraph));
		// ���ʉ�
		m_soundPlayer_p->pushSoundQueue(m_bombSound, adjustPanSound(x, m_camera->getX()));
		// ��ʂ̗h��
		m_camera->shakingStart(20, 20);
	}
	else if (attackObject->getBomb()) {
		// ����
		BombObject* bomb = new BombObject(x, y, 500, 500, attackObject->getDamage(), new Animation(x, y, 3, m_bombGraph));
		bomb->setCharacterId(attackObject->getCharacterId());
		bomb->setGroupId(attackObject->getGroupId());
		m_attackObjects.push_back(bomb);
		// ���ʉ�
		m_soundPlayer_p->pushSoundQueue(m_bombSound, adjustPanSound(x, m_camera->getX()));
		// ��ʂ̗h��
		m_camera->shakingStart(20, 20);
	}
}

// Battle: �_���[�W�G�t�F�N�g�쐬
void World::createDamageEffect(int x, int y, int sum) {
	for (int i = 0; i < sum; i++) {
		Animation* animation = new Animation(x + GetRand(100) - 50, y + GetRand(100) - 50, 120, m_characterDamageGraph);
		animation->setVx(GetRand(30) - 15);
		animation->setVy(GetRand(30) - 31);
		animation->setMovable(true);
		m_animations.push_back(animation);
	}
}

// Battle: �{�X�����ꂽ�Ƃ��̔����G�t�F�N�g
void World::createBossDeadEffect() {
	if (m_bossDeadEffectCnt > 0) {
		m_bossDeadEffectCnt--;
		if (m_bossDeadEffectCnt % 20 == 0) {
			for (unsigned int i = 0; i < m_characters.size(); i++) {
				if (m_characters[i]->getBossFlag()) {
					int x1 = m_characters[i]->getX();
					int y1 = m_characters[i]->getY();
					int x2 = x1 + m_characters[i]->getWide();
					int y2 = y1 + m_characters[i]->getHeight();
					int x = GetRand(x2 - x1) + x1;
					int y = GetRand(y2 - y1) + y1;
					createBomb(x, y, nullptr);
					createDamageEffect(x, y, 2);
					break;
				}
			}
		}
	}
}

// �e�L�������ڕW�n�_�ֈړ����邾�� �S�����B������true��Ԃ�
bool World::moveGoalCharacter() {
	// deleteFlag��true�̃I�u�W�F�N�g���폜����B
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	// ��ŃL�������폜��������X�V��������K�v
	updateCharacter();

	// �L�����N�^�[�̓���
	bool allCharacterAlreadyGoal = true;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP��0�Ȃ�X�L�b�v
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) { 
			continue;
		}

		// �s���O�̏���
		controller->init();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		atariCharacterAndObject(controller, m_stageObjects, true);
		atariCharacterAndObject(controller, m_stageObjects, false); // 1��ڂŎΖʂɂ��邩���킩��A����ɂ���ď������ς�邽��2��ڂ��K�v
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects, false);
		}

		// �ڕW�n�_�ֈړ����鑀�� original�̃n�[�g�̓t���[�Y
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}

		// ���f original�̃n�[�g�̓t���[�Y
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// �I�u�W�F�N�g�Ƃ̊ђʔ���
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// �I�u�W�F�N�g�̓���
	controlObject();

	// �A�C�e���̓���
	controlItem();

	// �J�����̍X�V
	updateCamera();

	// �T�E���h�v���C���[�̃p���ݒ�p
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// �A�j���[�V�����̍X�V
	updateAnimation();

	return allCharacterAlreadyGoal;
}

bool World::dealBrightValue() {
	// ��ʈÓ]�� �G���A�ړ����v���C���[���ꎞ
	if (m_brightValue != 255 || playerDead()) {
		m_brightValue = max(0, m_brightValue - 10);
		if (!playerDead()) { return true; }
	}
	return false;
}

// ��b������
void World::talk() {
	if (m_conversation_p != nullptr) {
		updateCamera();
		m_conversation_p->play();
		// ��b�I��
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// ���[�r�[�𗬂�
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// ���[�r�[�I��
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}