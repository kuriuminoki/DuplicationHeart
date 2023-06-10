#include "Character.h"
#include "DxLib.h"


Character::Character() :
	Character(100, 100, 0, 0, 1)
{

}

Character::Character(int maxHp, int hp, int x, int y, int moveSpeed) {
	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_handle = -1;

	m_moveSpeed = moveSpeed;
}

void Character::debugCharacter(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Character**");
	DrawFormatString(x, y + 10, color, "HP=%d/%d, (x,y)=(%d,%d)", m_hp, m_maxHp, m_x, m_y);
	DrawFormatString(x, y + 20, color, "(wide, height)=(%d,%d), handle=%d", m_wide, m_height, m_handle);
}

void Character::setHandle(int handle) {

	m_handle = handle;

	//画像の縦幅と横幅を取得する。
	GetGraphSize(m_handle, &m_wide, &m_height);
}


/*
* ハート
*/
Heart::Heart(int maxHp, int hp, int x, int y, int moveSpeed) :
	Character(maxHp, hp, x, y, moveSpeed)
{
	m_standHandle = LoadGraph("picture/stick/stand.png");
}

void Heart::debug(int x, int y, int color) {
	DrawFormatString(x, y, color, "**Heart**");
	DrawFormatString(x, y + 10, color, "standHandle=%d", m_standHandle);
	debugCharacter(x + 10, y + 20, color);
}