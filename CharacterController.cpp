#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


// Brainクラス
Brain::Brain() {
	m_characterAction = NULL;
}


/*
* キーボード
*/
KeyboardBrain::KeyboardBrain(const Camera* camera) {
	m_camera = camera;
}

void KeyboardBrain::bulletTargetPoint(int& x, int& y) {
	// マウスの位置
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// カメラで座標を補正
	m_camera->getMouse(&mouseX, &mouseY);

	x = mouseX;
	y = mouseY;
}

// 移動（上下左右の入力）
void KeyboardBrain::moveOrder(int& right, int& left, int& up, int& down) {
	right = controlD();
	left = controlA();
	up = controlW();
	down = controlS();
}

// ジャンプ
int KeyboardBrain::jumpOrder() {
	return controlSpace();
}

// しゃがみ
int KeyboardBrain::squatOrder() {
	return controlS();
}

// 近距離攻撃
int KeyboardBrain::slashOrder() {
	return rightClick();
}

// 遠距離攻撃
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
	else { // ターゲットに向かって射撃攻撃
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

// 攻撃対象を決める(targetのままか、characterに変更するか)
void NormalAI::searchTarget(Character* character) {
	if (m_target == NULL || m_target->getHp() == 0) {
		// 自分自身や味方じゃなければ
		if (character->getId() != m_characterAction->getCharacter()->getId()) {
			m_target = character;
		}
	}
}

// 攻撃対象を変更する必要があるならtrueでアピールする。
bool NormalAI::needSearchTarget() const {
	if (m_target == NULL || GetRand(99) == 0) {
		return true;
	}
	return false;
}


/*
* コントローラ
*/
CharacterController::CharacterController(Brain* brain, CharacterAction* characterAction) {

	// 初期化
	m_brain = brain;
	m_characterAction = characterAction;

	// BrainにActionを注入
	m_brain->setCharacterAction(m_characterAction);
}

CharacterController::CharacterController() :
	CharacterController(NULL, NULL)
{

}

// Actionインスタンスはここでのみデリートする
CharacterController::~CharacterController() {
	if (m_characterAction != NULL) {
		delete m_characterAction;
	}
	if (m_brain != NULL) {
		delete m_brain;
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
void CharacterController::setActionBoost() {
	m_characterAction->setBoost();
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

// 攻撃対象を変更（するかも）
void CharacterController::searchTargetCandidate(Character* character) {
	m_brain->searchTarget(character);
}

// 行動の結果反映
void CharacterController::action() {
	// 物理演算
	m_characterAction->action();
}


/*
* キーボードによるキャラコントロール マウスも使うのでCameraが必要
*/
NormalController::NormalController(Brain* brain, CharacterAction* characterAction):
	CharacterController(brain, characterAction)
{

}

void NormalController::control() {
	// 移動 stickなどの入力状態を更新する
	int rightStick, leftStick, upStick, downStick;
	m_brain->moveOrder(rightStick, leftStick, upStick, downStick);

	// stickに応じて移動
	m_characterAction->move(rightStick, leftStick, upStick, downStick);

	// ジャンプ
	int jump = m_brain->jumpOrder();
	m_characterAction->jump(jump);

	// しゃがみ
	int squat = m_brain->squatOrder();
	m_characterAction->setSquat(squat);
}

Object* NormalController::bulletAttack() {
	// 遠距離攻撃の命令がされているなら
	if (m_brain->bulletOrder() > 0) {
		// 攻撃目標
		int targetX, targetY;
		m_brain->bulletTargetPoint(targetX, targetY);
		// 目標に向かって射撃
		return m_characterAction->bulletAttack(targetX, targetY);
	}
	return NULL;
}

Object* NormalController::slashAttack() {
	// 近距離攻撃の命令がされたか、した後で今が攻撃タイミングなら
	if (m_brain->slashOrder() == 1 || m_characterAction->getSlashCnt() > 0) {
		return m_characterAction->slashAttack();
	}
	return NULL;
}