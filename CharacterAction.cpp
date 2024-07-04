#include "CharacterAction.h"
#include "Character.h"
#include "Sound.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// �N���X��
const char* CharacterAction::ACTION_NAME = "CharacterAction";
const char* StickAction::ACTION_NAME = "StickAction";
const char* ValkiriaAction::ACTION_NAME = "ValkiriaAction";
const char* FlightAction::ACTION_NAME = "FlightAction";
const char* KoharuAction::ACTION_NAME = "KoharuAction";
const char* SunAction::ACTION_NAME = "SunAction";
const char* BossFreezeAction::ACTION_NAME = "BossFreezeAction";

// �N���X������CharacterAction���쐬����֐�
CharacterAction* createAction(const string actionName, Character* character, SoundPlayer* soundPlayer_p) {

	string tmp = actionName;

	bool heavy = false;
	if (tmp.size() >= 2) {
		string last = tmp.substr(tmp.size() - 2);
		if (last == "_x") {
			heavy = true;
			tmp.erase(tmp.size() - 2, 2);
		}
	}

	CharacterAction* action = nullptr;
	if (tmp == StickAction::ACTION_NAME) {
		action = new StickAction(character, soundPlayer_p);
	}
	else if(tmp == ValkiriaAction::ACTION_NAME) {
		action = new ValkiriaAction(character, soundPlayer_p);
	}
	else if (tmp == FlightAction::ACTION_NAME) {
		action = new FlightAction(character, soundPlayer_p);
	}
	else if (tmp == KoharuAction::ACTION_NAME) {
		action = new KoharuAction(character, soundPlayer_p);
	}
	else if (tmp == BossFreezeAction::ACTION_NAME) {
		action = new BossFreezeAction(character, soundPlayer_p);
	}
	else if (tmp == SunAction::ACTION_NAME) {
		action = new SunAction(character, soundPlayer_p, false);
	}

	action->setHeavy(heavy);

	return action;
}


/*
*  �L�����N�^�[�̊��N���X
*/
CharacterAction::CharacterAction(Character* character, SoundPlayer* soundPlayer_p) {
	m_character_p = character;
	m_soundPlayer_p = soundPlayer_p;

	//�������
	m_cnt = 0;
	m_prevLeftDirection = m_character_p->getLeftDirection();
	m_state = CHARACTER_STATE::STAND;
	m_characterVersion = character->getVersion();
	m_characterMoveSpeed = character->getMoveSpeed();
	m_grand = false;
	m_runCnt = -1;
	m_preJumpCnt = -1;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_grandLeftSlope = false;
	m_grandRightSlope = false;
	m_vx = 0;
	m_vy = 0;
	m_dx = 0;
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;
	m_bulletCnt = 0;
	m_slashCnt = 0;
	m_attackLeftDirection = false;
	m_landCnt = 0;
	m_boostCnt = 0;
	m_boostDone = 0;
	m_damageCnt = 0;
	m_heavy = false;
}

CharacterAction::CharacterAction() :
	CharacterAction(nullptr, nullptr)
{

}

void CharacterAction::setParam(CharacterAction* action) {
	action->setCnt(m_cnt);
	action->setState(m_state);
	action->setCharacterVersion(m_characterVersion);
	action->setCharacterMoveSpeed(m_characterMoveSpeed);
	action->setSimpleGrand(m_grand);
	action->setGrandLeftSlope(m_grandLeftSlope);
	action->setGrandRightSlope(m_grandRightSlope);
	action->setRunCnt(m_runCnt);
	action->setJumpCnt(m_preJumpCnt);
	action->setMoveRight(m_moveRight);
	action->setMoveLeft(m_moveLeft);
	action->setMoveUp(m_moveUp);
	action->setMoveDown(m_moveDown);
	action->setVx(m_vx);
	action->setVy(m_vy);
	action->setRightLock(m_rightLock);
	action->setLeftLock(m_leftLock);
	action->setUpLock(m_upLock);
	action->setDownLock(m_downLock);
	action->setBulletCnt(m_bulletCnt);
	action->setSlashCnt(m_slashCnt);
	action->setAttackLeftDirection(m_attackLeftDirection);
	action->setLandCnt(m_landCnt);
	action->setBoostCnt(m_boostCnt);
	action->setBoostDone(m_boostDone);
	action->setDamageCnt(m_damageCnt);
	action->setHeavy(m_heavy);
}

void CharacterAction::setState(CHARACTER_STATE state) {
	if (m_state == CHARACTER_STATE::DAMAGE) { return; }
	m_state = state;
}

