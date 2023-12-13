#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


class Camera {
private:
	// カメラの移動速度の初期値
	const int CAMERA_SPEED_DEFAULT = 5;

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

	// 画面の揺れ幅
	int m_shakingWidth;

	// 揺れる残り時間
	int m_shakingTime;

public:
	Camera();
	Camera(int x, int y, double ex, int speed=0);
	Camera(const Camera* original);

	// ゲッタ
	inline void getPoint(int* x, int* y) const { *x = m_x; *y = m_y; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getGx() const { return m_gx; }
	inline int getGy() const { return m_gy; }
	inline double getEx() const { return m_ex; }
	inline int getSpeed() const { return m_speed; }
	inline int getMaxSpeed() const { return m_maxSpeed; }
	inline int getShakingWidth() const { return m_shakingWidth; }
	inline int getShakingTime() const { return m_shakingTime; }

	// セッタ
	inline void setPoint(int x, int y) { m_x = x; m_y = y; }
	inline void setGPoint(int x, int y) { m_gx = x; m_gy = y; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setGx(int x) { m_gx = x; }
	inline void setGy(int y) { m_gy = y; }
	inline void setSpeed(int speed) { m_speed = speed; }
	inline void setEx(double ex) { m_ex = ex; }

	// カメラの動き
	void move();

	// カメラの座標を考慮した描画位置の補正
	void setCamera(int* x, int* y, double* ex) const;

	// マウスの位置を取得
	void getMouse(int* x, int* y) const;

	// カメラを揺らし始める
	void shakingStart(int width, int time);

	// カメラを揺らす
	void shaking();
};


#endif