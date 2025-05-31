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
#include "Item.h"
#include "Text.h"
#include "Brain.h"
#include "ControllerRecorder.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include "Game.h"
#include "GraphHandle.h"
#include "DxLib.h"
#include <algorithm>


using namespace std;


/*
* プレイヤー切り替え処理
*/
PlayerChanger::PlayerChanger(std::vector<CharacterController*> controllers_p, const Character* player_p) {
	m_nowCharacter_p = player_p;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		if (controllers_p[i]->getAction()->getCharacter()->getId() == m_nowCharacter_p->getId()) {
			m_prevBrainName = controllers_p[i]->getBrain()->getBrainName();
			if (m_prevBrainName == "KeyboardBrain") {
				m_prevBrainName = "FollowNormalAI";
			}
			break;
		}
	}
}

const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p) {
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		int groupId = target->getGroupId();
		int id = target->getId();
		if (id == m_nowCharacter_p->getId()) { continue; }
		// 仲間かどうか判定
		if (groupId == m_nowCharacter_p->getGroupId()) {
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			if (id > m_nowCharacter_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}

void PlayerChanger::changeCharacter(string prevBrainName, const Character* nextCharacter_p) {
	m_prevBrainName = prevBrainName;
	m_nowCharacter_p = nextCharacter_p;
}


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
		if (objects[i]->getStopCnt() > 0 && objects[i]->getStopCnt() != 30) {
			objects[i]->setStopCnt(objects[i]->getStopCnt() - 1);
			continue;
		}
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
* コンストラクタ
*/
World::World() {
	m_duplicationFlag = false;

	m_dispHpInfoFlag = false;

	m_brightValue = 255;

	m_resetBgmFlag = false;

	m_blindFlag = false;

	// 会話イベント
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// ムービー
	m_movie_p = nullptr;

	// スキル発動中
	m_skillFlag = false;

	// カメラの倍率の最大・最小値を解像度から決定
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;
	m_controlCharacterLock = false;

	m_date = 0;

	m_bossDeadEffectCnt = 0;

	m_money = 0;

}

/*
* オブジェクトのロードなど
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// サウンドプレイヤー
	m_soundPlayer_p = soundPlayer;

	// 主人公のスタート地点
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// エリアをロード
	const AreaReader data(fromAreaNum, toAreaNum, m_soundPlayer_p);
	m_camera = data.getCamera();
	m_focusId = data.getFocusId();
	m_playerId = data.getPlayerId();
	m_soundPlayer_p->setBGM(data.getBgm());
	m_characters = data.getCharacters();
	m_characterControllers = data.getCharacterControllers();
	m_stageObjects = data.getObjects();
	m_doorObjects = data.getDoorObjects();
	data.getBackGround(m_backGroundGraph, m_backGroundColor);

	// プレイヤーをセット
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}
	// プレイヤーの方向へ向かせる
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		m_characterControllers[i]->setPlayerDirection(m_player_p, true);
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	m_camera->setEx(m_cameraMaxEx);

	m_characterDeadGraph = new GraphHandles("picture/effect/dead", 5, 1.0, 0, true);
	m_characterDamageGraph = new GraphHandles("picture/effect/damage", 1, 0.2, 0, true);
	m_bombGraph = new GraphHandles("picture/effect/bomb", 9, 1.0, 0, true);
	m_characterDeadSound = LoadSoundMem("sound/battle/dead.wav");
	m_bombSound = LoadSoundMem("sound/battle/bomb.wav");
	m_doorSound = LoadSoundMem("sound/battle/door.wav");
	m_characterChangeSound = LoadSoundMem("sound/battle/characterChange.mp3");
	m_cameraInSound = LoadSoundMem("sound/battle/cameraIn.mp3");
	m_cameraOutSound = LoadSoundMem("sound/battle/cameraOut.mp3");

}

World::World(const World* original) :
	World()
{
	m_duplicationFlag = true;

	// エリアをコピー (プリミティブ型)
	m_areaNum = original->getAreaNum();
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_date = original->getDate();
	m_money = original->getMoney();
	m_areaLock = original->getAreaLock();
	m_controlCharacterLock = original->getControlCharacterLock();

	// エリアをコピー (コピー元と共有するもの)
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_characterDamageGraph = original->getCharacterDamageGraph();
	m_bombGraph = original->getBombGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_bombSound = original->getBombSound();
	m_doorSound = original->getDoorSound();
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();
	m_bossDeadEffectCnt = original->getBossDeadEffextCnt();

	// 新規作成するもの (ポインタが変わる)
	m_camera = new Camera(original->getCamera());
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player_p = copy; }
	}
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
	for (unsigned int i = 0; i < original->getItemVector().size(); i++) {
		Item* copy;
		copy = original->getItemVector()[i]->createCopy();
		m_itemVector.push_back(copy);
	}

	// 初期設定
	m_camera->setEx(m_cameraMaxEx);

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

	// アイテム削除
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// 全コントローラを削除する。
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 全キャラクターを削除する。
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// 背景
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
		delete m_characterDeadGraph;
		delete m_characterDamageGraph;
		delete m_bombGraph;
		DeleteSoundMem(m_characterDeadSound);
		DeleteSoundMem(m_bombSound);
		DeleteSoundMem(m_doorSound);
		DeleteSoundMem(m_characterChangeSound);
		DeleteSoundMem(m_cameraInSound);
		DeleteSoundMem(m_cameraOutSound);
	}

	if (m_objectConversation != nullptr) {
		delete m_objectConversation;
	}
}

// Drawer用：CharacterActionのvectorを返す
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// HPが０かつDeadGraphがないなら表示しない
		if (!m_characterControllers[i]->getAction()->getCharacter()->noDispForDead()) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
		else if (m_characterControllers[i]->getAction()->getCharacter()->getBossFlag() && m_bossDeadEffectCnt > 0) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
	}
	return actions;
}

// Drawer用：Objectのvectorを返す
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer用：キャラより後ろに描画するObjectのvectorを返す
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer用：Animationのvectorを返す
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// エフェクト
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// アイテム
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			// 消滅しそうなら点滅
			if (m_itemVector[i]->getCnt() < m_itemVector[i]->ERASE_CNT * 2 / 3 || m_itemVector[i]->getCnt() / 3 % 2 == 0) {
				allAnimations.push_back(m_itemVector[i]->getAnimation());
			}
		}
	}

	return allAnimations;
}

// 名前でキャラ検索
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// 名前でコントローラ検索
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// IDでキャラ検索
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID指定でBrain変更
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// ストーリーによる追加キャラクター
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// キャラクター
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// コントローラ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// ループによるキャラclear
void World::clearCharacter() {
	for (int i = (int)m_characterControllers.size() - 1; i >= 0; i--) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() != "ハート") {
			delete m_characterControllers[i];
			m_characterControllers.pop_back();
		}
	}
	for (int i = (int)m_characters.size() - 1; i >= 0; i--) {
		if (m_characters[i]->getName() != "ハート") {
			delete m_characters[i];
			m_characters.pop_back();
		}
	}
}

// ストーリーによるキャラの性能変化
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

// ストーリーによる追加オブジェクト
void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 壁や床
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// ドア
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// プレイヤーのHPが0ならtrue
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// プレイヤーのHPをMAXにする
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
	m_player_p->setSkillGage(m_player_p->SKILL_MAX);
}

void World::cureHpOfHearts(int value) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_player_p->getGroupId() == m_characters[i]->getGroupId()) {
			m_characters[i]->setHp(m_characters[i]->getHp() + value);
		}
	}
}

// 今操作しているキャラの名前を取得
string World::getControlCharacterName() const {
	return m_playerChanger->getNowPlayer()->getCharacterInfo()->name();
}


// スキル発動：ハートをFreezeにする
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// スキル発動：複製のハート追加用
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// スキル発動：複製のハート削除用
void World::popCharacterController(int id) {
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
	// TargetにしているAIは外してもらいたいのでHP=0にする。
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "ハート") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// スキル発動：レコーダを作成し使用を開始
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

// スキル発動：レコーダの時間を最初に戻す
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// スキル発動：レコーダの使用をやめて削除する
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "ハート") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// データ管理：キャラの状態を変更する いないなら作成する
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// キャラの設定
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// キャラを新規作成する場合（このエリアにいるはずのキャラだがまだいない）
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "ハート")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// コントローラ、アクション、Brainの設定
	size_t controllerSize = m_characterControllers.size();
	for (unsigned int i = 0; i < controllerSize; i++) {
		const Character* character = m_characterControllers[i]->getAction()->getCharacter();
		if (name == character->getName()) {
			CharacterController* controller = createControllerWithData(character, data);
			controller->setPlayerDirection(m_player_p, true);
			delete m_characterControllers[i];
			m_characterControllers[i] = controller;
		}
	}
}

// データ管理：キャラの状態を教える
void World::asignCharacterData(const char* name, CharacterData* data, int fromAreaNum, bool notCharacterPoint) const {
	size_t size = m_characterControllers.size();
	for (unsigned i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == name) {
			const Character* c = m_characterControllers[i]->getAction()->getCharacter();
			data->setVersion(c->getVersion());
			data->setHp(c->getHp());
			data->getSkillGage(c->getSkillGage());
			data->setInvincible(c->getInvincible());
			data->setId(c->getId());
			data->setGroupId(c->getGroupId());
			data->setAreaNum(fromAreaNum);
			if (!notCharacterPoint) {
				data->setX(c->getX());
				data->setY(c->getY() + c->getHeight()); // Y2座標を保存 ロード時は身長で補正
			}
			data->setBrainName(m_characterControllers[i]->getBrain()->getBrainName());
			data->setTargetName(m_characterControllers[i]->getBrain()->getTargetName());
			if (m_characterControllers[i]->getBrain()->getFollow() != nullptr) {
				data->setFollowName(m_characterControllers[i]->getBrain()->getFollow()->getName().c_str());
			}
			string actionName = m_characterControllers[i]->getAction()->getActionName();
			if (m_characterControllers[i]->getAction()->getHeavy()) {
				actionName += "_x";
			}
			data->setActionName(actionName.c_str());
			data->setSoundFlag(m_characterControllers[i]->getAction()->getSoundPlayer() != nullptr);
			data->setControllerName(m_characterControllers[i]->getControllerName());
			break;
		}
	}
}

// データ管理：Doorの状態を変更する いないなら作成する
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

// データ管理：Doorの状態を教える
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// ドアじゃない
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
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

// データ管理：プレイヤーとその仲間をドアの前に移動
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// プレイヤー
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// 仲間も移動
	setPlayerFollowerPoint();

	// カメラリセット
	cameraPointInit();
}

// プレイヤーを特定の座標へ移動
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// カメラリセット
	cameraPointInit();
}

// 仲間をプレイヤーの位置へ移動
void World::setPlayerFollowerPoint() {
	// プレイヤーの仲間
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getAction()->getCharacter();
		// 追跡対象がプレイヤーなら
		if (follow != nullptr && m_player_p->getGroupId() == follow->getGroupId()) {
			// Controllerに対応するCharacterに変更を加える
			for (unsigned int j = 0; j < m_characters.size(); j++) {
				if (m_characters[j]->getId() == follow->getId()) {
					m_characters[j]->setX(m_player_p->getX() + GetRand(50) - 25);
					m_characters[j]->setY(m_player_p->getY() + m_player_p->getHeight() - m_characters[j]->getHeight());
					// HP=0なら半分回復して復活
					if (m_characters[j]->getHp() == 0) { m_characters[j]->setHp(m_characters[j]->getMaxHp() / 2); }
					// gx, gyを更新するため
					m_characterControllers[i]->setBrainFollow(m_player_p);
					break;
				}
			}
		}
	}
}

void World::changePlayer(const Character* nextPlayer) {

	// 禁止されている
	if (m_controlCharacterLock) {
		return;
	}

	// 変更できるキャラがいない
	if (nextPlayer == nullptr || nextPlayer->getId() == m_playerChanger->getNowPlayer()->getId()) { 
		return;
	}

	// 効果音
	m_soundPlayer_p->pushSoundQueue(m_characterChangeSound);

	// 今操作中のキャラをNPCに変更(Brainを戻す)
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_playerChanger->getNowPlayer()->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// 次操作するキャラを修正(BrainをKeyboardにする)
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_playerChanger->changeCharacter(brainName, nextPlayer);
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// 追跡対象を操作キャラにする
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getGroupId() == m_characterControllers[i]->getAction()->getCharacter()->getGroupId()) {
			m_characterControllers[i]->setBrainFollow(nextPlayer);
		}
	}
	// カメラが注目するキャラも変更
	m_focusId = nextPlayer->getId();
}

// データ管理：カメラの位置をリセット
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getAtariCenterY() - m_characters[i]->getHeight() / 3);
			break;
		}
	}
}

// データ管理：キャラ1体の情報を世界に反映
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// このゲームで初登場じゃない
		character->setHp(data->hp());
		if (data->hp() == -1) { // -1は最大HPを表す
			character->setHp(character->getMaxHp());
		}
		character->setSkillGage(data->skillGage());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y座標は身長に合わせて調整
		character->setY(data->y() - character->getHeight());
	}
}

// データ管理：コントローラ1個の情報を世界に反映
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

/*
*  戦わせる
*/
void World::battle() {

	m_dispHpInfoFlag = true;

	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	
	// 画面暗転処理
	if (dealBrightValue()) { return; }

	// オブジェクトを調べた時のテキスト
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// 会話終了
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

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

	// アイテムの動き
	controlItem();

	// ボス撃破のエフェクト
	createBossDeadEffect();

	// カメラの更新
	updateCamera();

	// サウンドプレイヤーのパン設定用
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// アニメーションの更新
	updateAnimation();

	// キャラ変更
	if (!m_duplicationFlag && !m_skillFlag) {
		changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers));
	}

}