// �L�����𓮂��Ȃ�����
void CharacterAction::setRightLock(bool lock) { 
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

// Boost
void CharacterAction::setBoost(bool leftDirection) {
	if ((leftDirection && m_leftLock) || (!leftDirection && m_rightLock)) {
		return;
	}
	if (!damageFlag() && !m_grand && !m_boostDone) {
		m_boostCnt = BOOST_TIME;
		m_vy -= BOOST_SPEED;
		finishBullet();
		finishSlash();
		if (leftDirection) {
			m_vx -= BOOST_SPEED;
			m_boostDone = 2;
		}
		else {
			m_vx += BOOST_SPEED;
			m_boostDone = 1;
		}
		m_character_p->setLeftDirection(leftDirection);
	}
}
void CharacterAction::finishBoost() {
	m_boostCnt = 0;
	if (m_boostDone == 1) {
		m_vx -= BOOST_SPEED;
	}
	else if (m_boostDone == 2) {
		m_vx += BOOST_SPEED;
	}
	m_boostDone = false;
}

// �L�����N�^�[�̃Z�b�^
void CharacterAction::setCharacterX(int x) {
	m_character_p->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character_p->setY(y);
}
void CharacterAction::setCharacterLeftDirection(bool leftDirection) {
	m_character_p->setLeftDirection(leftDirection);
}
void CharacterAction::setCharacterFreeze(bool freeze) {
	m_character_p->setFreeze(freeze);
}

// �s���O�̏��� ���t���[���s��
void CharacterAction::init() {

	m_cnt++;

	m_dx = 0;

	// �X�L���Q�[�W�̉�
	if (m_cnt % 30 == 29) {
		m_character_p->setSkillGage(m_character_p->getSkillGage() + 1);
	}

	// �O�̃t���[����leftDirection��ۑ����Ă���
	m_prevLeftDirection = m_character_p->getLeftDirection();

	// ��������S�����ɓ�����悤�ɂ���
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	// �������񒈂ɕ�������
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;

	// prevHp��hp�ɒǂ�������
	m_character_p->setPrevHp(m_character_p->getPrevHp() - 1);

	// �L�����̃o�[�W�������ω������ꍇ
	if (m_characterVersion != m_character_p->getVersion()) {
		if (m_moveLeft) { m_vx += m_characterMoveSpeed; m_vx -= m_character_p->getMoveSpeed(); }
		if (m_moveRight) { m_vx -= m_characterMoveSpeed; m_vx += m_character_p->getMoveSpeed(); }
		if (m_moveUp) { m_vy += m_characterMoveSpeed; m_vy -= m_character_p->getMoveSpeed(); }
		if (m_moveDown) { m_vy -= m_characterMoveSpeed; m_vy += m_character_p->getMoveSpeed(); }
		m_characterVersion = m_character_p->getVersion();
		m_characterMoveSpeed = m_character_p->getMoveSpeed();
	}
}

void CharacterAction::bulletAction() {
	// �ˌ��̃C���^�[�o������
	if (m_bulletCnt > 0) {
		m_bulletCnt--;
		if (m_bulletCnt == 0) { finishBullet(); }
	}
}

void CharacterAction::slashAction() {
	// �a���̃C���^�[�o������
	if (m_slashCnt > 0) {
		m_slashCnt--;
		if (m_slashCnt == 0) { finishSlash(); }
	}
}

void CharacterAction::damageAction() {
	// �_���[�W�󂯏�Ԃ͍Œ�P�b�߂�����
	if (m_damageCnt > 0) { m_damageCnt--; }
}

void CharacterAction::otherAction() {
	// �A�j���[�V�����p�̃J�E���g
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) { m_boostCnt--; }
}

