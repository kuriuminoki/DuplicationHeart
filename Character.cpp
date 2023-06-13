#include "Character.h"
#include "Object.h"
#include "Define.h"
#include "DxLib.h"


Character::Character() :
	Character(100, 100, 0, 0)
{

}

Character::Character(int maxHp, int hp, int x, int y) {
	// IDを振る
	characterId++;
	m_id = characterId;

	m_name = "名無し";
	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_handle = -1;
	m_ex = 1.0;

	m_moveSpeed = 1;
	m_jumpHeight = 1;

	m_attackInfo = new AttackInfo();

	m_leftDirection = true;
}

Character::~Character() {
	delete m_attackInfo;
}

void Character::setHandle(int handle) {

	m_handle = handle;

	// 画像の縦幅と横幅を取得する。
	GetGraphSize(m_handle, &m_wide, &m_height);

	// 画像の拡大率も考慮してサイズを決定
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

// 移動する（座標を動かす）
void Character::moveRight(int d) {
	m_x += d;
}
void Character::moveLeft(int d) {
	m_x -= d;
}
void Character::moveUp(int d) {
	m_y -= d;
}
void Character::moveDown(int d) {
	m_y += d;
}


/*
* ハート
*/
Heart::Heart(int maxHp, int hp, int x, int y) :
	Character(maxHp, hp, x, y)
{
	// 名前
	m_name = NAME;

	// 画像の拡大率
	m_ex = DRAW_EX;

	// 走る速さ
	m_moveSpeed = MOVE_SPEED;

	// ジャンプの強さ
	m_jumpHeight = JUMP_HEIGHT;

	// 各画像のロード
	m_standHandle = LoadGraph("picture/stick/stand.png");
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy) {

	return NULL;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(int cnt, int gx, int gy) {

	return NULL;
}
