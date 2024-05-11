#include "Animation.h"
#include "Character.h"
#include "GraphHandle.h"
#include "Item.h"
#include "DxLib.h"

using namespace std;


/*
* アイテム
*/
Item::Item(const char* itemName, int x, int y) {
	m_itemName = itemName;
	m_x = x;
	m_y = y;
	m_vx = 0;
	m_vy = 0;
	m_grand = false;
	m_sound = LoadSoundMem(("sound/item/" + m_itemName + ".wav").c_str());
	m_handles = new GraphHandles(("picture/item/" + m_itemName).c_str(), 1, 0.05, 0.0, true);
	m_animation = new Animation(m_x, m_y, 10, m_handles);
	m_deleteFlag = false;
}

// デストラクタ
Item::~Item() {
	DeleteSoundMem(m_sound);
	delete m_handles;
	delete m_animation;
}

// コピー
void Item::setParam(Item* item) {
	item->setGrand(m_grand);
	item->setAnimation(m_animation->createCopy());
}

void Item::setY(int y) {
	int wide = 0, height = 0;
	getGraphSize(&wide, &height);
	m_y = y - (height / 2);
	m_animation->setY(m_y);
}

// 取得済みかつ効果音が再生中じゃないなら削除してもらう
bool Item::getDeleteFlag() const { 
	return m_deleteFlag;
}

// アイテムの大きさ
void Item::getGraphSize(int* wide, int* height) const {
	double ex = m_animation->getHandle()->getEx();
	GetGraphSize(m_animation->getHandle()->getHandle(), wide, height);
	*wide = (int)(*wide * ex);
	*height = (int)(*height * ex);
}

// 座標
void Item::getPoint(int* x1, int* y1, int* x2, int* y2) {
	int wide = 0, height = 0;
	getGraphSize(&wide, &height);
	*x1 = m_x - (wide / 2);
	*y1 = m_y - (height / 2);
	*x2 = *x1 + wide;
	*y2 = *y1 + height;
}

// 毎フレームの初期化処理
void Item::init() {
	m_grand = false;
}

// 動き 毎フレーム呼ぶ
void Item::action() {

	// アニメをリセット
	if (m_animation->getFinishFlag()) {
		m_animation->init();
	}

	// アニメを動かす
	m_animation->count();

	if (m_grand) {
		// 着地してる
		m_vy = 0;
	}
	
	// 移動
	m_x += m_vx;
	m_y += m_vy;
	m_animation->setX(m_x);
	m_animation->setY(m_y);

	// 重力
	m_vy += 1;

}

// プレイヤーとの当たり判定
bool Item::atariCharacter(Character* player) {

	// キャラの座標
	int cx1 = 0;
	int cy1 = 0;
	int cx2 = 0;
	int cy2 = 0;
	player->getAtariArea(&cx1, &cy1, &cx2, &cy2);
	
	// このアイテムの座標
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getPoint(&x1, &y1, &x2, &y2);

	// 当たったか判定
	if (x2 > cx1 && x1 < cx2 && y2 > cy1 && y1 < cy2) {
		arrangePlayer(player);
		m_deleteFlag = true;
		return true;
	}

	return false;

}


/*
* 体力回復アイテム
*/
CureItem::CureItem(const char* itemName, int x, int y, int cureValue):
	Item(itemName, x, y)
{
	m_cureValue = cureValue;
}

// スキル発動用
Item* CureItem::createCopy() {
	CureItem* item = new CureItem(m_itemName.c_str(), m_x, m_y, m_cureValue);
	setParam(item);
	return item;
}

// プレイヤーに対するアクション
void CureItem::arrangePlayer(Character* player) {
	// HP回復
	player->setHp(player->getHp() + m_cureValue);
}