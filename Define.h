#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include"DxLib.h"

//画面の大きさ
#define GAME_WIDE 1920
#define GAME_HEIGHT 1080

//キャラクターやオブジェクトの描画倍率
const double EXTEND = 0.35;

// DrawFormatString関数で表示される文字の大きさは20くらい
#define DRAW_FORMAT_STRING_SIZE 20

//色
const int BLACK = GetColor(0, 0, 0);
const int GRAY = GetColor(100, 100, 100);
const int GRAY2 = GetColor(200, 200, 200);
const int WHITE = GetColor(255, 255, 255);

#endif