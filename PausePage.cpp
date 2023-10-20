#include "PausePage.h"
#include "Button.h"
#include "Sound.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"


ControlBar::ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue) {
	m_drawX1 = x1;
	m_drawY1 = y1;
	m_drawX2 = x2;
	m_drawY2 = y2;

	m_drawWide = m_drawX2 - m_drawX1;
	m_drawHeight = m_drawY2 - m_drawY1;

	m_minValue = minValue;
	m_maxValue = maxValue;
	m_nowValue = initValue;

	m_buttonWide = m_drawWide / 10;
	m_controlButton = new Button("", m_drawX1, m_drawY1, m_buttonWide, m_drawHeight, WHITE, GRAY, -1, BLACK);
	m_buttonWide /= 2;

	m_controlFlag = false;
}

// 調整する
void ControlBar::play(int handX, int handY) {

	// 左クリック
	int click = leftClick();

	// 調節中のON/OFF
	if (click > 0 && m_controlButton->overlap(handX, handY)) {
		m_controlFlag = true;
	}
	if (click == 0) {
		m_controlFlag = false;
	}

	// マウスの座標に合わせて値を更新
	if (m_controlFlag) {
		if (handX > m_drawX2) { m_nowValue = m_maxValue; }
		else if (handX < m_drawX1) { m_nowValue = m_minValue; }
		else {
			int rate = (handX - m_drawX1) * 100 / m_drawWide;
			m_nowValue = (m_maxValue - m_minValue) * rate / 100;
		}
	}

	// 値に合わせてボタンの位置を更新
	m_controlButton->setX(m_drawX1 + m_nowValue * m_drawWide / 100 - m_controlButton->getWide() / 2);
}

// 描画
void ControlBar::draw(int handX, int handY) {
	// 音量調節領域
	DrawBox(m_drawX1 - m_buttonWide - 10, m_drawY1 - 10, m_drawX2 + m_buttonWide + 10, m_drawY2 + 10, BLACK, TRUE);
	DrawBox(m_drawX1 - m_buttonWide, m_drawY1, m_drawX2 + m_buttonWide, m_drawY2, GRAY2, TRUE);

	// ボタンの移動範囲
	DrawBox(m_drawX1, (m_drawY1 + m_drawY2) / 2 - 10, m_drawX2, (m_drawY1 + m_drawY2) / 2 + 10, BLACK, TRUE);

	// 音量調節ボタン
	m_controlButton->draw(handX, handY);
}


GamePause::GamePause(SoundPlayer* soundPlayer) {
	m_soundPlayer_p = soundPlayer;
	m_soundController = new ControlBar(SOUND_X1, SOUND_Y1, SOUND_X2, SOUND_Y2, SOUND_MIN, SOUND_MAX, m_soundPlayer_p->getVolume());
	m_handX = 0;
	m_handY = 0;
}

GamePause::~GamePause() {
	delete m_soundController;
}

void GamePause::play() {

	// マウスカーソルの位置取得
	GetMousePoint(&m_handX, &m_handY);

	m_soundController->play(m_handX, m_handY);
	m_soundPlayer_p->setVolume(m_soundController->getNowValue());
}

void GamePause::draw() const {

	m_soundController->draw(m_handX, m_handY);

}