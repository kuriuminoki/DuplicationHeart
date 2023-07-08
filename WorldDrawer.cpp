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

// 描画する
void WorldDrawer::draw() {
	// カメラを取得
	const Camera* camera = m_world->getCamera();

	// キャラを描画
	queue<const CharacterAction*> actions = m_world->getActions();
	// 各コントローラを動作させる
	size_t characterSum = actions.size();
	while (!actions.empty()) {
		// コントローラを取得
		const CharacterAction* action = actions.front();
		actions.pop();

		// キャラをDrawerにセット
		m_characterDrawer->setCharacterAction(action);

		// カメラを使ってキャラを描画
		m_characterDrawer->drawCharacter(camera);
	}
}