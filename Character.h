#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/*
* プレイヤーやエネミーの基底クラス
*/
class Character {
private:
	// 最大体力
	int m_maxHp;

	// 残り体力
	int m_hp;

	// X座標、Y座標
	int m_x, m_y;

	// 横幅と縦幅
	int m_wide, m_height;

	// 表示される画像
	int m_handle;

	// 移動スピード
	int m_moveSpeed;

public:
	// コンストラクタ
	Character();
	Character(int maxHp, int hp, int x, int y, int moveSpeed);

	// デバッグ
	void debugCharacter(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタとセッタ
	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	inline int getX() { return m_x; }

	inline void setX(int x) { m_x = x; }

	inline int getY() { return m_y; }

	inline void setY(int y) { m_y = y; }

	inline int getMoveSpeed() { return m_moveSpeed; }

	// 画像のセッタ。画像の横幅(wide)と縦幅(height)もセットする。
	void setHandle(int handle);

	// 立ち画像をセット
	virtual void switchStand() = 0;

};


/*
* ハート（主人公）
*/
class Heart :
	public Character
{
private:
	//立ち画像
	int m_standHandle;
	
public:
	// コンストラクタ
	Heart(int maxHp, int hp, int x, int y, int moveSpeed);

	// デバッグ
	void debug(int x, int y, int color);

	// 立ち画像をセット
	inline void switchStand() { setHandle(m_standHandle); }
};


#endif