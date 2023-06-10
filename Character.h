#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/*
* �v���C���[��G�l�~�[�̊��N���X
*/
class Character {
private:
	// �ő�̗�
	int m_maxHp;

	// �c��̗�
	int m_hp;

	// X���W�AY���W
	int m_x, m_y;

	// �����Əc��
	int m_wide, m_height;

	// �\�������摜
	int m_handle;

	// �ړ��X�s�[�h
	int m_moveSpeed;

public:
	// �R���X�g���N�^
	Character();
	Character(int maxHp, int hp, int x, int y, int moveSpeed);

	// �f�o�b�O
	void debugCharacter(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �Q�b�^�ƃZ�b�^
	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	inline int getX() { return m_x; }

	inline void setX(int x) { m_x = x; }

	inline int getY() { return m_y; }

	inline void setY(int y) { m_y = y; }

	inline int getMoveSpeed() { return m_moveSpeed; }

	// �摜�̃Z�b�^�B�摜�̉���(wide)�Əc��(height)���Z�b�g����B
	void setHandle(int handle);

	// �����摜���Z�b�g
	virtual void switchStand() = 0;

};


/*
* �n�[�g�i��l���j
*/
class Heart :
	public Character
{
private:
	//�����摜
	int m_standHandle;
	
public:
	// �R���X�g���N�^
	Heart(int maxHp, int hp, int x, int y, int moveSpeed);

	// �f�o�b�O
	void debug(int x, int y, int color);

	// �����摜���Z�b�g
	inline void switchStand() { setHandle(m_standHandle); }
};


#endif