void CharacterAction::moveAction() {
	// �ړ�
	if (!m_heavy && ((m_dx < 0 && !m_leftLock) || (m_dx > 0 && !m_rightLock))) {
		m_character_p->moveRight(m_dx);
	}
	if (m_vx > 0) {// �E
		if (m_rightLock) {
			stopMoveLeft(); // ���Ɉړ��������̂ɐ�����ѓ��ŉE�ֈړ����Ă���Ƃ��A�������񍶈ړ��ւ̓��͂��L�����Z�������Ȃ��ƃo�O��
			m_vx = 0;
		}
		else {
			m_character_p->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // ��
		if (m_leftLock) {
			stopMoveRight();// �E�Ɉړ��������̂ɐ�����ѓ��ō��ֈړ����Ă���Ƃ��A��������E�ړ��ւ̓��͂��L�����Z�������Ȃ��ƃo�O��
			m_vx = 0;
		}
		else {
			m_character_p->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // ��
		if (m_upLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // ��
		if (m_downLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveDown(m_vy);
		}
	}
}

void CharacterAction::action() {
	// ���(state)�ɉ����ĉ摜���Z�b�g
	switchHandle();

	bulletAction();

	slashAction();

	damageAction();

	otherAction();

	moveAction();
}

// �_���[�W
void CharacterAction::damage(int vx, int vy, int damageValue) {
	m_damageCnt = DAMAGE_TIME;
	if (!m_heavy) {
		setState(CHARACTER_STATE::DAMAGE);
		m_vx += vx;
		m_vy += vy;
		m_character_p->setLeftDirection(vx > 0);
		// ���ɕ�������
		m_grand = false;
		m_grandRightSlope = false;
		m_grandLeftSlope = false;
	}
	// HP����
	m_character_p->damageHp(damageValue);
	m_boostCnt = 0;
}

void CharacterAction::startBullet() {

}

void CharacterAction::finishBullet() {
	m_bulletCnt = 0;
}

void CharacterAction::startSlash() {

}

void CharacterAction::finishSlash() {
	m_slashCnt = 0;
}

bool CharacterAction::ableDamage() const {
	return !(m_state == CHARACTER_STATE::DAMAGE || m_damageCnt > 0 || m_boostCnt > max(0, BOOST_TIME - 10));
}

bool CharacterAction::ableAttack() const {
	return !(m_bulletCnt > 0 || m_slashCnt > 0);
}

bool CharacterAction::ableWalk() const {
	return !m_moveRight && !m_moveLeft && m_state != CHARACTER_STATE::SQUAT;
}

bool CharacterAction::ableChangeDirection() const {
	if (m_character_p->getHp() == 0
		|| m_bulletCnt > 0 || m_slashCnt > 0
		|| m_moveLeft || m_moveRight || m_moveUp || m_moveDown
		|| m_damageCnt > 0) {
		return false;
	}
	return true;
}

// ���n
void CharacterAction::setGrand(bool grand) {
	if (m_vy > 0) { // ���n���[�V�����ɂȂ�
		m_landCnt = LAND_TIME;
		finishSlash();
		// ���ʉ�
		if (m_soundPlayer_p != nullptr) {
			m_soundPlayer_p->pushSoundQueue(m_character_p->getLandSound(),
				adjustPanSound(m_character_p->getCenterX(),
					m_soundPlayer_p->getCameraX()));
		}
	}
	m_grand = grand;
	finishBoost();
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) {
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void CharacterAction::setSquat(bool squat) {
	if (m_state != CHARACTER_STATE::DAMAGE && m_state != CHARACTER_STATE::PREJUMP && m_state != CHARACTER_STATE::INIT) {
		if (squat && m_grand && m_slashCnt == 0) {
			// ���Ⴊ�߂��ԂȂ̂ł��Ⴊ��
			m_state = CHARACTER_STATE::SQUAT;
		}
		else {
			m_state = CHARACTER_STATE::STAND;
		}
	}
}

// �����n�߂�
void CharacterAction::startMoveLeft() {
	// ���֕����n�߂�
	m_moveLeft = true;
	m_vx -= m_character_p->getMoveSpeed();
}
void CharacterAction::startMoveRight() {
	// �E�֕����n�߂�
	m_moveRight = true;
	m_vx += m_character_p->getMoveSpeed();
}
void CharacterAction::startMoveUp() {
	// ��֕����n�߂�
	m_moveUp = true;
	m_vy -= m_character_p->getMoveSpeed();
}
void CharacterAction::startMoveDown() {
	// ���֕����n�߂�
	m_moveDown = true;
	m_vy += m_character_p->getMoveSpeed();
}

// �����̂���߂�
void CharacterAction::stopMoveLeft() {
	// ���֕����̂���߂�
	if (m_moveLeft) {
		m_vx += m_character_p->getMoveSpeed();
		m_moveLeft = false;
		m_runCnt = -1;
	}
	if (m_boostDone == 2) {
		finishBoost();
	}
}
void CharacterAction::stopMoveRight() {
	// �E�֕����̂���߂�
	if (m_moveRight) {
		m_vx -= m_character_p->getMoveSpeed();
		m_moveRight = false;
		m_runCnt = -1;
	}
	if (m_boostDone == 1) {
		finishBoost();
	}
}
void CharacterAction::stopMoveUp() {
	// ��֕����̂���߂�
	if (m_moveUp) {
		m_vy += m_character_p->getMoveSpeed();
		m_moveUp = false;
		m_runCnt = -1;
	}
}
void CharacterAction::stopMoveDown() {
	// ���֕����̂���߂�
	if (m_moveDown) {
		m_vy -= m_character_p->getMoveSpeed();
		m_moveDown = false;
		m_runCnt = -1;
	}
}

// �摜�̃T�C�Y�ύX�ɂ��ʒu���� (���W�͉摜�̍���ł��邱�Ƃɒ���)
void CharacterAction::afterChangeGraph(int beforeX1, int afterX1, int beforeY1, int afterY1, int beforeX2, int afterX2, int beforeY2, int afterY2) {
	int dy = 0;
	if (m_downLock) {
		if (afterY2 > beforeY2) {
			dy -= afterY2 - beforeY2;
		}
		else {
			dy += beforeY2 - afterY2;
		}
	}
	else if (m_upLock) {
		if (afterY1 < beforeY1) {
			dy += beforeY1 - afterY1;
		}
		else {
			dy -= afterY1 - beforeY1;
		}
	}
	else {
		dy = ((beforeY2 - afterY2) + (beforeY1 - afterY1)) / 2;
	}

	m_character_p->moveDown(dy);

	int dx = 0;
	if (m_rightLock) {
		if (afterX2 > beforeX2) {
			dx -= afterX2 - beforeX2;
		}
		else {
			dx += beforeX2 - afterX2;
		}
	}
	else if (m_leftLock) {
		if (afterX1 < beforeX1) {
			dx += beforeX1 - afterX1;
		}
		else {
			dx -= afterX1 - beforeX1;
		}
	}
	else {
		dx = ((beforeX2 - afterX2) + (beforeX1 - afterX1)) / 2;
	}

	m_character_p->moveRight(dx);
}


/*
* ����΂Ȃ����ʂ̖_�l��
*/
StickAction::StickAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* StickAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new StickAction(characters[i], m_soundPlayer_p);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}

void StickAction::action() {
	// �d�͂̏���
	if (!m_grand) {
		// �d��
		m_vy += G;
	}
	CharacterAction::action();
}

// ��Ԃɉ����ĉ摜�Z�b�g
void StickAction::switchHandle() {
	// �Z�b�g�O�̉摜�̃T�C�Y
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	bool nowLeftDirection = m_character_p->getLeftDirection();
	m_character_p->setLeftDirection(m_prevLeftDirection);
	m_character_p->getAtariArea(&x1, &y1, &x2, &y2);
	
	// ����摜
	if (m_grand && m_character_p->getHp() == 0 && m_character_p->haveDeadGraph() && m_state != CHARACTER_STATE::DAMAGE) {
		m_character_p->switchDead();
	}
	else if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			if (m_slashCnt > 0) {
				m_character_p->switchSlash();
			}
			else if (m_landCnt > 0) {
				m_character_p->switchLand();
			}
			else if (m_bulletCnt > 0) {
				if (m_runCnt != -1) {
					m_character_p->switchRunBullet(m_runCnt);
				}
				else {
					m_character_p->switchBullet();
				}
			}
			else if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::SQUAT:
			if (m_bulletCnt > 0) {
				m_character_p->switchSquatBullet();
			}
			else {
				m_character_p->switchSquat();
			}
			break;
		case CHARACTER_STATE::PREJUMP:
			m_character_p->switchPreJump(m_preJumpCnt);
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchSlash();
				}
				else if (m_bulletCnt > 0) {
					if (m_runCnt != -1) {
						m_character_p->switchRunBullet(m_runCnt);
					}
					else {
						m_character_p->switchBullet();
					}
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character_p->switchAirBullet();
			}
			else if (m_boostCnt > 0) {
				m_character_p->switchBoost();
			}
			else if (m_vy < 0) {
				m_character_p->switchJump();
			}
			else {
				m_character_p->switchDown();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchAirSlash();
				}
				else if (m_bulletCnt > 0) {
					m_character_p->switchAirBullet();
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}

	m_character_p->setLeftDirection(nowLeftDirection);

	// �Z�b�g��̉摜�̃T�C�Y
	int afterX1 = 0, afterY1 = 0, afterX2 = 0, afterY2 = 0;
	m_character_p->getAtariArea(&afterX1, &afterY1, &afterX2, &afterY2);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(x1, afterX1, y1, afterY1, x2, afterX2, y2, afterY2);

}

// ���� �_���[�W���A���Ⴊ�ݒ��A�ǂԂ��蒆�͕s��
void StickAction::walk(bool right, bool left) {
	// �E�֕����̂���߂�
	if (!right || m_rightLock || m_state == CHARACTER_STATE::SQUAT || damageFlag()) {
		stopMoveRight();
	}
	if (m_slashCnt > 0 && !m_attackLeftDirection && (m_rightLock || damageFlag()) && m_vx > 0) {
		finishSlash();
	}
	if (m_bulletCnt > 0 && !m_attackLeftDirection && m_rightLock && m_vx > 0) {
		finishBullet();
	}
	// ���֕����̂���߂�
	if (!left || m_leftLock || m_state == CHARACTER_STATE::SQUAT || damageFlag()) {
		stopMoveLeft();
	}
	if (m_slashCnt > 0 && m_attackLeftDirection && (m_leftLock || damageFlag()) && m_vx < 0) {
		finishSlash();
	}
	if (m_bulletCnt > 0 && m_attackLeftDirection && m_leftLock && m_vx < 0) {
		finishBullet();
	}
	if (damageFlag()) {
		return;
	}

	// �E�֕����n�߂�
	if (!m_rightLock && ableWalk() && right && (!left || !m_character_p->getLeftDirection())) { // �E�֕���
		startMoveRight();
	}
	// ���֕����n�߂�
	if (!m_leftLock && ableWalk() && left && (!right || m_character_p->getLeftDirection())) { // ���֕���
		startMoveLeft();
	}
	// �A�j���[�V�����p�ɃJ�E���g
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// �ړ�
void StickAction::move(bool right, bool left, bool up, bool down) {
	if ((m_state == CHARACTER_STATE::STAND || m_state == CHARACTER_STATE::SQUAT) && m_grand && m_slashCnt == 0 && m_bulletCnt == 0) {
		// �ړ������֌���
		if(left && !right){
			m_character_p->setLeftDirection(true);
		}
		if (right && !left) {
			m_character_p->setLeftDirection(false);
		}
	}
	// ���E�̈ړ�
	walk(right, left);
}

// �W�����v
void StickAction::jump(int cnt) {
	// �_���[�W��ԂȂ�W�����v�ł��Ȃ�����return
	if (damageFlag()) {
		// �󂯐g�͂ł���
		if (cnt == 1 || m_boostCnt > 0) {
			if (m_boostCnt == 0) {
				// �󂯐g�������u��
				m_vy -= m_character_p->getJumpHeight() / 2;
				m_grand = false;
				m_preJumpCnt = -1;
				stopMoveLeft();
				stopMoveRight();
				// ���ʉ�
				if (m_soundPlayer_p != nullptr) {
					m_soundPlayer_p->pushSoundQueue(m_character_p->getPassiveSound(),
						adjustPanSound(m_character_p->getCenterX(),
							m_soundPlayer_p->getCameraX()));
				}
			}
			// �_���[�W��Ԃ����������܂ł͂�����Boost
			m_boostCnt = BOOST_TIME;
		}
		return;
	}
	// �a�����̓W�����v�s��
	if (m_slashCnt > 0) { return; }
	// ���ɕ�������W�����v���~
	if (!m_grand) {
		m_preJumpCnt = -1;
		if (m_state == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
	}
	// �W�����v�̏����J�n
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
		setState(CHARACTER_STATE::PREJUMP);
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == getPreJumpMax()) {
			// �W�����v
			int rate = (100 * m_preJumpCnt) / getPreJumpMax();
			int power = (m_character_p->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			setState(CHARACTER_STATE::STAND);
			// ���ʉ�
			if (m_soundPlayer_p != nullptr) {
				m_soundPlayer_p->pushSoundQueue(m_character_p->getJumpSound(),
					adjustPanSound(m_character_p->getCenterX(),
						m_soundPlayer_p->getCameraX()));
			}
		}
		else {
			// �W�����v�O�̗��ߒ�
			m_preJumpCnt++;
		}
	}
}

// �ˌ��U��
Object* StickAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		finishBullet();
		return nullptr;
	}
	// �ˌ��\��ԂȂ�
	if (ableAttack()) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character_p->getBulletRapid();
		// �������֌���
		if (m_character_p->getCharacterInfo()->sameBulletDirection()) {
			m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		}
		startBullet();
		// �U����Ԃ�
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return nullptr;
}

// �a���U��
Object* StickAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		if (m_slashCnt > 0) { finishSlash(); }
		return nullptr;
	}
	// �U���J�n
	if (ableAttack()) {
		// �W�����v�̓L�����Z��
		m_preJumpCnt = -1;
		if (m_state == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
		m_attackLeftDirection = m_character_p->getCenterX() > gx;
		m_slashCnt = m_character_p->getSlashCountSum() + m_character_p->getSlashInterval();
		// �U���̕����֌���
		m_character_p->setLeftDirection(m_attackLeftDirection);
		// �a���J�n���̏���
		startSlash();
	}
	// �U���̃^�C�~���O����Ȃ��Ȃ�nullptr���Ԃ�
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_grand, m_soundPlayer_p);
}


