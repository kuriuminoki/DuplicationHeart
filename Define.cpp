#include "Define.h"

int GAME_WIDE = 1920;
int GAME_HEIGHT = 1080;

int SLASH_STOP_CNT = 9;


// 解像度の倍率
void getGameEx(double& exX, double& exY) {
	exX = (double)GAME_WIDE / GAME_WIDE_DEFAULT;
	exY = (double)GAME_HEIGHT / GAME_HEIGHT_DEFAULT;
}

// 解像度変更
void ChangeGameResolution() {
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
	InitGraph();
	InitSoundMem();
	InitFontToHandle();
	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	SetMouseDispFlag(MOUSE_DISP); //マウス表示
}