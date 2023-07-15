#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


class Camera {
private:
	// カメラの座標(画面の中心)
	int m_x, m_y;

	// 今カメラが向かっている座標
	int m_gx, m_gy;

	// カメラが動くスピード
	int m_speed;
	int m_maxSpeed;

	// カメラの倍率
	double m_ex;

	// 画面中心の座標
	int m_centerX, m_centerY;

public:
	Camera();
	Camera(int x, int y, double ex, int speed);

	// ゲッタとセッタ
	inline void getPoint(int* x, int* y) { *x = m_x; *y = m_y; }
	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline void setPoint(int x, int y) { m_x = x; m_y = y; }
	inline void setGPoint(int x, int y) { m_gx = x; m_gy = y; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setGx(int x) { m_gx = x; }
	inline void setGy(int y) { m_gy = y; }
	inline void setSpeed(int speed) { m_speed = speed; }
	inline double getEx() { return m_ex; }
	inline void setEx(double ex) { m_ex = ex; }

	// カメラの動き
	void move();

	// カメラの座標を考慮した描画位置の補正
	void setCamera(int* x, int* y, double* ex) const;

	// マウスの位置を取得
	void getMouse(int* x, int* y) const;
};


#endif