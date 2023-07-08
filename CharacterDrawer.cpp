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

// ƒLƒƒƒ‰‚ð•`‰æ‚·‚é
void CharacterDrawer::drawCharacter(const Camera* const camera) {
	// •`‰æ‚·‚éƒLƒƒƒ‰ƒNƒ^[
	const Character* character = m_characterAction->getCharacter();

	// ‰æ‘œ‚Ìî•ñ
	const GraphHandle* graphHandle = character->getGraphHandle();

	// À•W‚ÆŠg‘å—¦Žæ“¾
	int x, y;
	double ex;
	x = character->getX();
	y = character->getY();
	ex = graphHandle->getEx();

	// ƒJƒƒ‰‚Å’²®
	camera->setCamera(&x, &y, &ex);

	// •`‰æ
	graphHandle->draw(x, y, ex);
}