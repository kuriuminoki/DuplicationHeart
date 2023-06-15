#include "Character.h"
#include "Object.h"
#include "Define.h"
#include "DxLib.h"


AttackInfo::AttackInfo():
	AttackInfo(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
{

}
// CSVファイルから読み込む予定
AttackInfo::AttackInfo(const char* characterName) {

}
AttackInfo::AttackInfo(int bulletDamage, int bulletRx, int bulletRy, int bulletSpeed, int bulletRapid, int bulletDistance, int slashDamage, int slashRx, int slashRy, int slashCountSum) {
	m_bulletDamage = bulletDamage;
	m_bulletRx = bulletRx;
	m_bulletRy = bulletRy;
	m_bulletSpeed = bulletSpeed;
	m_bulletRapid = bulletRapid;
	m_bulletDistance = bulletDistance;
	m_slashDamage = slashDamage;
	m_slashRx = slashRx;
	m_slashRy = slashRy;
	m_slashCountSum = slashCountSum;
}


int Character::characterId;

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

	m_attackInfo = NULL;

	m_leftDirection = true;
}

Character::~Character() {
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
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

	m_slashHandleSum = 3;
	m_slashHandles = new int[m_slashHandleSum];

	m_attackInfo = new AttackInfo(0, 100, 100, 50, 30, 800, 0, 30, 30, 30);
}

// デストラクタ
Heart::~Heart() {
	DeleteGraph(m_standHandle);
	for (int i = 0; i < m_slashHandleSum; i++) {
		DeleteGraph(m_slashHandles[i]);
	}
	delete m_slashHandles;
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x, m_y,
		m_x + m_attackInfo->m_bulletRx, m_y + m_attackInfo->m_bulletRy,
		WHITE, gx, gy,
		m_attackInfo->m_bulletDamage, m_attackInfo->m_bulletSpeed, m_attackInfo->m_bulletDistance);
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(bool leftDirection) {
	int x2 = m_x;
	int y2 = m_y;
	if (leftDirection) { // 左向きに攻撃
		x2 -= m_attackInfo->m_slashRx;
		y2 -= m_attackInfo->m_slashRy;
	}
	else { // 右向きに攻撃
		x2 += m_attackInfo->m_slashRx;
		y2 += m_attackInfo->m_slashRy;
	}
	SlashObject* attackObject = new SlashObject(m_x, m_y, 
		m_x + m_attackInfo->m_slashRx, m_y + m_attackInfo->m_slashRy,
		m_slashHandles, m_slashHandleSum, 
		m_attackInfo->m_slashDamage, m_attackInfo->m_slashCountSum);
	return attackObject;
}
