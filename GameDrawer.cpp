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

	m_gameoverHandle = LoadGraph("picture/system/gameover.png");
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
	DeleteGraph(m_gameoverHandle);
}

void GameDrawer::draw() {

	// ゲームオーバー
	int gameoverCnt = m_game->getGameoverCnt();
	if (gameoverCnt > 0) {
		if ((gameoverCnt < 60 && gameoverCnt / 2 % 2 == 0) || gameoverCnt > 60) {
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
	m_worldDrawer->draw();

	// スキルの時間等を描画
	if (skill != nullptr) {
		int num = skill->getLoopNum();
		int now = skill->getLoopNow();
		if (now < num) {
			ostringstream oss;
			int cnt1 = (int)skill->getCnt();
			int cnt2 = (int)((skill->getCnt() * 10) - cnt1 * 10);
			oss << now + 1 << "/" << num << "：" << cnt1 << "." << cnt2;
			DrawStringToHandle((int)(700 * m_exX), (int)(50 * m_exY), oss.str().c_str(), BLACK, m_skillHandle);
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