#include "CharacterDrawer.h"
#include "Camera.h"


CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
	m_cnt = 0;
	m_handle = -1;
	m_x = 0;
	m_y = 0;
	m_ex = 0;
}

// �L������`�悷��
void CharacterDrawer::drawCharacter(const Camera* const camera) {
	
}