#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
#include "ObjectDrawer.h"
#include <queue>

using namespace std;


WorldDrawer::WorldDrawer(const World* world) {
	m_world = world;
	m_characterDrawer = new CharacterDrawer(NULL);
	m_objectDrawer = new ObjectDrawer(NULL);
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
	delete m_objectDrawer;
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
}