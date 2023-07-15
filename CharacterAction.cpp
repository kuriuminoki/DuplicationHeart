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
	m_runCnt = -1;
	m_squat = false;
	m_preJumpCnt = -1;
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

	m_attackLeftDirection = false;
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

// �L�����N�^�[�̃Z�b�^
void CharacterAction::setCharacterX(int x) {
	m_character->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character->setY(y);
}
void CharacterAction::setCharacterLeftDirection(bool leftDirection) {
	m_character->setLeftDirection(leftDirection);
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

	// �a���̃C���^�[�o������
	if (m_slashCnt > 0) { m_slashCnt--; }

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
	// �Z�b�g�O�̉摜�̃T�C�Y
	int wide, height;
	m_character->getHandleSize(wide, height);
	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			m_character->switchStand();
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			if (m_vy < 0) {
				m_character->switchJump();
			}
			else {
				m_character->switchDown();
			}
			break;
		}
	}
	// �Z�b�g��̉摜�̃T�C�Y
	int afterWide, afterHeight;
	m_character->getHandleSize(afterWide, afterHeight);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(wide, height, afterWide, afterHeight);
}

// �摜�̃T�C�Y�ύX�ɂ��ʒu����
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// ���֍s���Ȃ��Ȃ�
	if (m_downLock) {
		// ��֓�����
		m_character->moveUp((afterHeight - beforeHeight));
	}
	// ��֍s���Ȃ��Ȃ�
	if (m_upLock) {
		// ���֓�����
		m_character->moveDown((afterHeight - beforeHeight));
	}

	// �E�֍s���Ȃ��Ȃ�
	if (m_rightLock) {
		// ���֓�����
		m_character->moveLeft((afterWide - beforeWide));
	}
	// ���֍s���Ȃ��Ȃ�
	if (m_leftLock) {
		// �E�֓�����
		m_character->moveRight((afterWide - beforeWide));
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
void StickAction::jump(int cnt) {
	// �W�����v�O�̏�ԂȂ�
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == 10) {
			int rate = (100 * m_preJumpCnt) / 10;
			int power = (m_character->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
		}
		else {
			m_preJumpCnt++;
		}
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

// �a���U��
Object* StickAction::slashAttack() {
	// �U���J�n
	if (m_slashCnt == 0) {
		m_attackLeftDirection = m_character->getLeftDirection();
		m_slashCnt = m_character->getSlashCountSum();
	}
	// �U���̃^�C�~���O����Ȃ��Ȃ�NULL���Ԃ�
	return m_character->slashAttack(m_attackLeftDirection, m_slashCnt);
}