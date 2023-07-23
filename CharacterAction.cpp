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
	m_grandRightSlope = false;
	m_grandRightSlope = false;
	m_runCnt = -1;
	m_squat = false;
	m_preJumpCnt = -1;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_grandLeftSlope = false;
	m_grandRightSlope = false;
	m_vx = 0;
	m_vy = 0;

	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	m_bulletCnt = 0;

	m_slashCnt = 0;

	m_attackLeftDirection = false;

	m_landCnt = 0;
	m_boostCnt = 0;
	m_damageCnt = 0;
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL)
{

}

// �L�����𓮂��Ȃ�����
void CharacterAction::setRightLock(bool lock) { 
	// ���b�N����
	m_rightLock = lock;
}
void CharacterAction::setLeftLock(bool lock) { 
	m_leftLock = lock;
}
void CharacterAction::setUpLock(bool lock) { 
	m_upLock = lock;
}
void CharacterAction::setDownLock(bool lock) { 
	m_downLock = lock;
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

bool CharacterAction::ableDamage() const {
	if (m_state == CHARACTER_STATE::DAMAGE) { return false; }
	return true;
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
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
}

// ���n
void CharacterAction::setGrand(bool grand) {
	if (m_vy > 0) { // ���n���[�V�����ɂȂ�
		m_landCnt = LAND_TIME;
		m_slashCnt = 0;
	}
	m_grand = grand;
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) { 
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void CharacterAction::setSquat(bool squat) {
	if (squat && m_grand && m_state != CHARACTER_STATE::DAMAGE && m_state != CHARACTER_STATE::SLASH && m_state != CHARACTER_STATE::PREJUMP) {
		m_squat = true;
	}
	else {
		m_squat = false;
	}
}

void StickAction::action() {
	// ���(state)�ɉ����ĉ摜���Z�b�g
	switchHandle();

	// �ˌ��̃C���^�[�o������
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// �a���̃C���^�[�o������
	if (m_slashCnt > 0) { m_slashCnt--; }

	// �_���[�W�󂯏�Ԃ͍Œ�P�b�߂�����
	if (m_damageCnt > 0) { m_damageCnt--; }

	// �d�͂̏���
	// ���ɂ���
	if(!m_grand) { 
		// �d��
		m_vy += G;
	}

	// �A�j���[�V�����p�̃J�E���g
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) { m_boostCnt--; }

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
}

// ��Ԃɉ����ĉ摜�Z�b�g
void StickAction::switchHandle() {
	// �Z�b�g�O�̉摜�̃T�C�Y
	int wide, height;
	m_character->getHandleSize(wide, height);
	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			if (m_landCnt > 0) {
				m_character->switchLand();
			}
			else if (m_slashCnt > 0) {
				m_character->switchSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character->switchBullet();
			}
			else if (m_squat) {
				m_character->switchSquat();
			}
			else if (m_runCnt != -1) {
				m_character->switchRun(m_runCnt);
			}
			else {
				m_character->switchStand();
			}
			break;
		case CHARACTER_STATE::PREJUMP:
			m_character->switchPreJump(m_preJumpCnt);
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			if (m_slashCnt > 0) {
				m_character->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character->switchAirBullet();
			}
			else if (m_boostCnt > 0) {
				m_character->switchBoost();
			}
			else if (m_vy < 0) {
				m_character->switchJump();
			}
			else {
				m_character->switchDown();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character->switchDamage();
			break;
		}
	}
	// �Z�b�g��̉摜�̃T�C�Y
	int afterWide, afterHeight;
	m_character->getHandleSize(afterWide, afterHeight);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character->setLeftDirection(m_character->getLeftDirection());
}

// �摜�̃T�C�Y�ύX�ɂ��ʒu���� (���W�͉摜�̍���ł��邱�Ƃɒ���)
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// ���֍s���Ȃ��Ȃ�
	if (m_downLock) {
		// ��֓�����
		m_character->moveUp((afterHeight - beforeHeight));
	}
	// ��֍s���Ȃ��Ȃ�
	else if (m_upLock) {
		// ���֓������K�v�͂Ȃ��i�摜���������Ɋg�傳��邩��j
	}
	// �㉺�ǂ����ɂł��s����
	else {
		// �����ɍL����
		int d = afterHeight - beforeHeight;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character->moveUp((d - 1) / 2);
			}
			else {
				m_character->moveUp((d + 1) / 2);
			}
		}
		else {
			m_character->moveUp(d / 2);
		}
	}

	// �E�֍s���Ȃ��Ȃ�
	if (m_rightLock && !m_leftLock) {
		// ���֓�����
		m_character->moveLeft((afterWide - beforeWide));
	}
	// ���֍s���Ȃ��Ȃ�
	else if (m_leftLock && !m_rightLock) {
		// �E�֓������K�v�͂Ȃ��i�摜���E�����Ɋg�傳��邩��j
	}
	// ���E�ǂ����ɂł��s����A�������͂����Ȃ�
	else {
		// �����ɍL����
		int d = afterWide - beforeWide;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character->moveLeft((d - 1) / 2);
			}
			else {
				m_character->moveLeft((d + 1) / 2);
			}
		}
		else {
			m_character->moveLeft(d / 2);
		}
	}
}

