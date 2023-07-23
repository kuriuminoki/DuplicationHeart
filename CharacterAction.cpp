#include "CharacterAction.h"
#include "Character.h"
#include "Define.h"
#include "DxLib.h"


/*
*  キャラクターの基底クラス
*/
CharacterAction::CharacterAction(Character* character) {
	m_character = character;

	//初期状態
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_grandRightSlope = false;
	m_grandRightSlope = false;
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
	CharacterAction(NULL)
{

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
	m_character->setX(x);
}
void CharacterAction::setCharacterY(int y) {
	m_character->setY(y);
}
void CharacterAction::setCharacterLeftDirection(bool leftDirection) {
	m_character->setLeftDirection(leftDirection);
}

bool CharacterAction::ableDamage() const {
	if (m_state == CHARACTER_STATE::DAMAGE) { return false; }
	return true;
}


/*
* 空を飛ばない普通の棒人間
*/
StickAction::StickAction(Character* character):
	CharacterAction(character)
{

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

// 着地
void CharacterAction::setGrand(bool grand) {
	if (m_vy > 0) { // 着地モーションになる
		m_landCnt = LAND_TIME;
		m_slashCnt = 0;
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
			m_character->moveRight(m_vx);
		}
	}
	else if (m_vx < 0) { // 左
		if (m_leftLock) {
			m_vx = 0;
		}
		else {
			m_character->moveLeft(-m_vx);
		}
	}
	if (m_vy < 0) { // 上
		if (m_upLock) {
			m_vy = 0;
		}
		else {
			m_character->moveUp(-m_vy);
		}
	}
	else if (m_vy > 0) { // 下
		if (m_downLock) {
			m_vy = 0;
		}
		else {
			m_character->moveDown(m_vy);
		}
	}
}

// 状態に応じて画像セット
void StickAction::switchHandle() {
	// セット前の画像のサイズ
	int wide, height;
	m_character->getHandleSize(wide, height);
	if (m_grand) { // 地面にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態
			if (m_landCnt > 0) {
				m_character->switchLand();
			}
			else if (m_slashCnt > 0) {
				m_character->switchSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character->switchBullet();
			}
			else if (m_squat) {
				m_character->switchSquat();
			}
			else if (m_runCnt != -1) {
				m_character->switchRun(m_runCnt);
			}
			else {
				m_character->switchStand();
			}
			break;
		case CHARACTER_STATE::PREJUMP:
			m_character->switchPreJump(m_preJumpCnt);
			break;
		}
	}
	else { // 宙にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態(なにもなしの状態)
			if (m_slashCnt > 0) {
				m_character->switchAirSlash();
			}
			else if (m_bulletCnt > 0) {
				m_character->switchAirBullet();
			}
			else if (m_boostCnt > 0) {
				m_character->switchBoost();
			}
			else if (m_vy < 0) {
				m_character->switchJump();
			}
			else {
				m_character->switchDown();
			}
			break;
		case CHARACTER_STATE::DAMAGE:
			m_character->switchDamage();
			break;
		}
	}
	// セット後の画像のサイズ
	int afterWide, afterHeight;
	m_character->getHandleSize(afterWide, afterHeight);

	// サイズ変更による位置調整
	afterChangeGraph(wide, height, afterWide, afterHeight);

	m_character->setLeftDirection(m_character->getLeftDirection());
}

// 画像のサイズ変更による位置調整 (座標は画像の左上であることに注意)
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// 下へ行けないなら
	if (m_downLock) {
		// 上へ動かす
		m_character->moveUp((afterHeight - beforeHeight));
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
				m_character->moveUp((d - 1) / 2);
			}
			else {
				m_character->moveUp((d + 1) / 2);
			}
		}
		else {
			m_character->moveUp(d / 2);
		}
	}

	// 右へ行けないなら
	if (m_rightLock && !m_leftLock) {
		// 左へ動かす
		m_character->moveLeft((afterWide - beforeWide));
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
				m_character->moveLeft((d - 1) / 2);
			}
			else {
				m_character->moveLeft((d + 1) / 2);
			}
		}
		else {
			m_character->moveLeft(d / 2);
		}
	}
}

