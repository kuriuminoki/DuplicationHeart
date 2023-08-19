#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Camera.h"
#include "Control.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>


/*
* コントローラ
*/
CharacterController::CharacterController(Brain* brain, CharacterAction* characterAction) {

	// 初期化
	m_brain = brain;
	m_characterAction = characterAction;

	// BrainにActionを注入
	m_brain->setCharacterAction(m_characterAction);

	// レコーダはデフォルトで使わない
	m_stickRecorder = NULL;
	m_jumpRecorder = NULL;
	m_squatRecorder = NULL;
	m_slashRecorder = NULL;
	m_bulletRecorder = NULL;

	m_duplicationFlag = false;
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
	if (m_stickRecorder != NULL && !m_duplicationFlag) {
		delete m_stickRecorder;
		delete m_jumpRecorder;
		delete m_squatRecorder;
		delete m_slashRecorder;
		delete m_bulletRecorder;
	}
}

// レコーダを初期化
void CharacterController::initRecorder() {
	m_stickRecorder->init();
	m_jumpRecorder->init();
	m_squatRecorder->init();
	m_slashRecorder->init();
	m_bulletRecorder->init();
}

// レコードをやめる
void CharacterController::eraseRecorder() { 
	if (m_stickRecorder == NULL) { return; }
	delete m_stickRecorder;
	delete m_jumpRecorder;
	delete m_squatRecorder;
	delete m_slashRecorder;
	delete m_bulletRecorder;
	m_stickRecorder = NULL;
	m_jumpRecorder = NULL;
	m_squatRecorder = NULL;
	m_slashRecorder = NULL;
	m_bulletRecorder = NULL;
}

// 話しかけたり扉に入ったりするボタンがtrueか
bool CharacterController::getActionKey() const { return m_brain->actionOrder(); }

void CharacterController::setAction(CharacterAction* action) {
	delete m_characterAction;
	m_characterAction = action;
	m_brain->setCharacterAction(m_characterAction);
}
void CharacterController::setBrain(Brain* brain) {
	delete m_brain;
	m_brain = brain;
	m_brain->setCharacterAction(m_characterAction);
}

// レコーダのセッタ
void CharacterController::setStickRecorder(ControllerRecorder* recorder) {
	m_stickRecorder = recorder;
}
void CharacterController::setJumpRecorder(ControllerRecorder* recorder) {
	m_jumpRecorder = recorder;
}
void CharacterController::setSquatRecorder(ControllerRecorder* recorder) {
	m_squatRecorder = recorder;
}
void CharacterController::setSlashRecorder(ControllerRecorder* recorder) {
	m_slashRecorder = recorder;
}
void CharacterController::setBulletRecorder(ControllerRecorder* recorder) {
	m_bulletRecorder = recorder;
}

void CharacterController::setTarget(Character* character) { 
	m_brain->setTarget(character);
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

// 攻撃対象を変更
void CharacterController::searchTargetCandidate(Character* character) {
	m_brain->searchTarget(character);
}

// 追跡対象を変更
void CharacterController::searchFollowCandidate(Character* character) {
	m_brain->searchFollow(character);
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

CharacterController* NormalController::createCopy(std::vector<Character*> characters, const Camera* camera) {
	CharacterAction* action = m_characterAction->createCopy(characters);
	Brain* brain = m_brain->createCopy(characters, camera);
	brain->setCharacterAction(action);
	CharacterController* res = new NormalController(brain, action);
	// 複製はレコーダをデリートしないためFlagをtrueにする
	res->setDuplicationFlag(true);
	res->setStickRecorder(m_stickRecorder);
	res->setJumpRecorder(m_jumpRecorder);
	res->setSquatRecorder(m_squatRecorder);
	res->setSlashRecorder(m_slashRecorder);
	res->setBulletRecorder(m_bulletRecorder);
	return res;
}

void NormalController::control() {
	// 移動 stickなどの入力状態を更新する
	int rightStick = 0, leftStick = 0, upStick = 0, downStick = 0;
	if (m_stickRecorder != NULL) {
		if (m_stickRecorder->existRecord()) {
			int input = m_stickRecorder->checkInput();
			if (((input >> 0) & 1) == 1) { 
				rightStick = 1;
			}
			if (((input >> 1) & 1) == 1) { 
				leftStick = 1;
			}
			if (((input >> 2) & 1) == 1) { upStick = 1; }
			if (((input >> 3) & 1) == 1) { downStick = 1; }
		}
		else {
			m_brain->moveOrder(rightStick, leftStick, upStick, downStick);
			int bit = 0;
			if (rightStick > 0) { bit |= (1 << 0); }
			if (leftStick > 0) { bit |= (1 << 1); }
			if (upStick > 0) { bit |= (1 << 2); }
			if (downStick > 0) { bit |= (1 << 3); }
			m_stickRecorder->writeRecord(bit);
		}
		m_stickRecorder->addTime();
	}
	else {
		m_brain->moveOrder(rightStick, leftStick, upStick, downStick);
	}

	// stickに応じて移動
	m_characterAction->move(rightStick, leftStick, upStick, downStick);

	// ジャンプ
	int jump = 0;
	if (m_jumpRecorder != NULL) {
		if (m_jumpRecorder->existRecord()) {
			jump = m_jumpRecorder->checkInput();
		}
		else {
			jump = m_brain->jumpOrder();
			if (jump == 1) {
				jump;
			}
			m_jumpRecorder->writeRecord(jump);
		}
		m_jumpRecorder->addTime();
	}
	else {
		jump = m_brain->jumpOrder();
	}
	m_characterAction->jump(jump);

	// しゃがみ
	int squat = 0;
	if (m_squatRecorder != NULL) {
		if (m_squatRecorder->existRecord()) {
			squat = m_squatRecorder->checkInput();
		}
		else {
			squat = m_brain->squatOrder();
			m_squatRecorder->writeRecord(squat);
		}
		m_squatRecorder->addTime();
	}
	else {
		squat = m_brain->squatOrder();
	}
	m_characterAction->setSquat(squat);
}

Object* NormalController::bulletAttack() {
	int order = 0;
	if (m_bulletRecorder != NULL) {
		if (m_bulletRecorder->existRecord()) {
			order = m_bulletRecorder->checkInput();
		}
		else {
			order = m_brain->bulletOrder();
			m_bulletRecorder->writeRecord(order);
		}
		m_bulletRecorder->addTime();
	}
	else {
		order = m_brain->bulletOrder();
	}
	// 遠距離攻撃の命令がされているなら
	if (order > 0) {
		// 攻撃目標
		int targetX, targetY;
		m_brain->bulletTargetPoint(targetX, targetY);
		// 目標に向かって射撃
		return m_characterAction->bulletAttack(targetX, targetY);
	}
	return NULL;
}

Object* NormalController::slashAttack() {
	int order = 0;
	if (m_slashRecorder != NULL) {
		if (m_slashRecorder->existRecord()) {
			order = m_slashRecorder->checkInput();
		}
		else {
			order = m_brain->slashOrder();
			m_slashRecorder->writeRecord(order);
		}
		m_slashRecorder->addTime();
	}
	else {
		order = m_brain->slashOrder();
	}
	// 近距離攻撃の命令がされたか、した後で今が攻撃タイミングなら
	if (order == 1 || m_characterAction->getSlashCnt() > 0) {
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