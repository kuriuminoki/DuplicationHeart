#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "GraphHandle.h"
#include "Animation.h"
#include "DxLib.h"
#include <algorithm>
#include <cmath>


Object::Object() :
	Object(0, 0, 0, 0)
{

}

Object::Object(int x1, int y1, int x2, int y2) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	// �召�֌W�� 1 <= 2
	if (m_x1 > m_x2) { std::swap(m_x1, m_x2); }
	if (m_y1 > m_y2) { std::swap(m_y1, m_y2); }

	m_deleteFlag = false;
	m_ableDelete = false;

	m_effectHandles = NULL;
}

// �A�j���[�V�����쐬
Animation* Object::createAnimation() {
	if (m_effectHandles == NULL) {
		return NULL;
	}
	return new Animation((m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, 3, m_effectHandles);
}


/*
* �l�p�`�̃I�u�W�F�N�g
*/
BoxObject::BoxObject(int x1, int y1, int x2, int y2, int color) :
	Object(x1, y1, x2, y2)
{
	m_color = color;
}

// �L�����N�^�[�Ƃ̓����蔻��
bool BoxObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		if (characterY2 <= m_y1 && characterY2 + characterVy >= m_y1) {
			// ���n
			characterController->setCharacterGrand(true);
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y1 - characterHeight);
		}
		// ��Ɉړ����̃L�����������瓖�����Ă��邩����
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// �L�����͏�ֈړ��ł��Ȃ�
			characterController->setActionUpLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y2);
		}
	}

	// �L���������E�ړ��œ������Ă��邩����
	if (characterY2 + characterVy > m_y1 && characterY1 + characterVy < m_y2) {
		// �E�Ɉړ����̃L�����������瓖�����Ă��邩����
		if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
			// �i���Ƃ݂Ȃ��ď��z����
			if (characterY2 - STAIR_HEIGHT <= m_y1 && characterX2 == m_x1) {
				// �K�؂ȍ��W��
				characterController->setCharacterX(characterX1 + characterWide / 2);
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
		// ���Ɉړ����̃L�������E���瓖�����Ă��邩����
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			// �i���Ƃ݂Ȃ��ď��z����
			if (characterY2 - STAIR_HEIGHT <= m_y1 && characterX1 == m_x2) {
				// �K�؂ȍ��W��
				characterController->setCharacterX(characterX1 - characterWide / 2);
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
	}
	return false;
}

// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
void BoxObject::penetration(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	// ������I�u�W�F�N�g�̒��ɓ��荞��ł��܂�����
	bool slope = characterController->getAction()->getGrandLeftSlope() || characterController->getAction()->getGrandRightSlope();
	if (!slope && characterY2 > m_y1 && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		// �L���������ɂ͂ݏo���Ă���Ȃ�
		if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x1 - characterWide);
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
			}
			else {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x2);
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
			}
		}
		else if (characterY1 < m_y1 || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (m_y1 + m_y2)) {
				// �^���
				characterController->setCharacterY(m_y1 - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �^����
				characterController->setCharacterY(m_y2);
				// �L�����͏�ֈړ��ł��Ȃ�
				characterController->setActionUpLock(true);
			}
		}
	}
}

// ���I�u�W�F�N�g�Ƃ̓����蔻��
void BoxObject::atariObject(Object* object) {
	// �j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete()) { return; }
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->setDeleteFlag(true);
	}
}

void BoxObject::action() {

}

TriangleObject::TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown):
	Object(x1, y1, x2, y2)
{
	m_color = color;
	m_leftDown = leftDown;
}

// ���WX�ɂ�����Y���W�i�X������Z�o����j
int TriangleObject::getY(int x) const {
	if (x < m_x1) {
		if (m_leftDown) {
			return m_y2;
		}
		else {
			return m_y1;
		}
	}
	if (x > m_x2) {
		if (m_leftDown) {
			return m_y1;
		}
		else {
			return m_y2;
		}
	}
	// �X��
	// ���q
	double a1 = (double)(m_y1 - m_y2);
	if (!m_leftDown) {
		a1 *= -1;
	}
	// ����
	double a2 = (double)(m_x2 - m_x1);

	// �ؕ�
	double b;
	if (m_leftDown) {
		b = m_y1 - (a1 * m_x2 / a2);
	}
	else {
		b = m_y1 - (a1 * m_x1 / a2);
	}

	// ���W
	double y = a1 * x / a2 + b;

	return (int)y;
}

