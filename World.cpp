#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


/*
* オブジェクトのロードなど
*/
World::World(int areaNum) {
	// 主人公のスタート地点
	m_areaNum = areaNum;

	// エリアに存在するオブジェクトをロード
	Object* object1 = new BoxObject(0, 900, 1920, 1080, WHITE);
	m_objects.push(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_objects.push(object2);
	m_objects.push(object3);

	// 主人公をロード
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push(heart);

	// 主人公の動きを作成
	StickAction* heartAction = new StickAction(heart);

	//主人公のコントローラ作成
	CharacterKeyboardController* heartController = new CharacterKeyboardController(heartAction);
	m_characterControllers.push(heartController);
}

World::~World() {
	// 全オブジェクトを削除する。
	size_t objectSum = m_objects.size();
	while (objectSum > 0) {
		Object* object = m_objects.front();
		m_objects.pop();
		delete object;
		objectSum--;
	}

	// 全コントローラを削除する。
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		delete controller;
		characterSum--;
	}
}

// 戦わせる
void World::battle() {

	// 各コントローラを動作させる
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();

		// 行動前の処理
		controller->init();

		// 当たり判定
		size_t objectSum = m_objects.size();
		while (objectSum > 0) {
			Object* object = m_objects.front();
			m_objects.pop();
			// 当たり判定をここで行う
			object->atari(controller);
			m_objects.push(object);
			objectSum--;
		}

		// 操作
		controller->control();

		// 反映
		controller->action();

		m_characterControllers.push(controller);
		characterSum--;
	}
}

// 会話させる
void World::talk() {

}