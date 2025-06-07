#include "GameDrawer.h"
#include "Game.h"
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

	getGameEx(m_exX, m_exY);

	m_worldDrawer = new WorldDrawer(nullptr);

	m_skillHandle = CreateFontToHandle(nullptr, (int)(SKILL_SIZE * m_exX), 10);

	m_skillInfoHandle = LoadGraph("picture/battleMaterial/skillInfo.png");
	m_skillInfoBackHandle = LoadGraph("picture/battleMaterial/skillInfoBack.png");

	m_gameoverHandle = LoadGraph("picture/system/gameover.png");

	m_noticeSaveDataHandle = LoadGraph("picture/system/noticeSaveDone.png");
	m_noticeEx = 0.7;
	GetGraphSize(m_noticeSaveDataHandle, &m_noticeX, &m_noticeY);
	m_noticeX = (int)(m_noticeX * (m_exX / 2 * m_noticeEx));
	m_noticeY = (int)(m_noticeY * (m_exY / 2 * m_noticeEx));
	m_noticeX += (int)(10 * m_exX);
	m_noticeY = GAME_HEIGHT - m_noticeY - (int)(10 * m_exY);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
	DeleteGraph(m_skillInfoHandle);
	DeleteGraph(m_skillInfoBackHandle);
	DeleteGraph(m_gameoverHandle);
	DeleteGraph(m_noticeSaveDataHandle);
}

void GameDrawer::draw() {

	// ゲームオーバー
	int gameoverCnt = m_game->getGameoverCnt();
	if (gameoverCnt > 0) {
		if ((gameoverCnt < FPS_N && gameoverCnt / 2 % 2 == 0) || gameoverCnt > FPS_N) {
			DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, min(m_exX, m_exY) * 0.7, 0.0, m_gameoverHandle, TRUE);
		}

		return;
	}

	// 世界を描画
	const HeartSkill* skill = m_game->getSkill();
	if (skill != nullptr) {
		m_worldDrawer->setWorld(skill->getWorld());
	}
	else {
		m_worldDrawer->setWorld(m_game->getWorld());
	}
	m_worldDrawer->draw(m_game->afterSkillUsableStoryNum());

	// セーブ完了通知
	int noticeSaveDone = m_game->getGameData()->getNoticeSaveDone();
	int alpha = 0;
	if (noticeSaveDone > 0) {
		if (noticeSaveDone * 3 > m_game->getGameData()->NOTICE_SAVE_DONE_TIME * 2) {
			alpha = min(255, (m_game->getGameData()->NOTICE_SAVE_DONE_TIME - noticeSaveDone) * 3);
		}
		else if (noticeSaveDone * 3 > m_game->getGameData()->NOTICE_SAVE_DONE_TIME ) {
			alpha = 255;
		}
		else {
			alpha = max(0, noticeSaveDone * 3);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(m_noticeX, m_noticeY, min(m_exX, m_exY) * m_noticeEx, 0.0, m_noticeSaveDataHandle, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}

	// スキルの時間等を描画
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
			oss << now + 1 << "/" << num << "：" << cnt1 << "." << cnt2;
			DrawStringToHandle((int)(900 * m_exX), (int)(30 * m_exY), oss.str().c_str(), BLACK, m_skillHandle);
		}
	}

	// 一時停止画面
	if (m_game->getGamePause() != nullptr) {
		SetMouseDispFlag(TRUE);//マウス表示
		m_game->getGamePause()->draw();
	}
	else{
		SetMouseDispFlag(MOUSE_DISP);//マウス表示
	}

}