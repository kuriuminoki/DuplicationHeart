#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
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
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
	delete m_animationDrawer;
	delete m_conversationDrawer;
}

// 描画する
void WorldDrawer::draw() {
	int bright = m_world->getBrightValue();
	SetDrawBright(bright, bright, bright);
	// 背景
	int groundGraph = m_world->getBackGroundGraph();
	if (groundGraph != -1) {

	}
	else {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, m_world->getBackGroundColor(), TRUE);
	}

	// カメラを取得
	const Camera* camera = m_world->getCamera();

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
	size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		// キャラをDrawerにセット
		m_characterDrawer->setCharacterAction(actions[i]);

		// カメラを使ってキャラを描画
		m_characterDrawer->drawCharacter(camera, bright);
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

	m_targetDrawer.setEx(camera->getEx());
	m_targetDrawer.draw();
	SetDrawBright(255, 255, 255);

	// ムービー
	const Movie* movie = m_world->getMovie();
	if (movie != nullptr) {
		m_animationDrawer->setAnimation(movie->getAnimation());
		m_animationDrawer->drawAnimation();
	}

	// テキストイベント
	const Conversation* conversation = m_world->getConversation();
	if (conversation != nullptr) {
		m_conversationDrawer->setConversation(conversation);
		m_conversationDrawer->draw();
	}
}