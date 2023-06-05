#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED
//////キーボードからの入力を格納し、返す関数群///////////

/////////////入力状態/////////////////////

//スティックの入力状態
enum {
	RIGHT_KEY,
	LEFT_KEY,
	UP_KEY,
	DOWN_KEY
};
////////////////////////////////////////////

int updateKey();

void mouseLimit(int& hand_x, const int hand_y);
//左クリックの状態
void mouseClick();
int leftClick();
int rightClick();

//スティックの入力状態
int controlStick();

//FPS表示のオンオフ
int controlDebug();

//スペースキー
int controlSpace();

//ESCキー：ゲーム終了
int controlEsc();

#endif