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
#include "Text.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include "Game.h"
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
	m_duplicationFlag = false;

	m_brightValue = 255;

	// サウンドプレイヤー
	m_soundPlayer_p = soundPlayer;

	// 会話イベント
	m_conversation_p = nullptr;

	// ムービー
	m_movie_p = nullptr;

	// スキル発動中
	m_skillFlag = false;

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

	// プレイヤーをセット
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player = m_characters[i];
			break;
		}
	}
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
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
	}
}

World::World(const World* original) {
	m_duplicationFlag = true;
	m_brightValue = 255;
	m_conversation_p = nullptr;
	m_movie_p = nullptr;
	m_skillFlag = false;
	m_areaNum = original->getAreaNum();

	// エリアをコピー
	m_camera = new Camera(original->getCamera());
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_soundPlayer_p = original->getSoundPlayer();
	// キャラをコピー
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player = copy; }
	}
	// コントローラをコピー
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// BrainとActionコピー用にCharacterとカメラを渡す
		copy = original->getCharacterControllers()[i]->createCopy(m_characters, m_camera);
		m_characterControllers.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getStageObjects().size(); i++) {
		Object* copy;
		copy = original->getStageObjects()[i]->createCopy();
		m_stageObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getDoorObjects().size(); i++) {
		Object* copy;
		copy = original->getDoorObjects()[i]->createCopy();
		m_doorObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAttackObjects().size(); i++) {
		Object* copy;
		copy = original->getAttackObjects()[i]->createCopy();
		m_attackObjects.push_back(copy);
	}
	for (unsigned int i = 0; i < original->getAnimations().size(); i++) {
		Animation* copy;
		copy = original->getAnimations()[i]->createCopy();
		m_animations.push_back(copy);
	}
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();
}

// スキル発動
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// ストーリーやイベントによる追加キャラクター
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// キャラクター
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// コントローラ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// ストーリーやイベントによる追加オブジェクト
void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 壁や床
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// ドア
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

std::vector<CharacterController*> World::getCharacterControllers() const {
	return m_characterControllers;
}
std::vector<Character*> World::getCharacters() const {
	return m_characters;
}
std::vector<Object*> World::getStageObjects() const {
	return m_stageObjects;
}
std::vector<Object*> World::getDoorObjects() const {
	return m_doorObjects;
}
std::vector<Object*> World::getAttackObjects() const {
	return m_attackObjects;
}
std::vector<Animation*> World::getAnimations() const {
	return m_animations;
}

void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

void World::popCharacter(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "ハート") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			for (unsigned int j = 0; j < m_characterControllers.size(); j++) {
				if (m_characterControllers[j]->getBrain()->getTargetId() == id) {
					m_characterControllers[j]->setTarget(nullptr);
				}
			}
			//m_characters[i]->setHp(0);
			delete m_characters[i];
			m_characters[i] = m_characters.back();
			m_characters.pop_back();
			i--;
		}
	}
}


// レコーダを作成し使用を開始
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// レコーダの時間を最初に戻す
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// レコーダの使用をやめて削除する
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// キャラ1体の情報を世界に反映
void World::asignedCharacter(Character* character, CharacterData* data) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// このゲームで初登場じゃない
		character->setHp(data->hp());
	}
	//character->setId(data.id());
	character->setGroupId(data->groupId());
	character->setX(data->x());
	// Y座標は身長に合わせて調整
	character->setY(data->y() - character->getHeight());
}

// コントローラ1個の情報を世界に反映
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Actionを作成
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brainを作成
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controllerを作成
	return createController(data->controllerName(), brain, action);
}

// キャラの状態を変更する いないなら作成する
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// キャラの設定
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data);
			flag = true;
		}
	}
	// キャラを新規作成する場合（このエリアにいるはずのキャラだがまだいない）
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "ハート")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data);
		m_characters.push_back(character);
		m_characterControllers.push_back(createControllerWithData(character, data));
		return;
	}
	// コントローラ、アクション、Brainの設定
	size_t controllerSize = m_characterControllers.size();
	for (unsigned int i = 0; i < controllerSize; i++) {
		const Character* character = m_characterControllers[i]->getAction()->getCharacter();
		if (name == character->getName()) {
			CharacterController* controller = createControllerWithData(character, data);
			delete m_characterControllers[i];
			m_characterControllers[i] = controller;
		}
	}
}

