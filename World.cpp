#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Animation.h"
#include "Sound.h"
#include "CsvReader.h"
#include "Control.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>


using namespace std;


// vectorに入った全オブジェクトを削除する
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vectorに入ったdeleteFlagがtarueのオブジェクトを削除する
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
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

// vectorに入った全オブジェクトを動かす
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

// キャラクターとオブジェクトの貫通時判定
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 当たり判定をここで行う
		objects[i]->penetration(controller);
	}
}


/*
* オブジェクトのロードなど
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) {
	m_brightValue = 255;

	// サウンドプレイヤー
	m_soundPlayer_p = soundPlayer;

	// 主人公のスタート地点
	m_areaNum = toAreaNum;

	// エリアをロード
	const AreaReader data(fromAreaNum, toAreaNum, m_soundPlayer_p);
	m_camera = data.getCamera();
	m_focusId = data.getFocusId();
	m_playerId = data.getPlayerId();
	m_soundPlayer_p->setBGM(data.getBgm(), data.getBgmVolume());
	m_characters = data.getCharacters();
	m_characterControllers = data.getCharacterControllers();
	m_stageObjects = data.getObjects();
	m_doorObjects = data.getDoorObjects();
	data.getBackGround(m_backGroundGraph, m_backGroundColor);
}

World::~World() {
	// カメラを削除する
	delete m_camera;

	// 全オブジェクトを削除する。
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// 攻撃エフェクト削除
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// 全コントローラを削除する。
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 全キャラクターを削除する。
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	// 背景
	DeleteGraph(m_backGroundGraph);
}

// キャラの状態を変更する
void World::asignedCharacterData(const char* name, int hp) {
	size_t size = m_characters.size();
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			m_characters[i]->setHp(hp);
		}
	}
}

// キャラの状態を教える
void World::asignCharacterData(const char* name, int& hp) {
	size_t size = m_characters.size();
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			hp = m_characters[i]->getHp();
			return;
		}
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

// Objectのvectorを返す
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// キャラより後ろに描画するObjectのvectorを返す
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Animationのvectorを返す
vector<const Animation*> World::getAnimations() const {

	vector<const Animation*> allAnimations;
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	return allAnimations;
}

// 戦わせる
void World::battle() {
	// 画面暗転中
	if (m_brightValue != 255) {
		m_brightValue = max(0, m_brightValue - 10);
		return;
	}

	// HP0のキャラコントローラ削除
	cleanCharacterController();

	// deleteFlagがtrueのオブジェクトを削除する。
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// キャラの更新（攻撃対象の変更）
	// 上でキャラを削除したから更新したから必要
	updateCharacter();

	// キャラクターの動き
	controlCharacter();

	// オブジェクトの動き
	controlObject();

	// カメラの更新
	updateCamera();

	// サウンドプレイヤーのパン設定用
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// アニメーションの更新
	updateAnimation();

}

// ＨＰ０のキャラコントローラ削除
void World::cleanCharacterController() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() == 0) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
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
	// キャラとカメラの距離の最大
	int max_dx = 0, max_dy = 0;
	// 画面内に入れようとする距離の最大　これより離れたキャラは無視
	const int MAX_DISABLE = 2000;
	for (unsigned int i = 0; i < size; i++) {
		// 今フォーカスしているキャラの座標に合わせる
		if (m_focusId == m_characters[i]->getId()) {
			x = m_characters[i]->getCenterX();
			y = m_characters[i]->getCenterY();
			m_camera->setGPoint(x, y);
		}
		else if (m_characters[i]->getHp() > 0) {
			int dx = abs(m_camera->getX() - m_characters[i]->getX()) + m_characters[i]->getWide();
			if (dx < MAX_DISABLE) {
				max_dx = max(max_dx, dx);
				max_dy = max(max_dy, abs(m_camera->getY() - m_characters[i]->getY()) + m_characters[i]->getHeight());
			}
		}
	}
	// カメラはゆっくり動く
	m_camera->move();

	// カメラの拡大・縮小
	// 大きく変更する必要がある場合ほど、大きく拡大率を変更する。
	double nowEx = m_camera->getEx();
	int leftShift = controlLeftShift();
	if (leftShift > 0) {
		m_camera->setEx(max(nowEx - 0.01, 0.1));
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		max_dx = (int)(max_dx * nowEx);
		max_dy = (int)(max_dy + nowEx);
		if (nowEx > 0.5 && (max_dx > nowWide || max_dy > nowHeight)) {
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.05, d / 200000));
		}
		else if (nowEx < 1.5 && (max_dx < nowWide && max_dy < nowHeight)) {
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.05, d / 200000));
		}
	}
}

// アニメーションの更新
void World::updateAnimation() {
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}
}

// キャラクターとオブジェクトの当たり判定
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 当たり判定をここで行う
		if (objects[i]->atari(controller)) {
			// 当たった場合 エフェクト作成
			int x = controller->getAction()->getCharacter()->getCenterX();
			int y = controller->getAction()->getCharacter()->getCenterY();
			m_animations.push_back(objects[i]->createAnimation(x, y, 3));
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
		}
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

// キャラクターと扉オブジェクトの当たり判定
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 当たり判定をここで行う
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			// 当たった場合 エリア移動が発生
			m_areaNum = objects[i]->getAreaNum();
			// 画面を暗転
			m_brightValue--;
		}
	}
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
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

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

		// オブジェクトとの貫通判定
		penetrationCharacterAndObject(controller, m_stageObjects);
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

	// 攻撃<->攻撃の当たり判定
	atariAttackAndAttack();
}

// 壁や床<->攻撃の当たり判定
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 攻撃が壁床に当たっているか判定
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// 当たった場合 エフェクト作成
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[i]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
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

// 攻撃<->攻撃の当たり判定
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// 攻撃が壁床に当たっているか判定
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// 当たった場合 エフェクト作成
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[j]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
		}
	}
}

// 会話させる
void World::talk() {

}


Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return NULL;
}

CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return NULL;
}