#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


// Brain�N���X
Brain::Brain() {
	m_characterAction = NULL;
}


/*
* �L�[�{�[�h
*/
KeyboardBrain::KeyboardBrain(const Camera* camera) {
	m_camera = camera;
}

void KeyboardBrain::bulletTargetPoint(int& x, int& y) {
	// �}�E�X�̈ʒu
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// �J�����ō��W��␳
	m_camera->getMouse(&mouseX, &mouseY);

	x = mouseX;
	y = mouseY;
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


NormalAI::NormalAI() {
	m_target = NULL;
}

void NormalAI::bulletTargetPoint(int& x, int& y) {
	if (m_target == NULL) {
		x = 0;
		y = 0;
	}
	else { // �^�[�Q�b�g�Ɍ������Ďˌ��U��
		x = m_target->getCenterX();
		y = m_target->getCenterY();
	}
}

void NormalAI::moveOrder(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}

int NormalAI::jumpOrder() {
	return controlSpace();
}

int NormalAI::squatOrder() {
	return controlS();
}

int NormalAI::slashOrder() {
	return rightClick();
}

int NormalAI::bulletOrder() {
	return leftClick();
}

// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
void NormalAI::searchTarget(Character* character) {
	if (m_target == NULL || m_target->getHp() == 0) {
		// �������g�□������Ȃ����
		if (character->getId() != m_characterAction->getCharacter()->getId()) {
			m_target = character;
		}
	}
}

// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
bool NormalAI::needSearchTarget() const {
	if (m_target == NULL || GetRand(99) == 0) {
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
		return m_characterAction->slashAttack();
	}
	return NULL;
}