#include "PausePage.h"
#include "Button.h"
#include "Sound.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"

#include <sstream>
#include <string>


using namespace std;


// �w�i
TitleBackGround::TitleBackGround() {
	double exX, exY;
	getGameEx(exX, exY);
	m_haikei = LoadGraph("picture/system/savedata.png");
	GetGraphSize(m_haikei, &m_haikeiWide, &m_haikeiHeight);
	m_haikeiEx = min(exX, exY);
	m_haikeiWide = (int)(m_haikeiWide * m_haikeiEx);
	m_haikeiHeight = (int)(m_haikeiHeight * m_haikeiEx);
	m_haikeiX = m_haikeiWide / 2;
	m_haikeiY = m_haikeiHeight / 2;
}

TitleBackGround::~TitleBackGround() {
	DeleteGraph(m_haikei);
}

void TitleBackGround::draw() {
	// �w�i�摜
	m_haikeiX--; m_haikeiY++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRotaGraph(m_haikeiX, m_haikeiY, m_haikeiEx, 0, m_haikei, TRUE);
	DrawRotaGraph(m_haikeiX + m_haikeiWide, m_haikeiY, m_haikeiEx, 0, m_haikei, TRUE);
	DrawRotaGraph(m_haikeiX, m_haikeiY - m_haikeiHeight, m_haikeiEx, 0, m_haikei, TRUE);
	DrawRotaGraph(m_haikeiX + m_haikeiWide, m_haikeiY - m_haikeiHeight, m_haikeiEx, 0, m_haikei, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (m_haikeiX <= -m_haikeiWide / 2 && m_haikeiY >= GAME_HEIGHT + m_haikeiHeight / 2) {
		m_haikeiX += m_haikeiWide;
		m_haikeiY -= m_haikeiHeight;
	}
}


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
	delete m_controlButton;
	DeleteFontToHandle(m_font);
}

// ��������
void ControlBar::play(int handX, int handY) {

	// ���N���b�N
	int click = leftClick();

	// ���ߒ���ON/OFF
	if (click > 0 && m_controlButton->overlap(handX, handY)) {
		m_controlFlag = true;
	}
	if (click == 0) {
		m_controlFlag = false;
	}

	// �}�E�X�̍��W�ɍ��킹�Ēl���X�V
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

	// �l�ɍ��킹�ă{�^���̈ʒu���X�V
	m_controlButton->setX(m_drawX1 + rate * m_drawWide / 100 - m_controlButton->getWide() / 2);
}

// �`��
void ControlBar::draw(int handX, int handY) {
	int d = (int)(10 * m_exX);

	ostringstream oss;
	oss << m_name << " : " << m_nowValue;
	DrawStringToHandle(m_drawX1 - m_buttonWide - d, m_drawY1 - d - m_fontSize, oss.str().c_str(), WHITE, m_font);

	// ���ʒ��ߗ̈�
	DrawBox(m_drawX1 - m_buttonWide - d, m_drawY1 - d, m_drawX2 + m_buttonWide + d, m_drawY2 + d, BLACK, TRUE);
	DrawBox(m_drawX1 - m_buttonWide, m_drawY1, m_drawX2 + m_buttonWide, m_drawY2, GRAY2, TRUE);

	// �{�^���̈ړ��͈�
	DrawBox(m_drawX1, (m_drawY1 + m_drawY2) / 2 - d, m_drawX2, (m_drawY1 + m_drawY2) / 2 + d, BLACK, TRUE);

	// ���ʒ��߃{�^��
	m_controlButton->draw(handX, handY);

	// DrawFormatString(m_drawX1, m_drawY1 - 50, BLACK, "Value = %d, x = %d", m_nowValue, m_drawX1 + m_nowValue * m_drawWide / 100 - m_controlButton->getWide() / 2);
}



/*
* �ꎞ��~���
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

	// �}�E�X�J�[�\���̈ʒu�擾
	GetMousePoint(&m_handX, &m_handY);

	m_soundController->play(m_handX, m_handY);
	m_soundPlayer_p->setVolume(m_soundController->getNowValue());

}

void GamePause::draw() const {

	m_soundController->draw(m_handX, m_handY);

}


/*
* �`���[�g���A���̕`��
*/
TutorialDisp::TutorialDisp(int font_p, int fontSize, double exX, double exY) {
	m_font_p = font_p;
	m_fontSize = fontSize;
	m_exX = exX;
	m_exY = exY;
	setPoint(0, 0, 0, 0);
}

