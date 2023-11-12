#ifndef PAUSE_PAGE_H_INCLUDED
#define PAUSE_PAGE_H_INCLUDED


#include <string>

class Button;
class SoundPlayer;


// マウスでボタンを左右に動かして値を調整する機能
class ControlBar {

	// 設定する項目の名前
	std::string m_name;
	int m_font;
	int m_fontSize;

	// 1920を基準としたGAME_WIDEの倍率
	double m_exX;
	// 1080を基準としたGAME_HEIGHTの倍率
	double m_exY;

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
	ControlBar(int x1, int y1, int x2, int y2, int minValue, int maxValue, int initValue, std::string name);
	~ControlBar();

	// 調整する
	void play(int handX, int handY);

	// 描画
	void draw(int handX, int handY);
	
	// ゲッタ
	inline int getNowValue() { return m_nowValue; }
	inline int getLeftX() { return m_drawX1 - m_buttonWide; }
	inline int getRightX() { return m_drawX2 + m_buttonWide; }

	// セッタ
	inline void setValue(int value) { m_nowValue = value; }
};


// ポーズ画面
class GamePause {
protected:

	// マウスカーソルの位置
	int m_handX, m_handY;

private:

	// 音量調節機能
	ControlBar* m_soundController;

	// 音量調節の対象
	SoundPlayer* m_soundPlayer_p;

	const int SOUND_X1 = 100;
	const int SOUND_Y1 = 400;
	const int SOUND_X2 = 600;
	const int SOUND_Y2 = 600;
	const int SOUND_MIN = 0;
	const int SOUND_MAX = 100;

public:
	GamePause(SoundPlayer* soundPlayer);
	~GamePause();

	int getNewSoundVolume();

	void play();

	void draw() const;
};


/*
* タイトル画面からいけるオプション画面 解像度も変えられる
*/
class TitleOption :
	public GamePause 
{
	// 解像度調節機能
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

	// 解像度のテンプレート
	// フォント
	int m_font;
	int m_fontSize;
	// 1920を基準としたGAME_WIDEの倍率
	double m_exX;
	// 1080を基準としたGAME_HEIGHTの倍率
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

public:
	TitleOption(SoundPlayer* soundPlayer);
	~TitleOption();

	void play();

	void draw() const;

	// ゲッタ
	inline int getNewGameWide() { return m_newWide; }
	inline int getNewGameHeight() { return m_newHeight; }

};


#endif