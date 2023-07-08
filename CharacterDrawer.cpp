#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"


CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
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
	// ‰æ‘œ‚Ì’†S‚ðÀ•W‚Æ‚·‚é
	x = character->getX() + (character->getWide() / 2);
	y = character->getY() + (character->getHeight() / 2);
	// ‰æ‘œŒÅ—L‚ÌŠg‘å—¦Žæ“¾
	ex = graphHandle->getEx();

	// ƒJƒƒ‰‚Å’²®
	camera->setCamera(&x, &y, &ex);

	// •`‰æ
	graphHandle->draw(x, y, ex);
}