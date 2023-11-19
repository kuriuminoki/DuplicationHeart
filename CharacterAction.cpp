#include "CharacterAction.h"
#include "Character.h"
#include "Sound.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// クラス名
const char* CharacterAction::ACTION_NAME = "CharacterAction";
const char* StickAction::ACTION_NAME = "StickAction";
const char* ValkiriaAction::ACTION_NAME = "ValkiriaAction";
const char* FlightAction::ACTION_NAME = "FlightAction";

// クラス名からCharacterActionを作成する関数
CharacterAction* createAction(const string actionName, Character* character, SoundPlayer* soundPlayer_p) {

	string tmp = actionName;

	bool heavy = false;
	if (tmp.size() >= 2) {
		string last = tmp.substr(tmp.size() - 2);
		if (last == "_x") {
			heavy = true;
			tmp.erase(tmp.size() - 2, 2);
		}
	}

	CharacterAction* action = nullptr;
	if (tmp == StickAction::ACTION_NAME) {
		action = new StickAction(character, soundPlayer_p);
	}
	else if(tmp == ValkiriaAction::ACTION_NAME) {
		action = new ValkiriaAction(character, soundPlayer_p);
	}
	else if (tmp == FlightAction::ACTION_NAME) {
		action = new FlightAction(character, soundPlayer_p);
	}

	action->setHeavy(heavy);

	return action;
}


/*
*  キャラクターの基底クラス
*/
CharacterAction::CharacterAction(Character* character, SoundPlayer* soundPlayer_p) {
	m_character_p = character;
	m_soundPlayer_p = soundPlayer_p;

	//初期状態
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_runCnt = -1;
	m_squat = false;
	m_preJumpCnt = -1;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_grandLeftSlope = false;
	m_grandRightSlope = false;
	m_vx = 0;
	m_vy = 0;

	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	m_bulletCnt = 0;

	m_slashCnt = 0;

	m_attackLeftDirection = false;

	m_landCnt = 0;
	m_boostCnt = 0;
	m_damageCnt = 0;
	m_heavy = false;
}

CharacterAction::CharacterAction() :
	CharacterAction(nullptr, nullptr)
{

}

void CharacterAction::setParam(CharacterAction* action) {
	action->setState(m_state);
	action->setSimpleGrand(m_grand);
	action->setGrandLeftSlope(m_grandLeftSlope);
	action->setGrandRightSlope(m_grandRightSlope);
	action->setRunCnt(m_runCnt);

	action->setSquat(m_squat);
	action->setJumpCnt(m_preJumpCnt);
	action->setMoveRight(m_moveRight);
	action->setMoveLeft(m_moveLeft);
	action->setMoveUp(m_moveUp);
	action->setMoveDown(m_moveDown);
	action->setVx(m_vx);
	action->setVy(m_vy);
	action->setRightLock(m_rightLock);
	action->setLeftLock(m_leftLock);
	action->setUpLock(m_upLock);
	action->setDownLock(m_downLock);
	action->setBulletCnt(m_bulletCnt);
	action->setSlashCnt(m_slashCnt);
	action->setAttackLeftDirection(m_attackLeftDirection);
	action->setLandCnt(m_landCnt);
	action->setBoostCnt(m_boostCnt);
	action->setDamageCnt(m_damageCnt);
	action->setHeavy(m_heavy);
}

void CharacterAction::setState(CHARACTER_STATE state) {
	if (m_state == CHARACTER_STATE::DAMAGE) { return; }
	m_state = state;
}

// キャラを動けなくする
void CharacterAction::setRightLock(bool lock) { 
	m_rightLock = lock;
}
void CharacterAction::setLeftLock(bool lock) { 
	m_leftLock = lock;
}
void CharacterAction::setUpLock(bool lock) { 
	m_upLock = lock;
}
void CharacterAction::setDownLock(bool lock) { 
	m_downLock = lock;
}

// キャラクターのセッタ
void CharacterAction::setCharacterX(int x) {
	m_character_p->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character_p->setY(y);
}
void CharacterAction::setCharacterLeftDirection(bool leftDirection) {
	m_character_p->setLeftDirection(leftDirection);
}
void CharacterAction::setCharacterFreeze(bool freeze) {
	m_character_p->setFreeze(freeze);
}

// 行動前の処理 毎フレーム行う
void CharacterAction::init() {
	// いったん全方向に動けるようにする
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	// いったん宙に浮かせる
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;

	// prevHpをhpに追いつかせる
	m_character_p->setPrevHp(m_character_p->getPrevHp() - 1);
}

