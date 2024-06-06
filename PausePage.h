#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


#include <string>

class Button;
class SoundPlayer;


/*
* �Z�[�u�f�[�^�I����ʂ�^�C�g���̃I�v�V������ʂɎg���w�i�摜
*/
class TitleBackGround {
private:

	// �w�i�摜
	int m_haikei;

	// ���W
	int m_haikeiX, m_haikeiY;

	// �摜�T�C�Y
	int m_haikeiWide, m_haikeiHeight;

	// �摜�{��
	double m_haikeiEx;

public:

	TitleBackGround();
	~TitleBackGround();

	void draw();

};


// �}�E�X�Ń{�^�������E�ɓ������Ēl�𒲐�����@�\
class ControlBar {

	// �ݒ肷�鍀�ڂ̖��O
	std::string m_name;
	int m_font;
	int m_fontSize;

	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;

	// �`��͈�
	int m_drawX1;
	int m_drawY1;
	int m_drawX2;
	int m_drawY2;

	int m_drawWide;
	int m_drawHeight;

	// ���ߕ�
	int m_minValue;
	int m_maxValue;
	int m_nowValue;

	// �����p�̃{�^��
	Button* m_controlButton;
	int m_buttonWide;

	// ���������i���N���b�N���������j
	bool m_controlFlag;

public:
	ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue, std::string name);
	~ControlBar();

	// ��������
	void play(int handX, int handY);

	// �`��
	void draw(int handX, int handY, std::string appendix = "");
	
	// �Q�b�^
	inline int getNowValue() { return m_nowValue; }
	inline int getLeftX() { return m_drawX1 - m_buttonWide; }
	inline int getRightX() { return m_drawX2 + m_buttonWide; }

	// �Z�b�^
	inline void setValue(int value) { m_nowValue = value; }
};


/*
* �I�v�V�������
*/
class GamePause {
protected:

	// �}�E�X�J�[�\���̈ʒu
	int m_handX, m_handY;

private:

	// ���ʒ��ߋ@�\
	ControlBar* m_soundController;

	// ���ʒ��߂̑Ώ�
	SoundPlayer* m_soundPlayer_p;

	const int SOUND_X1 = 100;
	const int SOUND_Y1 = 400;
	const int SOUND_X2 = 600;
	const int SOUND_Y2 = 600;
	const int SOUND_MIN = 0;
	const int SOUND_MAX = 100;

public:
	GamePause(SoundPlayer* soundPlayer);
	virtual ~GamePause();

	int getNewSoundVolume();

	void play();

	void draw() const;
};


/*
* �`���[�g���A���̕`��
*/
class TutorialDisp {
private:

	// �t�H���g
	int m_font_p;
	int m_fontSize;

	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;

	// �`��͈� �ݒ肵�Ȃ��Ă�����
	int m_x1, m_y1, m_x2, m_y2;

public:

	TutorialDisp(int font_p, int fontSize, double exX, double exY);

	void setPoint(int x1, int y1, int x2, int y2) {
		m_x1 = x1;
		m_y1 = y1;
		m_x2 = x2;
		m_y2 = y2;
	}

	void draw();
	void draw(int x1, int y1, int x2, int y2);
};


/*
* �Q�[�����ɊJ���I�v�V������� �^�C�g���ɖ߂�{�^����`���[�g���A��������
*/
class BattleOption :
	public GamePause
{
private:

	// �w�i�摜
	int m_backgroundGraph;

    // �t�H���g
	int m_font;
	int m_fontSize;

	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;

	// �{�^���̍��W
	const int TITLE_X1 = 100;
	const int TITLE_Y1 = 800;
	const int TITLE_X2 = 600;
	const int TITLE_Y2 = 1000;

	// �^�C�g���֖߂�{�^��
	Button* m_titleButton;
	bool m_titleFlag;

	// �������
	TutorialDisp* m_tutorialDisp;

public:
	BattleOption(SoundPlayer* soundPlayer);
	~BattleOption();

	void play();

	void draw() const;

	// �Q�b�^
	bool getTitleFlag() { return m_titleFlag; }
};


/*
* �^�C�g����ʂ��炢����I�v�V������� �𑜓x���ς�����
*/
class TitleOption :
	public GamePause 
{
	// �𑜓x���ߋ@�\
	ControlBar* m_gameWideController;
	ControlBar* m_gameHeightController;

	int m_newWide;
	int m_newHeight;

	const int WIDE_X1 = 800;
	const int WIDE_Y1 = 200;
	const int WIDE_X2 = 1300;
	const int WIDE_Y2 = 300;

	const int HEIGHT_X1 = 800;
	const int HEIGHT_Y1 = 400;
	const int HEIGHT_X2 = 1300;
	const int HEIGHT_Y2 = 500;

	// �𑜓x�̃e���v���[�g
	// �t�H���g
	int m_font;
	int m_fontSize;
	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;
	Button* m_tmpApplyButton;
	Button* m_leftButton;
	Button* m_rightButton;
	static const int TMP_SUM = 24;
	const int TMP[TMP_SUM][2] = { {3840, 2160}, {2560, 1600}, {2560, 1440}, {2048, 1536}, {1920, 1440},
		{1920, 1200}, {1920, 1080}, {1680, 1050}, {1600, 1200}, {1600, 900}, {1440, 1080},
		{1440, 900}, {1366, 768}, {1360, 768}, {1280, 1024}, {1280, 960}, {1280, 800},
		{1280, 768}, {1280, 720}, {1176, 664}, {1152, 864}, {1024, 768}, {800, 600},
		{640, 480} };
	int m_nowTmpIndex;

	// �w�i
	TitleBackGround* m_haikei;

public:
	TitleOption(SoundPlayer* soundPlayer);
	~TitleOption();

	void play();

	void draw() const;

	// �Q�b�^
	inline int getNewGameWide() { return m_newWide; }
	inline int getNewGameHeight() { return m_newHeight; }

};


#endif