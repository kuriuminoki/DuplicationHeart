#ifndef TITLE_H_INCLUDED
#define TITLE_H_INCLUDED

#include <string>
#include <vector>


class SoundPlayer;
class Button;
class TitleOption;
class Movie;
class AnimationDrawer;
class GameData;
class ControlBar;
class TitleBackGround;


/*
* �Z�[�u�f�[�^�I�����
*/
class SelectSaveData {
private:
	
	// �Z�[�u�f�[�^�̐�
	static const int GAME_DATA_SUM = 3;
	
	// �Z�[�u�f�[�^
	GameData* m_gameData[GAME_DATA_SUM];

	// �t�H���g
	int m_font;

	// �Z�[�u�f�[�^�̑I���{�^��
	Button* m_dataButton[GAME_DATA_SUM];
	
	// �Z�[�u�f�[�^�폜�{�^��
	Button* m_dataInitButton[GAME_DATA_SUM];
	int m_initCnt; // �������̎���

	// �g�p����Z�[�u�f�[�^�����܂��Ă��Ȃ��Ƃ�
	static const int NOT_DECIDE_DATA = -1;

	// �g�p����Z�[�u�f�[�^�̃C���f�b�N�X
	int m_useSaveDataIndex;

	// �J�n���郋�[�v�ԍ�
	ControlBar* m_startLoop[GAME_DATA_SUM];

	// �w�i
	TitleBackGround* m_haikei;

	// �`���v�^�[��
	std::vector<std::string> m_chapterNames;

public:

	SelectSaveData(int* screen);

	~SelectSaveData();

	int getSoundVolume();

	// �Z�[�u�f�[�^��1�ł����݂��邩
	bool saveDataExist();

	// �ő��Loop
	int getLatestLoop();

	// �Z�[�u�f�[�^�I����ʂ̏���
	bool play(int handX, int handY);

	// �`��
	void draw(int handX, int handY);

	// �g�p����Z�[�u�f�[�^�̃f�B���N�g����
	const char* useDirName();

	// �n�߂郋�[�v �w�肪�Ȃ��Ȃ�-1
	int startLoop();

	// �S�Z�[�u�f�[�^���ʂ̃f�[�^���Z�[�u(�^�C�g����ʂ̃I�v�V�����p)
	void saveCommon(int soundVolume);

};


/*
* �^�C�g�����
*/
class Title {
private:

	// �}�E�X�J�[�\���̈ʒu
	int m_handX, m_handY;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer;

	// �^�C�g���̉摜
	int m_titleGraph;

	// �I�v�V�������
	TitleOption* m_option;

	// OP���[�r�[
	Movie* m_movie;

	// �Z�[�u�f�[�^�I�����
	SelectSaveData* m_selectSaveData;

	// ���ǂ̉��
	enum TITLE_STATE {
		OP,
		TITLE,
		SELECT,
		OPTION
	};
	TITLE_STATE m_state;

	// �{�^��
	int m_font;
	Button* m_selectButton;
	Button* m_optionButton;

	// �߂�{�^��
	Button* m_cancelButton;

public:

	Title(int* screen);

	~Title();

	// �^�C�g����ʂ̏��� �p���A�ċN���A�Q�[���J�n
	enum TITLE_RESULT {
		CONTINUE,
		REBOOT,
		START
	};
	TITLE_RESULT play();

	// �`��
	void draw();

	// �g�p����Z�[�u�f�[�^�̃t�H���_��
	inline const char* useSaveFile() { 
		return m_selectSaveData->useDirName();
	}

	// �n�߂郋�[�v�ԍ� �w�肪�Ȃ��Ȃ�-1
	inline int startLoop() {
		return m_selectSaveData->startLoop();
	}

};


#endif