//  Battle：カメラの更新
void World::updateCamera() {

	// カメラを揺らす
	m_camera->shaking();

	// キャラとカメラの距離の最大値を調べる
	int max_dx = 0, max_dy = 0;
	// 画面内に入れようとする距離の最大　これより離れたキャラは無視
	const int MAX_DISABLE = 2500;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// 今フォーカスしているキャラの座標に合わせる
		if (m_focusId == m_characters[i]->getId()) {
			int gy = m_characters[i]->getAtariCenterY() - m_characters[i]->getHeight() / 3;
			m_camera->setGPoint(m_characters[i]->getAtariCenterX(), gy);
			int dy = abs(m_camera->getY() - gy);
			max_dy = max(max_dy, dy * dy / 100);
		}
		// フォーカスしているキャラ以外なら距離を調べる
		else if (m_characters[i]->getHp() > 0) {
			int x = m_characters[i]->getX();
			if (m_camera->getX() < x) { x += m_characters[i]->getWide() * 2; }
			else { x -= m_characters[i]->getWide(); }
			int dx = abs(m_camera->getX() - x);
			if (dx < MAX_DISABLE) {
				max_dx = max(max_dx, dx);
				int y = m_characters[i]->getY();
				if (m_camera->getY() < y) { y += m_characters[i]->getHeight() * 3 / 2; }
				else { y -= m_characters[i]->getHeight() / 2; }
				max_dy = max(max_dy, abs(m_camera->getY() - y));
			}
		}
	}

	// カメラを目標位置へ近づける
	m_camera->move();

	// カメラの拡大・縮小
	// 大きく変更する必要がある場合ほど、大きく拡大率を変更する。
	double nowEx = m_camera->getEx();
	int shift = controlLeftShift() + controlRightShift();
	if (shift == 1) {
		bool zoomOutMode = m_camera->getZoomOutMode();
		m_camera->setZoomOutMode(!zoomOutMode);
		if (zoomOutMode) {
			m_soundPlayer_p->pushSoundQueue(m_cameraInSound);
		}
		else {
			m_soundPlayer_p->pushSoundQueue(m_cameraOutSound);
		}
	}
	if (m_camera->getZoomOutMode()) {
		if (nowEx > m_cameraMinEx) {
			m_camera->setEx(max(nowEx - 0.02 * m_exX, 0.1));
		}
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		if (nowEx > m_cameraMinEx && (max_dx > nowWide || max_dy > nowHeight)) {
			// 縮小
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.1, d / 100000) * m_exX);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// 拡大
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.005, d / 100000) * m_exX);
		}
	}
}

