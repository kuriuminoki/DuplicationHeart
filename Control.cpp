#include "DxLib.h"
#include "Control.h"


//////////キーボードの入力状態を格納する////////////////

int Key[256]; // キーが押されているフレーム数を格納する

			  // キーの入力状態を更新する
int updateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}

void mouseLimit(int& hand_x, const int hand_y) {
	if (hand_x < 0 && hand_y > 0 && hand_y < 480) {
		hand_x = 0;
		SetMousePoint(0, hand_y);//マウスカーソルの位置
	}
	else if (hand_x > 640 && hand_y > 0 && hand_y < 480) {
		hand_x = 640;
		SetMousePoint(640, hand_y);//マウスカーソルの位置
	}
}

///////////マウスの入力状態を返す//////////////
static int left_cnt = 0;
static int right_cnt = 0;
void mouseClick() {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { left_cnt++; }
	else { left_cnt = 0; }
	if (GetMouseInput() == MOUSE_INPUT_RIGHT) { right_cnt++; }
	else { right_cnt = 0; }
}
int leftClick() {
	return left_cnt;
}
int rightClick() {
	return right_cnt;
}


//左右の移動
int controlStick()
{
	//同時入力には短時間押している方を採用
	if (Key[KEY_INPUT_D] >= 1 && Key[KEY_INPUT_A] >= 1) {
		if (Key[KEY_INPUT_D] < Key[KEY_INPUT_A]) {
			return RIGHT_KEY;
		}
		else {
			return LEFT_KEY;
		}
	}
	else if (Key[KEY_INPUT_D] >= 1) { // 右キーが押されていたら
		return RIGHT_KEY;
	}
	else if (Key[KEY_INPUT_A] >= 1) { // 左キーが押されていたら
		return LEFT_KEY;
	}
	return FALSE;
}

//デバッグモード起動用
int controlDebug() {
	if (Key[KEY_INPUT_F] == 1) { // Fキーが押されていたら
		return TRUE;
	}
	return FALSE;
}

//spaceキー
int controlSpace()
{
	// spaceキーが押された瞬間
	if (Key[KEY_INPUT_SPACE] == 1) {
		return TRUE;
	}
	return FALSE;
}

//ゲーム終了用
int controlEsc() {
	if (Key[KEY_INPUT_ESCAPE] == 1) { //ESCキーが1カウント押されていたら
		return TRUE;
	}
	return FALSE;
}