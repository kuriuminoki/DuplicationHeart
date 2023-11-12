#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include"DxLib.h"

// フルスクリーンならFALSE
static int WINDOW = TRUE;
// マウスを表示するならFALSE
static int MOUSE_DISP = TRUE;

//画面の大きさ
//#define GAME_WIDE 3840
//#define GAME_HEIGHT 2160
//#define GAME_WIDE 2560
//#define GAME_HEIGHT 1600
//#define GAME_WIDE 2560
//#define GAME_HEIGHT 1440
//#define GAME_WIDE 1920
//#define GAME_HEIGHT 1080
#define GAME_WIDE_MAX 3840
#define GAME_HEIGHT_MAX 2160
#define GAME_WIDE_MIN 640
#define GAME_HEIGHT_MIN 480
extern int GAME_WIDE;
extern int GAME_HEIGHT;

// 解像度の倍率
void getGameEx(double& exX, double& exY);

#define GAME_COLOR_BIT_NUM 16

// DrawFormatString関数で表示される文字の大きさは20くらい
#define DRAW_FORMAT_STRING_SIZE 20

//色
const int BLACK = GetColor(0, 0, 0);
const int GRAY = GetColor(100, 100, 100);
const int GRAY2 = GetColor(200, 200, 200);
const int WHITE = GetColor(255, 255, 255);
const int RED = GetColor(255, 0, 0);
const int LIGHT_RED = GetColor(255, 100, 100);
const int BLUE = GetColor(0, 0, 255);
const int LIGHT_BLUE = GetColor(100, 100, 255);

#endif