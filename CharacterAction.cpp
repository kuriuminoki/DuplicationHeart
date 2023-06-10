#include "CharacterAction.h"
#include "Character.h"
#include "DxLib.h"


/*
*  キャラクターの基底クラス
*/
CharacterAction::CharacterAction(Character* character) {
	m_character = character;

	//初期状態
	m_state = CHARACTER_STATE::STAND;
	m_grand = false;
	m_vx = 0;
	m_vy = 0;
}

void CharacterAction::debugAction(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + 10, color, "state=%d, grand=%d, (vx,vy)=(%d,%d)", m_state, m_grand, m_vx, m_vy);
}



/*
* 空を飛ばない普通の棒人間
*/
void StickAction::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**StickAction**");
	debugAction(x + 10, y + 10, color);
}

void StickAction::action() {

	// 移動後の座標
	int nextX = m_character->getX();
	int nextY = m_character->getY();

	if (m_grand) { // 地面にいる

	}
	else { // 宙にいる
		// 重力
		m_vy += G;
		nextY += m_vy;
	}

	// 移動
	m_character->setX(nextX);
	m_character->setY(nextY);
}

// 状態に応じて画像セット
void StickAction::switchHandle() {
	switch (m_state) {
	case STAND: //立ち状態
		m_character->switchStand();
		break;
	}
}

// 歩く
void StickAction::walk(bool right, bool left) {
	// 移動後の座標
	int nextX = m_character->getX();
	int nextY = m_character->getY();

	if (right) { // 右へ歩く
		nextX += m_character->getMoveSpeed();
	}
	else if (left) { // 左へ歩く
		nextX -= m_character->getMoveSpeed();
	}

	// 移動
	m_character->setX(nextX);
	m_character->setY(nextY);
}

// 移動
void StickAction::move(bool right, bool left, bool up, bool down) {
	walk(right, left);
}