#include "CharacterController.h"
#include "CharacterAction.h"
#include "Control.h"
#include "DxLib.h"


/*
* �R���g���[��
*/
CharacterController::CharacterController(CharacterAction* characterAction) {
	m_characterAction = characterAction;
}


/*
* �L�[�{�[�h
*/
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlA();
	left = controlD();
	up = controlW();
	down = controlS();
}


/*
* �L�[�{�[�h�ɂ��L�����R���g���[��
*/
void CharacterKeyboardController::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	DrawFormatString(x, y + 10, color, "(��, ��, ��, ��)=(%d,%d,%d,%d)", m_rightStick, m_leftStick, m_upStick, m_downStick);
}

void CharacterKeyboardController::control() {
	// �������Z
	m_characterAction->action();
	
	// �ړ�
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);
}