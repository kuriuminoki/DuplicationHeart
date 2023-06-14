#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

class CharacterController;


/*
* オブジェクトの基底クラス
*/
class Object {
protected:
	// 左上の座標
	int m_x1, m_y1;

	// 右下の座標+1
	int m_x2, m_y2;

	// 削除フラグ trueならWorldに消してもらう
	bool m_deleteFlag;

public:
	Object();
	Object(int x1, int y1, int x2, int y2);

	void debugObject(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタ
	inline bool getDeleteFlag() { return m_deleteFlag; }

	// キャラとの当たり判定
	virtual void atari(CharacterController* character) = 0;

	// 動くオブジェクト用 毎フレーム行う
	virtual void action() = 0;
};


/*
* 四角形のオブジェクト　床や壁
*/
class BoxObject :
	public Object 
{
private:
	// 段差とみなして自動で乗り越えられる高さ
	const int STAIR_HEIGHT = 50;

	// オブジェクトの色
	int m_color;

public:
	BoxObject(int x1, int y1, int x2, int y2, int color);

	void debug(int x, int y, int color);

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

/*
* 直角三角形のオブジェクトつまり坂
*/
class TriangleObject :
	public Object
{
private:
	// オブジェクトの色
	int m_color;

	// 左向きに下がっている坂
	bool m_leftDown;

	// 座標XにおけるY座標（傾きから算出する）
	int getY(int x);
public:
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown);

	void debug(int x, int y, int color);

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

/*
* 直進する弾のオブジェクト
*/
class BulletObject :
	public Object
{
private:
	// この攻撃を出したキャラのＩＤ 自滅やチームキル防止用
	int m_characterId;

	// オブジェクトの色
	int m_color;

	// 半径
	int m_rx;
	int m_ry;

	// スピード
	int m_v;
	int m_vx;
	int m_vy;

	// 目標地点
	int m_gx;
	int m_gy;

	// 残りの飛距離
	int m_d;

	// ダメージ
	int m_damage;

public:
	BulletObject(int x1, int y1, int x2, int y2, int color, int gx, int gy, int damage, int speed, int distance);

	void debug(int x, int y, int color);

	// セッタ
	inline void setCharacterId(int id) { m_characterId = id; }

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

#endif