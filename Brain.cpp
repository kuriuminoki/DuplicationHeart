#include "Brain.h"
#include "Character.h"
#include "CharacterAction.h"
#include "Camera.h"
#include "Control.h"
#include "Object.h"
#include "DxLib.h"
#include <cmath>


using namespace std;


// �N���X��
const char* Brain::BRAIN_NAME = "Brain";
const char* KeyboardBrain::BRAIN_NAME = "KeyboardBrain";
const char* Freeze::BRAIN_NAME = "Freeze";
const char* NormalAI::BRAIN_NAME = "NormalAI";
const char* ParabolaAI::BRAIN_NAME = "ParabolaAI";
const char* FollowNormalAI::BRAIN_NAME = "FollowNormalAI";
const char* FollowParabolaAI::BRAIN_NAME = "FollowParabolaAI";
const char* ValkiriaAI::BRAIN_NAME = "ValkiriaAI";
const char* FlightAI::BRAIN_NAME = "FlightAI";
const char* FollowFlightAI::BRAIN_NAME = "FollowFlightAI";

// �N���X������Brain���쐬����֐�
Brain* createBrain(const string brainName, const Camera* camera_p) {
	Brain* brain = nullptr;
	if (brainName == KeyboardBrain::BRAIN_NAME) {
		brain = new KeyboardBrain(camera_p);
	}
	else if (brainName == NormalAI::BRAIN_NAME) {
		brain = new NormalAI();
	}
	else if (brainName == ParabolaAI::BRAIN_NAME) {
		brain = new ParabolaAI();
	}
	else if (brainName == FollowNormalAI::BRAIN_NAME) {
		brain = new FollowNormalAI();
	}
	else if (brainName == FollowParabolaAI::BRAIN_NAME) {
		brain = new FollowParabolaAI();
	}
	else if (brainName == Freeze::BRAIN_NAME) {
		brain = new Freeze();
	}
	else if (brainName == ValkiriaAI::BRAIN_NAME) {
		brain = new ValkiriaAI();
	}
	else if (brainName == FlightAI::BRAIN_NAME) {
		brain = new FlightAI();
	}
	else if (brainName == FollowFlightAI::BRAIN_NAME) {
		brain = new FollowFlightAI();
	}
	return brain;
}


// Brain�N���X
Brain::Brain() {
	m_characterAction_p = NULL;
}


/*
* �L�[�{�[�h
*/
KeyboardBrain::KeyboardBrain(const Camera* camera) {
	m_camera_p = camera;
}

void KeyboardBrain::bulletTargetPoint(int& x, int& y) {
	// �}�E�X�̈ʒu
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// �J�����ō��W��␳
	m_camera_p->getMouse(&mouseX, &mouseY);

	x = mouseX;
	y = mouseY;
}

// �b�������������������
bool KeyboardBrain::actionOrder() {
	return controlW() == 1;
}

// �ړ��i�㉺���E�̓��́j
void KeyboardBrain::moveOrder(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}

// �W�����v
int KeyboardBrain::jumpOrder() {
	return controlSpace();
}

// ���Ⴊ��
int KeyboardBrain::squatOrder() {
	return controlS();
}

// �ߋ����U��
int KeyboardBrain::slashOrder() {
	return rightClick();
}

// �������U��
int KeyboardBrain::bulletOrder() {
	return leftClick();
}


/*
* Normal AI
*/
NormalAI::NormalAI() {
	m_target_p = NULL;
	m_gx = 0;
	m_gy = 0;
	m_rightKey = 0;
	m_leftKey = 0;
	m_upKey = 0;
	m_downKey = 0;
	m_jumpCnt = 0;
	m_squatCnt = 0;
	m_moveCnt = 0;
}


Brain* NormalAI::createCopy(std::vector<Character*> characters, const Camera* camera) {
	NormalAI* res = new NormalAI();

	if (m_target_p != NULL) {
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (m_target_p->getId() == characters[i]->getId()) {
				res->setTarget(characters[i]);
				break;
			}
		}
	}
	setParam(res);
	return res;
}

