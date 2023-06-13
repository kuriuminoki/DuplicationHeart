#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


class Object;


class AttackInfo {
public:
	// 弾丸のダメージ
	int m_bulletDamage;

	// 弾丸の大きさ
	int m_bulletRx, m_bulletRy;

	// 弾丸のスピード
	int m_bulletSpeed;

	// 弾丸の連射力
	int m_bulletRapid;

	// 斬撃のダメージ
	int m_slashDamage;

	// 斬撃の大きさ
	int m_slashRx, m_slashRy;

	// 斬撃の全体フレーム
	int m_slashCountSum;
};


/*
* プレイヤーやエネミーの基底クラス
*/
class Character {
protected:
	static int characterId;

	// ID
	int m_id;

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

	// 画像の拡大率
	double m_ex;

	// 移動スピード
	int m_moveSpeed;

	// ジャンプの強さ
	int m_jumpHeight;

	// キャラ名
	const char* m_name;

	// 攻撃の情報
	AttackInfo* m_attackInfo;

	// 左を向いている
	bool m_leftDirection;

public:
	// コンストラクタ
	Character();
	Character(int maxHp, int hp, int x, int y);
	~Character();

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

	inline int getWide() { return m_wide; }

	inline int getHeight() { return m_height; }

	inline int getMoveSpeed() { return m_moveSpeed; }

	inline int getJumpHeight() { return m_jumpHeight; }

	inline const char* getName() { return m_name; }

	inline void setEx(double ex) { m_ex = ex; }

	// 画像のセッタ。画像の横幅(wide)と縦幅(height)もセットする。
	void setHandle(int handle);

	// 立ち画像をセット
	virtual void switchStand() = 0;

	// 移動する（座標を動かす）
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// 射撃攻撃をする(キャラごとに違う)
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// 斬撃攻撃をする(キャラごとに違う)
	virtual Object* slashAttack(int cnt, int gx, int gy) = 0;
};


/*
* ハート（主人公）
*/
class Heart :
	public Character
{
private:
	const char* const NAME = "ハート";
	// 画像の拡大率
	const double DRAW_EX = 0.5;

	// 走るスピード
	const int MOVE_SPEED = 10;

	// ジャンプの強さ
	const int JUMP_HEIGHT = 30;

	//立ち画像
	int m_standHandle;
	
public:
	// コンストラクタ
	Heart(int maxHp, int hp, int x, int y);

	// デバッグ
	void debug(int x, int y, int color);

	// 立ち画像をセット
	inline void switchStand() { setHandle(m_standHandle); }

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(int cnt, int gx, int gy);
};


#endif