/*
* ���@���L���A�pAction �a�����Ɉړ�����
*/
ValkiriaAction::ValkiriaAction(Character* character, SoundPlayer* soundPlayer_p) :
	StickAction(character, soundPlayer_p)
{
	m_slashNow = false;
}

CharacterAction* ValkiriaAction::createCopy(vector<Character*> characters) {
	ValkiriaAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new ValkiriaAction(characters[i], m_soundPlayer_p);
			res->setSlashNow(m_slashNow);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}

// ���n ���@���L���A�͎a�����ɒ��n���Ă����n���[�V�����ɂȂ�Ȃ�
void ValkiriaAction::setGrand(bool grand) {
	if (m_vy > 0) { // ���n���[�V�����ɂȂ�
		if (m_slashCnt == 0) { // �a�����ł͂Ȃ�
			m_landCnt = LAND_TIME;
			// ���ʉ�
			if (m_soundPlayer_p != nullptr) {
				m_soundPlayer_p->pushSoundQueue(m_character_p->getLandSound(),
					adjustPanSound(m_character_p->getCenterX(),
						m_soundPlayer_p->getCameraX()));
			}
		}
	}
	m_grand = grand;
	finishBoost();
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) {
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void ValkiriaAction::startSlash() {
	if (m_attackLeftDirection) {
		if (!m_leftLock) {
			m_vx -= SLASH_MOVE_SPEED;
			m_slashNow = true;
		}
	}
	else {
		if (!m_rightLock) {
			m_vx += SLASH_MOVE_SPEED;
			m_slashNow = true;
		}
	}
}

void ValkiriaAction::finishSlash() {
	CharacterAction::finishSlash();
	if (m_slashNow) {
		if (m_attackLeftDirection && !m_leftLock) {
			m_vx += SLASH_MOVE_SPEED;
		}
		else if (!m_rightLock) {
			m_vx -= SLASH_MOVE_SPEED;
		}
	}
	m_slashNow = false;
}

// �_���[�W���󂯂� ���@���L���A�͎a������HP�����邾��
void ValkiriaAction::damage(int vx, int vy, int damageValue) {
	if (m_slashCnt > 0) {
		// HP����
		m_character_p->damageHp(damageValue / 2);
	}
	else {
		CharacterAction::damage(vx, vy, damageValue);
	}
}


/*
* ����ԃL����
*/
FlightAction::FlightAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* FlightAction::createCopy(std::vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new FlightAction(characters[i], m_soundPlayer_p);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}

// �L�����̉摜�����(state)�ɉ����ĕύX
void FlightAction::switchHandle() {
	// �Z�b�g�O�̉摜�̃T�C�Y
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	bool nowLeftDirection = m_character_p->getLeftDirection();
	m_character_p->setLeftDirection(m_prevLeftDirection);
	m_character_p->getAtariArea(&x1, &y1, &x2, &y2);

	if (m_grand) { // �n�ʂɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������
			if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character_p->switchAirBullet();
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}
	else { // ���ɂ���Ƃ�
		switch (m_state) {
		case CHARACTER_STATE::STAND: //�������(�Ȃɂ��Ȃ��̏��)
			if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				if (m_runCnt != -1) {
					m_character_p->switchRunBullet(m_runCnt);
				}
				else {
					m_character_p->switchAirBullet();
				}
			}
			else if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else if (m_vy != 0) {
				m_character_p->switchDown();
			}
			else {
				m_character_p->switchJump();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}

	m_character_p->setLeftDirection(nowLeftDirection);

	// �Z�b�g��̉摜�̃T�C�Y
	int afterX1 = 0, afterY1 = 0, afterX2 = 0, afterY2 = 0;
	m_character_p->getAtariArea(&afterX1, &afterY1, &afterX2, &afterY2);

	// �T�C�Y�ύX�ɂ��ʒu����
	afterChangeGraph(x1, afterX1, y1, afterY1, x2, afterX2, y2, afterY2);

}

void FlightAction::damageAction() {
	// �_���[�W�󂯏�Ԃ͍Œ�P�b�߂�����
	if (m_damageCnt > 0) {
		m_damageCnt--;
		if (m_damageCnt == 0 && !m_heavy) {
			m_vx = 0;
			m_vy = 0;
			m_state = CHARACTER_STATE::STAND;
		}
	}
}
void FlightAction::otherAction() {
	// �A�j���[�V�����p�̃J�E���g
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) {
		m_boostCnt--;
		if (m_boostCnt == 0) {
			finishBoost();
		}
	}
}
void FlightAction::moveAction() {

	CharacterAction::moveAction();

	if (m_vy < 0) { // ��
		if (m_upLock) {
			stopMoveDown();
		}
	}
	else if (m_vy > 0) { // ��
		if (m_downLock) {
			stopMoveUp();
		}
	}

}

void FlightAction::walk(bool right, bool left, bool up, bool down) {
	// �E�֕����̂���߂�
	if (!right || m_rightLock || m_state == CHARACTER_STATE::SQUAT || damageFlag()) {
		stopMoveRight();
	}
	if (m_slashCnt > 0 && !m_attackLeftDirection && (m_rightLock || damageFlag())) {
		finishSlash();
	}
	// ���֕����̂���߂�
	if (!left || m_leftLock || m_state == CHARACTER_STATE::SQUAT || damageFlag()) {
		stopMoveLeft();
	}
	if (m_slashCnt > 0 && m_attackLeftDirection && (m_leftLock || damageFlag())) {
		finishSlash();
	}
	// ��֕����̂���߂�
	if (!up || m_upLock || damageFlag()) {
		stopMoveUp();
	}
	// ���֕����̂���߂�
	if (!down || m_downLock || damageFlag()) {
		stopMoveDown();
	}
	if (damageFlag()) {
		return;
	}
	// �E�֕����n�߂�
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && (!left || !m_character_p->getLeftDirection()) && m_state != CHARACTER_STATE::SQUAT) { // �E�֕���
		startMoveRight();
	}
	// ���֕����n�߂�
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && (!right || m_character_p->getLeftDirection()) && m_state != CHARACTER_STATE::SQUAT) { // ���֕���
		startMoveLeft();
	}
	// ��֕����n�߂�
	if (!m_upLock && !m_moveDown && !m_moveUp && up && !down) { // ��֕���
		startMoveUp();
	}
	// ���֕����n�߂�
	if (!m_downLock && !m_moveUp && !m_moveDown && down && !up) { // ���֕���
		startMoveDown();
	}
	// �A�j���[�V�����p�ɃJ�E���g
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// �ړ� �����͂S������
void FlightAction::move(bool right, bool left, bool up, bool down) {
	if ((m_state == CHARACTER_STATE::STAND || m_state == CHARACTER_STATE::SQUAT) && m_slashCnt == 0 && m_bulletCnt == 0) {
		// �ړ������֌���
		if (left && !right) {
			m_character_p->setLeftDirection(true);
		}
		if (right && !left) {
			m_character_p->setLeftDirection(false);
		}
	}
	// �ړ�
	walk(right, left, up, down);
}

