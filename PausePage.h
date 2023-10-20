#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


class Button;
class SoundPlayer;


// マウスでボタンを左右に動かして値を調整する機能
class ControlBar {

	// 描画範囲
	int m_drawX1;
	int m_drawY1;
	int m_drawX2;
	int m_drawY2;

	int m_drawWide;
	int m_drawHeight;

	// 調節幅
	int m_minValue;
	int m_maxValue;
	int m_nowValue;

	// 調整用のボタン
	Button* m_controlButton;
	int m_buttonWide;

	// 今調整中（左クリック長押し中）
	bool m_controlFlag;

public:
	ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue);

	// 調整する
	void play(int handX, int handY);

	// 描画
	void draw(int handX, int handY);
	
	// ゲッタ
	inline int getNowValue() { return m_nowValue; }
};


// ポーズ画面
class GamePause {
private:

	// マウスカーソルの位置
	int m_handX, m_handY;

	// 音量調節機能
	ControlBar* m_soundController;

	// 音量調節の対象
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