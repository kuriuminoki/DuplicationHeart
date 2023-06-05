#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/*
  プレイヤーやエネミーの基底クラス
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

public:
	Character();

	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	// 画像のセッタ。画像の横幅(wide)と縦幅(height)もセットする。
	void setHandle(int handle);
};


#endif