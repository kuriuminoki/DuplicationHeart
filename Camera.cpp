#include "Camera.h"
#include "Define.h"


Camera::Camera() :
	Camera(0, 0, 1.0)
{

}

Camera::Camera(int x, int y, double ex) {
	m_x = x;
	m_y = y;
	m_ex = ex;
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
}

// カメラで座標と拡大率を調整する
void Camera::setCamera(int handle, int* x, int* y, double* ex) {
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