// ダメージ
void CharacterAction::damage(int vx, int vy, int damageValue) {
	m_damageCnt = DAMAGE_TIME;
	setState(CHARACTER_STATE::DAMAGE);
	if (!m_heavy) {
		m_vx += vx;
		m_vy += vy;
		m_character_p->setLeftDirection(m_vx > 0);
		// 宙に浮かせる
		m_grand = false;
		m_grandRightSlope = false;
		m_grandLeftSlope = false;
	}
	// HP減少
	m_character_p->damageHp(damageValue);
	m_boostCnt = 0;
}

void CharacterAction::startSlash() {

}

void CharacterAction::finishSlash() {
	m_slashCnt = 0;
}

bool CharacterAction::ableDamage() const {
	if (m_state == CHARACTER_STATE::DAMAGE) { return false; }
	return true;
}

bool CharacterAction::ableAttack() const {
	if (m_bulletCnt > 0 || m_slashCnt > 0) {
		return false;
	}
	return true;
}

// 着地
void CharacterAction::setGrand(bool grand) {
	if (m_vy > 0) { // 着地モーションになる
		m_landCnt = LAND_TIME;
		finishSlash();
		// 効果音
		if (m_soundPlayer_p != nullptr) {
			m_soundPlayer_p->pushSoundQueue(m_character_p->getLandSound(),
				adjustPanSound(m_character_p->getCenterX(),
					m_soundPlayer_p->getCameraX()));
		}
	}
	m_grand = grand;
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) {
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void CharacterAction::setSquat(bool squat) {
	if (squat && m_grand && m_state != CHARACTER_STATE::DAMAGE && m_state != CHARACTER_STATE::SLASH && m_state != CHARACTER_STATE::PREJUMP) {
		// しゃがめる状態なのでしゃがむ
		m_squat = true;
	}
	else {
		// しゃがめない状態
		m_squat = false;
	}
}

// 歩くのをやめる
void CharacterAction::stopMoveLeft() {
	// 左へ歩くのをやめる
	if (m_moveLeft) {
		m_vx += m_character_p->getMoveSpeed();
		m_moveLeft = false;
		m_runCnt = -1;
	}
}
void CharacterAction::stopMoveRight() {
	// 右へ歩くのをやめる
	if (m_moveRight) {
		m_vx -= m_character_p->getMoveSpeed();
		m_moveRight = false;
		m_runCnt = -1;
	}
}
void CharacterAction::stopMoveUp() {
	// 上へ歩くのをやめる
	if (m_moveUp) {
		m_vy += m_character_p->getMoveSpeed();
		m_moveUp = false;
		m_runCnt = -1;
	}
}
void CharacterAction::stopMoveDown() {
	// 下へ歩くのをやめる
	if (m_moveDown) {
		m_vy -= m_character_p->getMoveSpeed();
		m_moveDown = false;
		m_runCnt = -1;
	}
}

// 画像のサイズ変更による位置調整 (座標は画像の左上であることに注意)
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// 下へ行けないなら
	if (m_downLock) {
		// 上へ動かす
		m_character_p->moveUp((afterHeight - beforeHeight));
	}
	// 上へ行けないなら
	else if (m_upLock) {
		// 下へ動かす必要はない（画像が下方向に拡大されるから）
	}
	// 上下どっちにでも行ける
	else {
		// 両方に広げる
		int d = afterHeight - beforeHeight;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character_p->moveUp((d - 1) / 2);
			}
			else {
				m_character_p->moveUp((d + 1) / 2);
			}
		}
		else {
			m_character_p->moveUp(d / 2);
		}
	}

	// 右へ行けないなら
	if (m_rightLock && !m_leftLock) {
		// 左へ動かす
		m_character_p->moveLeft((afterWide - beforeWide));
	}
	// 左へ行けないなら
	else if (m_leftLock && !m_rightLock) {
		// 右へ動かす必要はない（画像が右方向に拡大されるから）
	}
	// 左右どっちにでも行ける、もしくはいけない
	else {
		// 両方に広げる
		int d = afterWide - beforeWide;
		if (d % 2 == 1) {
			if (d < 0) {
				m_character_p->moveLeft((d - 1) / 2);
			}
			else {
				m_character_p->moveLeft((d + 1) / 2);
			}
		}
		else {
			m_character_p->moveLeft(d / 2);
		}
	}
}


