#include "Animation.h"
#include "Character.h"
#include "GraphHandle.h"
#include "Item.h"
#include "DxLib.h"

using namespace std;


/*
* �A�C�e��
*/
Item::Item(const char* itemName, int x, int y) {
	m_itemName = itemName;
	m_x = x;
	m_y = y;
	m_cnt = 0;
	m_vx = 0;
	m_vy = 0;
	m_grand = false;
	m_sound = LoadSoundMem(("sound/item/" + m_itemName + ".wav").c_str());
	m_deleteFlag = false;
	m_exRate = 1.0;
}

// �f�X�g���N�^
Item::~Item() {
	DeleteSoundMem(m_sound);
	delete m_handles;
	delete m_animation;
}

// �R�s�[
void Item::setParam(Item* item) {
	item->setCnt(m_cnt);
	item->setGrand(m_grand);
	item->setAnimation(m_animation->createCopy());
	item->setVx(m_vx);
	item->setVy(m_vy);
	item->setExRate(m_exRate);
}

void Item::loadItemGraph() {
	m_handles = new GraphHandles(("picture/item/" + m_itemName).c_str(), 1, 0.05 * m_exRate, 0.0, true);
	m_animation = new Animation(m_x, m_y, 10, m_handles);
}

void Item::setY(int y) {
	int wide = 0, height = 0;
	getGraphSize(&wide, &height);
	m_y = y - (height / 2);
	m_animation->setY(m_y);
}

// �擾�ς݂����ʉ����Đ�������Ȃ��Ȃ�폜���Ă��炤
bool Item::getDeleteFlag() const { 
	return m_deleteFlag;
}

// �A�C�e���̑傫��
void Item::getGraphSize(int* wide, int* height) const {
	double ex = m_animation->getHandle()->getEx();
	GetGraphSize(m_animation->getHandle()->getHandle(), wide, height);
	*wide = (int)(*wide * ex);
	*height = (int)(*height * ex);
}

// ���W
void Item::getPoint(int* x1, int* y1, int* x2, int* y2) {
	int wide = 0, height = 0;
	getGraphSize(&wide, &height);
	wide = (int)(wide * m_exRate);
	height = (int)(height * m_exRate);
	*x1 = m_x - (wide / 2);
	*y1 = m_y - (height / 2);
	*x2 = *x1 + wide;
	*y2 = *y1 + height;
}

// ���t���[���̏���������
void Item::init() {
	m_grand = false;
}

// ���� ���t���[���Ă�
void Item::action() {

	m_cnt++;
	if (m_cnt > getEraseCnt()) {
		m_deleteFlag = true;
	}

	// �A�j�������Z�b�g
	if (m_animation->getFinishFlag()) {
		m_animation->init();
	}

	// �A�j���𓮂���
	m_animation->count();

	// �d�͂̉e�����󂯂�
	if (getEnableGravity()) {
		if (m_grand) {
			// ���n���Ă�
			m_vx = 0;
			m_vy = 0;
		}

		// �ړ�
		m_x += m_vx;
		m_y += m_vy;
		m_animation->setX(m_x);
		m_animation->setY(m_y);

		if (!m_grand) {
			// �d��
			m_vy += 1;
		}
	}

}

// �v���C���[�Ƃ̓����蔻��
bool Item::atariCharacter(Character* player) {

	// �L�����̍��W
	int cx1 = 0;
	int cy1 = 0;
	int cx2 = 0;
	int cy2 = 0;
	player->getAtariArea(&cx1, &cy1, &cx2, &cy2);
	
	// ���̃A�C�e���̍��W
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getPoint(&x1, &y1, &x2, &y2);

	// ��������������
	if (x2 > cx1 && x1 < cx2 && y2 > cy1 && y1 < cy2) {
		arrangePlayer(player);
		m_deleteFlag = true;
		return true;
	}

	return false;

}


/*
* �̗͉񕜃A�C�e��
*/
CureItem::CureItem(const char* itemName, int x, int y, int cureValue):
	Item(itemName, x, y)
{
	m_cureValue = cureValue;
	loadItemGraph();
}

// �X�L�������p
Item* CureItem::createCopy() {
	CureItem* item = new CureItem(m_itemName.c_str(), m_x, m_y, m_cureValue);
	setParam(item);
	return item;
}

// �v���C���[�ɑ΂���A�N�V����
void CureItem::arrangePlayer(Character* player) {
	// HP��
	player->setHp(player->getHp() + m_cureValue);
}


/*
* �����l���A�C�e��
*/
MoneyItem::MoneyItem(const char* itemName, int x, int y, int moneyValue) :
	Item(itemName, x, y)
{
	m_moneyValue = moneyValue;
	loadItemGraph();
}

// �X�L�������p
Item* MoneyItem::createCopy() {
	MoneyItem* item = new MoneyItem(m_itemName.c_str(), m_x, m_y, m_moneyValue);
	setParam(item);
	return item;
}

// �v���C���[�ɑ΂���A�N�V����
void MoneyItem::arrangePlayer(Character* player) {
	// �����l��
	player->setMoney(player->getMoney() + m_moneyValue);
}


/*
* �G�l���M�[�l���A�C�e��
*/
EnergyItem::EnergyItem(const char* itemName, int x, int y, int energyValue, int eraseTime) :
	Item(itemName, x, y)
{
	m_energyValue = energyValue;
	m_eraseTime = eraseTime;
	// �l���ʂɉ����ăT�C�Y�A�b�v
	m_exRate = 1.0 + energyValue / 10;
	loadItemGraph();
}

// �X�L�������p
Item* EnergyItem::createCopy() {
	EnergyItem* item = new EnergyItem(m_itemName.c_str(), m_x, m_y, m_energyValue, m_eraseTime);
	setParam(item);
	return item;
}

void EnergyItem::loadItemGraph() {
	m_handles = new GraphHandles(("picture/item/" + m_itemName).c_str(), 2, 0.2 * m_exRate, 0.0, true);
	m_animation = new Animation(m_x, m_y, 3, m_handles);
}

// �v���C���[�ɑ΂���A�N�V����
void EnergyItem::arrangePlayer(Character* player) {
	// �G�l���M�[�l��
	player->setSkillGage(player->getSkillGage() + m_energyValue);
}