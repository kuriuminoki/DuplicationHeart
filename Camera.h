#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


class Camera {
private:
	// �J�����̍��W(��ʂ̒��S)
	int m_x, m_y;

	// ���J�������������Ă�����W
	int m_gx, m_gy;

	// �J�����������X�s�[�h
	int m_speed;
	int m_maxSpeed;

	// �J�����̔{��
	double m_ex;

	// ��ʒ��S�̍��W
	int m_centerX, m_centerY;

public:
	Camera();
	Camera(int x, int y, double ex, int speed);

	// �Q�b�^�ƃZ�b�^
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

	// �J�����̓���
	void move();

	// �J�����̍��W���l�������`��ʒu�̕␳
	void setCamera(int* x, int* y, double* ex) const;

	// �}�E�X�̈ʒu���擾
	void getMouse(int* x, int* y) const;
};


#endif