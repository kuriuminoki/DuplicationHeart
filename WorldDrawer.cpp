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

	// キャラを描画
	queue<const CharacterAction*> actions = m_world->getActions();
	// 各Actionを描画
	while (!actions.empty()) {
		// Actionを取得
		const CharacterAction* action = actions.front();
		actions.pop();

		// キャラをDrawerにセット
		m_characterDrawer->setCharacterAction(action);

		// カメラを使ってキャラを描画
		m_characterDrawer->drawCharacter(camera);
	}

	queue<const Object*> objects = m_world->getObjects();
	// 各Objectを描画
	while (!objects.empty()) {
		// Objectを取得
		const Object* object = objects.front();
		objects.pop();

		// ObjectをDrawerにセット
		m_objectDrawer->setObject(object);

		// カメラを使ってObjectを描画
		m_objectDrawer->drawObject(camera);
	}
}