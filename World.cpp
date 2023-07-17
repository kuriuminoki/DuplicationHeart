#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// vectorに入った全オブジェクトを削除する
void deleteObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// キューに入った全オブジェクトを動かす
void actionObject(vector<Object*>& objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// オブジェクトの動き
		objects[i]->action();
		// deleteFlagがtrueなら削除する
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 末尾を削除する方が速い
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// キャラクターとオブジェクトの当たり判定
void atariCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 当たり判定をここで行う
		objects[i]->atari(controller);
		// deleteFlagがtrueなら削除する
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 末尾を削除する方が速い
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}


/*
* オブジェクトのロードなど
*/
World::World(int areaNum) {
	// 主人公のスタート地点
	m_areaNum = areaNum;

	// エリアに存在するオブジェクトをロード
	Object* object1 = new BoxObject(0, 900, 10000, 1080, WHITE);
	m_stageObjects.push_back(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	m_stageObjects.push_back(object2);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_stageObjects.push_back(object3);
	Object* object4 = new TriangleObject(2000, 600, 2600, 900, WHITE, true);
	m_stageObjects.push_back(object4);
	Object* object5 = new BoxObject(2600, 600, 4000, 900, WHITE);
	m_stageObjects.push_back(object5);

	// 主人公をロード キャラの削除はWorldがやる予定
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push_back(heart);

	// カメラを主人公注目、倍率1.0で作成
	m_playerId = heart->getId();
	m_focusId = m_playerId;
	m_camera = new Camera(0, 0, 1.0, CAMERA_SPEED_DEFAULT);
	m_camera->setPoint(heart->getCenterX(), heart->getCenterY());
	updateCamera();

	//主人公のコントローラ作成 BrainとActionの削除はControllerがやる。
	NormalController* heartController = new NormalController(new KeyboardBrain(m_camera), new StickAction(heart));
	m_characterControllers.push_back(heartController);

	// CPUをロード
	Heart* cp = new Heart(100, 100, 500, 0);
	m_characters.push_back(cp);
	//CPUのコントローラ作成 BrainとActionの削除はControllerがやる。
	NormalController* cpController = new NormalController(new NormalAI(), new StickAction(cp));
	m_characterControllers.push_back(cpController);
}

World::~World() {
	// カメラを削除する
	delete m_camera;

	// 全オブジェクトを削除する。
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 全コントローラを削除する。
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}
}

// CharacterActionのvectorを返す
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		actions.push_back(m_characterControllers[i]->getAction());
	}
	return actions;
}

// Objectのキューを返す
vector<const Object*> World::getObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// 戦わせる
void World::battle() {

	// HP0のキャラ削除
	cleanCharacter();

	// キャラの更新（攻撃対象の変更）
	updateCharacter();

	// キャラクターの動き
	controlCharacter();

	// オブジェクトの動き
	controlObject();

	// カメラの更新
	updateCamera();

}

// ＨＰ０のキャラ削除
void World::cleanCharacter() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getHp() == 0) {
			delete m_characters[i];
			m_characters[i] = m_characters.back();
			m_characters.pop_back();
			i--;
		}
	}
}

// キャラの更新（攻撃対象の変更）
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brainの要請で攻撃対象変更
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) { 
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

// カメラの更新
void World::updateCamera() {
	size_t size = m_characters.size();
	int x = 0, y = 0;
	for (unsigned int i = 0; i < size; i++) {
		// 今フォーカスしているキャラの座標に合わせる
		if (m_focusId == m_characters[i]->getId()) {
			x = m_characters[i]->getCenterX();
			y = m_characters[i]->getCenterY();
			m_camera->setGPoint(x, y);
		}
	}
	// カメラはゆっくり動く
	m_camera->move();
}

// キャラクターの動き
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];
		// 行動前の処理
		controller->init();

		// オブジェクトとの当たり判定
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);

		// 操作
		controller->control();

		// 射撃攻撃
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != NULL) { m_attackObjects.push_back(bulletAttack); }

		// 斬撃攻撃
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != NULL) { m_attackObjects.push_back(slashAttack); }

		// 反映
		controller->action();
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
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 攻撃が壁床に当たっているか判定
			m_stageObjects[j]->atariObject(m_attackObjects[i]);
			// 壁床のdeleteFlagがtrueなら削除する
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 攻撃のdeleteFlagがtrueなら削除する
		if (m_attackObjects[i]->getDeleteFlag()) {
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

// 会話させる
void World::talk() {

}