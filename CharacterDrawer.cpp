#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"


// 体力バーを表示
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int prevHp, int maxHp, int damageColor, int prevHpColor, int hpColor) {
	int wide = x2 - x1;
	int prev = wide * prevHp / maxHp;
	int remain = wide * hp / maxHp;
	DrawBox(x1, y1, x1 + remain, y2, hpColor, TRUE);
	DrawBox(x1 + remain, y1, x1 + prev, y2, prevHpColor, TRUE);
	DrawBox(x1 + prev, y1, x2, y2, damageColor, TRUE);
}

int CharacterDrawer::HP_COLOR = GetColor(0, 255, 0);
int CharacterDrawer::PREV_HP_COLOR = GetColor(255, 0, 0);
int CharacterDrawer::DAMAGE_COLOR = GetColor(0, 0, 0);

CharacterDrawer::CharacterDrawer(const CharacterAction* const characterAction) {
	m_characterAction = characterAction;
	m_cnt = 0;
	getGameEx(m_exX, m_exY);
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

	// 描画 ダメージ状態なら点滅
	if (m_characterAction->getState() == CHARACTER_STATE::DAMAGE && ++m_cnt / 2 % 2 == 1) {
		int dark = max(0, bright - 100);
		SetDrawBright(dark, dark, dark);
		graphHandle->draw(x, y, ex);
		SetDrawBright(bright, bright, bright);
	}
	else {
		graphHandle->draw(x, y, ex);
	}

	// 体力バー
	if (character->getDispHpCnt() > 0 && character->getName() != "ハート") {
		// 座標をカメラで調整
		x = character->getX() + (character->getWide() / 2);
		y = character->getY();
		ex = graphHandle->getEx();
		camera->setCamera(&x, &y, &ex);
		int wide = (int)(HP_BAR_WIDE / 2 * camera->getEx());
		int height = (int)(HP_BAR_HEIGHT * camera->getEx());
		y -= (int)(10 * camera->getEx());
		// 体力の描画
		drawHpBar(x - wide, y - height, x + wide, y, character->getHp(), character->getPrevHp(), character->getMaxHp(), DAMAGE_COLOR, PREV_HP_COLOR, HP_COLOR);
	}
}

void CharacterDrawer::drawPlayerHpBar(const Character* player, int hpBarGraph) {

	// 座標
	int x, y, wide, height;

	x = 30;
	y = 30;
	wide = 525;
	height = 150;

	// 解像度変更に対応
	x = (int)(x * m_exX);
	y = (int)(y * m_exY);
	wide = (int)(wide * m_exX);
	height = (int)(height * m_exY);

	DrawExtendGraph(x, y, x + wide, y + height, hpBarGraph, TRUE);

	int dx = (int)(40 * m_exX);
	int dy1 = (int)(80 * m_exY);
	int dy2 = (int)(15 * m_exY);

	// 体力の描画
	drawHpBar(x + dx, y + dy1, x + wide - dx, y + height - dy2, player->getHp(), player->getPrevHp(), player->getMaxHp(), DAMAGE_COLOR, PREV_HP_COLOR, HP_COLOR);

}