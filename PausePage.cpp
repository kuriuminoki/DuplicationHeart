#include "PausePage.h"
#include "Button.h"
#include "Sound.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


GamePause::GamePause(SoundPlayer* soundPlayer) {
	m_soundPlayer_p = soundPlayer;
	m_soundBarButton = new Button("", 100, 450, 50, 100, WHITE, GRAY, -1, BLACK);
	m_handX = 0;
	m_handY = 0;
	m_soundFlag = false;
}

void GamePause::play() {
	// マウスカーソルの位置取得
	GetMousePoint(&m_handX, &m_handY);

	// 左クリック
	int click = leftClick();

	// 音量調節
	if (click > 0 && m_soundBarButton->overlap(m_handX, m_handY)) {
		m_soundFlag = true;
	}
	if (click == 0) {
		m_soundFlag = false;
	}

	int length = SOUND_RIGHT_LIMIT - SOUND_LEFT_LIMIT;
	if (m_soundFlag) {
		int rate = (m_handX - SOUND_LEFT_LIMIT) * 100 / length;
		m_soundPlayer_p->setVolume(rate);
	}

	int soundVolume = m_soundPlayer_p->getVolume();
	m_soundBarButton->setX(SOUND_LEFT_LIMIT + soundVolume * length / 100 - m_soundBarButton->getWide() / 2);
}

void GamePause::draw() const {

	// 音量調節領域
	DrawBox(SOUND_LEFT_LIMIT - 60, SOUND_Y - 110, SOUND_RIGHT_LIMIT + 60, SOUND_Y + 110, BLACK, TRUE);
	DrawBox(SOUND_LEFT_LIMIT - 50, SOUND_Y - 100, SOUND_RIGHT_LIMIT + 50, SOUND_Y + 100, GRAY2, TRUE);

	// ボタンの移動範囲
	DrawBox(SOUND_LEFT_LIMIT, SOUND_Y - 10, SOUND_RIGHT_LIMIT, SOUND_Y + 10, BLACK, TRUE);

	// 音量調節ボタン
	m_soundBarButton->draw(m_handX, m_handY);

}