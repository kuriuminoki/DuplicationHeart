#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>


// Brainクラス
Brain::Brain() {
	m_characterAction_p = NULL;
}


/*
* キーボード
*/
KeyboardBrain::KeyboardBrain(const Camera* camera) {
	m_camera_p = camera;
}

void KeyboardBrain::bulletTargetPoint(int& x, int& y) {
	// マウスの位置
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// カメラで座標を補正
	m_camera_p->getMouse(&mouseX, &mouseY);

	x = mouseX;
	y = mouseY;
}

// 話しかけたり扉入ったり
bool KeyboardBrain::actionOrder() {
	return controlW() == 1;
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
	// 目標地点は現在地に設定
	m_gx = m_characterAction_p->getCharacter()->getX();
	m_gy = m_characterAction_p->getCharacter()->getY();
}

void NormalAI::bulletTargetPoint(int& x, int& y) {
	if (m_target_p == NULL) {
		x = 0;
		y = 0;
	}
	else { // ターゲットに向かって射撃攻撃
		x = m_target_p->getCenterX() + (GetRand(BULLET_ERROR) - BULLET_ERROR / 2);
		y = m_target_p->getCenterY() + (GetRand(BULLET_ERROR) - BULLET_ERROR / 2);
	}
}

void NormalAI::moveOrder(int& right, int& left, int& up, int& down) {
	// 現在地
	int x = m_characterAction_p->getCharacter()->getX();
	int y = m_characterAction_p->getCharacter()->getY();

	// (壁につっかえるなどで)移動できてないから諦める
	//DrawFormatString(800, 50, GetColor(255, 255, 255), "moveCnt = %d, x(%d) -> gx(%d)", m_moveCnt, x, m_gx);
	if (m_moveCnt == GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// 目標地点設定
	if (m_gx > x - GX_ERROR && m_gx < x + GX_ERROR && GetRand(99) == 0) {
		if (m_target_p != NULL) {
			// targetについていく
			m_gx = m_target_p->getCenterX() + GetRand(2000) - 1000;
		}
		else {
			// ランダムに設定
			m_gx = GetRand(200) + 100;
			if (GetRand(99) < GX_ERROR) { m_gx *= -1; }
			m_gx += x;
		}
	}

	// 目標に向かって走る
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

	// 反映
	right = m_rightKey;
	left = m_leftKey;
	up = m_upKey;
	down = m_downKey;
}

int NormalAI::jumpOrder() {
	// ダメージを食らったらリセット
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_jumpCnt = 0;
	}

	// ランダムでジャンプ
	if (m_squatCnt == 0 && GetRand(99) == 0) { m_jumpCnt = GetRand(15) + 5; }

	// 壁にぶつかったからジャンプ
	if (m_rightKey > 0 && m_characterAction_p->getRightLock()) { m_jumpCnt = 20; }
	else if (m_leftKey > 0 && m_characterAction_p->getLeftLock()) { m_jumpCnt = 20; }

	if (m_jumpCnt > 0) { m_jumpCnt--; }
	return m_jumpCnt;
}

int NormalAI::squatOrder() {
	// ダメージを食らったらリセット
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_squatCnt = 0;
	}

	// ランダムでしゃがむ
	if (m_characterAction_p->getGrand() && GetRand(99) == 0) { m_squatCnt = GetRand(60) + 30; }

	if (m_squatCnt > 0) { m_squatCnt--; }
	return m_squatCnt;
}

int NormalAI::slashOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return 0;
	}
	// 遠距離の敵には斬撃しない
	if (m_target_p != NULL && abs(m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX()) > 500) {
		return 0;
	}
	// ランダムで斬撃
	if (GetRand(50) == 0) {
		return 1;
	}
	return 0;
}

int NormalAI::bulletOrder() {
	if (m_target_p == NULL || m_target_p->getHp() == 0) { 
		return 0;
	}
	// ランダムで射撃
	if (GetRand(50) == 0) { 
		return 1;
	}
	return 0;
}

// 攻撃対象を決める(targetのままか、characterに変更するか)
void NormalAI::searchTarget(const Character* character) {
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		// 自分自身や味方じゃなければ
		if (character->getId() != m_characterAction_p->getCharacter()->getId() && character->getGroupId() != m_characterAction_p->getCharacter()->getGroupId()) {
			m_target_p = character;
		}
	}
}

// 攻撃対象を変更する必要があるならtrueでアピールする。
bool NormalAI::needSearchTarget() const {
	if (m_target_p == NULL || GetRand(99) == 0) {
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
		// 攻撃目標
		int targetX, targetY;
		m_brain->bulletTargetPoint(targetX, targetY);
		return m_characterAction->slashAttack(targetX, targetY);
	}
	return NULL;
}

void NormalController::damage(int vx, int vy, int damageValue) {
	m_characterAction->damage(vx, vy, damageValue);
}