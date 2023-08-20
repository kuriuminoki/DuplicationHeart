#include "GameDrawer.h"
#include "Game.h"
#include "World.h"
#include "WorldDrawer.h"
#include "Define.h"
#include "DxLib.h"
#include <string>
#include <sstream>


using namespace std;


GameDrawer::GameDrawer(const Game* game) {
	m_game = game;

	m_worldDrawer = new WorldDrawer(NULL);

	m_skillHandle = CreateFontToHandle(NULL, SKILL_SIZE, 10);
}

GameDrawer::~GameDrawer() {
	delete m_worldDrawer;
	DeleteFontToHandle(m_skillHandle);
}

void GameDrawer::draw() {
	// ¢ŠE‚ð•`‰æ
	HeartSkill* skill = m_game->getSkill();
	if (skill != NULL) {
		m_worldDrawer->setWorld(skill->getWorld());
	}
	else {
		m_worldDrawer->setWorld(m_game->getWorld());
	}
	m_worldDrawer->draw();

	// ƒXƒLƒ‹‚ÌŽžŠÔ“™‚ð•`‰æ
	if (skill != NULL) {
		int num = skill->getLoopNum();
		int now = skill->getLoopNow();
		if (now < num) {
			ostringstream oss;
			int cnt1 = (int)skill->getCnt();
			int cnt2 = (int)((skill->getCnt() * 10) - cnt1 * 10);
			oss << now + 1 << "/" << num << "F" << cnt1 << "." << cnt2;
			DrawStringToHandle(700, 50, oss.str().c_str(), BLACK, m_skillHandle);
		}
	}
}