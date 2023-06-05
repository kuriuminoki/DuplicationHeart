#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


/*
  �v���C���[��G�l�~�[�̊��N���X
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

public:
	Character();

	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	// �摜�̃Z�b�^�B�摜�̉���(wide)�Əc��(height)���Z�b�g����B
	void setHandle(int handle);
};


#endif