#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
#include "ObjectDrawer.h"
#include "AnimationDrawer.h"
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
	m_characterDrawer = new CharacterDrawer(NULL);
	m_objectDrawer = new ObjectDrawer(NULL);
	m_animationDrawer = new AnimationDrawer(NULL);
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
	delete m_animationDrawer;
}

// �`�悷��
void WorldDrawer::draw() {
	// �J�������擾
	const Camera* camera = m_world->getCamera();

	// �eAction��`��
	vector<const CharacterAction*> actions = m_world->getActions();
	size_t size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		// �L������Drawer�ɃZ�b�g
		m_characterDrawer->setCharacterAction(actions[i]);

		// �J�������g���ăL������`��
		m_characterDrawer->drawCharacter(camera);
	}

	// �eObject��`��
	vector<const Object*> objects = m_world->getObjects();
	size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(objects[i]);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}

	// �eAnimation��`��
	vector<const Animation*> animations = m_world->getAnimations();
	size = animations.size();
	for (unsigned int i = 0; i < size; i++) {
		// Animation��Drawer�ɃZ�b�g
		m_animationDrawer->setAnimation(animations[i]);

		// �J�������g���ăL������`��
		m_animationDrawer->drawAnimation(camera);
	}

	m_targetDrawer.setEx(camera->getEx());
	m_targetDrawer.draw();
}