#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
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

// アクションのセッタ
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

// キャラクターのセッタ
void CharacterController::setCharacterX(int x) {
	m_characterAction->setCharacterX(x);
}
void CharacterController::setCharacterY(int y) {
	m_characterAction->setCharacterY(y);
}

// 行動前の処理
void CharacterController::init() {
	m_characterAction->init();
}

// 行動の結果反映
void CharacterController::action() {
	// 物理演算
	m_characterAction->action();
}


/*
* キーボード
*/
CharacterKeyboard::CharacterKeyboard() {
	m_nowSpaceKey = 0;
	m_preSpaceKey = 0;
}
// 上下左右のキー
void CharacterKeyboard::controlStick(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}

// スペースキー
void CharacterKeyboard::controlJump(int& nowSpaceKey, int& preSpaceKey) {
	m_preSpaceKey = m_nowSpaceKey;
	m_nowSpaceKey = controlSpace();

	preSpaceKey = m_preSpaceKey;
	nowSpaceKey = m_nowSpaceKey;
}


/*
* キーボードによるキャラコントロール マウスも使うのでCameraが必要
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
	// キャラの向きを変える
	// マウスの情報取得（カメラを使用）
	int mouseX, mouseY;
	m_camera->getMouse(&mouseX, &mouseY);
	
	// マウスとキャラの位置関係を見る
	m_characterAction->setCharacterLeftDirection(mouseX < m_characterAction->getCharacter()->getX());

	// 移動 stickなどの入力状態を更新する
	m_keyboard.controlStick(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// stickに応じて移動
	m_characterAction->move(m_rightStick, m_leftStick, m_upStick, m_downStick);

	// ジャンプ
	int nowSpaceKey = 0;
	// 1フレーム前のスペースキーを取得
	m_keyboard.controlJump(nowSpaceKey, m_jumpKey);

	// ジャンプする
	if ((nowSpaceKey == 0 && m_jumpKey > 0 && m_jumpKey < JUMP_KEY_LONG) || m_jumpKey == JUMP_KEY_LONG) {
		m_characterAction->jump((100 * m_jumpKey) / JUMP_KEY_LONG);
	}
}

Object* CharacterKeyboardController::bulletAttack() {
	// 左クリックされているなら
	if (leftClick() > 0) {
		// マウスの情報取得
		int mouseX, mouseY;
		m_camera->getMouse(&mouseX, &mouseY);
		// マウスの位置に向かって射撃
		return m_characterAction->bulletAttack(mouseX, mouseY);
	}
	return NULL;
}

Object* CharacterKeyboardController::slashAttack() {
	// 右クリックされたか、した後で今が攻撃タイミングなら
	if (rightClick() == 1 || m_characterAction->getSlashCnt() > 0) {
		return m_characterAction->slashAttack();
	}
	return NULL;
}