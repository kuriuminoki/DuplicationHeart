#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED


#include <string>


class Animation;
class Character;
class GraphHandles;


enum ITEM_CODE {
	ITEM_CURE,
	ITEM_MONEY,
	ITEM_ENERGY
};


/*
* �A�C�e��
*/
class Item {
private:

	// �A�C�e����������܂ł̎���
	const int ERASE_CNT = 600;

protected:
	ITEM_CODE m_itemCode;

	int m_cnt;

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

	// �擾�ς�
	bool m_deleteFlag;

	// �T�C�Y�̔{��
	double m_exRate;

	virtual void loadItemGraph();

public:

	// �R���X�g���N�^
	Item(const char* itemName, int x, int y);

	// �f�X�g���N�^
	virtual ~Item();

	// �X�L�������p
	virtual Item* createCopy() = 0;

	// �R�s�[
	void setParam(Item* item);

	// �Q�b�^
	inline ITEM_CODE getItemCode() const { return m_itemCode; }
	inline int getCnt() const { return m_cnt; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline const char* getItemName() const { return m_itemName.c_str(); }
	inline bool getGrand() const { return m_grand; }
	inline int getSound() const { return m_sound; }
	inline const Animation* getAnimation() const { return m_animation; }
	double getExRate() const { m_exRate; }
	virtual int getEraseCnt() const { return ERASE_CNT; }
	virtual bool getEnableGravity() const { return true; }
	
	// �擾�ς݂����ʉ����Đ�������Ȃ��Ȃ�폜���Ă��炤
	bool getDeleteFlag() const;

	// �Z�b�^
	inline void setCnt(int cnt) { m_cnt = cnt; }
	inline void setGrand(bool grand) { m_grand = grand; }
	void setY(int y);
	inline void setVx(int vx) { m_vx = vx; }
	inline void setVy(int vy) { m_vy = vy; }
	inline void setAnimation(Animation* animation) { delete m_animation; m_animation = animation; }
	inline void setExRate(double exRate) { m_exRate = exRate; }

	// �A�C�e���̑傫��
	void getGraphSize(int* wide, int* height) const;

	// ���W
	void getPoint(int* x1, int* y1, int* x2, int* y2);

	// ���t���[���̏���������
	void init();

	// ���� ���t���[���Ă�
	void action();

	// �v���C���[�Ƃ̓����蔻��
	bool atariCharacter(Character* player);

private:

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

	// �X�L�������p
	Item* createCopy();

	// �v���C���[�ɑ΂���A�N�V����
	void arrangePlayer(Character* player);

};


/*
* �����A�C�e��
*/
class MoneyItem :
	public Item
{
private:

	// HP�̉񕜗�
	int m_moneyValue;

public:

	// �R���X�g���N�^
	MoneyItem(const char* itemName, int x, int y, int moneyValue);

	// �X�L�������p
	Item* createCopy();

	// �v���C���[�ɑ΂���A�N�V����
	void arrangePlayer(Character* player);

};


/*
* �G�l���M�[�A�C�e��
*/
class EnergyItem :
	public Item
{
private:

	// �A�C�e����������܂ł̎���
	int m_eraseTime;

	// HP�̉񕜗�
	int m_energyValue;

public:

	void loadItemGraph();

	// �R���X�g���N�^
	EnergyItem(const char* itemName, int x, int y, int energyValue, int eraseTime);

	// �X�L�������p
	Item* createCopy();

	// �v���C���[�ɑ΂���A�N�V����
	void arrangePlayer(Character* player);

	int getEraseCnt() const { return m_eraseTime; }
	bool getEnableGravity() const { return false; }

};


#endif