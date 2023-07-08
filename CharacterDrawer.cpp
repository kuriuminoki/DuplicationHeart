#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"


CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
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
	// �摜�̒��S�����W�Ƃ���
	x = character->getX() + (character->getWide() / 2);
	y = character->getY() + (character->getHeight() / 2);
	// �摜�ŗL�̊g�嗦�擾
	ex = graphHandle->getEx();

	// �J�����Œ���
	camera->setCamera(&x, &y, &ex);

	// �`��
	graphHandle->draw(x, y, ex);
}