#include "Character.h"
#include "Object.h"
#include "Define.h"
#include "DxLib.h"


AttackInfo::AttackInfo():
	AttackInfo(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
{

}
// CSV�t�@�C������ǂݍ��ޗ\��
AttackInfo::AttackInfo(const char* characterName) {

}
AttackInfo::AttackInfo(int bulletDamage, int bulletRx, int bulletRy, int bulletSpeed, int bulletRapid, int bulletDistance, int slashDamage, int slashRx, int slashRy, int slashCountSum) {
	m_bulletDamage = bulletDamage;
	m_bulletRx = bulletRx;
	m_bulletRy = bulletRy;
	m_bulletSpeed = bulletSpeed;
	m_bulletRapid = bulletRapid;
	m_bulletDistance = bulletDistance;
	m_slashDamage = slashDamage;
	m_slashRx = slashRx;
	m_slashRy = slashRy;
	m_slashCountSum = slashCountSum;
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

	m_name = "������";
	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_handle = -1;
	m_ex = 1.0;

	m_moveSpeed = 1;
	m_jumpHeight = 1;

	m_attackInfo = NULL;

	m_leftDirection = true;
}

Character::~Character() {
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
}

void Character::setHandle(int handle) {

	m_handle = handle;

	// �摜�̏c���Ɖ������擾����B
	GetGraphSize(m_handle, &m_wide, &m_height);

	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
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
	// ���O
	m_name = NAME;

	// �摜�̊g�嗦
	m_ex = DRAW_EX;

	// ���鑬��
	m_moveSpeed = MOVE_SPEED;

	// �W�����v�̋���
	m_jumpHeight = JUMP_HEIGHT;

	// �e�摜�̃��[�h
	m_standHandle = LoadGraph("picture/stick/stand.png");

	m_slashHandleSum = 3;
	m_slashHandles = new int[m_slashHandleSum];

	m_attackInfo = new AttackInfo(0, 100, 100, 50, 30, 800, 0, 30, 30, 30);
}

// �f�X�g���N�^
Heart::~Heart() {
	DeleteGraph(m_standHandle);
	for (int i = 0; i < m_slashHandleSum; i++) {
		DeleteGraph(m_slashHandles[i]);
	}
	delete m_slashHandles;
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x, m_y,
		m_x + m_attackInfo->m_bulletRx, m_y + m_attackInfo->m_bulletRy,
		WHITE, gx, gy,
		m_attackInfo->m_bulletDamage, m_attackInfo->m_bulletSpeed, m_attackInfo->m_bulletDistance);
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(bool leftDirection) {
	int x2 = m_x;
	int y2 = m_y;
	if (leftDirection) { // �������ɍU��
		x2 -= m_attackInfo->m_slashRx;
		y2 -= m_attackInfo->m_slashRy;
	}
	else { // �E�����ɍU��
		x2 += m_attackInfo->m_slashRx;
		y2 += m_attackInfo->m_slashRy;
	}
	SlashObject* attackObject = new SlashObject(m_x, m_y, 
		m_x + m_attackInfo->m_slashRx, m_y + m_attackInfo->m_slashRy,
		m_slashHandles, m_slashHandleSum, 
		m_attackInfo->m_slashDamage, m_attackInfo->m_slashCountSum);
	return attackObject;
}
