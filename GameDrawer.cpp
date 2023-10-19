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

	m_worldDrawer = new WorldDrawer(nullptr);

	m_skillHandle = CreateFontToHandle(nullptr, SKILL_SIZE, 10);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
}

void GameDrawer::draw() {

	// 世界を描画
	HeartSkill* skill = m_game->getSkill();
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
			DrawStringToHandle(700, 50, oss.str().c_str(), BLACK, m_skillHandle);
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