void TutorialDisp::draw() {
	draw(m_x1, m_y1, m_x2, m_y2);
}

void TutorialDisp::draw(int x1, int y1, int x2, int y2) {
	DrawBox(x1, y1, x2, y2, WHITE, TRUE);
	
	int i = m_fontSize;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�ړ��F�`�C�c�L�[", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "���Ⴊ�ށF�r�L�[", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�W�����v�F�X�y�[�X�L�[", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�ˌ��F���N���b�N�i�������ŘA�ˁj", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�a���F�E�N���b�N", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�L�����؂�ւ��F�d�L�[", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�X�L�������F�e�L�[", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "��b���X�L�b�v�F�y�L�[������", BLACK, m_font_p);
	i += m_fontSize * 2;
	DrawStringToHandle(x1 + m_fontSize, y1 + i, "�Y�[���A�E�g�F�V�t�g�L�[������", BLACK, m_font_p);
}


/*
* �Q�[�����ɊJ���I�v�V������� �^�C�g���ɖ߂�{�^����`���[�g���A��������
*/
BattleOption::BattleOption(SoundPlayer* soundPlayer):
	GamePause(soundPlayer)
{
	m_backgroundGraph = LoadGraph("picture/event/black.png");

	getGameEx(m_exX, m_exY);
	m_fontSize = (int)(50 * m_exX);
	m_font = CreateFontToHandle(nullptr, m_fontSize, 3);

	int x = (int)(TITLE_X1 * m_exX);
	int y = (int)(TITLE_Y1 * m_exY);
	int wide = (int)((TITLE_X2 - TITLE_X1) * m_exX);
	int height = (int)((TITLE_Y2 - TITLE_Y1) * m_exY);
	m_titleButton = new Button("Back to the title", x, y, wide, height, WHITE, RED, m_font, BLACK);
	m_titleFlag = false;

	m_tutorialDisp = new TutorialDisp(m_font, m_fontSize, m_exX, m_exY);
	m_tutorialDisp->setPoint(GAME_WIDE / 2 - (int)(100 * m_exX), (int)(50 * m_exY), GAME_WIDE - (int)(50 * m_exX), GAME_HEIGHT - (int)(50 * m_exY));
}
BattleOption::~BattleOption() {
	delete m_titleButton;
	DeleteGraph(m_backgroundGraph);
	DeleteFontToHandle(m_font);
	delete m_tutorialDisp;
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawExtendGraph(0, 0, GAME_WIDE, GAME_HEIGHT, m_backgroundGraph, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	GamePause::draw();

	m_titleButton->draw(m_handX, m_handY);

	m_tutorialDisp->draw();

}


/*
* �^�C�g����ʂ��炢����I�v�V������ʁ@GamePause�̋@�\�{�𑜓x�̕ύX���ł���B
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
	m_leftButton = new Button("��", m_gameWideController->getLeftX(), (int)((HEIGHT_Y2 + 50) * m_exY), (int)(100 * m_exX), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_rightButton = new Button("��", m_gameWideController->getRightX() - (int)(100 * m_exX), (int)((HEIGHT_Y2 + 50) * m_exY), (int)(100 * m_exX), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_tmpApplyButton = new Button("Apply", m_gameWideController->getLeftX(), (int)((HEIGHT_Y2 + 170) * m_exY), m_gameWideController->getRightX() - m_gameWideController->getLeftX(), (int)(100 * m_exY), WHITE, GRAY2, m_font, BLACK);
	m_nowTmpIndex = 0;

	// �w�i
	m_haikei = new TitleBackGround();
}

TitleOption::~TitleOption() {
	DeleteFontToHandle(m_font);
	delete m_gameWideController;
	delete m_gameHeightController;
	delete m_leftButton;
	delete m_rightButton;
	delete m_tmpApplyButton;
	delete m_haikei;
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

	// �w�i
	m_haikei->draw();

	GamePause::draw();

	m_gameWideController->draw(m_handX, m_handY);
	m_gameHeightController->draw(m_handX, m_handY);

	// �𑜓x�̃e���v���[�g
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