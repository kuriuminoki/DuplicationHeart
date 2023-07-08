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

	// �L������`��
	queue<const CharacterAction*> actions = m_world->getActions();
	// �eAction��`��
	while (!actions.empty()) {
		// Action���擾
		const CharacterAction* action = actions.front();
		actions.pop();

		// �L������Drawer�ɃZ�b�g
		m_characterDrawer->setCharacterAction(action);

		// �J�������g���ăL������`��
		m_characterDrawer->drawCharacter(camera);
	}

	queue<const Object*> objects = m_world->getObjects();
	// �eObject��`��
	while (!objects.empty()) {
		// Object���擾
		const Object* object = objects.front();
		objects.pop();

		// Object��Drawer�ɃZ�b�g
		m_objectDrawer->setObject(object);

		// �J�������g����Object��`��
		m_objectDrawer->drawObject(camera);
	}
}