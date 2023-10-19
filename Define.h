#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include"DxLib.h"

// �t���X�N���[���Ȃ�FALSE
static int WINDOW = TRUE;
// �}�E�X��\������Ȃ�FALSE
static int MOUSE_DISP = FALSE;

//��ʂ̑傫��
//#define GAME_WIDE 3840
//#define GAME_HEIGHT 2160
//#define GAME_WIDE 2560
//#define GAME_HEIGHT 1600
//#define GAME_WIDE 2560
//#define GAME_HEIGHT 1440
#define GAME_WIDE 1920
#define GAME_HEIGHT 1080
//#define GAME_WIDE 1240
//#define GAME_HEIGHT 1024

// DrawFormatString�֐��ŕ\������镶���̑傫����20���炢
#define DRAW_FORMAT_STRING_SIZE 20

//�F
const int BLACK = GetColor(0, 0, 0);
const int GRAY = GetColor(100, 100, 100);
const int GRAY2 = GetColor(200, 200, 200);
const int WHITE = GetColor(255, 255, 255);
const int RED = GetColor(255, 0, 0);
const int BLUE = GetColor(0, 0, 255);
const int LIGHT_BLUE = GetColor(100, 100, 255);

#endif