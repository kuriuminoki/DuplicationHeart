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

	// 取得済み
	bool m_deleteFlag;

public:

	// コンストラクタ
	Item(const char* itemName, int x, int y);

	// デストラクタ
	virtual ~Item();

	// スキル発動用
	virtual Item* createCopy() = 0;

	// コピー
	void setParam(Item* item);

	// ゲッタ
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline const char* getItemName() const { return m_itemName.c_str(); }
	inline bool getGrand() const { return m_grand; }
	inline int getSound() const { return m_sound; }
	inline const Animation* getAnimation() const { return m_animation; }
	
	// 取得済みかつ効果音が再生中じゃないなら削除してもらう
	bool getDeleteFlag() const;

	// セッタ
	inline void setGrand(bool grand) { m_grand = grand; }
	void setY(int y);
	void setAnimation(Animation* animation) { delete m_animation; m_animation = animation; }

	// アイテムの大きさ
	void getGraphSize(int* wide, int* height) const;

	// 座標
	void getPoint(int* x1, int* y1, int* x2, int* y2);

	// 毎フレームの初期化処理
	void init();

	// 動き 毎フレーム呼ぶ
	void action();

	// プレイヤーとの当たり判定
	bool atariCharacter(Character* player);

private:

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

	// スキル発動用
	Item* createCopy();

	// セッタ
	inline void setCureValue(int cureValue) { m_cureValue = cureValue; }

	// プレイヤーに対するアクション
	void arrangePlayer(Character* player);

};


#endif