#include "PausePage.h"
#include "Button.h"
#include "Sound.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"

#include <sstream>
#include <string>


using namespace std;


ControlBar::ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue, string name) {
	
	m_name = name;

	getGameEx(m_exX, m_exY);
	m_fontSize = (int)(50 * m_exX);
	m_font = CreateFontToHandle(nullptr, m_fontSize, 3);

	m_drawX1 = (int)(x1 * m_exX);
	m_drawY1 = (int)(y1 * m_exY);
	m_drawX2 = (int)(x2 * m_exX);
	m_drawY2 = (int)(y2 * m_exY);

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

ControlBar::~ControlBar() {
	DeleteFontToHandle(m_font);
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
		if (handX > m_drawX2) { 
			m_nowValue = m_maxValue;
		}
		else if (handX < m_drawX1) { 
			m_nowValue = m_minValue;
		}
		else {
			int rate = (handX - m_drawX1) * 100 / m_drawWide;
			m_nowValue = m_minValue + (rate * (m_maxValue - m_minValue) / 100);
		}
	}

	int rate = (m_nowValue - m_minValue) * 100 / (m_maxValue - m_minValue);

	// 値に合わせてボタンの位置を更新
	m_controlButton->setX(m_drawX1 + rate * m_drawWide / 100 - m_controlButton->getWide() / 2);
}

// 描画
void ControlBar::draw(int handX, int handY) {
	int d = (int)(10 * m_exX);

	ostringstream oss;
	oss << m_name << " : " << m_nowValue;
	DrawStringToHandle(m_drawX1 - m_buttonWide - d, m_drawY1 - d - m_fontSize, oss.str().c_str(), WHITE, m_font);

	// 音量調節領域
	DrawBox(m_drawX1 - m_buttonWide - d, m_drawY1 - d, m_drawX2 + m_buttonWide + d, m_drawY2 + d, BLACK, TRUE);
	DrawBox(m_drawX1 - m_buttonWide, m_drawY1, m_drawX2 + m_buttonWide, m_drawY2, GRAY2, TRUE);

	// ボタンの移動範囲
	DrawBox(m_drawX1, (m_drawY1 + m_drawY2) / 2 - d, m_drawX2, (m_drawY1 + m_drawY2) / 2 + d, BLACK, TRUE);

	// 音量調節ボタン
	m_controlButton->draw(handX, handY);

	// DrawFormatString(m_drawX1, m_drawY1 - 50, BLACK, "Value = %d, x = %d", m_nowValue, m_drawX1 + m_nowValue * m_drawWide / 100 - m_controlButton->getWide() / 2);
}



/*
* 一時停止画面
*/
GamePause::GamePause(SoundPlayer* soundPlayer) {
	m_soundPlayer_p = soundPlayer;
	m_soundController = new ControlBar(SOUND_X1, SOUND_Y1, SOUND_X2, SOUND_Y2, SOUND_MIN, SOUND_MAX, m_soundPlayer_p->getVolume(), "Sound Volume");
	m_handX = 0;
	m_handY = 0;
}

GamePause::~GamePause() {
	delete m_soundController;
}