/*
* 空を飛ばない普通の棒人間
*/
StickAction::StickAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* StickAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new StickAction(characters[i], m_soundPlayer_p);
			// コピーする
			setParam(res);
		}
	}
	return res;
}

void StickAction::action() {
	// 状態(state)に応じて画像をセット
	switchHandle();

	// 射撃のインターバル処理
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// 斬撃のインターバル処理
	if (m_slashCnt > 0) { 
		m_slashCnt--;
		if (m_slashCnt == 0) { finishSlash(); }
	}

	// ダメージ受け状態は最低１秒近くある
	if (m_damageCnt > 0) { m_damageCnt--; }

	// 重力の処理
	// 宙にいる
	if(!m_grand) { 
		// 重力
		m_vy += G;
	}

	// アニメーション用のカウント
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) { m_boostCnt--; }

	// 移動
	if (m_vx > 0) {// 右
		if (m_rightLock) {
			stopMoveLeft(); // 左に移動したいのに吹っ飛び等で右へ移動しているとき、いったん左移動への入力をキャンセルさせないとバグる
			m_vx = 0;
		}
		else {
			m_character_p->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // 左
		if (m_leftLock) {
			stopMoveRight();// 右に移動したいのに吹っ飛び等で左へ移動しているとき、いったん右移動への入力をキャンセルさせないとバグる
			m_vx = 0;
		}
		else {
			m_character_p->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // 上
		if (m_upLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // 下
		if (m_downLock) {
			m_vy = 0;
		}
		else {
			m_character_p->moveDown(m_vy);
		}
	}
}

// 状態に応じて画像セット
void StickAction::switchHandle() {
	// セット前の画像のサイズ
	int wide, height;
	m_character_p->getHandleSize(wide, height);
	if (m_grand) { // 地面にいるとき
		switch (getState()) {
		case CHARACTER_STATE::STAND: //立ち状態
			if (m_slashCnt > 0) {
				m_character_p->switchSlash();
			}
			else if (m_landCnt > 0) {
				m_character_p->switchLand();
			}
			else if (m_bulletCnt > 0) {
				if (m_runCnt != -1) {
					m_character_p->switchRunBullet(m_runCnt);
				}
				else if (m_squat) {
					m_character_p->switchSquatBullet();
				}
				else {
					m_character_p->switchBullet();
				}
			}
			else if (m_squat) {
				m_character_p->switchSquat();
			}
			else if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::PREJUMP:
			m_character_p->switchPreJump(m_preJumpCnt);
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchSlash();
				}
				else if (m_bulletCnt > 0) {
					if (m_runCnt != -1) {
						m_character_p->switchRunBullet(m_runCnt);
					}
					else if (m_squat) {
						m_character_p->switchSquatBullet();
					}
					else {
						m_character_p->switchBullet();
					}
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}
	else { // 宙にいるとき
		switch (getState()) {
		case CHARACTER_STATE::STAND: //立ち状態(なにもなしの状態)
			if (m_boostCnt > 0) {
				m_character_p->switchBoost();
			}
			else if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character_p->switchAirBullet();
			}
			else if (m_vy < 0) {
				m_character_p->switchJump();
			}
			else {
				m_character_p->switchDown();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			if (m_boostCnt > 0) {
				if (m_slashCnt > 0) {
					m_character_p->switchAirSlash();
				}
				else if (m_bulletCnt > 0) {
					m_character_p->switchAirBullet();
				}
				else {
					m_character_p->switchBoost();
				}
			}
			else {
				m_character_p->switchDamage();
			}
			break;
		}
	}
	// セット後の画像のサイズ
	int afterWide, afterHeight;
	m_character_p->getHandleSize(afterWide, afterHeight);

	// サイズ変更による位置調整
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character_p->setLeftDirection(m_character_p->getLeftDirection());
}

// 歩く ダメージ中、しゃがみ中、壁ぶつかり中は不可
void StickAction::walk(bool right, bool left) {
	// 右へ歩くのをやめる
	if (!right || m_rightLock || m_squat || damageFlag()) {
		stopMoveRight();
	}
	if (m_slashCnt > 0 && !m_attackLeftDirection && (m_rightLock || damageFlag())) {
		finishSlash();
	}
	// 左へ歩くのをやめる
	if (!left || m_leftLock || m_squat || damageFlag()) {
		stopMoveLeft();
	}
	if (m_slashCnt > 0 && m_attackLeftDirection && (m_leftLock || damageFlag())) {
		finishSlash();
	}
	if (damageFlag()) {
		return;
	}

	// 右へ歩き始める
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && (!left || !m_character_p->getLeftDirection()) && !m_squat) { // 右へ歩く
		m_vx += m_character_p->getMoveSpeed();
		m_moveRight = true;
	}
	// 左へ歩き始める
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && (!right || m_character_p->getLeftDirection()) && !m_squat) { // 左へ歩く
		m_vx -= m_character_p->getMoveSpeed();
		m_moveLeft = true;
	}
	// アニメーション用にカウント
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// 移動
void StickAction::move(bool right, bool left, bool up, bool down) {
	if (getState() == CHARACTER_STATE::STAND && m_grand && m_slashCnt == 0 && m_bulletCnt == 0) {
		// 移動方向へ向く
		if(left && !right){
			m_character_p->setLeftDirection(true);
		}
		if (right && !left) {
			m_character_p->setLeftDirection(false);
		}
	}
	// 左右の移動
	walk(right, left);
}

// ジャンプ
void StickAction::jump(int cnt) {
	// ダメージ状態ならジャンプできないためreturn
	if (damageFlag()) {
		// 受け身はできる
		if (cnt == 1 || m_boostCnt > 0) {
			if (m_boostCnt == 0) {
				// 受け身をした瞬間
				m_vy -= m_character_p->getJumpHeight() / 2;
				m_grand = false;
				m_preJumpCnt = -1;
				stopMoveLeft();
				stopMoveRight();
				// 効果音
				if (m_soundPlayer_p != nullptr) {
					m_soundPlayer_p->pushSoundQueue(m_character_p->getPassiveSound(),
						adjustPanSound(m_character_p->getCenterX(),
							m_soundPlayer_p->getCameraX()));
				}
			}
			// ダメージ状態が解除されるまではずっとBoost
			m_boostCnt = BOOST_TIME;
		}
		return;
	}
	// 斬撃中はジャンプ不可
	if (m_slashCnt > 0) { return; }
	// 宙に浮いたらジャンプ中止
	if (!m_grand) {
		m_preJumpCnt = -1;
		if (getState() == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
	}
	// ジャンプの準備開始
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
		setState(CHARACTER_STATE::PREJUMP);
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == getPreJumpMax()) {
			// ジャンプ
			int rate = (100 * m_preJumpCnt) / getPreJumpMax();
			int power = (m_character_p->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			setState(CHARACTER_STATE::STAND);
			// 効果音
			if (m_soundPlayer_p != nullptr) {
				m_soundPlayer_p->pushSoundQueue(m_character_p->getJumpSound(),
					adjustPanSound(m_character_p->getCenterX(),
						m_soundPlayer_p->getCameraX()));
			}
		}
		else {
			// ジャンプ前の溜め中
			m_preJumpCnt++;
		}
	}
}

// 射撃攻撃
Object* StickAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		m_bulletCnt = 0;
		return nullptr;
	}
	// 射撃可能状態なら
	if (ableAttack()) {
		// 射撃不可能状態にして
		m_bulletCnt = m_character_p->getBulletRapid();
		// 撃つ方向へ向く
		if (m_character_p->getCharacterInfo()->sameBulletDirection()) {
			m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		}
		// 攻撃を返す
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return nullptr;
}

// 斬撃攻撃
Object* StickAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		if (m_slashCnt > 0) { finishSlash(); }
		return nullptr;
	}
	// 攻撃開始
	if (ableAttack()) {
		// ジャンプはキャンセル
		m_preJumpCnt = -1;
		if (getState() == CHARACTER_STATE::PREJUMP) {
			setState(CHARACTER_STATE::STAND);
		}
		m_attackLeftDirection = m_character_p->getCenterX() > gx;
		m_slashCnt = m_character_p->getSlashCountSum() + m_character_p->getSlashInterval();
		// 攻撃の方向へ向く
		m_character_p->setLeftDirection(m_attackLeftDirection);
		// 斬撃開始時の処理
		startSlash();
	}
	// 攻撃のタイミングじゃないならnullptrが返る
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_soundPlayer_p);
}