// ���� �_���[�W���A���Ⴊ�ݒ��A�ǂԂ��蒆�͕s��
void StickAction::walk(bool right, bool left) {
	// �E�֕����̂���߂�
	if (m_moveRight && (!right || m_rightLock || m_squat || m_state == CHARACTER_STATE::DAMAGE)) {
		m_vx -= m_character->getMoveSpeed();
		m_moveRight = false;
		m_runCnt = -1;
	}
	// ���֕����̂���߂�
	if (m_moveLeft && (!left || m_leftLock || m_squat || m_state == CHARACTER_STATE::DAMAGE)) {
		m_vx += m_character->getMoveSpeed();
		m_moveLeft = false;
		m_runCnt = -1;
	}
	if (m_state == CHARACTER_STATE::DAMAGE) {
		return;
	}
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && !m_squat) { // �E�֕���
		m_vx += m_character->getMoveSpeed();
		m_moveRight = true;
	}
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && !m_squat) { // ���֕���
		m_vx -= m_character->getMoveSpeed();
		m_moveLeft = true;
	}
	// �A�j���[�V�����p�ɃJ�E���g
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// �ړ�
void StickAction::move(bool right, bool left, bool up, bool down) {
	if (m_state == CHARACTER_STATE::STAND && m_grand && m_slashCnt == 0 && m_bulletCnt == 0) {
		// �ړ������֌���
		if(left){
			m_character->setLeftDirection(true);
		}
		if (right) {
			m_character->setLeftDirection(false);
		}
	}
	// ���E�̈ړ�
	walk(right, left);
}

// �W�����v
void StickAction::jump(int cnt) {
	// ���ɕ�������W�����v���~
	if (!m_grand) {
		m_preJumpCnt = -1;
		if (m_state == CHARACTER_STATE::PREJUMP) {
			m_state = CHARACTER_STATE::STAND;
		}
	}
	// �W�����v�O�̏�ԂȂ�
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
		m_state = CHARACTER_STATE::PREJUMP;
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == PRE_JUMP_MAX) {
			int rate = (100 * m_preJumpCnt) / PRE_JUMP_MAX;
			int power = (m_character->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			m_state = CHARACTER_STATE::STAND;
		}
		else {
			m_preJumpCnt++;
		}
	}
}

// �ˌ��U��
Object* StickAction::bulletAttack(int gx, int gy) {
	if (m_state == CHARACTER_STATE::DAMAGE) {
		m_bulletCnt = 0;
		return NULL;
	}
	// �ˌ��\��ԂȂ�
	if (m_bulletCnt == 0 && m_slashCnt == 0) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character->getBulletRapid();
		// �������֌���
		m_character->setLeftDirection(m_character->getCenterX() > gx);
		// �U����Ԃ�
		return m_character->bulletAttack(gx, gy);
	}
	return NULL;
}

// �a���U��
Object* StickAction::slashAttack(int gx, int gy) {
	if (m_state == CHARACTER_STATE::DAMAGE) {
		m_slashCnt = 0;
		return NULL;
	}
	// �U���J�n
	if (m_slashCnt == 0) {
		m_attackLeftDirection = m_character->getCenterX() > gx;
		m_slashCnt = m_character->getSlashCountSum();
	}
	if (m_slashCnt > 0) {
		// �U���̕����֌���
		m_character->setLeftDirection(m_attackLeftDirection);
	}
	// �U���̃^�C�~���O����Ȃ��Ȃ�NULL���Ԃ�
	return m_character->slashAttack(m_attackLeftDirection, m_slashCnt);
}

void StickAction::damage(int vx, int vy, int damageValue, int soundHandle) {
	m_state = CHARACTER_STATE::DAMAGE;
	m_vx += vx;
	m_vy += vy;
	// �n�ʂɂ��Ă��Ă������̓_���[�W���[�V����
	if(m_vy >= 0 && m_grand){ m_damageCnt = 20; }
	m_character->setLeftDirection(m_vx > 0);
	m_grand = false;
	// ���ɕ�������
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
	// HP����
	m_character->damageHp(damageValue);
}