#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"


CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
}

// キャラを描画する
void CharacterDrawer::drawCharacter(const Camera* const camera) {
	// 描画するキャラクター
	const Character* character = m_characterAction->getCharacter();

	// 画像の情報
	const GraphHandle* graphHandle = character->getGraphHandle();

	// 座標と拡大率取得
	int x, y;
	double ex;
	// 画像の中心を座標とする
	x = character->getX() + (character->getWide() / 2);
	y = character->getY() + (character->getHeight() / 2);
	// 画像固有の拡大率取得
	ex = graphHandle->getEx();

	// カメラで調整
	camera->setCamera(&x, &y, &ex);

	// 描画
	graphHandle->draw(x, y, ex);
}