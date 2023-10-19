#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


class Button;
class SoundPlayer;


class GamePause {
private:

	// 音量調節の対象
	SoundPlayer* m_soundPlayer_p;

	// マウスカーソルの位置
	int m_handX, m_handY;

	// 音量調節用
	Button* m_soundBarButton;

	// 今調整中
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