// 歩く ダメージ中、しゃがみ中、壁ぶつかり中は不可
void StickAction::walk(bool right, bool left) {
	// 右へ歩くのをやめる
	if (m_moveRight && (!right || m_rightLock || m_squat || m_state == CHARACTER_STATE::DAMAGE)) {
		m_vx -= m_character->getMoveSpeed();
		m_moveRight = false;
		m_runCnt = -1;
	}
	// 左へ歩くのをやめる
	if (m_moveLeft && (!left || m_leftLock || m_squat || m_state == CHARACTER_STATE::DAMAGE)) {
		m_vx += m_character->getMoveSpeed();
		m_moveLeft = false;
		m_runCnt = -1;
	}
	if (m_state == CHARACTER_STATE::DAMAGE) {
		return;
	}
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right && !m_squat) { // 右へ歩く
		m_vx += m_character->getMoveSpeed();
		m_moveRight = true;
	}
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left && !m_squat) { // 左へ歩く
		m_vx -= m_character->getMoveSpeed();
		m_moveLeft = true;
	}
	// アニメーション用にカウント
	if (m_moveLeft || m_moveRight) {
		m_runCnt++;
	}
}

// 移動
void StickAction::move(bool right, bool left, bool up, bool down) {
	if (m_state == CHARACTER_STATE::STAND && m_grand && m_slashCnt == 0 && m_bulletCnt == 0) {
		// 移動方向へ向く
		if(left){
			m_character->setLeftDirection(true);
		}
		if (right) {
			m_character->setLeftDirection(false);
		}
	}
	// 左右の移動
	walk(right, left);
}

// ジャンプ
void StickAction::jump(int cnt) {
	// 宙に浮いたらジャンプ中止
	if (!m_grand) {
		m_preJumpCnt = -1;
		if (m_state == CHARACTER_STATE::PREJUMP) {
			m_state = CHARACTER_STATE::STAND;
		}
	}
	// ジャンプ前の状態なら
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
		m_state = CHARACTER_STATE::PREJUMP;
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == PRE_JUMP_MAX) {
			int rate = (100 * m_preJumpCnt) / PRE_JUMP_MAX;
			int power = (m_character->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
			m_state = CHARACTER_STATE::STAND;
		}
		else {
			m_preJumpCnt++;
		}
	}
}

// 射撃攻撃
Object* StickAction::bulletAttack(int gx, int gy) {
	if (m_state == CHARACTER_STATE::DAMAGE) {
		m_bulletCnt = 0;
		return NULL;
	}
	// 射撃可能状態なら
	if (m_bulletCnt == 0 && m_slashCnt == 0) {
		// 射撃不可能状態にして
		m_bulletCnt = m_character->getBulletRapid();
		// 撃つ方向へ向く
		m_character->setLeftDirection(m_character->getCenterX() > gx);
		// 攻撃を返す
		return m_character->bulletAttack(gx, gy);
	}
	return NULL;
}

// 斬撃攻撃
Object* StickAction::slashAttack(int gx, int gy) {
	if (m_state == CHARACTER_STATE::DAMAGE) {
		m_slashCnt = 0;
		return NULL;
	}
	// 攻撃開始
	if (m_slashCnt == 0) {
		m_attackLeftDirection = m_character->getCenterX() > gx;
		m_slashCnt = m_character->getSlashCountSum();
	}
	if (m_slashCnt > 0) {
		// 攻撃の方向へ向く
		m_character->setLeftDirection(m_attackLeftDirection);
	}
	// 攻撃のタイミングじゃないならNULLが返る
	return m_character->slashAttack(m_attackLeftDirection, m_slashCnt);
}

void StickAction::damage(int vx, int vy, int damageValue, int soundHandle) {
	m_state = CHARACTER_STATE::DAMAGE;
	m_vx += vx;
	m_vy += vy;
	// 地面についていても少しはダメージモーション
	if(m_vy >= 0 && m_grand){ m_damageCnt = 20; }
	m_character->setLeftDirection(m_vx > 0);
	m_grand = false;
	// 宙に浮かせる
	m_grand = false;
	m_grandRightSlope = false;
	m_grandLeftSlope = false;
	// HP現象
	m_character->damageHp(damageValue);
}