/*
* ヴァルキリア用Action 斬撃時に移動する
*/
ValkiriaAction::ValkiriaAction(Character* character, SoundPlayer* soundPlayer_p) :
	StickAction(character, soundPlayer_p)
{

}

CharacterAction* ValkiriaAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new ValkiriaAction(characters[i], m_soundPlayer_p);
			// コピーする
			setParam(res);
		}
	}
	return res;
}

// 着地 ヴァルキリアは斬撃中に着地しても着地モーションにならない
void ValkiriaAction::setGrand(bool grand) {
	if (m_vy > 0) { // 着地モーションになる
		if (m_slashCnt == 0) {
			m_landCnt = LAND_TIME;
			// 効果音
			if (m_soundPlayer_p != nullptr) {
				m_soundPlayer_p->pushSoundQueue(m_character_p->getLandSound(),
					adjustPanSound(m_character_p->getCenterX(),
						m_soundPlayer_p->getCameraX()));
			}
		}
	}
	m_grand = grand;
	if (m_state == CHARACTER_STATE::DAMAGE && m_damageCnt == 0) {
		m_vx = 0;
		m_vy = 0;
		m_state = CHARACTER_STATE::STAND;
	}
}

void ValkiriaAction::startSlash() {
	if (m_attackLeftDirection) {
		m_vx -= SLASH_MOVE_SPEED;
	}
	else {
		m_vx += SLASH_MOVE_SPEED;
	}
}

