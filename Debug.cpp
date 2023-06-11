#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Object.h"
#include "World.h"
#include "Game.h"
#include "Define.h"
#include "DxLib.h"

/*
* Game
*/
// Game�N���X�̃f�o�b�O
void Game::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**GAME**");
	m_world->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}


/*
* World
*/
// World�N���X�̃f�o�b�O
void World::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**World**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "CharacterSum=%d, ControllerSum=%d", m_characters.size(), m_characterControllers.size());
	m_characterControllers.front()->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	// �I�u�W�F�N�g
	int i = 0;
	size_t objectSum = m_objects.size();
	while (i < objectSum) {
		Object* object = m_objects.front();
		m_objects.pop();
		// �����蔻��������ōs��
		object->debug(x + 500, y + DRAW_FORMAT_STRING_SIZE * i * 4, color);
		m_objects.push(object);
		i++;
	}
}


/*
* Controller
*/
// CharacterController�N���X�̃f�o�b�O
void CharacterController::debugController(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterController**");
	m_characterAction->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}

// CharacterKeyboardController�N���X�̃f�o�b�O
void CharacterKeyboardController::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterKeyboardController**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "(��, ��, ��, ��)=(%d,%d,%d,%d), jump=%d", m_rightStick, m_leftStick, m_upStick, m_downStick, m_jumpKey);
	debugController(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Action
*/
// Action�N���X�̃f�o�b�O
void CharacterAction::debugAction(int x, int y, int color) {
	DrawFormatString(x, y, color, "**CharacterAction**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "state=%d, grand=%d, (vx,vy)=(%d,%d)", (int)m_state, m_grand, m_vx, m_vy);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "�������F(��, ��, ��, ��)=(%d,%d,%d,%d)", m_leftLock, m_rightLock, m_upLock, m_downLock);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 3, color, "�ړ����F(��, ��, ��, ��)=(%d,%d,%d,%d)", m_moveLeft, m_moveRight, m_moveUp, m_moveDown);
	m_character->debug(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 4, color);
}

// StickAction�̃f�o�b�O
void StickAction::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**StickAction**");
	debugAction(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
}


/*
* Character
*/
// Character�N���X�̃f�o�b�O
void Character::debugCharacter(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Character**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "HP=%d/%d, (x,y)=(%d,%d)", m_hp, m_maxHp, m_x, m_y);
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE * 2, color, "(wide, height)=(%d,%d), handle=%d", m_wide, m_height, m_handle);
	// �摜
	DrawRotaGraph(GAME_WIDE - (m_wide / 2), y + (m_height / 2), m_ex, 0.0, m_handle, FALSE, FALSE);
	DrawBox(m_x, m_y, m_x + m_wide, m_y + m_height, color, TRUE);
	DrawBox(m_x + 10, m_y + (m_height / 2) - 20, m_x + m_wide - 10, m_y + (m_height / 2) + 20, BLACK, TRUE);
	DrawFormatString(m_x + 10, m_y + (m_height / 2), WHITE, "[%s]", m_name);
}

// Heart�N���X�̃f�o�b�O
void Heart::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Heart**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
}


/*
* Object
*/
// Object�̃f�o�b�O
void Object::debugObject(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Object**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "((x1,y1),(x2,y2))=((%d,%d),(%d,%d))", m_x1, m_y1, m_x2, m_y2);
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, FALSE);
}

void BoxObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**BoxObject**");
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE, color);
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, TRUE);
}

void TriangleObject::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**TriangleObject**");
	DrawFormatString(x, y + DRAW_FORMAT_STRING_SIZE, color, "mideleY=%d", getY(m_x2 - m_x1));
	debugObject(x + DRAW_FORMAT_STRING_SIZE, y + DRAW_FORMAT_STRING_SIZE * 2, color);
	if (m_leftDown) {
		DrawTriangle(m_x2, m_y1, m_x2, m_y2, m_x1, m_y2, color, TRUE);
	}
	else {
		DrawTriangle(m_x1, m_y1, m_x2, m_y2, m_x1, m_y2, color, TRUE);
	}
}