// �W�����v cnt�t���[����
void FlightAction::jump(int cnt) {
	
}

void FlightAction::setBoost(bool leftDirection) {
	if ((leftDirection && m_leftLock) || (!leftDirection && m_rightLock)) {
		return;
	}
	if (!damageFlag() && !m_grand && !m_boostDone) {
		m_boostCnt = BOOST_TIME;
		finishBullet();
		finishSlash();
		if (leftDirection) {
			m_vx -= BOOST_SPEED;
			m_boostDone = 2;
		}
		else {
			m_vx += BOOST_SPEED;
			m_boostDone = 1;
		}
		m_character_p->setLeftDirection(leftDirection);
	}
}

// �ˌ��U��
Object* FlightAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		finishBullet();
		return nullptr;
	}
	// �ˌ��\��ԂȂ�
	if (ableAttack()) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character_p->getBulletRapid();
		// �������֌���
		if (m_character_p->getCharacterInfo()->sameBulletDirection()) {
			m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		}
		startBullet();
		// �U����Ԃ�
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return nullptr;
}

// �a���U��
Object* FlightAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		if (m_slashCnt > 0) { finishSlash(); }
		return nullptr;
	}
	// �U���J�n
	if (ableAttack()) {
		m_attackLeftDirection = m_character_p->getCenterX() > gx;
		m_slashCnt = m_character_p->getSlashCountSum() + m_character_p->getSlashInterval();
		// �U���̕����֌���
		m_character_p->setLeftDirection(m_attackLeftDirection);
		// �a���J�n���̏���
		startSlash();
	}
	// �U���̃^�C�~���O����Ȃ��Ȃ�nullptr���Ԃ�
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_grand, m_soundPlayer_p);
}