//  Battle：アニメーションの更新
void World::updateAnimation() {
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->setVy(m_animations[i]->getVy() + 1);
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}
}

//  Battle：キャラの更新（攻撃対象の変更）
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

//  Battle：キャラクターの動き
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// 斬撃が当たった時のヒットストップ
		if(controller->getAction()->getCharacter()->getStopCnt() > 0 && controller->getAction()->getCharacter()->getStopCnt() != SLASH_STOP_CNT){
			continue;
		}

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->noDispForDead()) {
			continue;
		}

		// 行動前の処理
		controller->init();

		// オブジェクトとの当たり判定
		atariCharacterAndObject(controller, m_stageObjects, true);
		atariCharacterAndObject(controller, m_stageObjects, false); // 1回目で斜面にいるかがわかり、それによって処理が変わるため2回目が必要
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects, false);
		}
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// 操作 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// 射撃攻撃
		vector<Object*>* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.insert(m_attackObjects.end(), bulletAttack->begin(), bulletAttack->end()); }

		// 斬撃攻撃
		vector<Object*>* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.insert(m_attackObjects.end(), slashAttack->begin(), slashAttack->end()); }
	}

	// キャラ間の当たり判定
	atariCharacterAndCharacter();

	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// 斬撃が当たった時のヒットストップ
		if (controller->getAction()->getCharacter()->getStopCnt() > 0) {
			controller->consumeStopCnt();
			// 1フレーム目はcontinueしない（斬撃が二十で発生しないため）
			if (controller->getAction()->getCharacter()->getStopCnt() != SLASH_STOP_CNT - 1) {
				continue;
			}
		}

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->noDispForDead()) {
			continue;
		}

		// 反映 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// オブジェクトとの貫通判定
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle：オブジェクトの動き
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