void NormalAI::setParam(NormalAI* brain) {
	// �ړ��p
	brain->setRightKey(m_rightKey);
	brain->setLeftKey(m_leftKey);
	brain->setUpKey(m_upKey);
	brain->setDownKey(m_downKey);
	// �W�����v�̒���
	brain->setJumpCnt(m_jumpCnt);
	// ���Ⴊ�ޒ���
	brain->setSquatCnt(m_squatCnt);
	// �ړ��ڕW
	brain->setGx(m_gx);
	brain->setGy(m_gy);
	brain->setMoveCnt(m_moveCnt);
}

void NormalAI::setCharacterAction(const CharacterAction* characterAction) {
	m_characterAction_p = characterAction;
	// �ڕW�n�_�͌��ݒn�ɐݒ�
	m_gx = m_characterAction_p->getCharacter()->getCenterX();
	m_gy = m_characterAction_p->getCharacter()->getCenterY();
}

void NormalAI::bulletTargetPoint(int& x, int& y) {
	if (m_target_p == NULL) {
		x = 0;
		y = 0;
	}
	else { // �^�[�Q�b�g�Ɍ������Ďˌ��U��
		x = m_target_p->getCenterX() + (GetRand(BULLET_ERROR) - BULLET_ERROR / 2);
		y = m_target_p->getCenterY() + (GetRand(BULLET_ERROR) - BULLET_ERROR / 2);
	}
}

void NormalAI::moveOrder(int& right, int& left, int& up, int& down) {
	// ���ݒn
	int x = m_characterAction_p->getCharacter()->getCenterX();
	int y = m_characterAction_p->getCharacter()->getCenterY();

	// (�ǂɂ�������Ȃǂ�)�ړ��ł��ĂȂ�������߂�
	if (m_moveCnt >= GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// �ڕW�n�_�ݒ�
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	// �_���[�W���󂯂��烊�Z�b�g
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_gx = x, m_gy = y;
	}
	else if (alreadyGoal && GetRand(MOVE_RAND) == 0) {
		if (m_target_p != NULL && abs(x - m_target_p->getCenterX()) < TARGET_DISTANCE) {
			// target�ɂ��Ă���
			m_gx = m_target_p->getCenterX() + GetRand(2000) - 1000;
		}
		else {
			// �����_���ɐݒ�
			m_gx = GetRand(200) - 400;
			m_gx += x;
		}
		if (abs(x - m_gx) < 50) { m_gx = x; }
	}
	stickOrder(right, left, up, down);
}

// �X�e�B�b�N����
void NormalAI::stickOrder(int& right, int& left, int& up, int& down) {
	// ���ݒn
	int x = m_characterAction_p->getCharacter()->getCenterX();
	int y = m_characterAction_p->getCharacter()->getCenterY();

	// �ڕW�Ɍ������đ���
	if (m_gx > x + GX_ERROR) {
		m_rightKey++;
		m_leftKey = 0;
		m_moveCnt++;
	}
	else if (m_gx < x - GX_ERROR) {
		m_rightKey = 0;
		m_leftKey++;
		m_moveCnt++;
	}
	else {
		if (m_moveCnt == 0) {
			m_rightKey = 0;
			m_leftKey = 0;
		}
		else {
			m_moveCnt = 0;
		}
	}

	// �ڕW�Ɍ������ď㉺�ړ�
	if (m_gy > y + GY_ERROR && !m_characterAction_p->getGrand()) {
		m_downKey++;// ���ɂ���Ȃ牺�ֈړ��A��������Ȃ��Ȃ炵�Ⴊ�񂶂Ⴄ����
		m_upKey = 0;
	}
	else if (m_gy < y - GY_ERROR) {
		m_downKey = 0;
		m_upKey++;
	}
	else {
		m_downKey = 0;
		m_upKey = 0;
	}

	// ���f
	right = m_rightKey;
	left = m_leftKey;
	up = m_upKey;
	down = m_downKey;
}

