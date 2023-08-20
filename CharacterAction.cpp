#include "CharacterAction.h"
#include "Character.h"
#include "Sound.h"
#include "Define.h"
#include "DxLib.h"
#include <vector>


using namespace std;


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
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL, NULL)
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
}

void CharacterAction::setState(CHARACTER_STATE state) {
	if (m_state == CHARACTER_STATE::DAMAGE) { return; }
	m_state = state;
}

// キャラを動けなくする
void CharacterAction::setRightLock(bool lock) { 
	// ロックして
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
		m_slashCnt = 0;
		// 効果音
		if (m_soundPlayer_p != NULL) {
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
		m_squat = true;
	}
	else {
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

/*
* 空を飛ばない普通の棒人間
*/
StickAction::StickAction(Character* character, SoundPlayer* soundPlayer_p):
	CharacterAction(character, soundPlayer_p)
{

}

CharacterAction* StickAction::createCopy(vector<Character*> characters) {
	CharacterAction* res = NULL;
	for (unsigned int i = 0; i < characters.size(); i++) {
		if (m_character_p->getId() == characters[i]->getId()) {
			res = new StickAction(characters[i], m_soundPlayer_p);
			// コピーする
			setParam(res);
		}
	}
	return res;
}

// 行動前の処理
void StickAction::init() {
	// いったん全方向に動けるようにする
	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	// いったん宙に浮かせる
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
}

void StickAction::action() {
	// 状態(state)に応じて画像をセット
	switchHandle();

	// 射撃のインターバル処理
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// 斬撃のインターバル処理
	if (m_slashCnt > 0) { m_slashCnt--; }

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
			m_vx = 0;
		}
		else {
			m_character_p->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // 左
		if (m_leftLock) {
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
			if (m_landCnt > 0) {
				m_character_p->switchLand();
			}
			else if (m_slashCnt > 0) {
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

// 歩く ダメージ中、しゃがみ中、壁ぶつかり中は不可
void StickAction::walk(bool right, bool left) {
	// 右へ歩くのをやめる
	if (!right || m_rightLock || m_squat || damageFlag()) {
		stopMoveRight();
	}
	// 左へ歩くのをやめる
	if (!left || m_leftLock || m_squat || damageFlag()) {
		stopMoveLeft();
	}
	if (damageFlag()) {
		return;
	}
	bool walkStart = false;
	// 右へ歩き始める
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && (!left || !m_character_p->getLeftDirection()) && !m_squat) { // 右へ歩く
		m_vx += m_character_p->getMoveSpeed();
		m_moveRight = true;
		walkStart = true;
	}
	// 左へ歩き始める
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && (!right || m_character_p->getLeftDirection()) && !m_squat) { // 左へ歩く
		m_vx -= m_character_p->getMoveSpeed();
		m_moveLeft = true;
		walkStart = true;
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
				if (m_soundPlayer_p != NULL) {
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
		if (cnt == 0 || m_preJumpCnt == PRE_JUMP_MAX) {
			// ジャンプ
			int rate = (100 * m_preJumpCnt) / PRE_JUMP_MAX;
			int power = (m_character_p->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			setState(CHARACTER_STATE::STAND);
			// 効果音
			if (m_soundPlayer_p != NULL) {
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
		return NULL;
	}
	// 射撃可能状態なら
	if (ableAttack()) {
		// 射撃不可能状態にして
		m_bulletCnt = m_character_p->getBulletRapid();
		// 撃つ方向へ向く
		m_character_p->setLeftDirection(m_character_p->getCenterX() > gx);
		// 攻撃を返す
		return m_character_p->bulletAttack(gx, gy, m_soundPlayer_p);
	}
	return NULL;
}

// 斬撃攻撃
Object* StickAction::slashAttack(int gx, int gy) {
	if (damageFlag() && m_boostCnt == 0) {
		m_slashCnt = 0;
		return NULL;
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
	}
	// 攻撃のタイミングじゃないならNULLが返る
	return m_character_p->slashAttack(m_attackLeftDirection, m_slashCnt, m_soundPlayer_p);
}

// ダメージを受ける
void StickAction::damage(int vx, int vy, int damageValue) {
	setState(CHARACTER_STATE::DAMAGE);
	m_vx += vx;
	m_vy += vy;
	// 地面についていても少しはダメージモーション
	if(m_vy >= 0 && m_grand){ m_damageCnt = 20; }
	m_character_p->setLeftDirection(m_vx > 0);
	m_grand = false;
	// 宙に浮かせる
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
	// HP減少
	m_character_p->damageHp(damageValue);
	m_boostCnt = 0;
}