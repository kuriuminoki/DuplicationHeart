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
	m_noonHaikei = LoadGraph("picture/stageMaterial/noon.jpg");
	m_eveningHaikei = LoadGraph("picture/stageMaterial/evening.jpg");
	m_nightHaikei = LoadGraph("picture/stageMaterial/night.jpg");
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
	delete m_animationDrawer;
	delete m_conversationDrawer;
	DeleteGraph(m_hpBarGraph);
	DeleteGraph(m_noonHaikei);
	DeleteGraph(m_eveningHaikei);
	DeleteGraph(m_nightHaikei);
}


// �`�悷��
void WorldDrawer::draw() {
	
	int bright = m_world->getBrightValue();
	SetDrawBright(bright, bright, bright);

	// �J�������擾
	const Camera* camera = m_world->getCamera();

	// ���
	if (!m_world->getBlindFlag()) {
		drawBattleField(camera, bright);
	}

	// ���[�r�[
	Movie* movie = m_world->getMovie();
	if (movie != nullptr) {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
		movie->draw();
	}

	// �e�L�X�g�C�x���g
	const Conversation* conversation = m_world->getConversation();
	if (conversation != nullptr) {
		m_conversationDrawer->setConversation(conversation);
		m_conversationDrawer->draw();
	}
	else {
		// StageObject�𒲂ׂ��ۂ̃e�L�X�g�C�x���g
		conversation = m_world->getObjectConversation();
		if (conversation != nullptr) {
			m_conversationDrawer->setConversation(conversation);
			m_conversationDrawer->draw();
		}
	}

	if (!m_world->getBlindFlag() && movie == nullptr && conversation == nullptr) {
		// �^�[�Q�b�g
		m_targetDrawer.setEx(camera->getEx());
		m_targetDrawer.draw();
	}

	SetDrawBright(255, 255, 255);
}


// ���̕`��
void WorldDrawer::drawBattleField(const Camera* camera, int bright) {
	// �w�i
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

	// �eObject��`��
	vector<const Object*> objects = m_world->getBackObjects();
	size_t size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(objects[i]);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}

	// �eAction��`��
	vector<const CharacterAction*> actions = m_world->getActions();
	size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		// �L������Drawer�ɃZ�b�g
		m_characterDrawer->setCharacterAction(actions[i]);

		// �J�������g���ăL������`��
		m_characterDrawer->drawCharacter(camera, bright);
	}

	// �eObject��`��
	objects = m_world->getFrontObjects();
	size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(objects[i]);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}

	// �eAnimation��`��
	vector<const Animation*> animations = m_world->getConstAnimations();
	size = animations.size();
	for (unsigned int i = 0; i < size; i++) {
		// Animation��Drawer�ɃZ�b�g
		m_animationDrawer->setAnimation(animations[i]);

		// �J�������g���ăL������`��
		m_animationDrawer->drawAnimation(camera);
	}

	// �n�[�g�̏��
	size = m_world->getCharacters().size();
	for (unsigned int i = 0; i < size; i++) {
		if (m_world->getCharacters()[i]->getName() == "�n�[�g") {
			m_characterDrawer->drawPlayerHpBar(m_world->getCharacters()[i], m_hpBarGraph);
		}
	}
}