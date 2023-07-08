#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
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
* �L�[�{�[�h�ɂ��L�����R���g���[�� �}�E�X���g���̂�Camera���K�v
*/
CharacterKeyboardController::CharacterKeyboardController(CharacterAction* characterAction, const Camera* camera):
	CharacterController(characterAction)
{
	m_camera = camera;
	m_rightStick = 0;
	m_leftStick = 0;
	m_upStick = 0;
	m_downStick = 0;
	m_jumpKey = 0;
}

void CharacterKeyboardController::control() {
	// �L�����̌�����ς���
	// �}�E�X�̏��擾�i�J�������g�p�j
	int mouseX, mouseY;
	m_camera->getMouse(&mouseX, &mouseY);
	
	// �}�E�X�ƃL�����̈ʒu�֌W������
	m_characterAction->setCharacterLeftDirection(mouseX < m_characterAction->getCharacter()->getX());

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

Object* CharacterKeyboardController::bulletAttack() {
	// ���N���b�N����Ă���Ȃ�
	if (leftClick() > 0) {
		// �}�E�X�̏��擾
		int mouseX, mouseY;
		m_camera->getMouse(&mouseX, &mouseY);
		// �}�E�X�̈ʒu�Ɍ������Ďˌ�
		return m_characterAction->bulletAttack(mouseX, mouseY);
	}
	return NULL;
}

Object* CharacterKeyboardController::slashAttack() {
	// �E�N���b�N���ꂽ���A������ō����U���^�C�~���O�Ȃ�
	if (rightClick() == 1 || m_characterAction->getSlashCnt() > 0) {
		return m_characterAction->slashAttack();
	}
	return NULL;
}