/*
* �R�n���pAction �a�����Ɉړ�����
*/
KoharuAction::KoharuAction(Character* character, SoundPlayer* soundPlayer_p) :
	StickAction(character, soundPlayer_p)
{

}

CharacterAction* KoharuAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new KoharuAction(characters[i], m_soundPlayer_p);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}

// �ˌ��U��
Object* KoharuAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		finishBullet();
		return nullptr;
	}
	// �ˌ��\��ԂȂ�
	if (ableAttack()) {
		// �ˌ��s�\��Ԃɂ���
		m_bulletCnt = m_character_p->getBulletRapid();
		stopMoveRight();
		stopMoveLeft();
		if (m_bulletCnt == 0) {
			if (gx > m_character_p->getCenterX()) {
				m_vx -= BULLET_MOVE_SPEED;
			}
			else {
				m_vx += BULLET_MOVE_SPEED;
			}
		}
		// �������֌���
		if (m_character_p->getCharacterInfo()->sameBulletDirection()) {
			m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		}
		startBullet();
		// �U����Ԃ�
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return nullptr;
}

void KoharuAction::startBullet() {
	if (m_character_p->getLeftDirection()) {
		m_vx += BULLET_MOVE_SPEED;
	}
	else {
		m_vx -= BULLET_MOVE_SPEED;
	}
}

