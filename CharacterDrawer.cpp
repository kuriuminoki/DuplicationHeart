#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"


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
	// �`�悷��L�����N�^�[
	const Character* character = m_characterAction->getCharacter();

	// �摜�̏��
	const GraphHandle* graphHandle = character->getGraphHandle();

	// ���W�Ɗg�嗦�擾
	int x, y;
	double ex;
	x = character->getX();
	y = character->getY();
	ex = graphHandle->getEx();

	// �J�����Œ���
	camera->setCamera(&x, &y, &ex);

	// �`��
	graphHandle->draw(x, y, ex);
}