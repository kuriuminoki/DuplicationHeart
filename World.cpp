#include "World.h"
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

	// 主人公をロード
	Heart* heart = new Heart(100, 100, 0, 0, 5);
	m_characters.push(heart);

	// 主人公の動きを作成
	StickAction* heartAction = new StickAction(heart);

	//主人公のコントローラ作成
	CharacterKeyboardController* heartController = new CharacterKeyboardController(heartAction);
	m_characterControllers.push(heartController);
}

World::~World() {
	// 全コントローラを削除する。
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		delete controller;
		characterSum--;
	}
}

void World::battle() {

	// 各コントローラを動作させる
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		controller->control();
		m_characterControllers.push(controller);
		characterSum--;
	}
}

void World::talk() {

}