#include "GameDrawer.h"
#include "Game.h"
#include "Story.h"
#include "Timer.h"
#include "World.h"
#include "WorldDrawer.h"
#include "PausePage.h"
#include "Define.h"
#include "DxLib.h"
#include <string>
#include <sstream>


using namespace std;


GameDrawer::GameDrawer(const Game* game) {
	m_game = game;

	m_screenEffectHandle = LoadGraph("picture/battleMaterial/screen.png");

	getGameEx(m_exX, m_exY);

	m_worldDrawer = new WorldDrawer(nullptr);

	m_skillHandle = CreateFontToHandle(nullptr, (int)(SKILL_SIZE * m_exX), 10);
	m_quickModeFontHandle = CreateFontToHandle(nullptr, (int)(QUICK_SIZE * m_exX), 5);

	m_skillInfoHandle = LoadGraph("picture/battleMaterial/skillInfo.png");
	m_skillInfoBackHandle = LoadGraph("picture/battleMaterial/skillInfoBack.png");

	m_timeBarNoonHandle = LoadGraph("picture/battleMaterial/timeBarNoon.png");
	m_timeBarEveningHandle = LoadGraph("picture/battleMaterial/timeBarEvening.png");
	m_timeBarNightHandle = LoadGraph("picture/battleMaterial/timeBarNight.png");
	m_needleHandle = LoadGraph("picture/battleMaterial/needle.png");

	m_gameoverHandle = LoadGraph("picture/system/gameover.png");

	m_quickModeHandle = LoadGraph("picture/battleMaterial/quickMode.png");
	m_quickModeCnt = 0;

	m_noticeSaveDataHandle = LoadGraph("picture/system/noticeSaveDone.png");
	m_noticeEx = 0.7;
	GetGraphSize(m_noticeSaveDataHandle, &m_noticeX, &m_noticeY);
	m_noticeX = (int)(m_noticeX * (m_exX / 2 * m_noticeEx));
	m_noticeY = (int)(m_noticeY * (m_exY / 2 * m_noticeEx));
	m_noticeX += (int)(10 * m_exX);
	m_noticeY = GAME_HEIGHT - m_noticeY - (int)(10 * m_exY);

	m_tmpScreenR = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
	m_tmpScreenG = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
	m_tmpScreenB = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
	DeleteFontToHandle(m_quickModeFontHandle);
	DeleteGraph(m_screenEffectHandle);
	DeleteGraph(m_skillInfoHandle);
	DeleteGraph(m_skillInfoBackHandle);
	DeleteGraph(m_timeBarNoonHandle);
	DeleteGraph(m_timeBarEveningHandle);
	DeleteGraph(m_timeBarNightHandle);
	DeleteGraph(m_needleHandle);
	DeleteGraph(m_gameoverHandle);
	DeleteGraph(m_quickModeHandle);
	DeleteGraph(m_noticeSaveDataHandle);
	DeleteGraph(m_tmpScreenR);
	DeleteGraph(m_tmpScreenG);
	DeleteGraph(m_tmpScreenB);
}

