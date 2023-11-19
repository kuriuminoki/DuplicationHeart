#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "Sound.h"
#include "Game.h"
#include "DxLib.h"
#include <cstdlib>


using namespace std;


Character* createCharacter(const char* characterName, int hp, int x, int y, int groupId) {
	Character* character = nullptr;
	string name = characterName;
	if (name == "�e�X�g") {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�n�[�g") {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�V�G�X�^") {
		character = new Siesta(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�q�G�����L�[") {
		character = new Hierarchy(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "���@���L���A") {
		character = new Valkyria(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�g���C") {
		character = new Troy(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�_�l��" || name == "�N���l��") {
		character = new SlashOnly(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�ΐl��" || name == "�t�F�[���[�X") {
		character = new BulletOnly(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "��C") {
		character = new ParabolaOnly(name.c_str(), hp, x, y, groupId);
	}
	else {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	return character;
}


CharacterInfo::CharacterInfo() :
	CharacterInfo("test")
{

}
// CSV�t�@�C������ǂݍ���
CharacterInfo::CharacterInfo(const char* characterName) {
	CsvReader reader("data/characterInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// �p�����[�^��ݒ�
	m_name = data["name"];
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
	m_sameBulletDirection = (bool)stoi(data["sameBulletDirection"]);

	// ���ʉ������[�h
	string filePath = "sound/stick/";
	string fileName;
	fileName = filePath + data["jumpSound"];
	m_jumpSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["passiveSound"];
	m_passiveSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["landSound"];
	m_landSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["runSound"];
}

CharacterInfo::~CharacterInfo() {
	// �T�E���h���폜
	DeleteSoundMem(m_jumpSound);
	DeleteSoundMem(m_passiveSound);
	DeleteSoundMem(m_landSound);
}


AttackInfo::AttackInfo():
	AttackInfo("test", 1.0)
{

}
// CSV�t�@�C������ǂݍ���
AttackInfo::AttackInfo(const char* characterName, double drawEx) {
	CsvReader reader("data/attackInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// �p�����[�^��ݒ�
	m_bulletHp = stoi(data["bulletHp"]);
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_slashHp = stoi(data["slashHp"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashInterval = stoi(data["slashInterval"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);

	// �U���̃G�t�F�N�g
	string path = "picture/effect/";
	m_bulletEffectHandles = new GraphHandles((path + data["bulletEffect"]).c_str(), 4, drawEx, 0.0, true);
	m_slashEffectHandles = new GraphHandles((path + data["slashEffect"]).c_str(), 4, drawEx, 0.0, true);
	// �T�E���h
	path = "sound/stick/";
	m_bulletSoundHandle = LoadSoundMem((path + data["bulletSound"]).c_str());
	m_slashSoundHandle = LoadSoundMem((path + data["slashSound"]).c_str());
	m_bulletStartSoundHandle = LoadSoundMem((path + data["bulletStartSound"]).c_str());
	m_slashStartSoundHandle = LoadSoundMem((path + data["slashStartSound"]).c_str());
}

AttackInfo::~AttackInfo() {
	// �摜���폜
	delete m_bulletEffectHandles;
	delete m_slashEffectHandles;
	// �T�E���h���폜
	DeleteSoundMem(m_bulletSoundHandle);
	DeleteSoundMem(m_slashSoundHandle);
	DeleteSoundMem(m_bulletStartSoundHandle);
	DeleteSoundMem(m_slashStartSoundHandle);
}


/*
* Character�N���X
*/
int Character::characterId;

Character::Character() :
	Character(100, 0, 0, 0)
{

}

Character::Character(int hp, int x, int y, int groupId) {
	// ID��U��
	characterId++;
	m_id = characterId;

	m_groupId = groupId;

	m_hp = hp;
	m_prevHp = m_hp;
	m_x = x;
	m_y = y;

	m_leftDirection = true;
	m_freeze = false;

	m_characterInfo = nullptr;
	m_attackInfo = nullptr;
	m_graphHandle = nullptr;
	m_faceHandle = nullptr;
	m_duplicationFlag = false;
}

Character::~Character() {
	// CharacterInfo�̍폜
	if (m_characterInfo != nullptr) {
		delete m_characterInfo;
	}
	// AttackInfo�̍폜
	if (m_attackInfo != nullptr && !m_duplicationFlag) {
		delete m_attackInfo;
	}
	// GraphHandle�̍폜
	if (m_graphHandle != nullptr) {
		delete m_graphHandle;
	}
	// FaceHandle�̍폜
	if (m_faceHandle != nullptr) {
		delete m_faceHandle;
	}
}

void Character::setParam(Character* character) {
	character->setY(m_y);
	character->setId(m_id);
	character->setLeftDirection(m_leftDirection);
	character->setHp(m_hp);
	character->setPrevHp(m_prevHp);
	character->getCharacterGraphHandle()->setGraph(getGraphHandle());
}

GraphHandle* Character::getGraphHandle() const {
	return m_graphHandle->getHandle();
}

void Character::getHandleSize(int& wide, int& height) const {
	// ���Z�b�g���Ă���摜�̏c���Ɖ������擾����B
	wide = getWide();
	height = getHeight();
}

int Character::getCenterX() const { 
	return m_x + (getWide() / 2);
}
int Character::getCenterY() const { 
	return m_y + (getHeight() / 2);
}
int Character::getWide() const {
	return m_graphHandle->getWide();
}
int Character::getHeight() const {
	return m_graphHandle->getHeight();
}

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->getHandle()->setReverseX(m_leftDirection);
}

// HP����
void Character::damageHp(int value) {
	m_hp = max(0, m_hp - value);
}

// �ړ�����i���W�𓮂����j
void Character::moveRight(int d) {
	m_x += d;
}
void Character::moveLeft(int d) {
	m_x -= d;
}
void Character::moveUp(int d) {
	m_y -= d;
}
void Character::moveDown(int d) {
	m_y += d;
}

// �����摜���Z�b�g
void Character::switchStand(int cnt) { m_graphHandle->switchStand(); }
// �����ˌ��摜���Z�b�g
void Character::switchBullet(int cnt) { m_graphHandle->switchBullet(); }
// �����a���摜���Z�b�g
void Character::switchSlash(int cnt) { m_graphHandle->switchSlash(); }
// ���Ⴊ�݉摜���Z�b�g
void Character::switchSquat(int cnt) { m_graphHandle->switchSquat(); }
// ���Ⴊ�݉摜���Z�b�g
void Character::switchSquatBullet(int cnt) { m_graphHandle->switchSquatBullet(); }
// ����摜���Z�b�g
void Character::switchRun(int cnt) { m_graphHandle->switchRun(); }
// ����ˌ��摜���Z�b�g
void Character::switchRunBullet(int cnt) { m_graphHandle->switchRunBullet(); }
// ���n�摜���Z�b�g
void Character::switchLand(int cnt) { m_graphHandle->switchLand(); }
// �㏸�摜���Z�b�g
void Character::switchJump(int cnt) { m_graphHandle->switchJump(); }
// �~���摜���Z�b�g
void Character::switchDown(int cnt) { m_graphHandle->switchDown(); }
// �W�����v�O�摜���Z�b�g
void Character::switchPreJump(int cnt) { m_graphHandle->switchPreJump(); }
// �_���[�W�摜���Z�b�g
void Character::switchDamage(int cnt) { m_graphHandle->switchDamage(); }
// �u�[�X�g�摜���Z�b�g
void Character::switchBoost(int cnt) { m_graphHandle->switchBoost(); }
// �󒆎ˌ��摜���Z�b�g
void Character::switchAirBullet(int cnt) { m_graphHandle->switchAirBullet(); }
// �󒆎a���摜���Z�b�g
void Character::switchAirSlash(int cnt) { m_graphHandle->switchAirSlash(); }


/*
* �n�[�g
*/
Heart::Heart(const char* name, int hp, int x, int y, int groupId) :
	Character(hp, x, y, groupId)
{
	// �L�����ŗL�̏��ݒ�
	m_characterInfo = new CharacterInfo(name);
	m_attackInfo = new AttackInfo(name, m_characterInfo->handleEx());

	m_hp = m_characterInfo->maxHp();
	m_prevHp = m_hp;

	// �e�摜�̃��[�h
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);

	m_bulletColor = WHITE;

	// �Ƃ肠���������摜�ŃX�^�[�g
	switchStand();
	m_y -= getHeight();
}

Heart::Heart(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Character(hp, x, y, groupId)
{
	m_duplicationFlag = true;
	m_attackInfo = attackInfo;
	m_characterInfo = new CharacterInfo(name);
	m_hp = m_characterInfo->maxHp();
	m_prevHp = m_hp;
	// �e�摜�̃��[�h
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);

	m_bulletColor = WHITE;

	// �Ƃ肠���������摜�ŃX�^�[�g
	//switchStand();
	//m_y -= getHeight();
}

// �f�X�g���N�^
Heart::~Heart() {

}

Character* Heart::createCopy() {
	Character* res = new Heart(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// ����摜���Z�b�g
void Heart::switchRun(int cnt) { 
	if (m_graphHandle->getRunHandle() == nullptr) { return; }
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunHandle()->getSize());
	m_graphHandle->switchRun(index);
}

// ����ˌ��摜���Z�b�g
void Heart::switchRunBullet(int cnt) {
	if (m_graphHandle->getRunBulletHandle() == nullptr) { 
		switchRun(cnt);
		return;
	}
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunBulletHandle()->getSize());
	m_graphHandle->switchRunBullet(index);
}

// �W�����v�O�摜���Z�b�g
void Heart::switchPreJump(int cnt) { 
	if (m_graphHandle->getPreJumpHandle() == nullptr) { return; }
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_graphHandle->getPreJumpHandle()->getSize());
	m_graphHandle->switchPreJump(index);
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), m_bulletColor, gx, gy, m_attackInfo);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	// �`�[���L���h�~
	attackObject->setGroupId(m_groupId);
	// ���ʉ�
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	// �U���͈͂�����
	int centerX = getCenterX();
	int height = m_attackInfo->slashLenY() / 2;
	int centerY = getCenterY();
	int x2 = centerX;
	if (leftDirection) { // �������ɍU��
		x2 -= m_attackInfo->slashLenX();
	}
	else { // �E�����ɍU��
		x2 += m_attackInfo->slashLenX();
	}

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// �U���̕���
	slashHandles->setReverseX(m_leftDirection);
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
		// ���ʉ�
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// ���Ŗh�~
		attackObject->setCharacterId(m_id);
		// �`�[���L���h�~
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* �V�G�X�^
*/
Siesta::Siesta(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Siesta::Siesta(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo):
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Siesta::createCopy() {
	Character* res = new Siesta(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Siesta::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	ParabolaBullet *attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandle(), gx, gy, m_attackInfo);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	// �`�[���L���h�~
	attackObject->setGroupId(m_groupId);
	// ���ʉ�
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Siesta::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	// �U���͈͂�����
	int centerX = getCenterX();
	int height = getHeight();
	int x1 = centerX;
	int x2 = centerX;
	if (leftDirection) { // �������ɍU��
		x1 += 50;
		x2 -= m_attackInfo->slashLenX();
	}
	else { // �E�����ɍU��
		x1 -= 50;
		x2 += m_attackInfo->slashLenX();
	}

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// �U���̕���
	slashHandles->setReverseX(m_leftDirection);
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
		// ���ʉ�
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// ���Ŗh�~
		attackObject->setCharacterId(m_id);
		// �`�[���L���h�~
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* �q�G�����L�[
*/
Hierarchy::Hierarchy(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Hierarchy::Hierarchy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Hierarchy::createCopy() {
	Character* res = new Hierarchy(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Hierarchy::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandle(), gx, gy, m_attackInfo);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	// �`�[���L���h�~
	attackObject->setGroupId(m_groupId);
	// ���ʉ�
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Hierarchy::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	return nullptr;
}


/*
* ���@���L���A
*/
Valkyria::Valkyria(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Valkyria::Valkyria(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Valkyria::createCopy() {
	Character* res = new Valkyria(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �W�����v�O�摜���Z�b�g
void Valkyria::switchPreJump(int cnt) {
	if (m_graphHandle->getPreJumpHandle() == nullptr) { return; }
	int index = 0;
	if (cnt >= 25) { index = 3; }
	else if (cnt >= 20) { index = 2; }
	else if (cnt >= 15) { index = 1; }
	m_graphHandle->switchPreJump(index);
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Valkyria::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	return nullptr;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Valkyria::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	// �U���͈͂�����
	int attackWide, attackHeight;
	GetGraphSize(m_graphHandle->getStandSlashHandle()->getHandle(0), &attackWide, &attackHeight);
	attackWide = (int)(attackWide * m_graphHandle->getStandSlashHandle()->getGraphHandle()->getEx());
	attackHeight = (int)(attackHeight * m_graphHandle->getStandSlashHandle()->getGraphHandle()->getEx());
	int x1 = m_x;
	int x2 = m_x + attackWide;

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// �U���̕���
	slashHandles->setReverseX(m_leftDirection);
	// �L�����̐g��
	int height = attackHeight;
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum() - 1) {
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - 12, m_attackInfo);
		// ���ʉ�
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - slashCountSum - 6, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - 2 * slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// ���Ŗh�~
		attackObject->setCharacterId(m_id);
		// �`�[���L���h�~
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* �g���C
*/
Troy::Troy(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{
	m_bulletColor = BLUE;
}
Troy::Troy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{
	m_bulletColor = BLUE;
}

Character* Troy::createCopy() {
	Character* res = new Troy(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Troy::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	return nullptr;
}


/*
* ���ʂ̎ˌ��݂̂�����L����
*/
BulletOnly::BulletOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
BulletOnly::BulletOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* BulletOnly::createCopy() {
	Character* res = new BulletOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

/*
* ���ʂ̎a���݂̂�����L����
*/
SlashOnly::SlashOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
SlashOnly::SlashOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* SlashOnly::createCopy() {
	Character* res = new SlashOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}


/*
* ParabolaBullet�݂̂����L����
*/
ParabolaOnly::ParabolaOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
ParabolaOnly::ParabolaOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* ParabolaOnly::createCopy() {
	Character* res = new ParabolaOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* ParabolaOnly::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	ParabolaBullet* attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_bulletColor, gx, gy, m_attackInfo);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	// �`�[���L���h�~
	attackObject->setGroupId(m_groupId);
	// ���ʉ�
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}