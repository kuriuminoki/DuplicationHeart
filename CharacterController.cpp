#include "CharacterController.h"
#include "CharacterAction.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


/*
* コントローラ
*/
CharacterController::CharacterController(CharacterAction* characterAction) {
	m_characterAction = characterAction;
}

CharacterController::CharacterController() :
	CharacterController(NULL)
{

}

// Actionインスタンスはここでのみデリートする
CharacterController::~CharacterController() {
	if (m_characterAction != NULL) {
		delete m_characterAction;
	}
}


/*
* キーボード
*/
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}


/*
* キーボードによるキャラコントロール
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
	// 物理演算
	m_characterAction->action();
	
	// 移動
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// 画像変化
	m_characterAction->switchHandle();
}