#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
#include "Character.h"
#include "ObjectDrawer.h"
#include "AnimationDrawer.h"
#include "Animation.h"
#include "TextDrawer.h"
#include "Text.h"
#include "Define.h"
#include "DxLib.h"
#include <queue>

using namespace std;


TargetDrawer::TargetDrawer() {
	m_ex = TARGET_EX;
	m_targetHandle1 = LoadGraph("picture/battleMaterial/mouseTarget1.png");
	m_targetAngle1 = 0.0;
	m_targetHandle2 = LoadGraph("picture/battleMaterial/mouseTarget2.png");
	m_targetAngle2 = 0.0;
	m_targetHandle3 = LoadGraph("picture/battleMaterial/mouseTarget3.png");
	m_targetAngle3 = 0.0;
}

TargetDrawer::~TargetDrawer() {
	DeleteGraph(m_targetHandle1);
	DeleteGraph(m_targetHandle2);
	DeleteGraph(m_targetHandle3);
}

void TargetDrawer::draw() {
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	DrawRotaGraph(mouseX, mouseY, m_ex, m_targetAngle1, m_targetHandle1, TRUE);
	DrawRotaGraph(mouseX, mouseY, m_ex, m_targetAngle2, m_targetHandle2, TRUE);
	DrawRotaGraph(mouseX, mouseY, m_ex, m_targetAngle3, m_targetHandle3, TRUE);
	anime();
}

void TargetDrawer::anime() {
	m_targetAngle2 += PI / 90;
	m_targetAngle3 -= PI / 180;
}


WorldDrawer::WorldDrawer(const World* world) {
	m_world = world;
	m_characterDrawer = new CharacterDrawer(nullptr);
	m_objectDrawer = new ObjectDrawer(nullptr);
	m_animationDrawer = new AnimationDrawer(nullptr);
	m_conversationDrawer = new ConversationDrawer(nullptr);
	m_hpBarGraph = LoadGraph("picture/battleMaterial/hpBar.png");
	m_skillBarGraph = LoadGraph("picture/battleMaterial/skillBar.png");
	m_bossHpBarGraph = LoadGraph("picture/battleMaterial/bossHpBar.png");
	m_noonHaikei = LoadGraph("picture/stageMaterial/noon.jpg");
	m_eveningHaikei = LoadGraph("picture/stageMaterial/evening.jpg");
	m_nightHaikei = LoadGraph("picture/stageMaterial/night.jpg");
	m_enemyNotice = LoadGraph("picture/battleMaterial/enemyNotice.png");
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
	delete m_animationDrawer;
	delete m_conversationDrawer;
	DeleteGraph(m_hpBarGraph);
	DeleteGraph(m_skillBarGraph);
	DeleteGraph(m_bossHpBarGraph);
	DeleteGraph(m_noonHaikei);
	DeleteGraph(m_eveningHaikei);
	DeleteGraph(m_nightHaikei);
	DeleteGraph(m_enemyNotice);
}


// 描画する
void WorldDrawer::draw(bool drawSkillBar) {
	
	int bright = m_world->getBrightValue();
	SetDrawBright(bright, bright, bright);

	// カメラを取得
	const Camera* camera = m_world->getCamera();

	// 戦場
	if (!m_world->getBlindFlag()) {
		drawBattleField(camera, bright, drawSkillBar);
	}

	// ムービー
	Movie* movie = m_world->getMovie();
	if (movie != nullptr) {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
		movie->draw();
	}

	// テキストイベント
	const Conversation* conversation = m_world->getConversation();
	if (conversation != nullptr) {
		m_conversationDrawer->setConversation(conversation);
		m_conversationDrawer->draw();
	}
	else {
		// StageObjectを調べた際のテキストイベント
		conversation = m_world->getObjectConversation();
		if (conversation != nullptr) {
			m_conversationDrawer->setConversation(conversation);
			m_conversationDrawer->draw();
		}
	}

	if (!m_world->getBlindFlag() && movie == nullptr && conversation == nullptr) {
		// ターゲット
		m_targetDrawer.setEx(camera->getEx());
		m_targetDrawer.draw();
	}

	SetDrawBright(255, 255, 255);
}


