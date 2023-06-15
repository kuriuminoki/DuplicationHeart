#include "Object.h"
#include "CharacterController.h"
#include "Define.h"
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
void BoxObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();
	int characterVx = characterController->getCharacterVx();
	int characterVy = characterController->getCharacterVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		if (characterY2 <= m_y1 && characterY2 + characterVy >= m_y1) {
			// ���n
			characterController->setCharacterGrand(true);
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
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
				DrawFormatString(1000, 100, WHITE, "ok");
				characterController->setCharacterX(characterX1 + characterController->getCharacterWide() / 2);
				characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
				// ���n
				characterController->setCharacterGrand(true);
				// �L�����͉��ֈړ��ł��Ȃ�
				characterController->setActionDownLock(true);
			}
			else {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
			}
		}
		// ���Ɉړ����̃L�������E���瓖�����Ă��邩����
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			// �i���Ƃ݂Ȃ��ď��z����
			if (characterY2 - STAIR_HEIGHT <= m_y1 && characterX1 == m_x2) {
				DrawFormatString(1000, 100, WHITE, "ok");
				// �K�؂ȍ��W��
				characterController->setCharacterX(characterX1 - characterController->getCharacterWide() / 2);
				characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
				// ���n
				characterController->setCharacterGrand(true);
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

	// ������I�u�W�F�N�g�̒��ɓ��荞��ł��܂�����
	if (characterVy != 0 && characterY2 > m_y1 && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		// �^���
		characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
		// ���n
		characterController->setCharacterGrand(true);
		// �L�����͉��ֈړ��ł��Ȃ�
		characterController->setActionDownLock(true);
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
int TriangleObject::getY(int x) {
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
void TriangleObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX1_5 = characterX1 + (characterController->getCharacterWide() / 2);
	int characterY1_5 = characterY1 + (characterController->getCharacterHeight() / 2);
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();
	int characterVx = characterController->getCharacterVx();
	int characterVy = characterController->getCharacterVy();

	// �L�������㉺�ړ��œ������Ă��邩����
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// ����Ă���Ƃ��͂��̏�������true�ɂȂ�
		if (characterY2 == getY(characterX1_5 - characterVx)) {
			// �O�̃t���[���ł͒��n���Ă����̂ŁA�����������n
			characterController->setCharacterGrand(true);
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
		}
		// ���Ɉړ����̃L�������ォ�瓖�����Ă��邩����
		else if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// ���n
			characterController->setCharacterGrand(true);
			// �L�����͉��ֈړ��ł��Ȃ�
			characterController->setActionDownLock(true);
			// ������Ԃ܂ł͈ړ�������
			characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
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
		// �L�����͉��ֈړ��ł��Ȃ�
		characterController->setActionDownLock(true);
		// �K�؂ȍ����ֈړ�
		characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
	}

	// ��̉s�p�i��[�j�̓����蔻��
	if (m_y2 > characterY1 && m_y2 < characterY2) {
		if (m_leftDown) {// ����������
			// �E�Ɉړ����̃L�������E���瓖�����Ă��邩����
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// �L�����͉E�ֈړ��ł��Ȃ�
				characterController->setActionRightLock(true);
				// ������Ԃ܂ł͈ړ�������
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
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
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
			}
		}
	}
}

void TriangleObject::action() {

}


BulletObject::BulletObject(int x1, int y1, int x2, int y2, int color, int gx, int gy, int damage, int speed, int distance) :
	Object(x1, y1, x2, y2)
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;

	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = (m_x2 - m_x1) / 2;
	m_ry = (m_y2 - m_y1) / 2;
	m_damage = damage;

	// �p�x���v�Z���AVX��VY������
	int dx = gx - x1;
	int dy = gy - y1;
	double r = std::atan2((double)dy, (double)dx);
	m_v = speed;
	m_vx = (int)speed * std::cos(r);
	m_vy = (int)speed * std::sin(r);

	m_d = distance;
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
void BulletObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		// �ђʒe����Ȃ��Ȃ����
		m_deleteFlag = true;
	}
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


SlashObject::SlashObject(int x1, int y1, int x2, int y2, int* handle, int handleSum, int damage, int slashCountSum) :
	Object(x1, y1, x2, y2)
{
	// �K�v�Ȃ�ォ��Z�b�^�Őݒ�
	m_characterId = -1;

	// �摜
	m_handle = handle;
	m_handleSum = handleSum;
	
	// �_���[�W
	m_damage = damage;

	// �S�̃t���[��
	m_slashCountSum = slashCountSum;

	// �J�E���g
	m_cnt = 0;
}

// �L�����Ƃ̓����蔻��
// �������Ă���Ȃ�L�����𑀍삷��B
void SlashObject::atari(CharacterController* characterController) {
	// �L�����̏��@���W�ƈړ��X�s�[�h
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();

	// �����蔻��
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		// �ђʒe����Ȃ��Ȃ����
		m_deleteFlag = true;
	}
}

void SlashObject::action() {
	m_cnt++;

	if (m_cnt == m_slashCountSum) {
		m_deleteFlag = true;
	}
}