int NormalAI::jumpOrder() {
	// �_���[�W��H������烊�Z�b�g
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_jumpCnt = 0;
		if (GetRand(120) == 0) { return 1; }
	}

	int maxJump = m_characterAction_p->getPreJumpMax();
	int minJump = maxJump / 3;

	if (m_jumpCnt == 0) {
		// �����_���ŃW�����v
		if (m_squatCnt == 0 && GetRand(99) == 0) { m_jumpCnt = GetRand(maxJump - minJump) + minJump; }

		// �ǂɂԂ���������W�����v
		if (m_rightKey > 0 && m_characterAction_p->getRightLock()) { m_jumpCnt = maxJump; }
		else if (m_leftKey > 0 && m_characterAction_p->getLeftLock()) { m_jumpCnt = maxJump; }
	}

	if (m_jumpCnt > 0) { m_jumpCnt--; }
	return m_jumpCnt == 0 ? 0 : maxJump - m_jumpCnt;
}

int NormalAI::squatOrder() {
	// �_���[�W��H������烊�Z�b�g
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_squatCnt = 0;
	}

	// �ڕW�n�_�ɂ��Ȃ��Ȃ炵�Ⴊ�܂Ȃ�
	int x = m_characterAction_p->getCharacter()->getCenterX();
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	if (!alreadyGoal) { m_squatCnt = 0; }

	// �����_���ł��Ⴊ��
	if (alreadyGoal && m_characterAction_p->getGrand() && GetRand(99) == 0) {
		m_squatCnt = GetRand(120) + 60;
	}

	if (m_squatCnt > 0) { m_squatCnt--; }
	return m_squatCnt;
}

int NormalAI::slashOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return 0;
	}
	// �������̓G�ɂ͎a�����Ȃ�
	if (abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) > 500) {
		return 0;
	}
	// �����_���Ŏa��
	if (GetRand(50) == 0) {
		return 1;
	}
	return 0;
}

int NormalAI::bulletOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return 0;
	}
	int x = m_characterAction_p->getCharacter()->getCenterX();
	if (abs(x - m_target_p->getCenterX()) > TARGET_DISTANCE) {
		return 0;
	}
	// �����_���Ŏˌ�
	int rapid = m_characterAction_p->getCharacter()->getAttackInfo()->bulletRapid();
	if (GetRand(rapid) == 0) {
		return 1;
	}
	return 0;
}

// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
void NormalAI::searchTarget(const Character* character) {
	if (GetRand(99) < 50) {
		int x = m_characterAction_p->getCharacter()->getCenterX();
		// ����������
		if (abs(x - character->getCenterX()) > TARGET_DISTANCE) {
			return;
		}
		// ��������Ȃ����&��������Ȃ����
		int groupId = character->getGroupId();
		if (groupId != m_characterAction_p->getCharacter()->getGroupId() && groupId != -1) {
			m_target_p = character;
		}
	}
}

// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
bool NormalAI::needSearchTarget() const {
	// �^�[�Q�b�g�����߂Ă��Ȃ�����
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return true;
	}
	// �C�܂���ŁAor�s�K�؂ȑ��肾����
	if (GetRand(99) == 0 || m_target_p->getGroupId() == -1) {
		return true;
	}
	// ���̃^�[�Q�b�g�͋�������������
	if (abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) > TARGET_DISTANCE) {
		return true;
	}
	return false;
}

int  NormalAI::getTargetId() const { return m_target_p == nullptr ? -1 : m_target_p->getId(); }

const char*  NormalAI::getTargetName() const { return m_target_p == nullptr ? "" : m_target_p->getName().c_str(); }

