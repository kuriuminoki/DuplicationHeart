#include "Control.h"
#include "Define.h"
#include "Game.h"
#include "GameDrawer.h"
#include "Title.h"
#include "DxLib.h"


///////fps�̒���///////////////
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
	DrawFormatString(0, 0, WHITE, "�f�o�b�O���[�h�F%.1f FPS", mFps);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

//////////���C���֐�///////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowSizeChangeEnableFlag(TRUE);//window�T�C�Y�ύX�\
	SetUseDirectInputFlag(TRUE);
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, 16);

	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	
	//SetAlwaysRunFlag(TRUE);//��ʂ���ɃA�N�e�B�u

	// �E�B���h�E�̖��O
	SetMainWindowText("������Heart");

	////�}�E�X�֘A////
	SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
	//SetMousePoint(320, 240);//�}�E�X�J�[�\���̏����ʒu
	
	// �摜�̊g�又������
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetFullScreenScalingMode(DX_DRAWMODE_NEAREST);

	// �Q�[���{��
	Game* game = nullptr;
	// �Q�[���`��p
	GameDrawer* gameDrawer = nullptr;
	// �^�C�g�����
	Title* title = new Title();
	// �Q�[�����Ȃ�true �^�C�g����ʂȂ�false
	bool gamePlay = false;

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		updateKey();
		mouseClick();

		/////���C��////
		if (gamePlay) {
			if (game->play()) {
				delete gameDrawer;
				gameDrawer = new GameDrawer(game);
			}
			gameDrawer->draw();
		}
		else {
			bool result = title->play();
			title->draw();
			if (result) {
				game = new Game(title->useSaveFile());
				gameDrawer = new GameDrawer(game);
				delete title;
				gamePlay = true;
			}
		}
		///////////////

		//FPS����
		if (controlDebug() == TRUE) {
			if (debug == FALSE) { debug = TRUE; }
			else { debug = FALSE; }
		}
		Update();
		// �f�o�b�O
		if (debug == TRUE) {
			Draw(0, 0, BLACK);
			if (game != nullptr) {
				game->debug(0, DRAW_FORMAT_STRING_SIZE, BLACK);
			}
		}
		Wait();
		if (controlEsc() == TRUE) { DxLib_End(); }
		//FPS���삱���܂�
	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}