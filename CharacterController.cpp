#include "CharacterController.h"
#include "CharacterAction.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


/*
* �R���g���[��
*/
CharacterController::CharacterController(CharacterAction* characterAction) {
	m_characterAction = characterAction;
}

CharacterController::CharacterController() :
	CharacterController(NULL)
{

}

// Action�C���X�^���X�͂����ł̂݃f���[�g����
CharacterController::~CharacterController() {
	if (m_characterAction != NULL) {
		delete m_characterAction;
	}
}

// �L�����N�^�[�̏��擾
// �L�����N�^�[��X���W�擾
int CharacterController::getCharacterX() {
	return m_characterAction->getCharacterX();
}

// �L�����N�^�[��Y���W�擾
int CharacterController::getCharacterY() {
	return m_characterAction->getCharacterY();
}

// �L�����N�^�[�̉����擾
int CharacterController::getCharacterWide() {
	return m_characterAction->getCharacterWide();
}

// �L�����N�^�[�̏c���擾
int CharacterController::getCharacterHeight() {
	return m_characterAction->getCharacterHeight();
}

// �A�N�V�����̏��擾
// �L�����N�^�[��VX�擾
int CharacterController::getCharacterVx() {
	return m_characterAction->getVx();
}

// �L�����N�^�[��VY�擾
int CharacterController::getCharacterVy() {
	return m_characterAction->getVy();
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

// �s���̌��ʔ��f
void CharacterController::action() {
	// �������Z
	m_characterAction->action();
}


/*
* �L�[�{�[�h
*/
CharacterKeyboard::CharacterKeyboard() {
	m_nowSpaceKey = 0;
	m_preSpaceKey = 0;
}
// �㉺���E�̃L�[
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}

// �X�y�[�X�L�[
void CharacterKeyboard::controlJump(int& nowSpaceKey, int& preSpaceKey) {
	m_preSpaceKey = m_nowSpaceKey;
	m_nowSpaceKey = controlSpace();

	preSpaceKey = m_preSpaceKey;
	nowSpaceKey = m_nowSpaceKey;
}


/*
* �L�[�{�[�h�ɂ��L�����R���g���[��
*/
CharacterKeyboardController::CharacterKeyboardController(CharacterAction* characterAction):
	CharacterController(characterAction)
{
	m_rightStick = 0;
	m_leftStick = 0;
	m_upStick = 0;
	m_downStick = 0;
	m_jumpKey = 0;
}

void CharacterKeyboardController::control() {
	// �ړ� stick�Ȃǂ̓��͏�Ԃ��X�V����
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// stick�ɉ����Ĉړ�
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// �W�����v
	int nowSpaceKey = 0;
	// 1�t���[���O�̃X�y�[�X�L�[���擾
	m_keyboard.controlJump(nowSpaceKey, m_jumpKey);

	// �W�����v����
	if ((nowSpaceKey == 0 && m_jumpKey > 0 && m_jumpKey < JUMP_KEY_LONG) || m_jumpKey == JUMP_KEY_LONG) {
		m_characterAction->jump((100 * m_jumpKey) / JUMP_KEY_LONG);
	}
}