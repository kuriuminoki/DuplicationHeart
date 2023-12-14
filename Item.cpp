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
	m_vx = 0;
	m_vy = 0;
	m_grand = false;
	m_sound = LoadSoundMem(("sound/item/" + m_itemName + ".wav").c_str());
	m_handles = new GraphHandles(("picture/item/" + m_itemName).c_str(), 1, 1.0, 0.0, true);
	m_animation = new Animation(m_x, m_y, 10, m_handles);
}

// �f�X�g���N�^
Item::~Item() {
	DeleteSoundMem(m_sound);
	delete m_handles;
	delete m_animation;
}

// �A�C�e���̑傫��
void Item::getGraphSize(int* wide, int* height) const {
	GetGraphSize(m_animation->getHandle()->getHandle(), wide, height);
}

// ���t���[���̏���������
void Item::init() {
	m_grand = false;
}

// ���� ���t���[���Ă�
void Item::action() {

	// �A�j�������Z�b�g
	if (m_animation->getFinishFlag()) {
		m_animation->init();
	}

	// �A�j���𓮂���
	m_animation->count();

	if (m_grand) {
		// ���n���Ă�
		m_vy = 0;
	}
	else {
		// �d��
		m_y += 1;
	}
	
	// �ړ�
	m_x += m_vx;
	m_y += m_vy;

}


/*
* �̗͉񕜃A�C�e��
*/
CureItem::CureItem(const char* itemName, int x, int y, int cureValue):
	Item(itemName, x, y)
{
	m_cureValue = cureValue;
}

// �v���C���[�ɑ΂���A�N�V����
void CureItem::arrangePlayer(Character* player) {
	// HP��
	player->setHp(player->getHp() + m_cureValue);
}