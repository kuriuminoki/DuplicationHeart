#include "Title.h"
#include "PausePage.h"
#include "Animation.h"
#include "AnimationDrawer.h"
#include "Game.h"
#include "Button.h"
#include "Define.h"
#include "Control.h"
#include "Sound.h"
#include "DxLib.h"

#include <sstream>
#include <string>


using namespace std;


/*
* セーブデータ選択画面
*/
SelectSaveData::SelectSaveData() {

	m_initCnt = 0;

	for (int i = 0; i < GAME_DATA_SUM; i++) {
		ostringstream oss;
		oss << "savedata/" << i + 1 << "/";
		m_gameData[i] = new GameData(oss.str().c_str());
	}

	// ボタン
	double exX, exY;
	getGameEx(exX, exY);
	m_font = CreateFontToHandle(nullptr, (int)(50 * exX), 3);
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		string text;
		ostringstream oss;
		if (m_gameData[i]->getExist()) { 
			oss << "Chapter" << m_gameData[i]->getStoryNum();
			text = oss.str();
		}
		else {
			text = "New Game";
		}
		m_dataButton[i] = new Button(text, (int)(100 * exX), (int)(300 * exY + (i * 150 * exY)), (int)(500 * exX), (int)(100 * exY), WHITE, GRAY2, m_font, BLACK);
		m_dataInitButton[i] = new Button("削除", (int)(650 * exX), (int)(300 * exY + (i * 150 * exY)), (int)(100 * exX), (int)(100 * exY), LIGHT_RED, RED, m_font, BLACK);
		int latestStoryNum = m_gameData[i]->getLatestStoryNum();
		if (latestStoryNum > 1) {
			m_startStoryNum[i] = new ControlBar(900, 350 + (i * 150), 1600, 400 + (i * 150), 1, latestStoryNum, latestStoryNum, "チャプター");
		}
		else {
			m_startStoryNum[i] = nullptr;
		}
	}

	// 背景画像
	m_haikei = LoadGraph("picture/system/savedata.png");
	GetGraphSize(m_haikei, &m_haikeiWide, &m_haikeiHeight);
	m_haikeiEx = min(exX, exY);
	m_haikeiWide = (int)(m_haikeiWide * m_haikeiEx);
	m_haikeiHeight = (int)(m_haikeiHeight * m_haikeiEx);
	m_haikeiX = m_haikeiWide / 2;
	m_haikeiY = m_haikeiHeight / 2;

}

SelectSaveData::~SelectSaveData() {
	DeleteFontToHandle(m_font);
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		delete m_gameData[i];
		delete m_dataButton[i];
		delete m_dataInitButton[i];
		delete m_startStoryNum[i];
	}
	DeleteGraph(m_haikei);
}

int SelectSaveData::getSoundVolume() {
	return m_gameData[0]->getSoundVolume();
}

// セーブデータが1つでも存在するか
bool SelectSaveData::saveDataExist() {
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		if (m_gameData[i]->getExist()) { return true; }
	}
	return false;
}

// セーブデータ選択画面の処理
bool SelectSaveData::play(int handX, int handY) {

	// セーブデータを選ぶ
	m_useSaveDataIndex = NOT_DECIDE_DATA;
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		if (m_dataButton[i]->overlap(handX, handY) && leftClick() == 1) {
			m_useSaveDataIndex = i;
			return true;
		}
		if (m_dataInitButton[i]->overlap(handX, handY) && leftClick() > 0) {
			if (leftClick() == 1) { m_initCnt++; }
			else if (m_initCnt > 0 && m_initCnt < 70) { 
				m_initCnt++;
				m_dataInitButton[i]->setColor(GetColor(255, 100 + m_initCnt * 2, 100 + m_initCnt * 2));
			}
			else { 
				if (m_initCnt == 70) {
					// セーブデータ削除
					m_gameData[i]->removeSaveData();
					m_dataButton[i]->setString("New Game");
					delete m_startStoryNum[i];
					m_startStoryNum[i] = nullptr;
				}
				m_initCnt = 0;
				m_dataInitButton[i]->setColor(LIGHT_RED);
			}
		}
		if (leftClick() == 0) { 
			m_initCnt = 0;
			m_dataInitButton[i]->setColor(LIGHT_RED);
		}
		if (m_startStoryNum[i] != nullptr) {
			m_startStoryNum[i]->play(handX, handY);
		}
	}

	return false;
}

void SelectSaveData::draw(int handX, int handY) {

	// 背景画像
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
	
	// ボタン
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		m_dataButton[i]->draw(handX, handY);
		m_dataInitButton[i]->draw(handX, handY);
		if (m_startStoryNum[i] != nullptr) {
			m_startStoryNum[i]->draw(handX, handY);
		}
	}

}

// 使用するセーブデータのディレクトリ名
const char* SelectSaveData::useDirName() {
	if (m_useSaveDataIndex == NOT_DECIDE_DATA) { return ""; }
	return m_gameData[m_useSaveDataIndex]->getSaveFilePath();
}

