#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <cstdlib>


using namespace std;


CharacterInfo::CharacterInfo() :
	CharacterInfo("test")
{

}
// CSV�t�@�C������ǂݍ���
CharacterInfo::CharacterInfo(const char* characterName) {
	CsvReader reader("data/characterInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);

	// �p�����[�^��ݒ�
	m_name = data["name"];
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
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

	// �p�����[�^��ݒ�
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);

	// �U���̃G�t�F�N�g
	m_bulletEffectHandles = new GraphHandles("picture/effect/�I�����W", 4, drawEx, 0.0, true);
	m_slashEffectHandles = m_bulletEffectHandles;
	// �T�E���h
	m_bulletSoundHandle = LoadSoundMem("sound/stick/bullet.wav");
	m_slashSoundHandle = LoadSoundMem("sound/stick/slash.wav");
}

AttackInfo::~AttackInfo() {
	// �摜���폜
	delete m_bulletEffectHandles;
	delete m_slashEffectHandles;
	// �T�E���h���폜
	DeleteSoundMem(m_bulletSoundHandle);
	DeleteSoundMem(m_slashSoundHandle);
}


/*
* Character�N���X
*/
int Character::characterId;

Character::Character() :
	Character(100, 100, 0, 0, 0)
{

}

Character::Character(int maxHp, int hp, int x, int y, int groupId) {
	// ID��U��
	characterId++;
	m_id = characterId;

	m_groupId = groupId;
	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_leftDirection = true;

	m_characterInfo = NULL;
	m_attackInfo = NULL;
	m_graphHandle = NULL;
}

Character::~Character() {
	// CharacterInfo�̍폜
	if (m_characterInfo != NULL) {
		delete m_characterInfo;
	}
	// AttackInfo�̍폜
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
	// GraphHandle�̍폜
	if (m_graphHandle != NULL) {
		delete m_graphHandle;
	}
}

const GraphHandle* Character::getGraphHandle() const {
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
// ����摜���Z�b�g
void Character::switchRun(int cnt) { m_graphHandle->switchRun(); }
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
Heart::Heart(int maxHp, int hp, int x, int y, int groupId) :
	Character(maxHp, hp, x, y, groupId)
{
	// �L�����ŗL�̏��ݒ�
	m_characterInfo = new CharacterInfo(NAME);
	m_attackInfo = new AttackInfo(NAME, m_characterInfo->handleEx());

	// �e�摜�̃��[�h
	m_graphHandle = new CharacterGraphHandle(NAME, m_characterInfo->handleEx());

	// �Ƃ肠���������摜�ŃX�^�[�g
	switchStand();
}

// �f�X�g���N�^
Heart::~Heart() {

}

// ����摜���Z�b�g
void Heart::switchRun(int cnt) { 
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunHandle()->getSize());
	m_graphHandle->switchRun(index);
}
// �W�����v�O�摜���Z�b�g
void Heart::switchPreJump(int cnt) { 
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_graphHandle->getPreJumpHandle()->getSize());
	m_graphHandle->switchPreJump(index);
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), WHITE, gx, gy, m_attackInfo);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	// �`�[���L���h�~
	attackObject->setGroupId(m_groupId);
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(bool leftDirection, int cnt) {
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
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = NULL;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// �U���̕���
	slashHandles->setReverseX(m_leftDirection);
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
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
	if (attackObject != NULL) {
		// ���Ŗh�~
		attackObject->setCharacterId(m_id);
		// �`�[���L���h�~
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}
