#include "CharacterDrawer.h"
#include "Camera.h"
#include "Character.h"
#include "CharacterAction.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"


// �̗̓o�[��\��
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

// �L������`�悷��
void CharacterDrawer::drawCharacter(const Camera* const camera, int bright) {
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

	// �`�� �_���[�W��ԂȂ�_��
	if (m_characterAction->getState() == CHARACTER_STATE::DAMAGE && ++m_cnt / 2 % 2 == 1) {
		int dark = max(0, bright - 100);
		SetDrawBright(dark, dark, dark);
		graphHandle->draw(x, y, ex);
		SetDrawBright(bright, bright, bright);
	}
	else {
		graphHandle->draw(x, y, ex);
	}

	// �̗̓o�[
	if (character->getDispHpCnt() > 0 && character->getName() != "�n�[�g") {
		// ���W���J�����Œ���
		x = character->getX() + (character->getWide() / 2);
		y = character->getY();
		ex = graphHandle->getEx();
		camera->setCamera(&x, &y, &ex);
		int wide = (int)(HP_BAR_WIDE / 2 * camera->getEx());
		int height = (int)(HP_BAR_HEIGHT * camera->getEx());
		y -= (int)(10 * camera->getEx());
		// �̗͂̕`��
		drawHpBar(x - wide, y - height, x + wide, y, character->getHp(), character->getPrevHp(), character->getMaxHp(), DAMAGE_COLOR, PREV_HP_COLOR, HP_COLOR);
	}
}

void CharacterDrawer::drawPlayerHpBar(const Character* player, int hpBarGraph) {

	// ���W
	int x, y, wide, height;

	x = 30;
	y = 30;
	wide = 525;
	height = 150;

	// �𑜓x�ύX�ɑΉ�
	x = (int)(x * m_exX);
	y = (int)(y * m_exY);
	wide = (int)(wide * m_exX);
	height = (int)(height * m_exY);

	DrawExtendGraph(x, y, x + wide, y + height, hpBarGraph, TRUE);

	int dx = (int)(40 * m_exX);
	int dy1 = (int)(80 * m_exY);
	int dy2 = (int)(15 * m_exY);

	// �̗͂̕`��
	drawHpBar(x + dx, y + dy1, x + wide - dx, y + height - dy2, player->getHp(), player->getPrevHp(), player->getMaxHp(), DAMAGE_COLOR, PREV_HP_COLOR, HP_COLOR);

}