// 戦場の描画
void WorldDrawer::drawBattleField(const Camera* camera, int bright, bool drawSkillBar) {
	// 背景
	int groundGraph = m_world->getBackGroundGraph();
	if (groundGraph != -1) {

	}
	else {
		int date = m_world->getDate();
		int wide = 0, height = 0;
		double ex = 1.0;
		switch (date) {
		case 0:
			GetGraphSize(m_noonHaikei, &wide, &height);
			ex = max((double)GAME_WIDE / wide, (double)GAME_HEIGHT / height);
			DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, ex, 0.0, m_noonHaikei, TRUE);
			break;
		case 1:
			GetGraphSize(m_eveningHaikei, &wide, &height);
			ex = max((double)GAME_WIDE / wide, (double)GAME_HEIGHT / height);
			DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, ex, 0.0, m_eveningHaikei, TRUE);
			break;
		case 2:
			GetGraphSize(m_nightHaikei, &wide, &height);
			ex = max((double)GAME_WIDE / wide, (double)GAME_HEIGHT / height);
			DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, ex, 0.0, m_nightHaikei, TRUE);
			break;
		default:
			DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, m_world->getBackGroundColor(), TRUE);
			break;
		}
	}

	// 各Objectを描画
	vector<const Object*> objects = m_world->getBackObjects();
	size_t size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// ObjectをDrawerにセット
		m_objectDrawer->setObject(objects[i]);

		// カメラを使ってObjectを描画
		m_objectDrawer->drawObject(camera);
	}

	// 各Actionを描画
	vector<const CharacterAction*> actions = m_world->getActions();
	int player = -1;
	const CharacterAction* bossCharacterAction = nullptr;
	size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		if (actions[i]->getCharacter()->getId() == m_world->getPlayerId()) {
			player = i;
		}
	}
	for (unsigned int i = 0; i < size; i++) {
		// プレイヤーは後で手前に描画
		if (i != player) {
			// キャラをDrawerにセット
			m_characterDrawer->setCharacterAction(actions[i]);
			int enemyNotice = -1, groupId = actions[i]->getCharacter()->getGroupId();
			// 中立と味方なら通知しない
			if (groupId != -1 && player != -1 && groupId != actions[player]->getCharacter()->getGroupId()) {
				enemyNotice = m_enemyNotice;
			}
			// カメラを使ってキャラを描画
			m_characterDrawer->drawCharacter(camera, enemyNotice, bright);
			// ボスがいるなら保持しておく
			if (actions[i]->getCharacter()->getBossFlag()) { bossCharacterAction = actions[i]; }
		}
	}
	// プレイヤーは手前に描画
	if (player != -1) {
		m_characterDrawer->setCharacterAction(actions[player]);
		m_characterDrawer->drawCharacter(camera, -1, bright);
	}

	// 各Objectを描画
	objects = m_world->getFrontObjects();
	size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// ObjectをDrawerにセット
		m_objectDrawer->setObject(objects[i]);

		// カメラを使ってObjectを描画
		m_objectDrawer->drawObject(camera);
	}

	// 各Animationを描画
	vector<const Animation*> animations = m_world->getConstAnimations();
	size = animations.size();
	for (unsigned int i = 0; i < size; i++) {
		// AnimationをDrawerにセット
		m_animationDrawer->setAnimation(animations[i]);

		// カメラを使ってキャラを描画
		m_animationDrawer->drawAnimation(camera);
	}

	// ハートの情報
	if (player != -1) {
		const int x = 30;
		const int y = 30;
		const int wide = 700;
		const int height = 70;
		const Character* playerCharacter = actions[player]->getCharacter();
		m_characterDrawer->drawPlayerHpBar(x, y, wide, height, playerCharacter, m_hpBarGraph);
		if (drawSkillBar) {
			m_characterDrawer->drawPlayerSkillBar(x, y + height + 10, wide, 30, playerCharacter, m_skillBarGraph);
		}
	}

	// ボスの情報
	const int bX = 30;
	const int bY = 950;
	const int bWide = 1500;
	const int bHeight = 80;
	if (bossCharacterAction != nullptr) {
		m_characterDrawer->drawBossHpBar(bX, bY, bWide, bHeight, bossCharacterAction->getCharacter(), m_bossHpBarGraph);
	}

}