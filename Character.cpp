#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "Sound.h"
#include "Game.h"
#include "DxLib.h"
#include <cstdlib>
#include <sstream>


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
	else if (name == "���@���L���A" || name == "�t�����`") {
		character = new Valkyria(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�g���C") {
		character = new Troy(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "�R�n��") {
		character = new Koharu(name.c_str(), hp, x, y, groupId);
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
	else if (name == "�T��") {
		character = new Sun(name.c_str(), hp, x, y, groupId);
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
	setParam(data);
	m_name = data["name"];

	// ���ʉ������[�h
	string filePath = "sound/stick/";
	string fileName;
	fileName = filePath + data["jumpSound"];
	if (fileName != "null") {
		m_jumpSound = LoadSoundMem(fileName.c_str());
	}
	fileName = filePath + data["passiveSound"];
	if (fileName != "null") {
		m_passiveSound = LoadSoundMem(fileName.c_str());
	}
	fileName = filePath + data["landSound"];
	if (fileName != "null") {
		m_landSound = LoadSoundMem(fileName.c_str());
	}
}

CharacterInfo::~CharacterInfo() {
	// �T�E���h���폜
	DeleteSoundMem(m_jumpSound);
	DeleteSoundMem(m_passiveSound);
	DeleteSoundMem(m_landSound);
}

// �o�[�W�����ύX
void CharacterInfo::changeVersion(int version) {
	ostringstream oss;
	oss << "v" << version << ":" << m_name;

	CsvReader reader("data/characterInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", oss.str().c_str());
	if (data.size() == 0) { data = reader.findOne("name", m_name.c_str()); }
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// �p�����[�^��ݒ�
	setParam(data);
}

void CharacterInfo::setParam(map<string, string>& data) {
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
	m_sameBulletDirection = (bool)stoi(data["sameBulletDirection"]);
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
	setParam(data);

	// �U���̃G�t�F�N�g
	string path = "picture/effect/";
	m_bulletEffectHandles = new GraphHandles((path + data["bulletEffect"]).c_str(), stoi(data["bulletEffectSum"]), drawEx, 0.0, true);
	m_slashEffectHandles = new GraphHandles((path + data["slashEffect"]).c_str(), stoi(data["slashEffectSum"]), drawEx, 0.0, true);
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

// �o�[�W�����ύX
void AttackInfo::changeVersion(const char* characterName, int version) {
	ostringstream oss;
	oss << "v" << version << ":" << characterName;

	CsvReader reader("data/attackInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", oss.str().c_str());
	if (data.size() == 0) { data = reader.findOne("name", characterName); }
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// �p�����[�^��ݒ�
	setParam(data);
}

void AttackInfo::setParam(map<string, string>& data) {
	m_bulletHp = stoi(data["bulletHp"]);
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_bulletBomb = (bool)stoi(data["bulletBomb"]);
	m_slashHp = stoi(data["slashHp"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashInterval = stoi(data["slashInterval"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);
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

	m_version = 1;
	m_hp = hp;
	m_prevHp = m_hp;
	m_dispHpCnt = 0;
	m_skillGage = SKILL_MAX;
	m_invincible = false;
	m_x = x;
	m_y = y;

	m_leftDirection = true;
	m_freeze = false;
	m_bossFlag = false;

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
	character->changeInfoVersion(m_version);
	character->setY(m_y);
	character->setId(m_id);
	character->setLeftDirection(m_leftDirection);
	character->setHp(m_hp);
	character->setPrevHp(m_prevHp);
	character->setSkillGage(m_skillGage);
	character->setInvincible(m_invincible);
	character->setBossFlag(m_bossFlag);
	character->getCharacterGraphHandle()->setGraph(m_graphHandle->getDispGraphHandle(), m_graphHandle->getDispGraphIndex());
}

GraphHandle* Character::getGraphHandle() const {
	return m_graphHandle->getHandle();
}

void Character::getHandleSize(int& wide, int& height) const {
	// ���Z�b�g���Ă���摜�̏c���Ɖ������擾����B
	wide = getWide();
	height = getHeight();
}

// �����蔻��͈̔͂��擾
void Character::getAtariArea(int* x1, int* y1, int* x2, int* y2) const {
	m_graphHandle->getAtari(x1, y1, x2, y2);
	if (m_leftDirection) {
		int wide = getWide();
		*x1 = wide - *x1;
		*x2 = wide - *x2;
		int tmp = *x1;
		*x1 = *x2;
		*x2 = tmp;
	}
	*x1 = *x1 + m_x;
	*y1 = *y1 + m_y;
	*x2 = *x2 + m_x;
	*y2 = *y2 + m_y;
}

// �����蔻��͈̔͂��擾
void Character::getDamageArea(int* x1, int* y1, int* x2, int* y2) const {
	m_graphHandle->getDamage(x1, y1, x2, y2);
	if (m_leftDirection) {
		int wide = getWide();
		*x1 = wide - *x1;
		*x2 = wide - *x2;
		int tmp = *x1;
		*x1 = *x2;
		*x2 = tmp;
	}
	*x1 = *x1 + m_x;
	*y1 = *y1 + m_y;
	*x2 = *x2 + m_x;
	*y2 = *y2 + m_y;
}

// Info�̃o�[�W������ύX����
void Character::changeInfoVersion(int version) {
	m_version = version;
	m_characterInfo->changeVersion(version);
	m_attackInfo->changeVersion(m_characterInfo->name().c_str(), version);
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
int Character::getAtariCenterX() const {
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getAtariArea(&x1, &y1, &x2, &y2);
	return (x1 + x2) / 2;
}
int Character::getAtariCenterY() const {
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getAtariArea(&x1, &y1, &x2, &y2);
	return (y1 + y2) / 2;
}

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->getHandle()->setReverseX(m_leftDirection);
}

// HP����
void Character::damageHp(int value) {

	// ���G
	if (m_invincible) { return; }
	
	// ���G����Ȃ��Ȃ�HP����
	m_hp = max(0, m_hp - value);
	m_dispHpCnt = 60;
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

bool Character::haveDeadGraph() const {
	return !(m_graphHandle->getDeadHandle() == nullptr);
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
// ����摜���Z�b�g
void Character::switchDead(int cnt) { m_graphHandle->switchDead(); }
// �{�X�̏����A�j���[�V�������Z�b�g
void Character::switchInit(int cnt) { m_graphHandle->switchInit(); }
// �ǉ��摜���Z�b�g
void Character::switchSpecial1(int cnt) { m_graphHandle->switchSpecial1(); }


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
	// �摜�̔��]
	setLeftDirection(m_leftDirection);

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
	// �摜�̔��]
	setLeftDirection(m_leftDirection);

	m_bulletColor = WHITE;

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
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchRun(index);
}

// ����ˌ��摜���Z�b�g
void Heart::switchRunBullet(int cnt) {
	if (m_graphHandle->getRunBulletHandle() == nullptr) { 
		switchRun(cnt);
		return;
	}
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunBulletHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchRunBullet(index);
}

// �W�����v�O�摜���Z�b�g
void Heart::switchPreJump(int cnt) { 
	if (m_graphHandle->getPreJumpHandle() == nullptr) { return; }
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_graphHandle->getPreJumpHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchPreJump(index);
}

// �����a���摜���Z�b�g
void Heart::switchSlash(int cnt) {
	if (m_graphHandle->getStandSlashHandle() == nullptr) { return; }
	int index = (getSlashCountSum() + getSlashInterval() - cnt) / 3;
	if (cnt > 6) {
		index = min(m_graphHandle->getStandSlashHandle()->getGraphHandles()->getSize() - 2, index);
	}
	m_graphHandle->switchSlash(index);
}

// �����ˌ��摜���Z�b�g
void Heart::switchBullet(int cnt) {
	if (m_graphHandle->getStandBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getStandBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchBullet(index);
}

// �󒆎a���摜���Z�b�g
void Heart::switchAirSlash(int cnt) {
	if (m_graphHandle->getAirSlashHandle() == nullptr) { return; }
	int index = (getSlashCountSum() + getSlashInterval() - cnt) / 3;
	if (cnt > 6) {
		index = min(m_graphHandle->getAirSlashHandle()->getGraphHandles()->getSize() - 2, index);
	}
	m_graphHandle->switchAirSlash(index);
}

// �󒆎ˌ��摜���Z�b�g
void Heart::switchAirBullet(int cnt) {
	if (m_graphHandle->getAirBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getAirBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchAirBullet(index);
}

// ���Ⴊ�ݎˌ��摜���Z�b�g
void Heart::switchSquatBullet(int cnt) {
	if (m_graphHandle->getSquatBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getSquatBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchSquatBullet(index);
}

// �ˌ��U��������
Object* Heart::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	// �e�̍쐬
	BulletObject* attackObject;
	if (m_graphHandle->getBulletHandle() != nullptr) {
		attackObject = new BulletObject(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	}
	else {
		attackObject = new BulletObject(getCenterX(), getCenterY(), m_bulletColor, gx, gy, m_attackInfo);
	}
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

// �a���U��������
Object* Heart::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
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
	GraphHandlesWithAtari* slashHandles = m_graphHandle->getAirSlashEffectHandle();
	if (grand || slashHandles == nullptr) {
		// �n��ɂ���A�������͋󒆎a���摜���Ȃ��Ȃ�n��p�̉摜���g��
		slashHandles = m_graphHandle->getSlashHandle();
	}
	// �U���̕���
	slashHandles->getGraphHandles()->setReverseX(m_leftDirection);
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
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
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
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

// �ˌ��U��������
Object* Siesta::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid() / 2) { return nullptr; }
	ParabolaBullet *attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
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

// �a���U��������
Object* Siesta::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	// �U���͈͂�����
	int centerX = getCenterX();
	int height = getHeight();
	int x1 = centerX;
	int x2 = x1;
	if (leftDirection) { // �������ɍU��
		x1 += 100;
		x2 = x1 - m_attackInfo->slashLenX();
	}
	else { // �E�����ɍU��
		x1 -= 100;
		x2 = x1 + m_attackInfo->slashLenX();
	}

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle()->getGraphHandles();
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

// �ˌ��U��������
Object* Hierarchy::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	//gx = GetRand(600) - 300 + getCenterX();
	//gy = getCenterY() - GetRand(300);
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
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

// �a���U��������
Object* Hierarchy::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
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

// �a���U��������
Object* Valkyria::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	// �U���͈͂�����
	int attackWide, attackHeight;
	GetGraphSize(m_graphHandle->getStandSlashHandle()->getGraphHandles()->getHandle(0), &attackWide, &attackHeight);
	attackWide = (int)(attackWide * m_graphHandle->getStandSlashHandle()->getGraphHandles()->getGraphHandle()->getEx());
	attackHeight = (int)(attackHeight * m_graphHandle->getStandSlashHandle()->getGraphHandles()->getGraphHandle()->getEx());
	int x1 = m_x;
	int x2 = m_x + attackWide;

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle()->getGraphHandles();
	// �U���̕���
	slashHandles->setReverseX(m_leftDirection);
	// �L�����̐g��
	int height = attackHeight;
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum() - 1) {
		index = 0 % slashHandles->getSize();
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
		index = 1 % slashHandles->getSize();
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - slashCountSum - 6, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2 % slashHandles->getSize();
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

// �a���U��������
Object* Troy::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	return nullptr;
}


/*
* �R�n��
*/
Koharu::Koharu(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Koharu::Koharu(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Koharu::createCopy() {
	Character* res = new Koharu(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �ˌ��U��������
Object* Koharu::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	// �o�Y�[�J�̏e������o��悤�Ɍ�����
	gy = getY() + getHeight() - 160;
	BulletObject* attackObject = new BulletObject(getCenterX(), gy, m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
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
	m_bulletColor = GRAY;
}
ParabolaOnly::ParabolaOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{
	m_bulletColor = GRAY;
}

Character* ParabolaOnly::createCopy() {
	Character* res = new ParabolaOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �ˌ��U��������
Object* ParabolaOnly::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
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


/*
* Boss1: �T��
*/
Sun::Sun(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Sun::Sun(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Sun::createCopy() {
	Character* res = new Sun(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// �{�X�̏����A�j���[�V�������Z�b�g
void Sun::switchInit(int cnt) { 
	if (m_graphHandle->getInitHandle() == nullptr) { return; }
	if (cnt < 0) {
		m_graphHandle->switchSpecial1();
		return;
	}
	int index = min(cnt / RUN_ANIME_SPEED, m_graphHandle->getInitHandle()->getGraphHandles()->getSize() - 1);
	m_graphHandle->switchInit(index);
}

Object* Sun::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	int x = getCenterX() + GetRand(400) - 200;
	int y = getCenterY() + GetRand(400) - 200;
	ParabolaBullet* attackObject = new ParabolaBullet(x, y, m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
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