// �Ε����˂̌v�Z������
void setParabolaBulletTarget(int& x, int& y, const CharacterAction* characterAction_p, const Character* target_p) {
	if (target_p == NULL) {
		x = 0;
		y = 0;
	}
	else { // �^�[�Q�b�g�Ɍ������Ďˌ��U��
		const int G = -ParabolaBullet::G;
		int dx = target_p->getCenterX() - characterAction_p->getCharacter()->getCenterX();
		int gx = abs(dx);
		int gy = -(target_p->getCenterY() - characterAction_p->getCharacter()->getCenterY());
		int v = characterAction_p->getCharacter()->getAttackInfo()->bulletSpeed();
		double A = (G * gx * gx) / (2 * v * v);
		double a = gx / A;
		double b = 1 - (gy / A);
		double routeInside = a * a / 4 - b;
		if (routeInside >= 0) {
			double route = sqrt(routeInside);
			if (GetRand(99) < 50) { route *= -1; }
			double r = atan(route - (a / 2));
			if (dx > 0) {
				x = (int)(characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(characterAction_p->getCharacter()->getCenterY() - v * sin(r));
		}
		else {
			// �˒��O�Ȃ�45�x�œ�����
			double r = 3.14 / 4;
			if (dx > 0) {
				x = (int)(characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(characterAction_p->getCharacter()->getCenterY() - v * sin(r));
		}
	}
}

void ParabolaAI::bulletTargetPoint(int& x, int& y) {
	setParabolaBulletTarget(x, y, m_characterAction_p, m_target_p);
}

void FollowParabolaAI::bulletTargetPoint(int& x, int& y) {
	setParabolaBulletTarget(x, y, m_characterAction_p, m_target_p);
}


/*
* �L�����ɂ��Ă���NormalAI
*/
FollowNormalAI::FollowNormalAI() :
	NormalAI()
{
	m_follow_p = NULL;
}

Brain* FollowNormalAI::createCopy(std::vector<Character*> characters, const Camera* camera) {
	FollowNormalAI* res = new FollowNormalAI();
	if (m_follow_p != NULL) {
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (m_follow_p->getId() == characters[i]->getId()) {
				res->setFollow(characters[i]);
				break;
			}
		}
	}
	if (m_target_p != NULL) {
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (m_target_p->getId() == characters[i]->getId()) {
				res->setTarget(characters[i]);
				break;
			}
		}
	}
	setParam(res);
	return res;
}

int FollowNormalAI::getFollowId() const { return m_follow_p == nullptr ? -1 : m_follow_p->getId(); }

const char* FollowNormalAI::getFollowName() const { return m_follow_p == nullptr ? "�n�[�g" : m_follow_p->getName().c_str(); }
const Character* FollowNormalAI::getFollow() const {
	return m_follow_p;
}

bool FollowNormalAI::checkAlreadyFollow() {
	if (m_follow_p == nullptr) { return true; }
	int followX = m_follow_p->getCenterX();
	return  m_gx < followX + FOLLOW_X_ERROR && m_gx > followX - FOLLOW_X_ERROR;
}

void FollowNormalAI::moveOrder(int& right, int& left, int& up, int& down) {
	// ���ݒn
	int x = m_characterAction_p->getCharacter()->getCenterX();
	int y = m_characterAction_p->getCharacter()->getCenterY();

	// (�ǂɂ�������Ȃǂ�)�ړ��ł��ĂȂ�������߂�
	if (m_moveCnt >= GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// �ڕW�n�_�ݒ�p�p�����[�^
	int followX = m_follow_p->getCenterX();
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	bool alreadyFollow = checkAlreadyFollow();

	// �ڕW�n�_�ݒ�
	if ((alreadyGoal && GetRand(MOVE_RAND) == 0) || !alreadyFollow) {
		if (m_follow_p != NULL) {
			// follow�ɂ��Ă���
			m_gx = m_follow_p->getCenterX() + GetRand(FOLLOW_X_ERROR * 2) - FOLLOW_X_ERROR;
		}
		else {
			// �����_���ɐݒ�
			m_gx = GetRand(400) - 200;
			m_gx += x;
		}
		if (abs(x - m_gx) < 50) { m_gx = x; }
	}

	stickOrder(right, left, up, down);
}

// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
void FollowNormalAI::searchFollow(const Character* character) {
	// �����̂�
	if (character->getId() != m_characterAction_p->getCharacter()->getId() && character->getGroupId() == m_characterAction_p->getCharacter()->getGroupId()) {
		m_follow_p = character;
	}
}

// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
bool FollowNormalAI::needSearchFollow() const {
	if (m_follow_p == NULL || m_follow_p->getHp() == 0) {
		return true;
	}
	return false;
}


/*
* ���@���L���A�pAI
*/
bool ValkiriaAI::checkAlreadyFollow() {
	// �퓬���̓G���߂��ɂ���Ȃ�n�[�g�Ƃ̋������C�ɂ��Ȃ�
	if (m_target_p != nullptr) {
		if (m_target_p->getHp() > 0) {
			if (abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) <= 2000) {
				return true;
			}
		}
	}
	// �n�[�g�ɋ߂�������
	return FollowNormalAI::checkAlreadyFollow();
}

int ValkiriaAI::slashOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return 0;
	}
	// �������̓G�ɂ͎a�����Ȃ�
	if (abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) > 2000) {
		return 0;
	}
	// �����_���Ŏa��
	if (GetRand(30) == 0) {
		return 1;
	}
	return 0;
}

