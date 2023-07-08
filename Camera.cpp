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

// �J�����ō��W�Ɗg�嗦�𒲐�����
void Camera::setCamera(int handle, int* x, int* y, double* ex) {
	// �J��������̂�����v�Z
	int dx = *x - m_x;
	int dy = *y - m_y;

	// �g�嗦����ŋ������ς��
	dx = (int)(dx * m_ex);
	dy = (int)(dy * m_ex);
	
	// ��ʂ̒��S����ɍ��W������
	*x = m_centerX + dx;
	*y = m_centerY + dy;

	// �g�嗦
	*ex *= m_ex;
}