void KoharuAction::finishBullet() {
	if (m_bulletCnt > 0) { 
		StickAction::finishBullet();
	}
	if (m_character_p->getLeftDirection() && !m_rightLock) {
		m_vx -= BULLET_MOVE_SPEED;
	}
	else if(!m_leftLock){
		m_vx += BULLET_MOVE_SPEED;
	}
}

bool KoharuAction::ableAttack() const {
	return !(m_bulletCnt > 0 || m_slashCnt > 0);
}

bool KoharuAction::ableWalk() const {
	return StickAction::ableWalk() && m_bulletCnt == 0;
}


/*
* �s���J�n�O��Boss
*/
BossFreezeAction::BossFreezeAction(Character* character, SoundPlayer* soundPlayer_p) :
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* BossFreezeAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new BossFreezeAction(characters[i], m_soundPlayer_p);
			// �R�s�[����
			setParam(res);
		}
	}
	return res;
}
void BossFreezeAction::switchHandle() {
	m_character_p->switchSpecial1();
}


/*
* Boss1: �T��
*/
SunAction::SunAction(Character* character, SoundPlayer* soundPlayer_p, bool duplicationFlag) :
	FlightAction(character, soundPlayer_p)
{
	m_state = CHARACTER_STATE::INIT;
	m_initCnt = -60;
	m_initHp = m_character_p->getHp();
	if (!duplicationFlag) {
		m_character_p->setHp(min(1, m_initHp));
	}
	m_startAnimeCnt = 0;
}