int ValkiriaAI::bulletOrder() {
	return 0;
}

void ValkiriaAI::moveOrder(int& right, int& left, int& up, int& down) {
	if (m_characterAction_p->getSlashCnt() > 0) {
		// �U�����͈ړ����Ȃ�
		right = 0; left = 0; up = 0; down = 0;
	}
	else {
		FollowNormalAI::moveOrder(right, left, up, down);
	}
}


void FlightAI::moveOrder(int& right, int& left, int& up, int& down) {
	// ���ݒn
	int x = m_characterAction_p->getCharacter()->getCenterX();
	int y = m_characterAction_p->getCharacter()->getCenterY();

	// (�ǂɂ�������Ȃǂ�)�ړ��ł��ĂȂ�������߂�
	if (m_moveCnt >= GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
		m_try = false;
	}

	// �ǂɂԂ���������㉺�ړ�
	if ((m_rightKey > 0 && m_characterAction_p->getRightLock()) || (m_leftKey > 0 && m_characterAction_p->getLeftLock())) {
		if (!m_try) {
			if (GetRand(100) < 50) {
				m_gy = y - 1000;
			}
			else {
				m_gy = y + 1000;
			}
			m_try = true;
		}
		else if (m_upKey > 0 && m_characterAction_p->getUpLock()) {
			m_gy = y + 2000;
		}
		else if (m_downKey > 0 && m_characterAction_p->getDownLock()) {
			m_gy = y - 2000;
		}
	}
	else {
		if(m_try){ m_gy = y; }
		m_try = false;
	}

	// �ڕW�n�_�ݒ�
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	// �_���[�W���󂯂��烊�Z�b�g
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_gx = x, m_gy = y;
		m_try = false;
	}
	else if (alreadyGoal && GetRand(MOVE_RAND) == 0) {
		if (m_target_p != NULL && abs(x - m_target_p->getCenterX()) < TARGET_DISTANCE) {
			// target�ɂ��Ă���
			m_gx = m_target_p->getCenterX() + GetRand(2000) - 1000;
			m_gy = m_target_p->getCenterY() + GetRand(800) - 700;
		}
		else {
			// �����_���ɐݒ�
			m_gx = GetRand(200) - 400;
			m_gx += x;
			m_gy = y + (GetRand(200) - 100);
		}
		if (abs(x - m_gx) < 50) { m_gx = x; }
		m_try = false;
	}
	stickOrder(right, left, up, down);
}