void ValkiriaAction::finishSlash() {
	if (m_attackLeftDirection && !m_leftLock) {
		m_vx += SLASH_MOVE_SPEED;
	}
	else if(!m_rightLock) {
		m_vx -= SLASH_MOVE_SPEED;
	}
	m_slashCnt = 0;
}

// ダメージを受ける ヴァルキリアは斬撃中はHPが減るだけ
void ValkiriaAction::damage(int vx, int vy, int damageValue) {
	if (m_slashCnt > 0) {
		// HP減少
		m_character_p->damageHp(damageValue / 2);
	}
	else {
		CharacterAction::damage(vx, vy, damageValue);
	}
}


/*
* 空を飛ぶキャラ
*/
FlightAction::FlightAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* FlightAction::createCopy(std::vector<Character*> characters) {
	CharacterAction* res = nullptr;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new FlightAction(characters[i], m_soundPlayer_p);
			// コピーする
			setParam(res);
		}
	}
	return res;
}

// キャラの画像を状態(state)に応じて変更
void FlightAction::switchHandle() {
	// セット前の画像のサイズ
	int wide, height;
	m_character_p->getHandleSize(wide, height);
	if (m_grand) { // 地面にいるとき
		switch (getState()) {
		case CHARACTER_STATE::STAND: //立ち状態
			if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character_p->switchAirBullet();
			}
			else {
				m_character_p->switchStand();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}
	else { // 宙にいるとき
		switch (getState()) {
		case CHARACTER_STATE::STAND: //立ち状態(なにもなしの状態)
			if (m_slashCnt > 0) {
				m_character_p->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				if (m_runCnt != -1) {
					m_character_p->switchRunBullet(m_runCnt);
				}
				else {
					m_character_p->switchAirBullet();
				}
			}
			else if (m_runCnt != -1) {
				m_character_p->switchRun(m_runCnt);
			}
			else if (m_vy != 0) {
				m_character_p->switchDown();
			}
			else {
				m_character_p->switchJump();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character_p->switchDamage();
			break;
		}
	}
	// セット後の画像のサイズ
	int afterWide, afterHeight;
	m_character_p->getHandleSize(afterWide, afterHeight);

	// サイズ変更による位置調整
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character_p->setLeftDirection(m_character_p->getLeftDirection());
}

// 物理演算 毎フレーム行う
void FlightAction::action() {
	// 状態(state)に応じて画像をセット
	switchHandle();

	// 射撃のインターバル処理
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// 斬撃のインターバル処理
	if (m_slashCnt > 0) {
		m_slashCnt--;
		if (m_slashCnt == 0) { finishSlash(); }
	}

	// ダメージ受け状態は最低１秒近くある
	if (m_damageCnt > 0) { 
		m_damageCnt--;
		if (m_damageCnt == 0) {
			m_vx = 0;
			m_vy = 0;
			m_state = CHARACTER_STATE::STAND;
		}
	}

	// アニメーション用のカウント
	if (m_landCnt > 0) { m_landCnt--; }
	if (m_boostCnt > 0) { m_boostCnt--; }

	// 移動
	if (m_vx > 0) {// 右
		if (m_rightLock) {
			stopMoveLeft(); // 左に移動したいのに吹っ飛び等で右へ移動しているとき、いったん左移動への入力をキャンセルさせないとバグる
			m_vx = 0;
		}
		else {
			m_character_p->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // 左
		if (m_leftLock) {
			stopMoveRight();// 右に移動したいのに吹っ飛び等で左へ移動しているとき、いったん右移動への入力をキャンセルさせないとバグる
			m_vx = 0;
		}
		else {
			m_character_p->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // 上
		if (m_upLock) {
			stopMoveDown();
			m_vy = 0;
		}
		else {
			m_character_p->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // 下
		if (m_downLock) {
			stopMoveUp();
			m_vy = 0;
		}
		else {
			m_character_p->moveDown(m_vy);
		}
	}
}

void FlightAction::walk(bool right, bool left, bool up, bool down) {
	// 右へ歩くのをやめる
	if (!right || m_rightLock || m_squat || damageFlag()) {
		stopMoveRight();
	}
	if (m_slashCnt > 0 && !m_attackLeftDirection && (m_rightLock || damageFlag())) {
		finishSlash();
	}
	// 左へ歩くのをやめる
	if (!left || m_leftLock || m_squat || damageFlag()) {
		stopMoveLeft();
	}
	if (m_slashCnt > 0 && m_attackLeftDirection && (m_leftLock || damageFlag())) {
		finishSlash();
	}
	// 上へ歩くのをやめる
	if (!up || m_upLock || damageFlag()) {
		stopMoveUp();
	}
	// 下へ歩くのをやめる
	if (!down || m_downLock || damageFlag()) {
		stopMoveDown();
	}
	if (damageFlag()) {
		return;
	}
	// 右へ歩き始める
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && (!left || !m_character_p->getLeftDirection()) && !m_squat) { // 右へ歩く
		m_vx += m_character_p->getMoveSpeed();
		m_moveRight = true;
		if(m_grand){
			m_vy -= m_character_p->getMoveSpeed();
			m_moveUp = true;
		}
	}
	// 左へ歩き始める
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && (!right || m_character_p->getLeftDirection()) && !m_squat) { // 左へ歩く
		m_vx -= m_character_p->getMoveSpeed();
		m_moveLeft = true;
		if (m_grand) {
			m_vy -= m_character_p->getMoveSpeed();
			m_moveUp = true;
		}
	}
	// 上へ歩き始める
	if (!m_upLock && !m_moveDown && !m_moveUp && up && !down) { // 上へ歩く
		m_vy -= m_character_p->getMoveSpeed();
		m_moveUp = true;
	}
	// 下へ歩き始める
	if (!m_downLock && !m_moveUp && !m_moveDown && down && !up) { // 下へ歩く
		m_vy += m_character_p->getMoveSpeed();
		m_moveDown = true;
	}
	// アニメーション用にカウント
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// 移動 引数は４方向分
void FlightAction::move(bool right, bool left, bool up, bool down) {
	if (getState() == CHARACTER_STATE::STAND && m_slashCnt == 0 && m_bulletCnt == 0) {
		// 移動方向へ向く
		if (left && !right) {
			m_character_p->setLeftDirection(true);
		}
		if (right && !left) {
			m_character_p->setLeftDirection(false);
		}
	}
	// 移動
	walk(right, left, up, down);
}

// ジャンプ cntフレーム目
void FlightAction::jump(int cnt) {
	
}

// 射撃攻撃
Object* FlightAction::bulletAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		m_bulletCnt = 0;
		return nullptr;
	}
	// 射撃可能状態なら
	if (ableAttack()) {
		// 射撃不可能状態にして
		m_bulletCnt = m_character_p->getBulletRapid();
		// 撃つ方向へ向く
		if (m_character_p->getCharacterInfo()->sameBulletDirection()) {
			m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		}
		// 攻撃を返す
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return nullptr;
}

// 斬撃攻撃
Object* FlightAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		if (m_slashCnt > 0) { finishSlash(); }
		return nullptr;
	}
	// 攻撃開始
	if (ableAttack()) {
		m_attackLeftDirection = m_character_p->getCenterX() > gx;
		m_slashCnt = m_character_p->getSlashCountSum() + m_character_p->getSlashInterval();
		// 攻撃の方向へ向く
		m_character_p->setLeftDirection(m_attackLeftDirection);
		// 斬撃開始時の処理
		startSlash();
	}
	// 攻撃のタイミングじゃないならnullptrが返る
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_soundPlayer_p);
}