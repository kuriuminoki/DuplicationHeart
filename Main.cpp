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
	DrawFormatString(0, 0, BLUE, "�f�o�b�O���[�h�F%.1f FPS, �𑜓x�F%d*%d", mFps, GAME_WIDE, GAME_HEIGHT);
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
	GameData* gameData = new GameData();
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
	delete gameData;

	ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	
	//SetAlwaysRunFlag(TRUE);//��ʂ���ɃA�N�e�B�u

	// �E�B���h�E�̖��O
	SetMainWindowText("������Heart");

	////�}�E�X�֘A////
	SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
	//SetMousePoint(320, 240);//�}�E�X�J�[�\���̏����ʒu
	
	// �摜�̊g�又������
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//SetDrawMode(DX_DRAWMODE_NEAREST);
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
				//InitGraph�����s���ꂽ�̂�Drawer����蒼��
				delete gameDrawer;
				gameDrawer = new GameDrawer(game);
			}
			if (game->getRebootFlag()) {
				// �Q�[�����ċN���A�^�C�g���֖߂�
				delete game;
				delete gameDrawer;
				gamePlay = false;
				SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
				title = new Title();
			}
			else{ gameDrawer->draw(); }
		}
		else {
			Title::TITLE_RESULT result = title->play();
			title->draw();
			if (result == Title::START) {
				game = new Game(title->useSaveFile());
				gameDrawer = new GameDrawer(game);
				delete title;
				gamePlay = true;
			}
			else if (result == Title::REBOOT) {
				// �Q�[�����ċN��
				delete title;
				ChangeWindowMode(WINDOW), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
				SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
				title = new Title();
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