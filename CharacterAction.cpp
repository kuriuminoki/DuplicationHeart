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
	m_runCnt = -1;
	m_squat = false;
	m_preJumpCnt = -1;
	m_moveRight = false;
	m_moveLeft = false;
	m_moveUp = false;
	m_moveDown = false;
	m_vx = 0;
	m_vy = 0;

	m_rightLock = false;
	m_leftLock = false;
	m_upLock = false;
	m_downLock = false;

	m_bulletCnt = 0;

	m_slashCnt = 0;

	m_attackLeftDirection = false;
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL)
{

}

// キャラを動けなくする
void CharacterAction::setRightLock(bool lock) { 
	// ロックして
	m_rightLock = lock;
	// 移動状態も解除
	m_moveRight = false;
}
void CharacterAction::setLeftLock(bool lock) { 
	m_leftLock = lock;
	m_moveLeft = false;
}
void CharacterAction::setUpLock(bool lock) { 
	m_upLock = lock;
	m_moveUp = false;
}
void CharacterAction::setDownLock(bool lock) { 
	m_downLock = lock;
	m_moveDown;
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
}

void StickAction::action() {
	// 射撃のインターバル処理
	if (m_bulletCnt > 0) { m_bulletCnt--; }

	// 斬撃のインターバル処理
	if (m_slashCnt > 0) { m_slashCnt--; }

	// 重力の処理
	// 宙にいる
	if(!m_grand) { 
		// 重力
		m_vy += G;
	}

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

	// 状態(state)に応じて画像をセット
	switchHandle();
}

// 状態に応じて画像セット
void StickAction::switchHandle() {
	// セット前の画像のサイズ
	int wide, height;
	m_character->getHandleSize(wide, height);
	if (m_grand) { // 地面にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態
			m_character->switchStand();
			break;
		}
	}
	else { // 宙にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態(なにもなしの状態)
			if (m_vy < 0) {
				m_character->switchJump();
			}
			else {
				m_character->switchDown();
			}
			break;
		}
	}
	// セット後の画像のサイズ
	int afterWide, afterHeight;
	m_character->getHandleSize(afterWide, afterHeight);

	// サイズ変更による位置調整
	afterChangeGraph(wide, height, afterWide, afterHeight);
}

// 画像のサイズ変更による位置調整
void CharacterAction::afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight) {
	// 下へ行けないなら
	if (m_downLock) {
		// 上へ動かす
		m_character->moveUp((afterHeight - beforeHeight));
	}
	// 上へ行けないなら
	if (m_upLock) {
		// 下へ動かす
		m_character->moveDown((afterHeight - beforeHeight));
	}

	// 右へ行けないなら
	if (m_rightLock) {
		// 左へ動かす
		m_character->moveLeft((afterWide - beforeWide));
	}
	// 左へ行けないなら
	if (m_leftLock) {
		// 右へ動かす
		m_character->moveRight((afterWide - beforeWide));
	}
}

// 歩く ダメージ中は不可
void StickAction::walk(bool right, bool left) {
	if (m_moveRight && !right) { // 右へ歩くのをやめる
		m_vx -= m_character->getMoveSpeed();
		m_moveRight = false;
	}
	if (m_moveLeft && !left) { // 左へ歩くのをやめる
		m_vx += m_character->getMoveSpeed();
		m_moveLeft = false;
	}
	if (!m_rightLock && !m_moveRight && !m_moveLeft && right) { // 右へ歩く
		m_vx += m_character->getMoveSpeed();
		m_moveRight = true;
	}
	if (!m_leftLock && !m_moveRight && !m_moveLeft && left) { // 左へ歩く
		m_vx -= m_character->getMoveSpeed();
		m_moveLeft = true;
	}
}

// 移動
void StickAction::move(bool right, bool left, bool up, bool down) {
	// 左右の移動
	walk(right, left);
}

// ジャンプ
void StickAction::jump(int cnt) {
	// ジャンプ前の状態なら
	if (cnt > 0 && m_grand && m_preJumpCnt == -1) {
		m_preJumpCnt = 0;
	}
	if (m_grand && m_preJumpCnt >= 0) {
		if (cnt == 0 || m_preJumpCnt == 10) {
			int rate = (100 * m_preJumpCnt) / 10;
			int power = (m_character->getJumpHeight() * rate) / 100;
			m_vy -= power;
			m_grand = false;
			m_preJumpCnt = -1;
		}
		else {
			m_preJumpCnt++;
		}
	}
}

// 射撃攻撃
Object* StickAction::bulletAttack(int gx, int gy) {
	// 射撃可能状態なら
	if (m_bulletCnt == 0) {
		// 射撃不可能状態にして
		m_bulletCnt = m_character->getBulletRapid();
		// 攻撃を返す
		return m_character->bulletAttack(gx, gy);
	}
	return NULL;
}

// 斬撃攻撃
Object* StickAction::slashAttack() {
	// 攻撃開始
	if (m_slashCnt == 0) {
		m_attackLeftDirection = m_character->getLeftDirection();
		m_slashCnt = m_character->getSlashCountSum();
	}
	// 攻撃のタイミングじゃないならNULLが返る
	return m_character->slashAttack(m_attackLeftDirection, m_slashCnt);
}