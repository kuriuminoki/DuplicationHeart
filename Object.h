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

public:
	Object();
	Object(int x1, int y1, int x2, int y2);

	void debugObject(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// キャラとの当たり判定
	virtual void atari(CharacterController* character) = 0;
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
};

#endif