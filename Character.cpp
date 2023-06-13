#include "Character.h"
#include "Object.h"
#include "Define.h"
#include "DxLib.h"


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

	m_attackInfo = new AttackInfo();

	m_leftDirection = true;
}

Character::~Character() {
	delete m_attackInfo;
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
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy) {

	return NULL;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(int cnt, int gx, int gy) {

	return NULL;
}