// キャラの状態を教える
void World::asignCharacterData(const char* name, CharacterData* data, int fromAreaNum) const {
	size_t size = m_characterControllers.size();
	for (unsigned i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == name) {
			const Character* c = m_characterControllers[i]->getAction()->getCharacter();
			data->setVersion(c->getVersion());
			data->setHp(c->getHp());
			data->setId(c->getId());
			data->setGroupId(c->getGroupId());
			data->setAreaNum(fromAreaNum);
			data->setX(c->getX());
			data->setY(c->getY() + c->getHeight()); // Y2座標を保存 ロード時は身長で補正
			data->setBrainName(m_characterControllers[i]->getBrain()->getBrainName());
			data->setTargetName(m_characterControllers[i]->getBrain()->getTargetName());
			if (m_characterControllers[i]->getBrain()->getFollow() != nullptr) {
				data->setFollowName(m_characterControllers[i]->getBrain()->getFollow()->getName().c_str());
			}
			data->setActionName(m_characterControllers[i]->getAction()->getActionName());
			data->setSoundFlag(m_characterControllers[i]->getAction()->getSoundPlayer() != nullptr);
			data->setControllerName(m_characterControllers[i]->getControllerName());
			break;
		}
	}
}

// Doorの状態を変更する いないなら作成する
void World::asignedDoorData(DoorData* data) {
	if (data->from() != m_areaNum) { return; }
	bool flag = false;
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		if (data->to() == m_doorObjects[i]->getAreaNum()) {
			m_doorObjects[i]->setX1(data->x1());
			m_doorObjects[i]->setY1(data->y1());
			m_doorObjects[i]->setX2(data->x2());
			m_doorObjects[i]->setY2(data->y2());
			flag = true;
			break;
		}
	}
	if (!flag) {
		m_doorObjects.push_back(new DoorObject(data->x1(), data->y1(), data->x2(), data->y2(), data->fileName(), data->to()));
	}
}

// Doorの状態を教える
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// セーブデータにドアが存在するか
		bool flag = false;
		for (unsigned j = 0; j < size; j++) {
			if (data[j]->to() == m_doorObjects[i]->getAreaNum() && data[j]->from() == fromAreaNum) {
				data[j]->setX1(m_doorObjects[i]->getX1());
				data[j]->setY1(m_doorObjects[i]->getY1());
				data[j]->setX2(m_doorObjects[i]->getX2());
				data[j]->setY2(m_doorObjects[i]->getY2());
				data[j]->setFrom(fromAreaNum);
				data[j]->setTo(m_doorObjects[i]->getAreaNum());
				data[j]->setFileName(m_doorObjects[i]->getFileName());
				flag = true;
				break;
			}
		}
		if (!flag) { // 新たなドアならセーブデータに追加
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// プレイヤーとその仲間をドアの前に移動
void World::setPlayerOnDoor(int from) {
	int doorX1 = 0, doorY2 = 0;
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// プレイヤー
	m_player->setX(doorX1);
	m_player->setY(doorY2 - m_player->getHeight());

	// プレイヤーの仲間
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// 追跡対象がプレイヤーなら
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controllerに対応するCharacterに変更を加える
			for (unsigned int j = 0; j < m_characters.size(); j++) {
				if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_characters[j]->getId()) {
					m_characters[j]->setX(doorX1);
					m_characters[j]->setY(doorY2 - m_characters[j]->getHeight());
					break;
				}
			}
		}
	}
	cameraPointInit();
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
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	return allAnimations;
}

/*
*  戦わせる
*/
void World::battle() {
	// 画面暗転中 エリア移動かプレイヤーやられ時
	if (m_brightValue != 255 || playerDead()) {
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

	// スキル発動中は扉は入れない
	if (m_skillFlag) { return; }

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
		atariCharacterAndObject(controller, m_stageObjects);
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// 操作 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
		}

		// 射撃攻撃
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// 斬撃攻撃
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// 反映 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

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
	if (m_conversation_p != nullptr) {
		m_conversation_p->play();
		// 会話終了
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// ムービーを流す
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// ムービー終了
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}


Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// カメラの位置をリセット
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// プレイヤーのHPが0ならtrue
bool World::playerDead() {
	return m_player->getHp() <= 0;
}

// プレイヤーのHPをMAXにする
void World::playerHpReset() {
	m_player->setHp(m_player->getMaxHp());
}