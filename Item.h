#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


#include <string>


class Animation;
class Character;
class GraphHandles;


/*
* �A�C�e��
*/
class Item {
protected:

	// ���W
	int m_x, m_y;

	// ���x
	int m_vx, m_vy;

	// ���n���Ă���
	bool m_grand;

	// �A�C�e�����@�摜���[�h�p
	std::string m_itemName;

	// �摜
	GraphHandles* m_handles;

	// �A�j���[�V����
	Animation* m_animation;

	// ���ʉ�
	int m_sound;

public:

	// �R���X�g���N�^
	Item(const char* itemName, int x, int y);

	// �f�X�g���N�^
	~Item();

	// �Q�b�^
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline const char* getItemName() const { return m_itemName.c_str(); }
	inline bool getGrand() const { return m_grand; }
	inline int getSound() const { return m_sound; }
	inline const Animation* getAnimation() const { return m_animation; }

	// �Z�b�^
	inline void setGrand(bool grand) { m_grand = grand; }

	// �A�C�e���̑傫��
	void getGraphSize(int* wide, int* height) const;

	// ���t���[���̏���������
	void init();

	// ���� ���t���[���Ă�
	void action();

	// �v���C���[�ɑ΂���A�N�V����
	virtual void arrangePlayer(Character* player) {  }

};


/*
* �񕜃A�C�e��
*/
class CureItem :
	public Item
{
private:

	// HP�̉񕜗�
	int m_cureValue;

public:

	// �R���X�g���N�^
	CureItem(const char* itemName, int x, int y, int cureValue);

	// �v���C���[�ɑ΂���A�N�V����
	void arrangePlayer(Character* player);

};


#endif