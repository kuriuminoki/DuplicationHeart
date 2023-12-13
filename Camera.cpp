#include "Camera.h"
#include "Define.h"
#include "DxLib.h"


Camera::Camera() :
	Camera(0, 0, 1.0, 1)
{

}

Camera::Camera(int x, int y, double ex, int speed) {
	m_x = x;
	m_y = y;
	m_gx = x;
	m_gy = y;
	m_ex = ex;
	m_speed = 1;
	m_maxSpeed = speed == 0 ? CAMERA_SPEED_DEFAULT : speed;
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
	m_shakingWidth = 0;
	m_shakingTime = 0;
}

Camera::Camera(const Camera* original) {
	m_x = original->getX();
	m_y = original->getY();
	m_gx = original->getGx();
	m_gy = original->getGy();
	m_ex = original->getEx();
	m_speed = original->getSpeed();
	m_maxSpeed = original->getMaxSpeed();
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
	m_shakingWidth = original->getShakingWidth();
	m_shakingTime = original->getShakingTime();
}

// カメラの移動 目標地点が近いほど鈍感になる
void Camera::move() {
	if (m_x < m_gx) {
		m_x += (m_gx - m_x) / 2 + 1;
	}
	if (m_x > m_gx) {
		m_x -= (m_x - m_gx) / 2 + 1;
	}
	if (m_y < m_gy) {
		m_y += (m_gy - m_y) / 2 + 1;
	}
	if (m_y > m_gy) {
		m_y -= (m_y - m_gy) / 2 + 1;
	}
}

// カメラで座標と拡大率を調整する
void Camera::setCamera(int* x, int* y, double* ex) const {
	// カメラからのずれを計算
	int dx = *x - m_x;
	int dy = *y - m_y;

	// 拡大率次第で距離も変わる
	dx = (int)(dx * m_ex);
	dy = (int)(dy * m_ex);
	
	// 画面の中心を基準に座標を決定
	*x = m_centerX + dx;
	*y = m_centerY + dy;

	// 拡大率
	*ex *= m_ex;
}

void Camera::getMouse(int* x, int* y) const {
	GetMousePoint(x, y);

	// 画面中央からのずれを計算
	int dx = *x - m_centerX;
	int dy = *y - m_centerY;

	// m_xとm_yは画面中央に対応するWorldの座標
	*x = m_x + dx;
	*y = m_y + dy;
}

// カメラを揺らす
void Camera::shakingStart(int width, int time) {
	m_shakingWidth = width;
	m_shakingTime = time;
}

// カメラを揺らす
void Camera::shaking() {
	if (m_shakingTime == 0) { return; }
	m_x += GetRand(m_shakingWidth * 2) - m_shakingWidth;
	m_y += GetRand(m_shakingWidth * 2) - m_shakingWidth;
	m_shakingTime--;
}