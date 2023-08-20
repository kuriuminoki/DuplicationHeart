#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"
#include "DxLib.h"


// 体力バーを表示
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int maxHp, int damageColor, int hpColor) {
	int wide = x2 - x1;
	int damage = wide * (maxHp - hp) / maxHp;
	DrawBox(x1, y1, x1 + damage, y2, damageColor, TRUE);
	DrawBox(x1 + damage, y1, x2, y2, hpColor, TRUE);
}

int CharacterDrawer::HP_COLOR = GetColor(0, 255, 0);
int CharacterDrawer::DAMAGE_COLOR = GetColor(255, 0, 0);

CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
	m_cnt = 0;
}

// キャラを描画する
void CharacterDrawer::drawCharacter(const Camera* const camera, int bright) {
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
	if (m_characterAction->getState() == CHARACTER_STATE::DAMAGE && ++m_cnt / 2 % 2 == 1) {
		int dark = max(0, bright - 100);
		SetDrawBright(dark, dark, dark);
		graphHandle->draw(x, y, ex);
		SetDrawBright(bright, bright, bright);
	}
	else {
		graphHandle->draw(x, y, ex);
	}

	// 体力バーの座標をカメラで調整
	x = character->getX() + (character->getWide() / 2);
	y = character->getY();
	ex = graphHandle->getEx();
	camera->setCamera(&x, &y, &ex);
	int wide = (int)(HP_BAR_WIDE / 2 * camera->getEx());
	int height = (int)(HP_BAR_HEIGHT * camera->getEx());
	y -= (int)(10 * camera->getEx());
	// 体力の描画
	drawHpBar(x - wide, y - height, x + wide, y, character->getHp(), character->getMaxHp(), DAMAGE_COLOR, HP_COLOR);
}