void GameDrawer::draw(int screen) {

	// �Q�[���I�[�o�[
	int gameoverCnt = m_game->getGameoverCnt();
	if (gameoverCnt > 0) {
		if ((gameoverCnt < FPS_N && gameoverCnt / 2 % 2 == 0) || gameoverCnt > FPS_N) {
			DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, min(m_exX, m_exY) * 0.7, 0.0, m_gameoverHandle, TRUE);
		}

		return;
	}

	// ���E��`��
	const HeartSkill* skill = m_game->getSkill();
	if (skill != nullptr) {
		m_worldDrawer->setWorld(skill->getWorld());
	}
	else {
		m_worldDrawer->setWorld(m_game->getWorld());
	}
	m_worldDrawer->draw(m_game->afterSkillUsableLoop());

	// �X�L���̎��ԓ���`��
	if (skill != nullptr) {
		int num = skill->getLoopNum();
		int now = skill->getLoopNow();
		if (now < num) {
			ostringstream oss;
			int cnt1 = (int)skill->getCnt();
			int cnt2 = (int)((skill->getCnt() * 10) - cnt1 * 10);
			int x1 = (int)(850 * m_exX);
			int y1 = (int)(10 * m_exY);
			int x2 = (int)(1500 * m_exX);
			int y2 = (int)(150 * m_exY);
			int centerX = (x1 + x2) / 2;
			int dx = (int)((1500 - 850) * (skill->DUPLICATION_TIME - skill->getOriginalCnt()) * m_exX) / skill->DUPLICATION_TIME / 2;
			DrawExtendGraph(centerX - dx, y1, centerX + dx, y2, m_skillInfoBackHandle, TRUE);
			DrawExtendGraph(x1, y1, x2, y2, m_skillInfoHandle, TRUE);
			oss << now + 1 << "/" << num << "�F" << cnt1 << "." << cnt2;
			DrawStringToHandle((int)(900 * m_exX), (int)(30 * m_exY), oss.str().c_str(), BLACK, m_skillHandle);
		}
	}
	// �o�ߎ���
	else if (m_worldDrawer->getWorld()->getBrightValue() == 255 && !m_game->getStory()->eventNow()) {
		int date = m_game->getStory()->getDate();
		int barHandle = m_timeBarNoonHandle;
		if (date == 1) { barHandle = m_timeBarEveningHandle; }
		else if (date == 2) { barHandle = m_timeBarNightHandle; }
		int x = (int)(1350 * m_exX);
		int y = (int)(40 * m_exY);
		double ex = 0.08 * min(m_exX, m_exY);
		DrawRotaGraph(x, y, ex, 0.0, barHandle, TRUE);

		int time = m_game->getStory()->getTimer()->getTime();
		int lifespan = m_game->WORLD_LIFESPAN;
		int wide = (int)(390 * m_exX);
		int height = (int)(50 * m_exY);
		int leftX = (int)(1185 * m_exX);
		DrawRotaGraph(leftX + (time * wide / lifespan), y + height / 2, ex, 0.0, m_needleHandle, TRUE);
	}

	if (m_game->quickModeNow()) {
		m_quickModeCnt++;
		double angle = PI / 180 * m_quickModeCnt;
		DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, min(m_exX, m_exY) * 0.2, angle, m_quickModeHandle, TRUE);
		DrawStringToHandle((int)(20 * m_exX), GAME_HEIGHT - (int)(70 * m_exY), "�p�L�[�ŃX�s�[�h�A�b�v������", YELLOW, m_quickModeFontHandle);
	}
	else {
		m_quickModeCnt = 0;
	}

	// �Z�[�u�����ʒm
	int noticeSaveDone = m_game->getGameData()->getNoticeSaveDone();
	int alpha = 0;
	if (noticeSaveDone > 0) {
		if (noticeSaveDone * 3 > m_game->getGameData()->NOTICE_SAVE_DONE_TIME * 2) {
			alpha = min(255, (m_game->getGameData()->NOTICE_SAVE_DONE_TIME - noticeSaveDone) * 3);
		}
		else if (noticeSaveDone * 3 > m_game->getGameData()->NOTICE_SAVE_DONE_TIME) {
			alpha = 255;
		}
		else {
			alpha = max(0, noticeSaveDone * 3);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(m_noticeX, m_noticeY, min(m_exX, m_exY) * m_noticeEx, 0.0, m_noticeSaveDataHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}

	// �ꎞ��~���
	if (m_game->getGamePause() != nullptr) {
		SetMouseDispFlag(TRUE);//�}�E�X�\��
		m_game->getGamePause()->draw();
	}
	else{
		SetMouseDispFlag(MOUSE_DISP);//�}�E�X�\��
	}


	// ���������ʂ̉��H���s��
	SetDrawScreen(m_tmpScreenR);
	SetDrawBright(255, 0, 0);
	DrawGraph(0, 0, screen, TRUE);

	SetDrawScreen(m_tmpScreenG);
	SetDrawBright(0, 255, 0);
	DrawGraph(THIN, THIN / 2, screen, TRUE);
	GraphBlend(m_tmpScreenR, m_tmpScreenG, 255, DX_GRAPH_BLEND_SCREEN);

	SetDrawScreen(m_tmpScreenB);
	SetDrawBright(0, 0, 255);
	DrawGraph(THIN / 2, THIN, screen, TRUE);
	GraphBlend(m_tmpScreenR, m_tmpScreenB, 255, DX_GRAPH_BLEND_SCREEN);

	SetDrawScreen(screen);
	SetDrawBright(255, 255, 255);
	DrawGraph(0, 0, m_tmpScreenR, TRUE);
	GraphBlend(screen, m_screenEffectHandle, 100, DX_GRAPH_BLEND_OVERLAY);

	DrawBox(0, 0, THIN, GAME_HEIGHT, BLACK, TRUE);
	DrawBox(0, 0, GAME_WIDE, THIN, BLACK, TRUE);
	DrawBox(GAME_WIDE - THIN, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
	DrawBox(0, GAME_HEIGHT - THIN, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
}