// �L�����N�^�[�Ƃ̓����蔻��
bool TriangleObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ����Ă���Ƃ��͂��̏�������true�ɂȂ�
		if (characterY2 == getY(characterX1_5 - characterVx)) {
			// �O�̃t���[���ł͒��n���Ă����̂ŁA�����������n
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
		}
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		else if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// ���n
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
		}
		// ��Ɉړ����̃L�����������瓖�����Ă��邩����
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// �L�����͏�ֈړ��ł��Ȃ�
			characterController->setActionUpLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y2);
		}
	}

	// ����ړ�
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 <= m_y2 && characterY2 >= getY(characterX1_5)) {
		// ���n
		characterController->setCharacterGrand(true);
		if (m_leftDown) {
			characterController->setCharacterGrandRightSlope(true);
		}
		else {
			characterController->setCharacterGrandLeftSlope(true);
		}
		// �L�����͉��ֈړ��ł��Ȃ�
		characterController->setActionDownLock(true);
		// �K�؂ȍ����ֈړ�
		characterController->setCharacterY(getY(characterX1_5) - characterHeight);
	}

	// ��̉s�p�i��[�j�̓����蔻��
	if (m_y2 > characterY1 && m_y2 < characterY2) {
		if (m_leftDown) {// ����������
			// �E�Ɉړ����̃L�������E���瓖�����Ă��邩����
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
		else {
			// ���Ɉړ����̃L�����������瓖�����Ă��邩����
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
	}

	// �L���������E�ړ��œ������Ă��邩����
	if (characterY2 > m_y1 && characterY1 < m_y2) {
		if (m_leftDown) {// ����������
			// ���Ɉړ����̃L�������E���瓖�����Ă��邩����
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x2);
			}
		}
		else {
			// �E�Ɉړ����̃L�����������瓖�����Ă��邩����
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
	}
	return false;
}

// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
void TriangleObject::penetration(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int slopeY = getY(characterX1_5);
	// ������I�u�W�F�N�g�̒��ɓ��荞��ł��܂�����
	if (characterY2 > slopeY && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		if (characterY1 < slopeY || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (slopeY + m_y2)) {
				// �^���
				characterController->setCharacterY(slopeY - characterHeight);
				// ���n
				characterController->setCharacterGrand(true);
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �^����
				characterController->setCharacterY(m_y2);
				// �L�����͏�ֈړ��ł��Ȃ�
				characterController->setActionUpLock(true);
			}
		}
		// �L���������ɂ͂ݏo���Ă���Ȃ�
		else if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x1 - characterWide);
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
			}
			else {
				// ������Ԃ܂ňړ�������
				characterController->setCharacterX(m_x2);
				// �L�����͍��ֈړ��ł��Ȃ�
				characterController->setActionLeftLock(true);
			}
		}
	}
}

// ���I�u�W�F�N�g�Ƃ̓����蔻��
void TriangleObject::atariObject(Object* object) {
	// �j��s�\�I�u�W�F�N�g
	if (!object->getAbleDelete()) { return; }
	// �Εӂ��l�����ē����蔻����v�Z
	int y = object->getY1();
	if (m_leftDown) {
		y = getY(object->getX2());
	}
	else {
		y = getY(object->getX1());
	}
	// �������Ă���Ȃ�
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && y < object->getY2()) {
		object->setDeleteFlag(true);
	}
}

void TriangleObject::action() {

}


