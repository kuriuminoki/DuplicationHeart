#include "CharacterAction.h"
#include "Character.h"
#include "Define.h"
#include "DxLib.h"


/*
*  �L�����N�^�[�̊��N���X
*/
CharacterAction::CharacterAction(Character* character) {
	m_character = character;

	//�������
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_vx = 0;
	m_vy = 0;

	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	m_bulletCnt = 0;

	m_slashCnt = 0;
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL)
{

}

// �L�����𓮂��Ȃ�����
void CharacterAction::setRightLock(bool lock) { 
	// ���b�N����
	m_rightLock = lock;
	// �ړ���Ԃ�����
	m_moveRight = false;
}
void CharacterAction::setLeftLock(bool lock) { 
	m_leftLock = lock;
	m_moveLeft = false;
}
void CharacterAction::setUpLock(bool lock) { 
	m_upLock = lock;
	m_moveUp = false;
}
void CharacterAction::setDownLock(bool lock) { 
	m_downLock = lock;
	m_moveDown;
}

// �L�����N�^�[��X���W�擾
int CharacterAction::getCharacterX() {
	return m_character->getX();
}

// �L�����N�^�[��Y���W�擾
int CharacterAction::getCharacterY() {
	return m_character->getY();
}

// �L�����N�^�[�̉����擾
int CharacterAction::getCharacterWide() {
	return m_character->getWide();
}

// �L�����N�^�[�̏c���擾
int CharacterAction::getCharacterHeight() {
	return m_character->getHeight();
}

// �L�����N�^�[�̃Z�b�^
void CharacterAction::setCharacterX(int x) {
	m_character->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character->setY(y);
}


/*
* ����΂Ȃ����ʂ̖_�l��
*/
StickAction::StickAction(Character* character):
	CharacterAction(character)
{

}

// �s���O�̏���
void StickAction::init() {
	// ��������S�����ɓ�����悤�ɂ���
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	// �������񒈂ɕ�������
	m_grand = false;
}

void StickAction::action() {
	// �ˌ��̃C���^�[�o������
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// �d�͂̏���
	// ���ɂ���
	if(!m_grand) { 
		// �d��
		m_vy += G;
	}

	// �ړ�
	if (m_vx > 0) {// �E
		if (m_rightLock) {
			m_vx = 0;
		}
		else {
			m_character->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // ��
		if (m_leftLock) {
			m_vx = 0;
		}
		else {
			m_character->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // ��
		if (m_upLock) {
			m_vy = 0;
		}
		else {
			m_character->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // ��
		if (m_downLock) {
			m_vy = 0;
		}
		else {
			m_character->moveDown(m_vy);
		}
	}

	// ���(state)�ɉ����ĉ摜���Z�b�g
	switchHandle();
}

// ��Ԃɉ����ĉ摜�Z�b�g
void StickAction::switchHandle() {
	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			m_character->switchStand();
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			m_character->switchStand();
			break;
		}
	}
}

// ���� �_���[�W���͕s��
void StickAction::walk(bool right, bool left) {
	if (m_moveRight && !right) { // �E�֕����̂���߂�
		m_vx -= m_character->getMoveSpeed();
		m_moveRight = false;
	}
	if (m_moveLeft && !left) { // ���֕����̂���߂�
		m_vx += m_character->getMoveSpeed();
		m_moveLeft = false;
	}
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right) { // �E�֕���
		m_vx += m_character->getMoveSpeed();
		m_moveRight = true;
	}
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left) { // ���֕���
		m_vx -= m_character->getMoveSpeed();
		m_moveLeft = true;
	}
}

// �ړ�
void StickAction::move(bool right, bool left, bool up, bool down) {
	// ���E�̈ړ�
	walk(right, left);
}

// �W�����v
void StickAction::jump(int rate) {
	if (m_grand) {// �n��ɂ���Ȃ�
		int power = (m_character->getJumpHeight() * rate) / 100;
		m_vy -= power;
		m_grand = false;
	}
}

// �ˌ��U��
Object* StickAction::bulletAttack(int gx, int gy) {
	// �ˌ��\��ԂȂ�
	if (m_bulletCnt == 0) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character->getBulletRapid();
		// �U����Ԃ�
		return m_character->bulletAttack(gx, gy);
	}
	return NULL;
}