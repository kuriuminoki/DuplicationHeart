#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


class Button;
class SoundPlayer;


// �}�E�X�Ń{�^�������E�ɓ������Ēl�𒲐�����@�\
class ControlBar {

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
	ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue);

	// ��������
	void play(int handX, int handY);

	// �`��
	void draw(int handX, int handY);
	
	// �Q�b�^
	inline int getNowValue() { return m_nowValue; }
};


// �|�[�Y���
class GamePause {
private:

	// �}�E�X�J�[�\���̈ʒu
	int m_handX, m_handY;

	// ���ʒ��ߋ@�\
	ControlBar* m_soundController;

	// ���ʒ��߂̑Ώ�
	SoundPlayer* m_soundPlayer_p;

	const int SOUND_X1 = 100;
	const int SOUND_Y1 = 400;
	const int SOUND_X2 = 1100;
	const int SOUND_Y2 = 600;
	const int SOUND_MIN = 0;
	const int SOUND_MAX = 100;

public:
	GamePause(SoundPlayer* soundPlayer);
	~GamePause();

	void play();

	void draw() const;
};


#endif