BulletObject::BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo, GraphHandles* effectHandles) :
	Object(x - attackInfo->bulletRx(), y - attackInfo->bulletRx(), x + attackInfo->bulletRx(), y + attackInfo->bulletRy())
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;
	m_groupId = -1;

	// �e�p�����[�^�̐ݒ�
	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = attackInfo->bulletRx();
	m_ry = attackInfo->bulletRy();
	m_damage = attackInfo->bulletDamage();
	m_d = attackInfo->bulletDistance();
	m_ableDelete = true;

	// �p�x���v�Z���AVX��VY������
	int dx = gx - x;
	int dy = gy - y;
	double r = std::atan2((double)dy, (double)dx);
	m_v = attackInfo->bulletSpeed();
	m_vx = (int)(m_v * std::cos(r));
	m_vy = (int)(m_v * std::sin(r));

	// �G�t�F�N�g�̉摜
	m_effectHandles = effectHandles;
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
bool BulletObject::atari(CharacterController* characterController) {
	// ���Ŗh�~
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}

	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// �ђʒe����Ȃ��Ȃ����
		m_deleteFlag = true;
		characterController->damage(m_vx / 2, m_vy / 2, m_damage, -1);
		return true;
	}
	return false;
}

// ���I�u�W�F�N�g�Ƃ̓����蔻��
void BulletObject::atariObject(Object* object) {

}

void BulletObject::action() {
	m_x1 += m_vx;
	m_x2 += m_vx;
	m_y1 += m_vy;
	m_y2 += m_vy;
	m_d -= m_v;
	// �򋗗������������
	if (m_d <= 0) {
		m_deleteFlag = true;
	}
}


SlashObject::SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo, GraphHandles* effectHandles) :
	Object(x1, y1, x2, y2)
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;

	// �摜
	m_handle = handle;
	
	// �_���[�W
	m_damage = attackInfo->slashDamage();

	// �S�̃t���[��
	m_slashCountSum = slashCountSum;

	// �J�E���g
	m_cnt = 0;

	// �������(X�����̏���)
	m_slashImpactX = attackInfo->slashImpactX();

	// �������(Y�����̏���)
	m_slashImpactY = attackInfo->slashImpactY();


	// �G�t�F�N�g�̉摜
	m_effectHandles = effectHandles;
}

// �傫�����w�肵�Ȃ��ꍇ�B�摜����T�C�Y�擾�B�������ԁAAttackInfo
SlashObject::SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo, GraphHandles* effectHandles) {
	int x2 = 0;
	int y2 = 0;
	GetGraphSize(handle->getHandle(), &x2, &y2);
	x2 += x;
	y2 = y;
	SlashObject(x, y, x2, y2, handle, slashCountSum, attackInfo, effectHandles);
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
bool SlashObject::atari(CharacterController* characterController) {
	// ���Ŗh�~
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}

	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// �ђʒe����Ȃ��Ȃ����
		 // m_deleteFlag = true;
		if (characterX1 + characterX2 < m_x1 + m_x2) {
			characterController->damage(-m_slashImpactX, -m_slashImpactY, m_damage, -1);
		}
		else {
			characterController->damage(m_slashImpactX, -m_slashImpactY, m_damage, -1);
		}
		return true;
	}
	return false;
}

// ���I�u�W�F�N�g�Ƃ̓����蔻��
void SlashObject::atariObject(Object* object) {

}

void SlashObject::action() {
	// ���Ԍo��
	m_cnt++;

	// ���Ԃ������̂ŏ���
	if (m_cnt == m_slashCountSum) {
		m_deleteFlag = true;
	}
}


// �`��p
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void BoxObject::drawObject(int x1, int y1, int x2, int y2) const {
	DrawBox(x1, y1, x2, y2, m_color, TRUE);
}
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void TriangleObject::drawObject(int x1, int y1, int x2, int y2) const {
	if (m_leftDown) {
		DrawTriangle(x2, y1, x2, y2, x1, y2, m_color, TRUE);
	}
	else {
		DrawTriangle(x1, y1, x2, y2, x1, y2, m_color, TRUE);
	}
}
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void BulletObject::drawObject(int x1, int y1, int x2, int y2) const {
	int rx = abs(x2 - x1) / 2;
	int ry = abs(y2 - y1) / 2;
	DrawOval(x1 + rx, y1 + ry, rx, ry, m_color, TRUE);
}
// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
void SlashObject::drawObject(int x1, int y1, int x2, int y2) const {
	
}