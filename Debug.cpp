#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "World.h"
#include "Game.h"
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
// Worldクラスのデバッグ
void World::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**World**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "CharacterSum=%d, ControllerSum=%d", m_characters.size(), m_characterControllers.size());
	m_characterControllers.front()->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
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
void CharacterKeyboardController::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(←, →, ↑, ↓)=(%d,%d,%d,%d)", m_rightStick, m_leftStick, m_upStick, m_downStick);
	debugController(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Action
*/
// Actionクラスのデバッグ
void CharacterAction::debugAction(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "state=%d, grand=%d, (vx,vy)=(%d,%d)", (int)m_state, m_grand, m_vx, m_vy);
	m_character->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
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
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "HP=%d/%d, (x,y)=(%d,%d)", m_hp, m_maxHp, m_x, m_y);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "(wide, height)=(%d,%d), handle=%d", m_wide, m_height, m_handle);
	DrawRotaGraph(GAME_WIDE - (m_wide / 2), y + (m_height / 2), 1.0, 0.0, m_handle, FALSE, FALSE);
}

// Heartクラスのデバッグ
void Heart::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Heart**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}