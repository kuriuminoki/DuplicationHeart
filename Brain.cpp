#include "Brain.h"
#include "Character.h"
#include "CharacterAction.h"
#include "Camera.h"
#include "Control.h"
#include "Object.h"
#include "DxLib.h"
#include <cmath>


using namespace std;


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


Brain* NormalAI::createCopy(std::vector<Character*> characters, const Camera* camera) {
	NormalAI* res = new NormalAI();

	if (m_target_p != NULL) {
		for (unsigned int i = 0; i < characters.size(); i++) {
			if (m_target_p->getId() == characters[i]->getId()) {
				res->setTarget(characters[i]);
				break;
			}
		}
	}
	setParam(res);
	return res;
}

void NormalAI::setParam(NormalAI* brain) {
	// 移動用
	brain->setRightKey(m_rightKey);
	brain->setLeftKey(m_leftKey);
	brain->setUpKey(m_upKey);
	brain->setDownKey(m_downKey);
	// ジャンプの長さ
	brain->setJumpCnt(m_jumpCnt);
	// しゃがむ長さ
	brain->setSquatCnt(m_squatCnt);
	// 移動目標
	brain->setGx(m_gx);
	brain->setGy(m_gy);
	brain->setMoveCnt(m_moveCnt);
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
	if (m_moveCnt >= GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// 目標地点設定
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	// ダメージを受けたらリセット
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_gx = x, m_gy = y;
	}
	else if (alreadyGoal && GetRand(MOVE_RAND) == 0) {
		if (m_target_p != NULL && abs(x - m_target_p->getCenterX()) < TARGET_DISTANCE) {
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
	stickOrder(right, left, up, down);
}

// スティック操作
void NormalAI::stickOrder(int& right, int& left, int& up, int& down) {
	// 現在地
	int x = m_characterAction_p->getCharacter()->getX();
	int y = m_characterAction_p->getCharacter()->getY();

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
		if (GetRand(120) == 0) { return 1; }
	}

	// ランダムでジャンプ
	if (m_squatCnt == 0 && GetRand(99) == 0) { m_jumpCnt = GetRand(15) + 5; }

	// 壁にぶつかったからジャンプ
	if (m_rightKey > 0 && m_characterAction_p->getRightLock()) { m_jumpCnt = 20; }
	else if (m_leftKey > 0 && m_characterAction_p->getLeftLock()) { m_jumpCnt = 20; }

	if (m_jumpCnt > 0) { m_jumpCnt--; }
	return m_jumpCnt == 0 ? 0 : 20 - m_jumpCnt;
}

int NormalAI::squatOrder() {
	// ダメージを食らったらリセット
	if (m_characterAction_p->getState() == CHARACTER_STATE::DAMAGE) {
		m_squatCnt = 0;
	}

	// 目標地点にいないならしゃがまない
	int x = m_characterAction_p->getCharacter()->getX();
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	if (!alreadyGoal) { m_squatCnt = 0; }

	// ランダムでしゃがむ
	if (alreadyGoal && m_characterAction_p->getGrand() && GetRand(99) == 0) {
		m_squatCnt = GetRand(120) + 60;
	}

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
	int x = m_characterAction_p->getCharacter()->getX();
	if (abs(x - m_target_p->getCenterX()) > TARGET_DISTANCE) {
		return 0;
	}
	// ランダムで射撃
	if (GetRand(30) == 0) {
		return 1;
	}
	return 0;
}

// 攻撃対象を決める(targetのままか、characterに変更するか)
void NormalAI::searchTarget(const Character* character) {
	if (GetRand(99) < 50) {
		int x = m_characterAction_p->getCharacter()->getX();
		// 距離が遠い
		if (abs(x - character->getCenterX()) > TARGET_DISTANCE) {
			return;
		}
		// 味方じゃなければ&中立じゃなければ
		int groupId = character->getGroupId();
		if (groupId != m_characterAction_p->getCharacter()->getGroupId() && groupId != -1) {
			m_target_p = character;
		}
	}
}

// 攻撃対象を変更する必要があるならtrueでアピールする。
bool NormalAI::needSearchTarget() const {
	// ターゲットを決めていないから
	if (m_target_p == NULL || m_target_p->getHp() == 0) {
		return true;
	}
	// 気まぐれで、or不適切な相手だから
	if (GetRand(99) == 0 || m_target_p->getGroupId() == -1) {
		return true;
	}
	// 今のターゲットは距離が遠いから
	if (abs(m_target_p->getX() - m_characterAction_p->getCharacter()->getX()) > TARGET_DISTANCE) {
		return true;
	}
	return false;
}

int  NormalAI::getTargetId() const { return m_target_p == NULL ? NULL : m_target_p->getId(); }


void ParabolaAI::bulletTargetPoint(int& x, int& y) {
	if (m_target_p == NULL) {
		x = 0;
		y = 0;
	}
	else { // ターゲットに向かって射撃攻撃
		const int G = -ParabolaBullet::G;
		int dx = m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX();
		int gx = abs(dx);
		int dy = -(m_target_p->getCenterY() - m_characterAction_p->getCharacter()->getCenterY());
		int v = m_characterAction_p->getCharacter()->getAttackInfo()->bulletSpeed();
		double A = (G * gx * gx) / (2 * v * v);
		double a = gx / A;
		double b = 1 - (dy / A);
		double route = a * a / 4 - b;
		if (route >= 0) {
			double t = -sqrt(route) - (a / 2);
			double r = atan(t);
			if (dx > 0) {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(m_characterAction_p->getCharacter()->getCenterY() - v * sin(r)) - GetRand(100);
		}
		else {
			// 射程外なら45度で投げる
			double r = 3.14 / 4;
			if (dx > 0) {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(m_characterAction_p->getCharacter()->getCenterY() - v * sin(r)) + 50 - GetRand(100);
		}
	}
}

void FollowParabolaAI::bulletTargetPoint(int& x, int& y) {
	if (m_target_p == NULL) {
		x = 0;
		y = 0;
	}
	else { // ターゲットに向かって射撃攻撃
		const int G = -ParabolaBullet::G;
		int dx = m_target_p->getCenterX() - m_characterAction_p->getCharacter()->getCenterX();
		int gx = abs(dx);
		int dy = -(m_target_p->getCenterY() - m_characterAction_p->getCharacter()->getCenterY());
		int v = m_characterAction_p->getCharacter()->getAttackInfo()->bulletSpeed();
		double A = (G * gx * gx) / (2 * v * v);
		double a = gx / A;
		double b = 1 - (dy / A);
		double route = a * a / 4 - b;
		if (route >= 0) {
			double t = -sqrt(route) - (a / 2);
			double r = atan(t);
			if (dx > 0) {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(m_characterAction_p->getCharacter()->getCenterY() - v * sin(r)) - GetRand(100);
		}
		else {
			// 射程外なら45度で投げる
			double r = 3.14 / 4;
			if (dx > 0) {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() + v * cos(r));
			}
			else {
				x = (int)(m_characterAction_p->getCharacter()->getCenterX() - v * cos(r));
			}
			y = (int)(m_characterAction_p->getCharacter()->getCenterY() - v * sin(r)) + 50 - GetRand(100);
		}
	}
}


/*
* キャラについていくNormalAI
*/
FollowNormalAI::FollowNormalAI() :
	NormalAI()
{
	m_follow_p = NULL;
}

Brain* FollowNormalAI::createCopy(std::vector<Character*> characters, const Camera* camera) {
	FollowNormalAI* res = new FollowNormalAI();
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_follow_p->getId() == characters[i]->getId()) {
			res->setFollow(characters[i]);
			break;
		}
	}
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_target_p->getId() == characters[i]->getId()) {
			res->setTarget(characters[i]);
			break;
		}
	}
	setParam(res);
	return res;
}