// Battle：アイテムの動き
void World::controlItem() {

	if (m_itemVector.size() == 0) { return; }
	// 今操作しているキャラが当たり判定の対象
	Character* targetCharacter = nullptr;
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_playerChanger->getNowPlayer()->getId()) { 
			targetCharacter = m_characters[i];
			break;
		}
	}

	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// 取得済み
		if (m_itemVector[i]->getDeleteFlag()) {
			// 効果音が再生中でないなら削除
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// 初期化
		m_itemVector[i]->init();
		// 壁床との当たり判定
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			int vx = m_itemVector[i]->getVx();
			int vy = m_itemVector[i]->getVy();
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, vx, vy)) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
			m_itemVector[i]->setVx(vx);
			m_itemVector[i]->setVy(vy);
		}
		// キャラとの当たり判定
		if (targetCharacter != nullptr && m_itemVector[i]->atariCharacter(targetCharacter)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
			// ここでお金をWorldに反映
			m_money = min(m_money + targetCharacter->getMoney(), MAX_MONEY);
			targetCharacter->setMoney(0);
			break;
		}
		// 動き
		m_itemVector[i]->action();
	}
}

// Battle：キャラクター<->キャラクターの当たり判定
void World::atariCharacterAndCharacter() {
	static const int SPEED = 3;
	for (unsigned int i = 0; i < m_characterControllers.size() - 1; i++) {
		for (unsigned int j = i + 1; j < m_characterControllers.size(); j++) {

			CharacterController* controllerA = m_characterControllers[i];
			CharacterController* controllerB = m_characterControllers[j];

			if (m_duplicationFlag && controllerA->getAction()->getCharacter()->getId() == m_playerId) {
				continue;
			}
			if (m_duplicationFlag && controllerB->getAction()->getCharacter()->getId() == m_playerId) {
				continue;
			}
			// HPが0ならスキップ
			if (controllerA->getAction()->getCharacter()->noDispForDead() || controllerA->getAction()->getCharacter()->getBossFlag()) {
				continue;
			}
			if (controllerB->getAction()->getCharacter()->noDispForDead() || controllerB->getAction()->getCharacter()->getBossFlag()) {
				continue;
			}

			int ax1 = 0, ay1 = 0, ax2 = 0, ay2 = 0;
			controllerA->getAction()->getCharacter()->getAtariArea(&ax1, &ay1, &ax2, &ay2);

			int bx1 = 0, by1 = 0, bx2 = 0, by2 = 0;
			controllerB->getAction()->getCharacter()->getAtariArea(&bx1, &by1, &bx2, &by2);

			if (ax2 > bx1 && ax1 < bx2 && ay2 > by1 && ay1 < by2) {
				if ((ax1 + ax2) < (bx1 + bx2)) {
					controllerA->addActionDx(-SPEED);
					controllerB->addActionDx(SPEED);
				}
				else {
					controllerA->addActionDx(SPEED);
					controllerB->addActionDx(-SPEED);
				}
			}

		}
	}
}

