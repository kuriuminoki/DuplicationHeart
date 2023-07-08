#include "WorldDrawer.h"
#include "World.h"
#include "Camera.h"
#include "CharacterDrawer.h"
#include "CharacterAction.h"
#include <queue>

using namespace std;


WorldDrawer::WorldDrawer(const World* world) {
	m_world = world;
	m_characterDrawer = new CharacterDrawer(NULL);
}

WorldDrawer::~WorldDrawer() {
	delete m_characterDrawer;
}

// �`�悷��
void WorldDrawer::draw() {
	// �J�������擾
	const Camera* camera = m_world->getCamera();

	// �L������`��
	queue<const CharacterAction*> actions = m_world->getActions();
	// �e�R���g���[���𓮍삳����
	size_t characterSum = actions.size();
	while (!actions.empty()) {
		// �R���g���[�����擾
		const CharacterAction* action = actions.front();
		actions.pop();

		// �L������Drawer�ɃZ�b�g
		m_characterDrawer->setCharacterAction(action);

		// �J�������g���ăL������`��
		m_characterDrawer->drawCharacter(camera);
	}
}