int GamePause::getNewSoundVolume() {
	return m_soundPlayer_p->getVolume();
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


/*
* ゲーム中に開くオプション画面 タイトルに戻るボタンやチュートリアルがある
*/
BattleOption::BattleOption(SoundPlayer* soundPlayer):
	GamePause(soundPlayer)
{
	getGameEx(m_exX, m_exY);
	m_fontSize = (int)(50 * m_exX);
	m_font = CreateFontToHandle(nullptr, m_fontSize, 3);

	int x = (int)(TITLE_X1 * m_exX);
	int y = (int)(TITLE_Y1 * m_exY);
	int wide = (int)((TITLE_X2 - TITLE_X1) * m_exX);
	int height = (int)((TITLE_Y2 - TITLE_Y1) * m_exY);
	m_titleButton = new Button("Back to the title", x, y, wide, height, GRAY, RED, m_font, BLACK);
	m_titleFlag = false;
}
BattleOption::~BattleOption() {
	DeleteFontToHandle(m_font);
}

void BattleOption::play() {

	GamePause::play();

	if (leftClick() == 1) {
		if (m_titleButton->overlap(m_handX, m_handY)) {
			m_titleFlag = true;
		}
	}

}

void BattleOption::draw() const {

	GamePause::draw();

	m_titleButton->draw(m_handX, m_handY);

}


/*
* タイトル画面からいけるオプション画面　GamePauseの機能＋解像度の変更もできる。
*/
TitleOption::TitleOption(SoundPlayer* soundPlayer) :
	GamePause(soundPlayer)
{
	m_gameWideController = new ControlBar(WIDE_X1, WIDE_Y1, WIDE_X2, WIDE_Y2, GAME_WIDE_MIN, GAME_WIDE_MAX, GAME_WIDE, "Display resolution (wide)");
	m_gameHeightController = new ControlBar(HEIGHT_X1, HEIGHT_Y1, HEIGHT_X2, HEIGHT_Y2, GAME_HEIGHT_MIN, GAME_HEIGHT_MAX, GAME_HEIGHT, "Display resolution (height)");
	m_newWide = GAME_WIDE;
	m_newHeight = GAME_HEIGHT;

	getGameEx(m_exX, m_exY);
	m_fontSize = (int)(50 * m_exX);
	m_font = CreateFontToHandle(nullptr, m_fontSize, 3);
	m_leftButton = new Button("←", m_gameWideController->getLeftX(), (int)((HEIGHT_Y2 + 50) * m_exY), (int)(100 * m_exX), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_rightButton = new Button("→", m_gameWideController->getRightX() - (int)(100 * m_exX), (int)((HEIGHT_Y2 + 50) * m_exY), (int)(100 * m_exX), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_tmpApplyButton = new Button("Apply", m_gameWideController->getLeftX(), (int)((HEIGHT_Y2 + 170) * m_exY), m_gameWideController->getRightX() - m_gameWideController->getLeftX(), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_nowTmpIndex = 0;
}

TitleOption::~TitleOption() {
	DeleteFontToHandle(m_font);
	delete m_leftButton;
	delete m_rightButton;
	delete m_tmpApplyButton;
}

void TitleOption::play() {

	GamePause::play();

	m_gameWideController->play(m_handX, m_handY);
	m_gameHeightController->play(m_handX, m_handY);
	m_newWide = m_gameWideController->getNowValue();
	m_newHeight = m_gameHeightController->getNowValue();
	
	if (m_leftButton->overlap(m_handX, m_handY) && leftClick() == 1) {
		if (m_nowTmpIndex == 0) { 
			m_nowTmpIndex = TMP_SUM - 1;
		}
		else {
			m_nowTmpIndex = (m_nowTmpIndex - 1) % TMP_SUM;
		}
	}
	if (m_rightButton->overlap(m_handX, m_handY) && leftClick() == 1) {
		m_nowTmpIndex = (m_nowTmpIndex + 1) % TMP_SUM;
	}
	if (m_tmpApplyButton->overlap(m_handX, m_handY) && leftClick() == 1) {
		m_gameWideController->setValue(TMP[m_nowTmpIndex][0]);
		m_gameHeightController->setValue(TMP[m_nowTmpIndex][1]);
	}
}

void TitleOption::draw() const {

	GamePause::draw();

	m_gameWideController->draw(m_handX, m_handY);
	m_gameHeightController->draw(m_handX, m_handY);

	// 解像度のテンプレート
	m_tmpApplyButton->draw(m_handX, m_handY);
	m_leftButton->draw(m_handX, m_handY);
	ostringstream oss;
	oss << TMP[m_nowTmpIndex][0] << " : " << TMP[m_nowTmpIndex][1];
	DrawStringToHandle(m_gameWideController->getLeftX() + (int)(150 * m_exX), (int)((HEIGHT_Y2 + 75) * m_exY), oss.str().c_str(), WHITE, m_font);
	m_rightButton->draw(m_handX, m_handY);
	int x1 = m_gameWideController->getLeftX();
	int y1 = (int)((HEIGHT_Y2 + 300) * m_exY);
	DrawBox(x1, y1, x1 + (int)(TMP[m_nowTmpIndex][0] * m_exX) / 20, y1 + (int)(TMP[m_nowTmpIndex][1] * m_exY) / 20, LIGHT_BLUE, TRUE);
	DrawBox(x1, y1, x1 + (int)(GAME_WIDE * m_exX) / 20, y1 + (int)(GAME_HEIGHT * m_exY) / 20, RED, FALSE);

}