// 始めるチャプター
int SelectSaveData::startStoryNum() {
	if (m_useSaveDataIndex == NOT_DECIDE_DATA || m_startStoryNum[m_useSaveDataIndex] == nullptr) { return -1; }
	int storyNum = m_startStoryNum[m_useSaveDataIndex]->getNowValue();
	return storyNum == m_gameData[m_useSaveDataIndex]->getLatestStoryNum() ? -1 : storyNum;
}

// 全セーブデータ共通のデータをセーブ(タイトル画面のオプション用)
void SelectSaveData::saveCommon(int soundVolume) {
	m_gameData[0]->saveCommon(soundVolume, GAME_WIDE, GAME_HEIGHT);
}


/*
* タイトル画面
*/
Title::Title() {

	m_state = TITLE;

	m_handX = 0; m_handY = 0;

	m_soundPlayer = new SoundPlayer();

	m_titleGraph = LoadGraph("picture/movie/op/title/titleBlue.png");

	// セーブデータ選択画面
	m_selectSaveData = new SelectSaveData();

	// セーブデータがあるならOP用意と音量セット
	if (m_selectSaveData->saveDataExist()) { 
		m_soundPlayer->setVolume(m_selectSaveData->getSoundVolume());
		m_movie = new OpMovie(m_soundPlayer);
		m_animationDrawer = new AnimationDrawer(nullptr);
	}
	else {
		m_movie = nullptr;
		m_animationDrawer = nullptr;
	}

	double exX, exY;
	getGameEx(exX, exY);
	m_font = CreateFontToHandle(nullptr, (int)(50 * exX), 3);
	m_selectButton = new Button("Game Start", (int)(500 * exX), (int)(800 * exY), (int)(920 * exX), (int)(80 * exY), GRAY2, BLUE, m_font, BLACK);
	m_optionButton = new Button("Setting", (int)(500 * exX), (int)(900 * exY), (int)(920 * exX), (int)(80 * exY), GRAY2, BLUE, m_font, BLACK);
	m_cancelButton = new Button("Backward", (int)(50 * exX), (int)(50 * exY), (int)(300 * exX), (int)(100 * exY), GRAY2, WHITE, m_font, BLACK);

	// オプション画面
	m_option = new TitleOption(m_soundPlayer);

}

Title::~Title() {

	delete m_soundPlayer;

	DeleteGraph(m_titleGraph);

	delete m_selectSaveData;
	delete m_option;

	DeleteFontToHandle(m_font);
	delete m_selectButton;
	delete m_optionButton;
	delete m_cancelButton;

	if (m_movie != nullptr) {
		delete m_movie;
	}
	if (m_animationDrawer != nullptr) {
		delete m_animationDrawer;
	}

}

// タイトル画面の処理
Title::TITLE_RESULT Title::play() {
	
	// OP
	if (m_movie != nullptr) { 
		m_movie->play();
		// OPの終了
		if (m_movie->getFinishFlag() || leftClick() == 1) {
			delete m_movie;
			m_movie = nullptr;
		}
		return CONTINUE;
	}

	// マウスカーソルの位置
	GetMousePoint(&m_handX, &m_handY);

	string filePath = "";
	switch (m_state) {
	case TITLE: // タイトル画面
		if (m_selectButton->overlap(m_handX, m_handY) && leftClick() == 1) { m_state = SELECT; }
		if (m_optionButton->overlap(m_handX, m_handY) && leftClick() == 1) { m_state = OPTION; }
		break;
	case SELECT: // セーブデータ選択画面
		if (m_selectSaveData->play(m_handX, m_handY)) {
			filePath = m_selectSaveData->useDirName();
			if (filePath == "") { m_state = TITLE; }
			else { return START; }
		}
		// 戻るボタン
		if (m_cancelButton->overlap(m_handX, m_handY) && leftClick() == 1) {
			m_state = TITLE;
		}
		break;
	case OPTION: // オプション画面
		m_option->play();
		// 戻るボタン
		if (m_cancelButton->overlap(m_handX, m_handY) && leftClick() == 1) {
			// 解像度を更新して再起動
			GAME_WIDE = m_option->getNewGameWide();
			GAME_HEIGHT = m_option->getNewGameHeight();
			m_selectSaveData->saveCommon(m_option->getNewSoundVolume());
			SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
			return REBOOT;
		}
		break;
	}

	return CONTINUE;

}

// 描画
void Title::draw() {

	// OP
	if (m_movie != nullptr) {
		m_animationDrawer->setAnimation(m_movie->getAnimation());
		m_animationDrawer->drawAnimation();
		return;
	}

	switch (m_state) {
	case TITLE: // タイトル画面
		DrawExtendGraph(0, 0, GAME_WIDE, GAME_HEIGHT, m_titleGraph, TRUE);
		m_selectButton->draw(m_handX, m_handY);
		m_optionButton->draw(m_handX, m_handY);
		break;
	case SELECT: // セーブデータ選択画面
		m_selectSaveData->draw(m_handX, m_handY);
		m_cancelButton->draw(m_handX, m_handY);
		break;
	case OPTION:
		m_option->draw();
		m_cancelButton->draw(m_handX, m_handY);
		break;
	}

}