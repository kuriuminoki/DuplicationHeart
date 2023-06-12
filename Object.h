#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

class CharacterController;


/*
* �I�u�W�F�N�g�̊��N���X
*/
class Object {
protected:
	// ����̍��W
	int m_x1, m_y1;

	// �E���̍��W+1
	int m_x2, m_y2;

public:
	Object();
	Object(int x1, int y1, int x2, int y2);

	void debugObject(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �L�����Ƃ̓����蔻��
	virtual void atari(CharacterController* character) = 0;
};


/*
* �l�p�`�̃I�u�W�F�N�g�@�����
*/
class BoxObject :
	public Object 
{
private:
	// �i���Ƃ݂Ȃ��Ď����ŏ��z�����鍂��
	const int STAIR_HEIGHT = 50;

	// �I�u�W�F�N�g�̐F
	int m_color;

public:
	BoxObject(int x1, int y1, int x2, int y2, int color);

	void debug(int x, int y, int color);

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	void atari(CharacterController* character);
};

/*
* ���p�O�p�`�̃I�u�W�F�N�g�܂��
*/
class TriangleObject :
	public Object
{
private:
	// �I�u�W�F�N�g�̐F
	int m_color;

	// �������ɉ������Ă����
	bool m_leftDown;

	// ���WX�ɂ�����Y���W�i�X������Z�o����j
	int getY(int x);
public:
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown);

	void debug(int x, int y, int color);

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	void atari(CharacterController* character);
};

#endif