//  Battle：キャラクターとオブジェクトの当たり判定
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects, bool slope) {
	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (objects[i]->slopeFlag() != slope) { continue; }
		// 当たり判定をここで行う
		if (objects[i]->atari(controller)) {
			const Character* character = controller->getAction()->getCharacter();
			int targetX1 = 0, targetY1 = 0, targetX2 = 0, targetY2 = 0;
			character->getAtariArea(&targetX1, &targetY1, &targetX2, &targetY2);
			// エフェクト作成
			int x = (targetX1 + targetX2) / 2;
			int y = (targetY1 + targetY2) / 2;
			Animation* atariAnimation = objects[i]->createAnimation(x, y, 3); // 攻撃がキャラに当たったエフェクト
			if (atariAnimation != nullptr) {
				m_animations.push_back(atariAnimation);
			}
			createDamageEffect(x, y, GetRand(3) + 1); // 丸のエフェクト
			// 効果音
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);

			// 斬撃が当たった時のヒットストップ
			if (objects[i]->getStopCharacterId() != -1) {
				for (unsigned int j = 0; j < m_characterControllers.size(); j++) {
					if (objects[i]->getStopCharacterId() == m_characterControllers[j]->getAction()->getCharacter()->getId()) {
						m_characterControllers[j]->stopCharacter(SLASH_STOP_CNT);
						controller->stopCharacter(SLASH_STOP_CNT);
						break;
					}
				}
			}

			// HP = 0になったとき（やられたとき）
			if (character->noDispForDead()) {
				if (character->getBossFlag()) {
					m_bossDeadEffectCnt = 300;
				}
				else {
					m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
					m_camera->shakingStart(20, 20);
					m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
					if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && !character->getBossFlag()) {
						int r = GetRand(100);
						// スキル発動中でなければ雑魚キャラは確率でアイテムが落ちる
						if (r < 20) {
							m_itemVector.push_back(new CureItem("cure", x, y, 50));
						}
						else {
							for (int i = 0; i < r % 4; i++) {
								MoneyItem* money = new MoneyItem("money", x, y, 1);
								money->setVx(GetRand(30) - 15);
								money->setVy(GetRand(30) - 31);
								m_itemVector.push_back(money);
							}
						}
					}
				}
			}
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

