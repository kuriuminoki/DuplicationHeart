#include "Character.h"
#include "DxLib.h"


Character::Character() {

}

void Character::setHandle(int handle) {

	m_handle = handle;

	//�摜�̏c���Ɖ������擾����B
	GetGraphSize(m_handle, &m_wide, &m_height);
}