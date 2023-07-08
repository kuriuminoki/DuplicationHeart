#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// キューに入った全オブジェクトを削除する
void deleteObject(queue<Object*>& objects) {
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// 削除
		delete object;
		objectSum--;
	}
}

// キューに入った全オブジェクトを動かす
void actionObject(queue<Object*>& objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// オブジェクトの動き
		object->action();
		// deleteFlagがtrueなら削除する
		if (object->getDeleteFlag()) {
			delete object;
		}
		else {
			// 削除しない
			objects.push(object);
		}
		objectSum--;
	}
}

// キャラクターとオブジェクトの当たり判定
void atariCharacterAndObject(CharacterController* controller, queue<Object*> objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// 当たり判定をここで行う
		object->atari(controller);
		// deleteFlagがtrueなら削除する
		if (object->getDeleteFlag()) {
			delete object;
		}
		else {
			// 削除しない
			objects.push(object);
		}
		objectSum--;
	}
}


/*
* オブジェクトのロードなど
*/
World::World(int areaNum) {
	// 主人公のスタート地点
	m_areaNum = areaNum;

	// エリアに存在するオブジェクトをロード
	Object* object1 = new BoxObject(0, 900, 1920, 1080, WHITE);
	m_stageObjects.push(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_stageObjects.push(object3);
	m_stageObjects.push(object2);

	// 主人公をロード
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push(heart);

	// カメラを主人公注目、倍率1.0で作成
	m_camera = new Camera(heart->getX(), heart->getY(), 1.0);

	// 主人公の動きを作成
	StickAction* heartAction = new StickAction(heart);

	//主人公のコントローラ作成
	CharacterKeyboardController* heartController = new CharacterKeyboardController(heartAction);
	m_characterControllers.push(heartController);
}

World::~World() {
	// カメラを削除する
	delete m_camera;

	// 全オブジェクトを削除する。
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 全コントローラを削除する。
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		delete controller;
		characterSum--;
	}
}

// CharacterActionのキューを返す
queue<const CharacterAction*> World::getActions() const {
	// コントローラのコピーを作成（constメソッドにするため対策）
	queue<CharacterController*> controllers = m_characterControllers;

	queue<const CharacterAction*> actions;
	while (!controllers.empty()) {
		// コントローラを取得
		CharacterController* controller = controllers.front();
		controllers.pop();
		actions.push(controller->getAction());
	}
	return actions;
}

// 戦わせる
void World::battle() {

	// キャラクターの動き
	controlCharacter();

	// オブジェクトの動き
	controlObject();

}

// キャラクターの動き
void World::controlCharacter() {
	// 各コントローラを動作させる
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		// コントローラを取得
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();

		// 行動前の処理
		controller->init();

		// オブジェクトとの当たり判定
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);

		// 操作
		controller->control();

		// 射撃攻撃
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != NULL) { m_attackObjects.push(bulletAttack); }

		// 斬撃攻撃
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != NULL) { m_attackObjects.push(slashAttack); }

		// 反映
		controller->action();

		// 次のループへ
		m_characterControllers.push(controller);
		characterSum--;
	}
}

// オブジェクトの動き
void World::controlObject() {
	// 壁や床の動き
	actionObject(m_stageObjects);

	// 攻撃当たり判定の動き
	actionObject(m_attackObjects);

	// 壁や床<->攻撃の当たり判定
	atariStageAndAttack();
}

// 壁や床<->攻撃の当たり判定
void World::atariStageAndAttack() {
	size_t attackObjectSum = m_attackObjects.size();
	while (attackObjectSum > 0) {
		// 攻撃のオブジェクト
		Object* attackObject = m_attackObjects.front();
		m_attackObjects.pop();

		// 床や壁との当たり判定
		size_t stageObjectSum = m_stageObjects.size();
		while (stageObjectSum > 0) {
			// 攻撃のオブジェクト
			Object* stageObject = m_stageObjects.front();
			m_stageObjects.pop();

			// 当たっているか判定
			stageObject->atariObject(attackObject);

			// deleteFlagがtrueなら削除する
			if (stageObject->getDeleteFlag()) {
				delete stageObject;
			}
			else {
				// 削除しない
				m_stageObjects.push(stageObject);
			}
			stageObjectSum--;
		}

		// deleteFlagがtrueなら削除する
		if (attackObject->getDeleteFlag()) {
			delete attackObject;
		}
		else {
			// 削除しない
			m_attackObjects.push(attackObject);
		}

		attackObjectSum--;
	}
}

// 会話させる
void World::talk() {

}