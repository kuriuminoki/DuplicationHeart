#include "Define.h"

int GAME_WIDE = 1920;
int GAME_HEIGHT = 1080;

int SLASH_STOP_CNT = 9;


// �𑜓x�̔{��
void getGameEx(double& exX, double& exY) {
	exX = (double)GAME_WIDE / GAME_WIDE_DEFAULT;
	exY = (double)GAME_HEIGHT / GAME_HEIGHT_DEFAULT;
}

// �𑜓x�ύX
void ChangeGameResolution(int* screen) {
	InitGraph();
	InitSoundMem();
	InitFontToHandle();
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
	ChangeWindowMode(WINDOW), DxLib_Init();
	*screen = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
	SetDrawScreen(*screen);
	SetMouseDispFlag(MOUSE_DISP);
}