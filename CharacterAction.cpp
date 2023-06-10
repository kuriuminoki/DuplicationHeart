#include "CharacterAction.h"
#include "Character.h"
#include "DxLib.h"


/*
*  �L�����N�^�[�̊��N���X
*/
CharacterAction::CharacterAction(Character* character) {
	m_character = character;

	//�������
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_vx = 0;
	m_vy = 0;
}

void CharacterAction::debugAction(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + 10, color, "state=%d, grand=%d, (vx,vy)=(%d,%d)", m_state, m_grand, m_vx, m_vy);
}



/*
* ����΂Ȃ����ʂ̖_�l��
*/
void StickAction::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**StickAction**");
	debugAction(x + 10, y + 10, color);
}

void StickAction::action() {

	// �ړ���̍��W
	int nextX = m_character->getX();
	int nextY = m_character->getY();

	if (m_grand) { // �n�ʂɂ���

	}
	else { // ���ɂ���
		// �d��
		m_vy += G;
		nextY += m_vy;
	}

	// �ړ�
	m_character->setX(nextX);
	m_character->setY(nextY);
}

// ��Ԃɉ����ĉ摜�Z�b�g
void StickAction::switchHandle() {
	switch (m_state) {
	case STAND: //�������
		m_character->switchStand();
		break;
	}
}

// ����
void StickAction::walk(bool right, bool left) {
	// �ړ���̍��W
	int nextX = m_character->getX();
	int nextY = m_character->getY();

	if (right) { // �E�֕���
		nextX += m_character->getMoveSpeed();
	}
	else if (left) { // ���֕���
		nextX -= m_character->getMoveSpeed();
	}

	// �ړ�
	m_character->setX(nextX);
	m_character->setY(nextY);
}

// �ړ�
void StickAction::move(bool right, bool left, bool up, bool down) {
	walk(right, left);
}