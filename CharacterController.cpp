#include "CharacterController.h"
#include "CharacterAction.h"
#include "Control.h"
#include "DxLib.h"


/*
* コントローラ
*/
CharacterController::CharacterController(CharacterAction* characterAction) {
	m_characterAction = characterAction;
}


/*
* キーボード
*/
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlA();
	left = controlD();
	up = controlW();
	down = controlS();
}


/*
* キーボードによるキャラコントロール
*/
void CharacterKeyboardController::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	DrawFormatString(x, y + 10, color, "(←, →, ↑, ↓)=(%d,%d,%d,%d)", m_rightStick, m_leftStick, m_upStick, m_downStick);
}

void CharacterKeyboardController::control() {
	// 物理演算
	m_characterAction->action();
	
	// 移動
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);
}