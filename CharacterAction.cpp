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
	m_vx = 0;
	m_vy = 0;
}

CharacterAction::CharacterAction() :
	CharacterAction(NULL)
{

}


/*
* 空を飛ばない普通の棒人間
*/
StickAction::StickAction(Character* character):
	CharacterAction(character)
{

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
	if (m_grand) { // 地面にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態
			m_character->switchStand();
			break;
		}
	}
	else { // 宙にいるとき
		switch (m_state) {
		case CHARACTER_STATE::STAND: //立ち状態
			m_character->switchStand();
			break;
		}
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