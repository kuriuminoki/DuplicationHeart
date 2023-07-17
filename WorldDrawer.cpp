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

// 描画する
void WorldDrawer::draw() {
	// カメラを取得
	const Camera* camera = m_world->getCamera();

	// 各Actionを描画
	vector<const CharacterAction*> actions = m_world->getActions();
	size_t size = actions.size();
	for (unsigned int i = 0; i < size; i++) {
		// キャラをDrawerにセット
		m_characterDrawer->setCharacterAction(actions[i]);

		// カメラを使ってキャラを描画
		m_characterDrawer->drawCharacter(camera);
	}

	// 各Objectを描画
	vector<const Object*> objects = m_world->getObjects();
	size = objects.size();
	for (unsigned int i = 0; i < size; i++) {
		// ObjectをDrawerにセット
		m_objectDrawer->setObject(objects[i]);

		// カメラを使ってObjectを描画
		m_objectDrawer->drawObject(camera);
	}
}