int FollowNormalAI::getFollowId() const {
	if (m_follow_p == nullptr) { return -1; }
	return m_follow_p->getId();
}

void FollowNormalAI::moveOrder(int& right, int& left, int& up, int& down) {
	// 現在地
	int x = m_characterAction_p->getCharacter()->getX();
	int y = m_characterAction_p->getCharacter()->getY();

	// (壁につっかえるなどで)移動できてないから諦める
	if (m_moveCnt >= GIVE_UP_MOVE_CNT) {
		m_gx = x;
		m_gy = y;
	}

	// 目標地点設定用パラメータ
	int followX = m_follow_p->getCenterX();
	bool alreadyGoal = m_gx > x - GX_ERROR && m_gx < x + GX_ERROR;
	bool alreadyFollow = m_gx < followX + FOLLOW_X_ERROR && m_gx > followX - FOLLOW_X_ERROR;

	// 目標地点設定
	if ((alreadyGoal && GetRand(MOVE_RAND) == 0) || !alreadyFollow) {
		if (m_follow_p != NULL) {
			// followについていく
			m_gx = m_follow_p->getCenterX() + GetRand(FOLLOW_X_ERROR * 2) - FOLLOW_X_ERROR;
		}
		else {
			// ランダムに設定
			m_gx = GetRand(200) + 100;
			if (GetRand(99) < GX_ERROR) { m_gx *= -1; }
			m_gx += x;
		}
	}

	stickOrder(right, left, up, down);
}

// 追跡対象を決める(AIクラスでオーバライドする。)
void FollowNormalAI::searchFollow(const Character* character) {
	// 味方のみ
	if (character->getId() != m_characterAction_p->getCharacter()->getId() && character->getGroupId() == m_characterAction_p->getCharacter()->getGroupId()) {
		m_follow_p = character;
	}
}

// 追跡対象を変更する必要があるならtrueでアピールする(AIクラスでオーバライドする)。
bool FollowNormalAI::needSearchFollow() const {
	if (m_follow_p == NULL || m_follow_p->getHp() == 0) {
		return true;
	}
	return false;
}