CharacterAction* SunAction::createCopy(vector<Character*> characters) {
	SunAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new SunAction(characters[i], m_soundPlayer_p, true);
			// �R�s�[����
			setParam(res);
		}
	}
	res->setInitHp(m_initHp);
	res->setInitCnt(m_initCnt);
	res->setHideFlag(m_hideFlag);
	res->setStartAnimeCnt(m_startAnimeCnt);
	return res;
}

void SunAction::action() {
	// ���(state)�ɉ����ĉ摜���Z�b�g
	switchHandle();
	if (m_hideFlag && m_initCnt != 0) {
		// �B��r��
		m_initCnt--;
	}
	else if (!m_hideFlag && m_initCnt != NOT_HIDE_CNT) {
		// �o���r��
		m_initCnt++;
		if (m_state == CHARACTER_STATE::INIT) {
			m_character_p->setHp(min(m_character_p->getHp() + 10, m_initHp));
		}
		if (m_initCnt == NOT_HIDE_CNT && m_character_p->getHp() == m_initHp) {
			m_state = CHARACTER_STATE::STAND;
		}
	}
	else {
		m_startAnimeCnt++;
		// �B��E�o���̊J�n
		if (m_hideFlag) {
			// ���݉B����
			m_bulletCnt = 1;
			slashAction();
			damageAction();
			otherAction();
			moveAction();
			if (m_startAnimeCnt > 300 && (GetRand(120) == 0 || m_startAnimeCnt == 600)) {
				m_hideFlag = false;
				m_initCnt = 0;
				m_startAnimeCnt = 0;
			}
		}
		else {
			// ���ݏo�����
			bulletAction();
			slashAction();
			damageAction();
			otherAction();
			if (m_startAnimeCnt > 300 && (GetRand(300) == 0 || m_startAnimeCnt == 600)) {
				m_hideFlag = true;
				m_initCnt = NOT_HIDE_CNT - 1;
				m_startAnimeCnt = 0;
			}
		}
	}
}

// ��Ԃɉ����ĉ摜�Z�b�g
void SunAction::switchHandle() {

	m_prevLeftDirection = false;
	m_character_p->setLeftDirection(false);

	if (m_initCnt != NOT_HIDE_CNT) {
		// �Z�b�g�O�̉摜�̃T�C�Y
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		bool nowLeftDirection = false;
		m_character_p->setLeftDirection(false);
		m_character_p->getAtariArea(&x1, &y1, &x2, &y2);

		// �摜�Z�b�g
		if (m_initCnt == 0 && m_hideFlag) {
			m_character_p->switchSpecial1();
		}
		else {
			m_character_p->switchInit(m_initCnt);
		}

		m_character_p->setLeftDirection(nowLeftDirection);

		// �Z�b�g��̉摜�̃T�C�Y
		int afterX1 = 0, afterY1 = 0, afterX2 = 0, afterY2 = 0;
		m_character_p->getAtariArea(&afterX1, &afterY1, &afterX2, &afterY2);

		// �T�C�Y�ύX�ɂ��ʒu����
		afterChangeGraph(x1, afterX1, y1, afterY1, x2, afterX2, y2, afterY2);
	}
	else {
		FlightAction::switchHandle();
	}
}