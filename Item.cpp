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
	m_handles = new GraphHandles(("picture/item/" + m_itemName).c_str(), 1, 1.0, 0.0, true);
	m_animation = new Animation(m_x, m_y, 10, m_handles);
}

// デストラクタ
Item::~Item() {
	DeleteSoundMem(m_sound);
	delete m_handles;
	delete m_animation;
}

// アイテムの大きさ
void Item::getGraphSize(int* wide, int* height) const {
	GetGraphSize(m_animation->getHandle()->getHandle(), wide, height);
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
	else {
		// 重力
		m_y += 1;
	}
	
	// 移動
	m_x += m_vx;
	m_y += m_vy;

}


/*
* 体力回復アイテム
*/
CureItem::CureItem(const char* itemName, int x, int y, int cureValue):
	Item(itemName, x, y)
{
	m_cureValue = cureValue;
}

// プレイヤーに対するアクション
void CureItem::arrangePlayer(Character* player) {
	// HP回復
	player->setHp(player->getHp() + m_cureValue);
}