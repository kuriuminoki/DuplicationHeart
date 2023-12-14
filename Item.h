#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


#include <string>


class Animation;
class Character;
class GraphHandles;


/*
* アイテム
*/
class Item {
protected:

	// 座標
	int m_x, m_y;

	// 速度
	int m_vx, m_vy;

	// 着地している
	bool m_grand;

	// アイテム名　画像ロード用
	std::string m_itemName;

	// 画像
	GraphHandles* m_handles;

	// アニメーション
	Animation* m_animation;

	// 効果音
	int m_sound;

public:

	// コンストラクタ
	Item(const char* itemName, int x, int y);

	// デストラクタ
	~Item();

	// ゲッタ
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline const char* getItemName() const { return m_itemName.c_str(); }
	inline bool getGrand() const { return m_grand; }
	inline int getSound() const { return m_sound; }
	inline const Animation* getAnimation() const { return m_animation; }

	// セッタ
	inline void setGrand(bool grand) { m_grand = grand; }

	// アイテムの大きさ
	void getGraphSize(int* wide, int* height) const;

	// 毎フレームの初期化処理
	void init();

	// 動き 毎フレーム呼ぶ
	void action();

	// プレイヤーに対するアクション
	virtual void arrangePlayer(Character* player) {  }

};


/*
* 回復アイテム
*/
class CureItem :
	public Item
{
private:

	// HPの回復量
	int m_cureValue;

public:

	// コンストラクタ
	CureItem(const char* itemName, int x, int y, int cureValue);

	// プレイヤーに対するアクション
	void arrangePlayer(Character* player);

};


#endif