#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


class Button;
class SoundPlayer;


class GamePause {
private:

	// ���ʒ��߂̑Ώ�
	SoundPlayer* m_soundPlayer_p;

	// �}�E�X�J�[�\���̈ʒu
	int m_handX, m_handY;

	// ���ʒ��ߗp
	Button* m_soundBarButton;

	// ��������
	bool m_soundFlag;

	const int SOUND_LEFT_LIMIT = 100;
	const int SOUND_RIGHT_LIMIT = 1100;
	const int SOUND_Y = 500;

public:
	GamePause(SoundPlayer* soundPlayer);

	void play();

	void draw() const;
};


#endif