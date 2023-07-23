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
	AttackInfo("test")
{

}
// CSV�t�@�C������ǂݍ���
AttackInfo::AttackInfo(const char* characterName) {
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
}


int Character::characterId;

Character::Character() :
	Character(100, 100, 0, 0)
{

}

Character::Character(int maxHp, int hp, int x, int y) {
	// ID��U��
	characterId++;
	m_id = characterId;

	m_groupId = 0;

	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_graphHandle = NULL;

	m_characterInfo = NULL;
	m_attackInfo = NULL;

	m_leftDirection = true;
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
}

void Character::setHandle(GraphHandle* handle) {

	m_graphHandle = handle;

	// �摜�̏c���Ɖ������擾����B
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);

	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_characterInfo->handleEx());
	m_height = (int)(m_height * m_characterInfo->handleEx());
}

void Character::getHandleSize(int& wide, int& height) {
	// ���Z�b�g���Ă���摜�̏c���Ɖ������擾����B
	wide = m_wide;
	height = m_height;
}

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->setReverseX(m_leftDirection);
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


/*
* �n�[�g
*/
Heart::Heart(int maxHp, int hp, int x, int y) :
	Character(maxHp, hp, x, y)
{
	// �L�����ŗL�̏��ݒ�
	m_characterInfo = new CharacterInfo(NAME);
	m_attackInfo = new AttackInfo(NAME);

	// �e�摜�̃��[�h
	double ex = m_characterInfo->handleEx();
	m_standHandle = new GraphHandle("picture/stick/stand.png", ex);
	m_slashHandles = new GraphHandles("picture/stick/slashEffect", 3, ex, 0.0, true);
	m_squatHandle = new GraphHandle("picture/stick/squat.png", ex);
	m_standBulletHandle = new GraphHandle("picture/stick/bullet.png", ex);
	m_standSlashHandle = new GraphHandle("picture/stick/slash.png", ex);
	m_runHandles = new GraphHandles("picture/stick/run", 6, ex);
	m_landHandle = new GraphHandle("picture/stick/land.png", ex);
	m_jumpHandle = new GraphHandle("picture/stick/jump.png", ex);
	m_downHandle = new GraphHandle("picture/stick/down.png", ex);
	m_preJumpHandles = new GraphHandles("picture/stick/preJump", 2, ex);
	m_damageHandle = new GraphHandle("picture/stick/damage.png", ex);
	m_boostHandle = new GraphHandle("picture/stick/boost.png", ex);
	m_airBulletHandle = new GraphHandle("picture/stick/airBullet.png", ex);
	m_airSlashHandle = new GraphHandle("picture/stick/airSlash.png", ex);
	// �U���̃G�t�F�N�g
	m_bulletEffectHandles = new GraphHandles("picture/effect/�I�����W", 4, ex, 0.0, true);
	m_slashEffectHandles = m_bulletEffectHandles;

	// �Ƃ肠���������摜�ŃX�^�[�g
	switchStand();
}

// �f�X�g���N�^
Heart::~Heart() {
	// �摜���폜
	delete m_standHandle;
	delete m_slashHandles;
	delete m_squatHandle;
	delete m_standBulletHandle;
	delete m_standSlashHandle;
	delete m_runHandles;
	delete m_landHandle;
	delete m_jumpHandle;
	delete m_downHandle;
	delete m_preJumpHandles;
	delete m_damageHandle;
	delete m_boostHandle;
	delete m_airBulletHandle;
	delete m_airSlashHandle;
	delete m_bulletEffectHandles;
}

// ����摜���Z�b�g
void Heart::switchRun(int cnt) { 
	int index = (cnt / RUN_ANIME_SPEED) % (m_runHandles->getSize());
	setHandle(m_runHandles->getGraphHandle(index));
}
// �W�����v�O�摜���Z�b�g
void Heart::switchPreJump(int cnt) { 
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_preJumpHandles->getSize());
	setHandle(m_preJumpHandles->getGraphHandle(index));
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x + m_wide / 2, m_y + m_height / 2, WHITE, gx, gy, m_attackInfo, m_bulletEffectHandles);
	// ���Ŗh�~
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(bool leftDirection, int cnt) {
	// �U���͈͂�����
	int centerX = m_x + m_wide / 2;
	int height = m_attackInfo->slashLenY() / 2;
	int centerY = m_y + m_height / 2;
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
	m_slashHandles->setReverseX(m_leftDirection);
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	// ���Ŗh�~
	if (attackObject != NULL) {
		attackObject->setCharacterId(m_id);
	}
	return attackObject;
}
