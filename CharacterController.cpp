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


/*
* �L�[�{�[�h
*/
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
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
}

void CharacterKeyboardController::control() {
	// �������Z
	m_characterAction->action();
	
	// �ړ�
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// �摜�ω�
	m_characterAction->switchHandle();
}