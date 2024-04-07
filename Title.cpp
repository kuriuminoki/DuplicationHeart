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
* �Z�[�u�f�[�^�I�����
*/
SelectSaveData::SelectSaveData() {

	m_initCnt = 0;

	for (int i = 0; i < GAME_DATA_SUM; i++) {
		ostringstream oss;
		oss << "savedata/" << i + 1 << "/";
		m_gameData[i] = new GameData(oss.str().c_str());
	}
	ChangeGameResolution();

	// �{�^��
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
		m_dataInitButton[i] = new Button("�폜", (int)(650 * exX), (int)(300 * exY + (i * 150 * exY)), (int)(100 * exX), (int)(100 * exY), LIGHT_RED, RED, m_font, BLACK);
		int latestStoryNum = m_gameData[i]->getLatestStoryNum();
		if (latestStoryNum > 1) {
			m_startStoryNum[i] = new ControlBar(900, 350 + (i * 150), 1600, 400 + (i * 150), 1, latestStoryNum, latestStoryNum, "�`���v�^�[");
		}
		else {
			m_startStoryNum[i] = nullptr;
		}
	}

	// �w�i
	m_haikei = new TitleBackGround();

}

SelectSaveData::~SelectSaveData() {
	DeleteFontToHandle(m_font);
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		delete m_gameData[i];
		delete m_dataButton[i];
		delete m_dataInitButton[i];
		delete m_startStoryNum[i];
	}
	delete m_haikei;
}

int SelectSaveData::getSoundVolume() {
	return m_gameData[0]->getSoundVolume();
}

// �Z�[�u�f�[�^��1�ł����݂��邩
bool SelectSaveData::saveDataExist() {
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		if (m_gameData[i]->getExist()) { return true; }
	}
	return false;
}

int SelectSaveData::getLatestStoryNum() {
	int maxStoryNum = -1;
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		maxStoryNum = max(maxStoryNum, m_gameData[i]->getStoryNum());
	}
	return maxStoryNum;
}

// �Z�[�u�f�[�^�I����ʂ̏���
bool SelectSaveData::play(int handX, int handY) {

	// �Z�[�u�f�[�^��I��
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
					// �Z�[�u�f�[�^�폜
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

	// �w�i
	m_haikei->draw();
	
	// �{�^��
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		m_dataButton[i]->draw(handX, handY);
		m_dataInitButton[i]->draw(handX, handY);
		if (m_startStoryNum[i] != nullptr) {
			m_startStoryNum[i]->draw(handX, handY);
		}
	}

}

// �g�p����Z�[�u�f�[�^�̃f�B���N�g����
const char* SelectSaveData::useDirName() {
	if (m_useSaveDataIndex == NOT_DECIDE_DATA) { return ""; }
	return m_gameData[m_useSaveDataIndex]->getSaveFilePath();
}

// �n�߂�`���v�^�[
int SelectSaveData::startStoryNum() {
	if (m_useSaveDataIndex == NOT_DECIDE_DATA || m_startStoryNum[m_useSaveDataIndex] == nullptr) { return -1; }
	int storyNum = m_startStoryNum[m_useSaveDataIndex]->getNowValue();
	return storyNum == m_gameData[m_useSaveDataIndex]->getLatestStoryNum() ? -1 : storyNum;
}

// �S�Z�[�u�f�[�^���ʂ̃f�[�^���Z�[�u(�^�C�g����ʂ̃I�v�V�����p)
void SelectSaveData::saveCommon(int soundVolume) {
	m_gameData[0]->saveCommon(soundVolume, GAME_WIDE, GAME_HEIGHT);
}


