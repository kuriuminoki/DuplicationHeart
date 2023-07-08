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

// キャラを描画する
void CharacterDrawer::drawCharacter(const Camera* const camera) {
	// 描画するキャラクター
	const Character* character = m_characterAction->getCharacter();

	// 画像の情報
	const GraphHandle* graphHandle = character->getGraphHandle();

	// 座標と拡大率取得
	int x, y;
	double ex;
	x = character->getX();
	y = character->getY();
	ex = graphHandle->getEx();

	// カメラで調整
	camera->setCamera(&x, &y, &ex);

	// 描画
	graphHandle->draw(x, y, ex);
}