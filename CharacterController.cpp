#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>


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

void NormalAI::setCharacterAction(const CharacterAction* characterAction) { 
	m_characterAction_p = characterAction;
	// �ڕW�n�_�͌��ݒn�ɐݒ�
	m_gx = m_characterAction_p->getCharacter()->getX();
	m_gy = m_characterAction_p->getCharacter()->getY();
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
	int x = m_characterAction_p->getCharacter()->getX();
	int y = m_characterAction_p->getCharacter()->getY();

	// (�ǂɂ�������Ȃǂ�)�ړ��ł��ĂȂ�������߂�
	//DrawFormatString(800, 50, GetColor(255, 255, 255), "moveCnt = %d, x(%d) -> gx(%d)", m_moveCnt, x, m_gx);
	if (m_moveCnt == GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// �ڕW�n�_�ݒ�
	if (m_gx > x - GX_ERROR && m_gx < x + GX_ERROR && GetRand(99) == 0) {
		if (m_target_p != NULL) {
			// target�ɂ��Ă���
			m_gx = m_target_p->getCenterX() + GetRand(2000) - 1000;
		}
		else {
			// �����_���ɐݒ�
			m_gx = GetRand(200) + 100;
			if (GetRand(99) < GX_ERROR) { m_gx *= -1; }
			m_gx += x;
		}
	}

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
		m_rightKey = 0;
		m_leftKey = 0;
		m_moveCnt = 0;
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
	}

	// �����_���ŃW�����v
	if (m_squatCnt == 0 && GetRand(99) == 0) { m_jumpCnt = GetRand(15) + 5; }

	// �ǂɂԂ���������W�����v
	if (m_rightKey > 0 && m_characterAction_p->getRightLock()) { m_jumpCnt = 20; }
	else if (m_leftKey > 0 && m_characterAction_p->getLeftLock()) { m_jumpCnt = 20; }

	if (m_jumpCnt > 0) { m_jumpCnt--; }
	return m_jumpCnt;
}

int NormalAI::squatOrder() {
	// �_���[�W��H������烊�Z�b�g
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_squatCnt = 0;
	}

	// �����_���ł��Ⴊ��
	if (m_characterAction_p->getGrand() && GetRand(99) == 0) { m_squatCnt = GetRand(60) + 30; }

	if (m_squatCnt > 0) { m_squatCnt--; }
	return m_squatCnt;
}

int NormalAI::slashOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return 0;
	}
	// �������̓G�ɂ͎a�����Ȃ�
	if (m_target_p != NULL && abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) > 500) {
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
	// �����_���Ŏˌ�
	if (GetRand(50) == 0) { 
		return 1;
	}
	return 0;
}

// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
void NormalAI::searchTarget(const Character* character) {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		// �������g�□������Ȃ����
		if (character->getId() != m_characterAction_p->getCharacter()->getId() && character->getGroupId() != m_characterAction_p->getCharacter()->getGroupId()) {
			m_target_p = character;
		}
	}
}

// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
bool NormalAI::needSearchTarget() const {
	if (m_target_p == NULL || GetRand(99) == 0) {
		return true;
	}
	return false;
}


/*
* �R���g���[��
*/
CharacterController::CharacterController(Brain* brain, CharacterAction* characterAction) {

	// ������
	m_brain = brain;
	m_characterAction = characterAction;

	// Brain��Action�𒍓�
	m_brain->setCharacterAction(m_characterAction);
}

CharacterController::CharacterController() :
	CharacterController(NULL, NULL)
{

}

// Action�C���X�^���X�͂����ł̂݃f���[�g����
CharacterController::~CharacterController() {
	if (m_characterAction != NULL) {
		delete m_characterAction;
	}
	if (m_brain != NULL) {
		delete m_brain;
	}
}

// �A�N�V�����̃Z�b�^
void CharacterController::setCharacterGrand(bool grand) {
	m_characterAction->setGrand(grand);
}
void CharacterController::setCharacterGrandRightSlope(bool grand) {
	m_characterAction->setGrandRightSlope(grand);
}
void CharacterController::setCharacterGrandLeftSlope(bool grand) {
	m_characterAction->setGrandLeftSlope(grand);
}
void CharacterController::setActionRightLock(bool lock) {
	m_characterAction->setRightLock(lock);
}
void CharacterController::setActionLeftLock(bool lock) {
	m_characterAction->setLeftLock(lock);
}
void CharacterController::setActionUpLock(bool lock) {
	m_characterAction->setUpLock(lock);
}
void CharacterController::setActionDownLock(bool lock) {
	m_characterAction->setDownLock(lock);
}
void CharacterController::setActionBoost() {
	m_characterAction->setBoost();
}

// �L�����N�^�[�̃Z�b�^
void CharacterController::setCharacterX(int x) {
	m_characterAction->setCharacterX(x);
}
void CharacterController::setCharacterY(int y) {
	m_characterAction->setCharacterY(y);
}

// �s���O�̏���
void CharacterController::init() {
	m_characterAction->init();
}

// �U���Ώۂ�ύX�i���邩���j
void CharacterController::searchTargetCandidate(Character* character) {
	m_brain->searchTarget(character);
}

// �s���̌��ʔ��f
void CharacterController::action() {
	// �������Z
	m_characterAction->action();
}


/*
* �L�[�{�[�h�ɂ��L�����R���g���[�� �}�E�X���g���̂�Camera���K�v
*/
NormalController::NormalController(Brain* brain, CharacterAction* characterAction):
	CharacterController(brain, characterAction)
{

}

void NormalController::control() {
	// �ړ� stick�Ȃǂ̓��͏�Ԃ��X�V����
	int rightStick, leftStick, upStick, downStick;
	m_brain->moveOrder(rightStick, leftStick, upStick, downStick);

	// stick�ɉ����Ĉړ�
	m_characterAction->move(rightStick, leftStick, upStick, downStick);

	// �W�����v
	int jump = m_brain->jumpOrder();
	m_characterAction->jump(jump);

	// ���Ⴊ��
	int squat = m_brain->squatOrder();
	m_characterAction->setSquat(squat);
}

Object* NormalController::bulletAttack() {
	// �������U���̖��߂�����Ă���Ȃ�
	if (m_brain->bulletOrder() > 0) {
		// �U���ڕW
		int targetX, targetY;
		m_brain->bulletTargetPoint(targetX, targetY);
		// �ڕW�Ɍ������Ďˌ�
		return m_characterAction->bulletAttack(targetX, targetY);
	}
	return NULL;
}

Object* NormalController::slashAttack() {
	// �ߋ����U���̖��߂����ꂽ���A������ō����U���^�C�~���O�Ȃ�
	if (m_brain->slashOrder() == 1 || m_characterAction->getSlashCnt() > 0) {
		// �U���ڕW
		int targetX, targetY;
		m_brain->bulletTargetPoint(targetX, targetY);
		return m_characterAction->slashAttack(targetX, targetY);
	}
	return NULL;
}

void NormalController::damage(int vx, int vy, int damageValue) {
	m_characterAction->damage(vx, vy, damageValue);
}