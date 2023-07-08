#ifndef GRAPH_HANDLE_H_INCLUDED
#define GRAPH_HANDLE_H_INCLUDED

/*
* �摜�f�[�^(�n���h���A�g�嗦�A����)���܂Ƃ߂Ĉ������߂̃N���X
*/
class GraphHandle {
private:
	// �摜�n���h��
	int m_handle;

	// �g�嗦
	double m_ex;

	// ����
	double m_angle;

	// �����x��L���ɂ��邩
	bool m_trans;

	// ���]���邩
	bool m_reverseX;
	bool m_reverseY;

public:
	GraphHandle(const char* filePath, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandle();

	// �Q�b�^
	inline int getHandle() { return m_handle; }

	// �Z�b�^
	inline void setEx(double ex) { m_ex = ex; }
	inline void setAngle(double angle) { m_angle = angle; }
	inline void setTrans(bool trans) { m_trans = trans; }
	inline void setReverseX(bool reverse) { m_reverseX = reverse; }
	inline void setReverseY(bool reverse) { m_reverseY = reverse; }

	// �`�悷��
	void draw(int x, int y);
};


/*
* GraphHandle��z��Ƃ��Ă܂Ƃ߂Ĉ����N���X
*/
class GraphHandles {
private:
	GraphHandle** m_handles;
	int m_handleSum;

public:
	//  filePath�̖����ɂ�".png"�����Ȃ����ƁB
	GraphHandles(const char* filePath, int handleSum, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandles();

	// �Q�b�^
	inline GraphHandle* getGraphHandle(int i) { return m_handles[i]; }
	inline int getHandle(int i) { return m_handles[i]->getHandle(); }

	// �Z�b�^
	void setEx(double ex);
	void setAngle(double angle);
	void setTrans(bool trans);
	void setReverseX(bool reverse);
	void setReverseY(bool reverse);

	// �`�悷��
	void draw(int x, int y, int index);
};


#endif