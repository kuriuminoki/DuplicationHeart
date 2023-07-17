#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Object.h"
#include "World.h"
#include "Game.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"

/*
* Game
*/
// Gameクラスのデバッグ
void Game::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**GAME**");
	m_world->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}


/*
* World
*/
void debugObjects(int x, int y, int color, std::queue<Object*> objects) {
	int i = 0;
	size_t objectSum = objects.size();
	while (i < objectSum) {
		Object* object = objects.front();
		objects.pop();
		// 当たり判定をここで行う
		object->debug(x + 500, y + DRAW_FORMAT_STRING_SIZE * i * 4, color);
		objects.push(object);
		i++;
	}
}

// Worldクラスのデバッグ
void World::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**World**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "CharacterSum=%d, ControllerSum=%d", m_characters.size(), m_characterControllers.size());
	m_characterControllers.front()->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	// オブジェクト
	debugObjects(x, y, color, m_stageObjects);
	debugObjects(x + 500, y, RED, m_attackObjects);
}


/*
* Controller
*/
// CharacterControllerクラスのデバッグ
void CharacterController::debugController(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterController**");
	m_characterAction->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}

// CharacterKeyboardControllerクラスのデバッグ
void NormalController::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	debugController(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Action
*/
// Actionクラスのデバッグ
void CharacterAction::debugAction(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "state=%d, grand=%d, (vx,vy)=(%d,%d), runCnt=%d", (int)m_state, m_grand, m_vx, m_vy, m_runCnt);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "制限中：(←, →, ↑, ↓)=(%d,%d,%d,%d)", m_leftLock, m_rightLock, m_upLock, m_downLock);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 3, color, "移動中：(←, →, ↑, ↓)=(%d,%d,%d,%d)", m_moveLeft, m_moveRight, m_moveUp, m_moveDown);
	m_character->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 4, color);
}

// StickActionのデバッグ
void StickAction::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**StickAction**");
	debugAction(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}


/*
* Character
*/
// Characterクラスのデバッグ
void Character::debugCharacter(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Character**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "HP=%d/%d, (x,y)=(%d,%d), left=%d", m_hp, m_maxHp, m_x, m_y, m_leftDirection);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "(wide, height)=(%d,%d), handle=%d", m_wide, m_height, m_graphHandle->getHandle());
	// 画像
	m_graphHandle->draw(GAME_WIDE - (m_wide / 2), (m_height / 2), m_graphHandle->getEx());
	DrawBox(m_x, m_y, m_x + m_wide, m_y + m_height, color, TRUE);
	DrawBox(m_x + 10, m_y + (m_height / 2) - 20, m_x + m_wide - 10, m_y + (m_height / 2) + 20, BLACK, TRUE);
	DrawFormatString(m_x + 10, m_y + (m_height / 2), WHITE, "[%s]", m_characterInfo->name().c_str());
}

// Heartクラスのデバッグ
void Heart::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Heart**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Object
*/
// Objectのデバッグ
void Object::debugObject(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Object**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "((x1,y1),(x2,y2))=((%d,%d),(%d,%d))", m_x1, m_y1, m_x2, m_y2);
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}

/*
* 長方形の障害物
*/
void BoxObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**BoxObject**");
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
	DrawBox(m_x1, m_y1, m_x2, m_y2, RED, FALSE);
}

/*
* 直角三角形の障害物
*/
void TriangleObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**TriangleObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "mideleY=%d", getY(m_x2 - m_x1));
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	if (m_leftDown) {
		DrawTriangle(m_x2, m_y1, m_x2, m_y2, m_x1, m_y2, RED, FALSE);
	}
	else {
		DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x1, m_y2, RED, FALSE);
	}
}

/*
* 楕円の弾丸
*/
void BulletObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**BulletObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(gx,gy)=(%d,%d)", m_gx, m_gy);
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	DrawOval(m_x1 + m_rx, m_y1 + m_ry, m_rx, m_ry, m_color, TRUE);
}


/*
* 斬撃のデバッグ
*/
void SlashObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**SlashObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "cnt=%d", m_cnt);
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}