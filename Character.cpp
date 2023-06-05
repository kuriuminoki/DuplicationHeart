#include "Character.h"
#include "DxLib.h"


Character::Character() {

}

void Character::setHandle(int handle) {

	m_handle = handle;

	//‰æ‘œ‚Ìc•‚Æ‰¡•‚ğæ“¾‚·‚éB
	GetGraphSize(m_handle, &m_wide, &m_height);
}