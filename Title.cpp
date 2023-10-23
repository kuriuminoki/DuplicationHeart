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
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		ostringstream oss;
		oss << "savedata/" << i + 1 << "/";
		m_gameData[i] = new GameData(oss.str().c_str());
	}

	// �{�^��
	double exX = GAME_WIDE / 1920.0;
	double exY = GAME_HEIGHT / 1080.0;
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
		m_dataButton[i] = new Button(text, 100 * exX, 300 * exY + (i * 150 * exY), 800 * exX, 100 * exY, WHITE, GRAY2, m_font, BLACK);
	}
	m_cancelButton = new Button("Backward", 50 * exX, 50 * exY, 300 * exX, 100 * exY, GRAY2, WHITE, m_font, BLACK);

}

SelectSaveData::~SelectSaveData() {
	DeleteFontToHandle(m_font);
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		delete m_gameData[i];
		delete m_dataButton[i];
	}
	delete m_cancelButton;
}

// �Z�[�u�f�[�^��1�ł����݂��邩
bool SelectSaveData::saveDataExist() {
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		if (m_gameData[i]->getExist()) { return true; }
	}
	return false;
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
	}

	// �߂�{�^��
	if (m_cancelButton->overlap(handX, handY) && leftClick() == 1) {
		return true;
	}

	return false;
}

void SelectSaveData::draw(int handX, int handY) {
	
	// �{�^��
	for (int i = 0; i < GAME_DATA_SUM; i++) {
		m_dataButton[i]->draw(handX, handY);
	}
	m_cancelButton->draw(handX, handY);

}

// �g�p����Z�[�u�f�[�^�̃f�B���N�g����
const char* SelectSaveData::useDirName() {
	if (m_useSaveDataIndex == NOT_DECIDE_DATA) { return ""; }
	return m_gameData[m_useSaveDataIndex]->getSaveFilePath();
}


/*
* �^�C�g�����
*/
Title::Title() {

	m_state = TITLE;

	m_handX = 0; m_handY = 0;

	m_soundPlayer = new SoundPlayer();

	m_titleGraph = LoadGraph("picture/movie/op/title/titleBlue.png");

	m_selectSaveData = new SelectSaveData();

	// �Z�[�u�f�[�^������Ȃ�OP
	if (m_selectSaveData->saveDataExist()) { 
		m_movie = new OpMovie(m_soundPlayer);
		m_animationDrawer = new AnimationDrawer(nullptr);
	}
	else {
		m_movie = nullptr;
	}

	double exX = GAME_WIDE / 1920.0;
	double exY = GAME_HEIGHT / 1080.0;
	m_font = CreateFontToHandle(nullptr, (int)(50 * exX), 3);
	m_selectButton = new Button("Game Start", 500 * exX, 800 * exY, 920 * exX, 80 * exY, GRAY2, BLUE, m_font, BLACK);
	m_optionButton = new Button("Setting", 500 * exX, 900 * exY, 920 * exX, 80 * exY, GRAY2, BLUE, m_font, BLACK);

}

Title::~Title() {

	delete m_soundPlayer;

	DeleteGraph(m_titleGraph);

	delete m_selectSaveData;

	DeleteFontToHandle(m_font);
	delete m_selectButton;
	delete m_optionButton;

}

// �^�C�g����ʂ̏��� �I���Ȃ�true
bool Title::play() {
	
	// OP
	if (m_movie != nullptr) { 
		m_movie->play();
		// OP�̏I��
		if (m_movie->getFinishFlag() || leftClick() == 1) {
			delete m_movie;
			m_movie = nullptr;
		}
		return false;
	}

	// �}�E�X�J�[�\���̈ʒu
	GetMousePoint(&m_handX, &m_handY);

	string filePath = "";
	switch (m_state) {
	case TITLE: // �^�C�g�����
		if (m_selectButton->overlap(m_handX, m_handY) && leftClick() == 1) { m_state = SELECT; }
		break;
	case SELECT: // �Z�[�u�f�[�^�I�����
		if (m_selectSaveData->play(m_handX, m_handY)) {
			filePath = m_selectSaveData->useDirName();
			if (filePath == "") { m_state = TITLE; }
			else { return true; }
		}
		break;
	case OPTION:

		break;
	}

	return false;

}

// �`��
void Title::draw() {

	// OP
	if (m_movie != nullptr) {
		m_animationDrawer->setAnimation(m_movie->getAnimation());
		m_animationDrawer->drawAnimation();
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
		break;
	case OPTION:

		break;
	}

}