//  Battle：キャラクターと扉オブジェクトの当たり判定
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// スキル発動中は扉に入れない
	if (m_skillFlag) { return; }

	// 壁や床オブジェクトの処理 (当たり判定と動き)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// 当たり判定をここで行う
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// ドアじゃない
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// 当たった場合 エリア移動が発生
				m_nextAreaNum = objects[i]->getAreaNum();
				// 画面を暗転
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle：壁や床<->攻撃の当たり判定
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 攻撃が壁床に当たっているか判定
			if (m_stageObjects[j]->atariFromObject(m_attackObjects[i])) {
				// エフェクト作成
				Animation* atariAnimation = m_attackObjects[i]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				createDamageEffect(x, y, GetRand(3) + 1); // 丸のエフェクト
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// 壁床のdeleteFlagがtrueなら削除する
			if (m_stageObjects[j]->getDeleteFlag()) {
				createDamageEffect(x, y, 10);
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 攻撃のdeleteFlagがtrueなら削除する
		if (m_attackObjects[i]->getDeleteFlag()) {
			createBomb(x, y, m_attackObjects[i]);
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle：攻撃<->攻撃の当たり判定
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = 0; j < m_attackObjects.size(); j++) {
			if (i == j) { continue; }
			// 攻撃が他の攻撃に当たっているか判定
			if (m_attackObjects[j]->atariToObject(m_attackObjects[i]) && (m_attackObjects[i]->getDeleteFlag() || !m_attackObjects[i]->getAbleDelete())) {
				// エフェクト作成
				Animation* atariAnimation = m_attackObjects[i]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				createBomb(x, y, m_attackObjects[j]);
				int soundHandle = m_attackObjects[j]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
		}
	}
}

// Battle: 爆発を起こす
void World::createBomb(int x, int y, Object* attackObject) {
	if (attackObject == nullptr) {
		m_animations.push_back(new Animation(x, y, 3, m_bombGraph));
		// 効果音
		m_soundPlayer_p->pushSoundQueue(m_bombSound, adjustPanSound(x, m_camera->getX()));
		// 画面の揺れ
		m_camera->shakingStart(20, 20);
	}
	else if (attackObject->getBomb()) {
		// 爆発
		BombObject* bomb = new BombObject(x, y, 500, 500, attackObject->getDamage(), new Animation(x, y, 3, m_bombGraph));
		bomb->setCharacterId(attackObject->getCharacterId());
		bomb->setGroupId(attackObject->getGroupId());
		m_attackObjects.push_back(bomb);
		// 効果音
		m_soundPlayer_p->pushSoundQueue(m_bombSound, adjustPanSound(x, m_camera->getX()));
		// 画面の揺れ
		m_camera->shakingStart(20, 20);
	}
}

// Battle: ダメージエフェクト作成
void World::createDamageEffect(int x, int y, int sum) {
	for (int i = 0; i < sum; i++) {
		Animation* animation = new Animation(x + GetRand(100) - 50, y + GetRand(100) - 50, 120, m_characterDamageGraph);
		animation->setVx(GetRand(30) - 15);
		animation->setVy(GetRand(30) - 31);
		animation->setMovable(true);
		m_animations.push_back(animation);
	}
}

// Battle: ボスがやられたときの爆発エフェクト
void World::createBossDeadEffect() {
	if (m_bossDeadEffectCnt > 0) {
		m_bossDeadEffectCnt--;
		if (m_bossDeadEffectCnt % 20 == 0) {
			for (unsigned int i = 0; i < m_characters.size(); i++) {
				if (m_characters[i]->getBossFlag()) {
					int x1 = m_characters[i]->getX();
					int y1 = m_characters[i]->getY();
					int x2 = x1 + m_characters[i]->getWide();
					int y2 = y1 + m_characters[i]->getHeight();
					int x = GetRand(x2 - x1) + x1;
					int y = GetRand(y2 - y1) + y1;
					createBomb(x, y, nullptr);
					createDamageEffect(x, y, 2);
					break;
				}
			}
		}
	}
}

// 各キャラが目標地点へ移動するだけ 全員到達したらtrueを返す
bool World::moveGoalCharacter() {

	m_dispHpInfoFlag = false;

	// deleteFlagがtrueのオブジェクトを削除する。
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// キャラの更新（攻撃対象の変更）
	// 上でキャラを削除したから更新したから必要
	updateCharacter();

	// キャラクターの動き
	bool allCharacterAlreadyGoal = true;
	const size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->noDispForDead()) {
			continue;
		}

		// 行動前の処理
		controller->init();

		// オブジェクトとの当たり判定
		atariCharacterAndObject(controller, m_stageObjects, true);
		atariCharacterAndObject(controller, m_stageObjects, false); // 1回目で斜面にいるかがわかり、それによって処理が変わるため2回目が必要
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects, false);
		}

		// 目標地点へ移動する操作 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}
	}

	// キャラ間の当たり判定
	atariCharacterAndCharacter();

	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->noDispForDead()) {
			continue;
		}

		// 反映 originalのハートはフリーズ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// オブジェクトとの貫通判定
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// オブジェクトの動き
	controlObject();

	// アイテムの動き
	controlItem();

	// カメラの更新
	updateCamera();

	// サウンドプレイヤーのパン設定用
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// アニメーションの更新
	updateAnimation();

	return allCharacterAlreadyGoal;
}

bool World::dealBrightValue() {
	// 画面暗転中 エリア移動かプレイヤーやられ時
	if (m_brightValue != 255 || playerDead()) {
		m_brightValue = max(0, m_brightValue - 10);
		if (!playerDead()) { return true; }
	}
	return false;
}

// 会話させる
void World::talk() {
	moveGoalCharacter();
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