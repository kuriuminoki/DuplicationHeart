#include "Control.h"
#include "Define.h"
#include "Game.h"
#include "GameDrawer.h"
#include "DxLib.h"


///////fpsの調整///////////////
static int mStartTime;
static int mCount;
static int debug = FALSE;
static float mFps;
static const int N = 60;
static const int FPS = 60;

bool Update() {
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw(int x, int y, int color) {
	DrawFormatString(0, 0, WHITE, "デバッグモード：%.1f FPS", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

//////////メイン関数///////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowSizeChangeEnableFlag(TRUE);//windowサイズ変更可能
	SetUseDirectInputFlag(TRUE);
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, 16);

	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	
	//SetAlwaysRunFlag(TRUE);//画面を常にアクティブ

	// ウィンドウの名前
	SetMainWindowText("複製のHeart");

	////マウス関連////
	SetMouseDispFlag(MOUSE_DISP);//マウス表示
	//SetMousePoint(320, 240);//マウスカーソルの初期位置
	
	// 画像の拡大処理方式
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetFullScreenScalingMode(DX_DRAWMODE_NEAREST);

	// ゲーム本体
	Game game;
	// ゲーム描画用
	GameDrawer* gameDrawer = new GameDrawer(&game);

	

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		updateKey();
		mouseClick();

		/////メイン////
		if (game.play()) {
			delete gameDrawer;
			gameDrawer = new GameDrawer(&game);
		}
		gameDrawer->draw();
		///////////////

		//FPS操作
		if (controlDebug() == TRUE) {
			if (debug == FALSE) { debug = TRUE; }
			else { debug = FALSE; }
		}
		Update();
		// デバッグ
		if (debug == TRUE) {
			Draw(0, 0, BLACK);
			game.debug(0, DRAW_FORMAT_STRING_SIZE, BLACK);
		}
		Wait();
		if (controlEsc() == TRUE) { DxLib_End(); }
		//FPS操作ここまで
	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}