/*
* �^�C�g�����
*/
Title::Title() {

	// �Z�[�u�f�[�^�I�����
	m_selectSaveData = new SelectSaveData();

	m_state = TITLE;

	m_handX = 0; m_handY = 0;

	m_soundPlayer = new SoundPlayer();

	m_titleGraph = LoadGraph("picture/movie/op/title/titleBlue.png");

	// �Z�[�u�f�[�^������Ȃ特�ʃZ�b�g
	if (m_selectSaveData->saveDataExist()) { 
		m_soundPlayer->setVolume(m_selectSaveData->getSoundVolume());
		int s = m_selectSaveData->getLatestStoryNum();
		if (s >= 9) {
			// ���߂�OP������̂�9�͂Ȃ̂ŁA����ȍ~�̃Z�[�u�f�[�^������Ȃ�OP�p��
			m_movie = new OpMovie(m_soundPlayer);
		}
	}
	else {
		m_movie = nullptr;
	}

	double exX, exY;
	getGameEx(exX, exY);
	m_font = CreateFontToHandle(nullptr, (int)(50 * exX), 3);
	m_selectButton = new Button("Game Start", (int)(500 * exX), (int)(800 * exY), (int)(920 * exX), (int)(80 * exY), GRAY2, BLUE, m_font, BLACK);
	m_optionButton = new Button("Setting", (int)(500 * exX), (int)(900 * exY), (int)(920 * exX), (int)(80 * exY), GRAY2, BLUE, m_font, BLACK);
	m_cancelButton = new Button("Backward", (int)(50 * exX), (int)(50 * exY), (int)(300 * exX), (int)(100 * exY), GRAY2, WHITE, m_font, BLACK);

	// �I�v�V�������
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

}

// �^�C�g����ʂ̏���
Title::TITLE_RESULT Title::play() {
	
	// OP
	if (m_movie != nullptr) { 
		m_movie->play();
		// OP�̏I��
		if (m_movie->getFinishFlag() || leftClick() == 1) {
			delete m_movie;
			m_movie = nullptr;
		}
		return CONTINUE;
	}

	// �}�E�X�J�[�\���̈ʒu
	GetMousePoint(&m_handX, &m_handY);

	string filePath = "";
	switch (m_state) {
	case TITLE: // �^�C�g�����
		if (m_selectButton->overlap(m_handX, m_handY) && leftClick() == 1) { m_state = SELECT; }
		if (m_optionButton->overlap(m_handX, m_handY) && leftClick() == 1) { m_state = OPTION; }
		break;
	case SELECT: // �Z�[�u�f�[�^�I�����
		if (m_selectSaveData->play(m_handX, m_handY)) {
			filePath = m_selectSaveData->useDirName();
			if (filePath == "") { m_state = TITLE; }
			else { return START; }
		}
		// �߂�{�^��
		if (m_cancelButton->overlap(m_handX, m_handY) && leftClick() == 1) {
			m_state = TITLE;
		}
		break;
	case OPTION: // �I�v�V�������
		m_option->play();
		// �߂�{�^��
		if (m_cancelButton->overlap(m_handX, m_handY) && leftClick() == 1) {
			// �𑜓x���X�V
			GAME_WIDE = m_option->getNewGameWide();
			GAME_HEIGHT = m_option->getNewGameHeight();
			// �𑜓x�Ɖ��ʂ��Z�[�u
			m_selectSaveData->saveCommon(m_option->getNewSoundVolume());
			// �ċN���i�𑜓x�̓K�p�j
			return REBOOT;
		}
		break;
	}

	return CONTINUE;

}

// �`��
void Title::draw() {

	// OP
	if (m_movie != nullptr) {
		m_movie->draw();
		return;
	}

	switch (m_state) {
	case TITLE: // �^�C�g�����
		DrawExtendGraph(0, 0, GAME_WIDE, GAME_HEIGHT, m_titleGraph, TRUE);
		m_selectButton->draw(m_handX, m_handY);
		m_optionButton->draw(m_handX, m_handY);
		break;
	case SELECT: // �Z�[�u�f�[�^�I�����
		m_selectSaveData->draw(m_handX, m_handY);
		m_cancelButton->draw(m_handX, m_handY);
		break;
	case OPTION:
		m_option->draw();
		m_cancelButton->draw(m_handX, m_handY);
		break;
	}

}