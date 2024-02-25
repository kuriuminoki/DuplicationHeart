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
* 謫堺ｽ懊く繝｣繝ｩ螟画峩蜃ｦ逅
*/
PlayerChanger::PlayerChanger(std::vector<CharacterController*> controllers_p, const Character* player_p) {
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		if (controllers_p[i]->getAction()->getCharacter()->getId() == player_p->getId()) {
			m_prevBrainName = controllers_p[i]->getBrain()->getBrainName();
			if (m_prevBrainName == "KeyboardBrain") {
				m_prevBrainName = "FollowNormalAI";
			}
			break;
		}
	}
}

/*
* controllers_p縺九ｉplayer_p縺ｮ莉ｲ髢薙ｒ迚ｹ螳壹＠縲∵ｬ｡縺ｮ謫堺ｽ懊く繝｣繝ｩ繧定ｿ斐☆縲
* 螟画峩蜈医�繧ｭ繝｣繝ｩ縺ｯID縺ｧ豎ｺ縺ｾ繧九
* 繧ｫ繝｡繝ｩ縺ｮ繧ｻ繝�ヨ繝ｻplayer縺ｮBrain螟画峩縲…ontrollers_p縺ｮBrain螟画峩縺ｧ螳御ｺ
*/
const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p, const Character* player_p) {
	// E繧ｭ繝ｼ縺梧款縺輔ｌ縺ｦ縺�↑縺�↑繧我ｽ輔ｂ縺励↑縺
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		// 螻槭☆繧九げ繝ｫ繝ｼ繝
		int groupId = target->getGroupId();
		// ID
		int id = target->getId();
		if (id == player_p->getId()) { continue; }
		// 繝励Ξ繧､繝､繝ｼ縺ｮ莉ｲ髢薙�縺ｿ蟇ｾ雎｡
		if (groupId == player_p->getGroupId()) {
			// ID縺梧怙蟆上�繧ｭ繝｣繝ｩ
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			// ID縺後�繝ｬ繧､繝､繝ｼ縺ｮ谺｡縺ｫ蟆上＆縺�く繝｣繝ｩ
			if (id > player_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	// 繝励Ξ繧､繝､繝ｼ縺梧怙繧�D縺ｮ螟ｧ縺阪＞繧ｭ繝｣繝ｩ縺縺｣縺溷ｴ蜷�ullptr縺ｪ縺ｮ縺ｧID縺梧怙繧ょｰ上＆縺�く繝｣繝ｩ繧帝∈縺ｶ
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}


// vector縺ｫ蜈･縺｣縺溷�繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜑企勁縺吶ｋ
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector縺ｫ蜈･縺｣縺歸eleteFlag縺荊arue縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜑企勁縺吶ｋ
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag縺荊rue縺ｪ繧牙炎髯､縺吶ｋ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 譛ｫ蟆ｾ繧貞炎髯､縺吶ｋ譁ｹ縺碁溘＞
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector縺ｫ蜈･縺｣縺溷�繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜍輔°縺
void actionObject(vector<Object*>& objects) {
	// 螢√ｄ蠎翫が繝悶ず繧ｧ繧ｯ繝医�蜃ｦ逅 (蠖薙◆繧雁愛螳壹→蜍輔″)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 繧ｪ繝悶ず繧ｧ繧ｯ繝医�蜍輔″
		objects[i]->action();
		// deleteFlag縺荊rue縺ｪ繧牙炎髯､縺吶ｋ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 譛ｫ蟆ｾ繧貞炎髯､縺吶ｋ譁ｹ縺碁溘＞
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医�雋ｫ騾壽凾蛻､螳
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 螢√ｄ蠎翫が繝悶ず繧ｧ繧ｯ繝医�蜃ｦ逅 (蠖薙◆繧雁愛螳壹→蜍輔″)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 蠖薙◆繧雁愛螳壹ｒ縺薙％縺ｧ陦後≧
		objects[i]->penetration(controller);
	}
}


/*
* 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
*/
World::World() {
	m_duplicationFlag = false;

	m_brightValue = 255;

	// 莨夊ｩｱ繧､繝吶Φ繝
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// 繝繝ｼ繝薙�
	m_movie_p = nullptr;

	// 繧ｹ繧ｭ繝ｫ逋ｺ蜍穂ｸｭ
	m_skillFlag = false;

	// 繧ｫ繝｡繝ｩ縺ｮ蛟咲紫縺ｮ譛螟ｧ繝ｻ譛蟆丞､繧定ｧ｣蜒丞ｺｦ縺九ｉ豎ｺ螳
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;

	m_date = 0;

}

/*
* 繧ｪ繝悶ず繧ｧ繧ｯ繝医�繝ｭ繝ｼ繝峨↑縺ｩ
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// 繧ｵ繧ｦ繝ｳ繝峨�繝ｬ繧､繝､繝ｼ
	m_soundPlayer_p = soundPlayer;

	// 荳ｻ莠ｺ蜈ｬ縺ｮ繧ｹ繧ｿ繝ｼ繝亥慍轤ｹ
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// 繧ｨ繝ｪ繧｢繧偵Ο繝ｼ繝
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

	// 繝励Ξ繧､繝､繝ｼ繧偵そ繝�ヨ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	// 繝励Ξ繧､繝､繝ｼ縺ｮ譁ｹ蜷代∈蜷代°縺帙ｋ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		m_characterControllers[i]->setPlayerDirection(m_player_p, true);
	}

	m_camera->setEx(m_cameraMaxEx);

	m_characterDeadGraph = new GraphHandles("picture/effect/dead", 5, 1.0, 0, true);
	m_characterDeadSound = LoadSoundMem("sound/battle/dead.wav");
	m_doorSound = LoadSoundMem("sound/battle/door.wav");

}

World::World(const World* original) :
	World()
{
	m_duplicationFlag = true;
	m_areaNum = original->getAreaNum();

	// 繧ｨ繝ｪ繧｢繧偵さ繝斐�
	m_camera = new Camera(original->getCamera());
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_doorSound = original->getDoorSound();
	m_date = original->getDate();

	// 繧ｭ繝｣繝ｩ繧偵さ繝斐�
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player_p = copy; }
	}
	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);
	// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繧偵さ繝斐�
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// Brain縺ｨAction繧ｳ繝斐�逕ｨ縺ｫCharacter縺ｨ繧ｫ繝｡繝ｩ繧呈ｸ｡縺
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
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();

	m_camera->setEx(m_cameraMaxEx);

}

World::~World() {
	// 繧ｫ繝｡繝ｩ繧貞炎髯､縺吶ｋ
	delete m_camera;

	// 蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜑企勁縺吶ｋ縲
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// 謾ｻ謦�お繝輔ぉ繧ｯ繝亥炎髯､
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// 繧｢繧､繝�Β蜑企勁
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// 蜈ｨ繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ繧貞炎髯､縺吶ｋ縲
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 蜈ｨ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ繧貞炎髯､縺吶ｋ縲
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// 閭梧勹
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
		delete m_characterDeadGraph;
		DeleteSoundMem(m_characterDeadSound);
		DeleteSoundMem(m_doorSound);
	}

	if (m_objectConversation != nullptr) {
		delete m_objectConversation;
	}
}

// Drawer逕ｨ�咾haracterAction縺ｮvector繧定ｿ斐☆
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() > 0) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
	}
	return actions;
}

// Drawer逕ｨ�唹bject縺ｮvector繧定ｿ斐☆
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer逕ｨ�壹く繝｣繝ｩ繧医ｊ蠕後ｍ縺ｫ謠冗判縺吶ｋObject縺ｮvector繧定ｿ斐☆
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer逕ｨ�哂nimation縺ｮvector繧定ｿ斐☆
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// 繧ｨ繝輔ぉ繧ｯ繝
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// 繧｢繧､繝�Β
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			allAnimations.push_back(m_itemVector[i]->getAnimation());
		}
	}

	return allAnimations;
}

// 蜷榊燕縺ｧ繧ｭ繝｣繝ｩ讀懃ｴ｢
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// 蜷榊燕縺ｧ繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ讀懃ｴ｢
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// ID縺ｧ繧ｭ繝｣繝ｩ讀懃ｴ｢
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID謖�ｮ壹〒Brain螟画峩
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// 繧ｹ繝医�繝ｪ繝ｼ縺ｫ繧医ｋ霑ｽ蜉繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// ストーリーによるキャラの性能変化
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 螢√ｄ蠎
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// 繝峨い
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// 繝励Ξ繧､繝､繝ｼ縺ｮHP縺0縺ｪ繧液rue
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// 繝励Ξ繧､繝､繝ｼ縺ｮHP繧樽AX縺ｫ縺吶ｋ
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
}


// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ壹ワ繝ｼ繝医ｒFreeze縺ｫ縺吶ｋ
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "繝上�繝") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ夊､�｣ｽ縺ｮ繝上�繝郁ｿｽ蜉逕ｨ
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ夊､�｣ｽ縺ｮ繝上�繝亥炎髯､逕ｨ
void World::popCharacterController(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "繝上�繝") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	// Target縺ｫ縺励※縺�ｋAI縺ｯ螟悶＠縺ｦ繧ゅｉ縺�◆縺��縺ｧHP=0縺ｫ縺吶ｋ縲
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "繝上�繝") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ壹Ξ繧ｳ繝ｼ繝繧剃ｽ懈�縺嶺ｽｿ逕ｨ繧帝幕蟋
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "繝上�繝") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ壹Ξ繧ｳ繝ｼ繝縺ｮ譎る俣繧呈怙蛻昴↓謌ｻ縺
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "繝上�繝") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// 繧ｹ繧ｭ繝ｫ逋ｺ蜍包ｼ壹Ξ繧ｳ繝ｼ繝縺ｮ菴ｿ逕ｨ繧偵ｄ繧√※蜑企勁縺吶ｋ
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "繝上�繝") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// 繝��繧ｿ邂｡逅�ｼ壹く繝｣繝ｩ縺ｮ迥ｶ諷九ｒ螟画峩縺吶ｋ 縺�↑縺�↑繧我ｽ懈�縺吶ｋ
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// 繧ｭ繝｣繝ｩ縺ｮ險ｭ螳
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// 繧ｭ繝｣繝ｩ繧呈眠隕丈ｽ懈�縺吶ｋ蝣ｴ蜷茨ｼ医％縺ｮ繧ｨ繝ｪ繧｢縺ｫ縺�ｋ縺ｯ縺壹�繧ｭ繝｣繝ｩ縺縺後∪縺縺�↑縺�ｼ
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "繝上�繝")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// 繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ縲√い繧ｯ繧ｷ繝ｧ繝ｳ縲。rain縺ｮ險ｭ螳
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

// 繝��繧ｿ邂｡逅�ｼ壹く繝｣繝ｩ縺ｮ迥ｶ諷九ｒ謨吶∴繧
void World::asignCharacterData(const char* name, CharacterData* data, int fromAreaNum, bool notCharacterPoint) const {
	size_t size = m_characterControllers.size();
	for (unsigned i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == name) {
			const Character* c = m_characterControllers[i]->getAction()->getCharacter();
			data->setVersion(c->getVersion());
			data->setHp(c->getHp());
			data->setInvincible(c->getInvincible());
			data->setId(c->getId());
			data->setGroupId(c->getGroupId());
			data->setAreaNum(fromAreaNum);
			if (!notCharacterPoint) {
				data->setX(c->getX());
				data->setY(c->getY() + c->getHeight()); // Y2蠎ｧ讓吶ｒ菫晏ｭ 繝ｭ繝ｼ繝画凾縺ｯ霄ｫ髟ｷ縺ｧ陬懈ｭ｣
			}
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

// 繝��繧ｿ邂｡逅�ｼ咼oor縺ｮ迥ｶ諷九ｒ螟画峩縺吶ｋ 縺�↑縺�↑繧我ｽ懈�縺吶ｋ
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

// 繝��繧ｿ邂｡逅�ｼ咼oor縺ｮ迥ｶ諷九ｒ謨吶∴繧
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// 繝峨い縺倥ｃ縺ｪ縺
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
		// 繧ｻ繝ｼ繝悶ョ繝ｼ繧ｿ縺ｫ繝峨い縺悟ｭ伜惠縺吶ｋ縺
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
		if (!flag) { // 譁ｰ縺溘↑繝峨い縺ｪ繧峨そ繝ｼ繝悶ョ繝ｼ繧ｿ縺ｫ霑ｽ蜉
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// 繝��繧ｿ邂｡逅�ｼ壹�繝ｬ繧､繝､繝ｼ縺ｨ縺昴�莉ｲ髢薙ｒ繝峨い縺ｮ蜑阪↓遘ｻ蜍
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// 繝励Ξ繧､繝､繝ｼ
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// 莉ｲ髢薙ｂ遘ｻ蜍
	setPlayerFollowerPoint();

	// 繧ｫ繝｡繝ｩ繝ｪ繧ｻ繝�ヨ
	cameraPointInit();
}

// 繝励Ξ繧､繝､繝ｼ繧堤音螳壹�蠎ｧ讓吶∈遘ｻ蜍
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// 繧ｫ繝｡繝ｩ繝ｪ繧ｻ繝�ヨ
	cameraPointInit();
}

// 莉ｲ髢薙ｒ繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ縺ｸ遘ｻ蜍
void World::setPlayerFollowerPoint() {
	// 繝励Ξ繧､繝､繝ｼ縺ｮ莉ｲ髢
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// 霑ｽ霍｡蟇ｾ雎｡縺後�繝ｬ繧､繝､繝ｼ縺ｪ繧
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controller縺ｫ蟇ｾ蠢懊☆繧気haracter縺ｫ螟画峩繧貞刈縺医ｋ
			for (unsigned int j = 0; j < m_characters.size(); j++) {
				if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_characters[j]->getId()) {
					m_characters[j]->setX(m_player_p->getX());
					m_characters[j]->setY(m_player_p->getY() + m_player_p->getHeight() - m_characters[j]->getHeight());
					break;
				}
			}
		}
	}
}

// 繝��繧ｿ邂｡逅�ｼ壹き繝｡繝ｩ縺ｮ菴咲ｽｮ繧偵Μ繧ｻ繝�ヨ
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// 繝��繧ｿ邂｡逅�ｼ壹く繝｣繝ｩ1菴薙�諠�ｱ繧剃ｸ也阜縺ｫ蜿肴丐
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// 縺薙�繧ｲ繝ｼ繝縺ｧ蛻晉匳蝣ｴ縺倥ｃ縺ｪ縺
		character->setHp(data->hp());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y蠎ｧ讓吶�霄ｫ髟ｷ縺ｫ蜷医ｏ縺帙※隱ｿ謨ｴ
		character->setY(data->y() - character->getHeight());
	}
}

// 繝��繧ｿ邂｡逅�ｼ壹さ繝ｳ繝医Ο繝ｼ繝ｩ1蛟九�諠�ｱ繧剃ｸ也阜縺ｫ蜿肴丐
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Action繧剃ｽ懈�
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brain繧剃ｽ懈�
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controller繧剃ｽ懈�
	return createController(data->controllerName(), brain, action);
}

/*
*  謌ｦ繧上○繧
*/
void World::battle() {
	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	// 逕ｻ髱｢證苓ｻ｢荳ｭ 繧ｨ繝ｪ繧｢遘ｻ蜍輔°繝励Ξ繧､繝､繝ｼ繧�ｉ繧梧凾
	if (m_brightValue != 255 || playerDead()) {
		m_brightValue = max(0, m_brightValue - 10);
		if (!playerDead()) { return; }
	}

	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ隱ｿ縺ｹ縺滓凾縺ｮ繝�く繧ｹ繝
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// 莨夊ｩｱ邨ゆｺ
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

	// deleteFlag縺荊rue縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜑企勁縺吶ｋ縲
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 繧ｭ繝｣繝ｩ縺ｮ譖ｴ譁ｰ�域判謦�ｯｾ雎｡縺ｮ螟画峩�
	// 荳翫〒繧ｭ繝｣繝ｩ繧貞炎髯､縺励◆縺九ｉ譖ｴ譁ｰ縺励◆縺九ｉ蠢�ｦ
	updateCharacter();

	// 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蜍輔″
	controlCharacter();

	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医�蜍輔″
	controlObject();

	// 繧｢繧､繝�Β縺ｮ蜍輔″
	controlItem();

	// 繧ｫ繝｡繝ｩ縺ｮ譖ｴ譁ｰ
	adjustBattleCamera();

	// 繧ｵ繧ｦ繝ｳ繝峨�繝ｬ繧､繝､繝ｼ縺ｮ繝代Φ險ｭ螳夂畑
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
	updateAnimation();

	// 謫堺ｽ懊く繝｣繝ｩ螟画峩
	changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers, m_player_p));

}

void World::changePlayer(const Character* nextPlayer) {
	if (nextPlayer == nullptr) { return; }
	// 莉頑桃菴懊＠縺ｦ縺�ｋ繧ｭ繝｣繝ｩ繧誰PC縺ｮBrain縺ｫ縺吶ｋ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_player_p->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// 谺｡謫堺ｽ懊☆繧九く繝｣繝ｩ繧狸eyboardBrain縺ｫ縺吶ｋ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_playerChanger->setPrevBrainName(brainName);
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// 谺｡謫堺ｽ懊☆繧九く繝｣繝ｩ繧単layer縺ｨ縺励※繧ｻ繝�ヨ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (nextPlayer->getId() == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			m_playerId = m_player_p->getId();
			for (int j = 0; j < m_characterControllers.size(); j++) {
				if (m_characterControllers[j]->getBrain()->needSearchFollow()) {
					m_characterControllers[j]->searchFollowCandidate(m_player_p);
				}
			}
			break;
		}
	}
	m_focusId = m_player_p->getId();
}

//  Battle�壹き繝｡繝ｩ縺ｮ譖ｴ譁ｰ
void World::updateCamera(int gx, int gy, double gex) {

	// 繧ｫ繝｡繝ｩ繧呈昭繧峨☆
	m_camera->shaking();

	// 諡｡螟ｧ邇�そ繝�ヨ
	m_camera->setEx(gex);

	// 逶ｮ讓吩ｽ咲ｽｮ繧ｻ繝�ヨ
	m_camera->setGPoint(gx, gy);
	
	// 繧ｫ繝｡繝ｩ繧堤岼讓吩ｽ咲ｽｮ縺ｸ霑代▼縺代ｋ
	m_camera->move();
}

// Battle�壽姶髣倅ｸｭ縺ｮ繧ｫ繝｡繝ｩ謫堺ｽ
void World::adjustBattleCamera() {

	// 逶ｮ讓吝ｺｧ讓
	int gx = 0, gy = 0;
	// 繧ｭ繝｣繝ｩ縺ｨ繧ｫ繝｡繝ｩ縺ｮ霍晞屬縺ｮ譛螟ｧ蛟､繧定ｪｿ縺ｹ繧
	int max_dx = 0, max_dy = 0;
	// 画面内に入れようとする距離の最大　これより離れたキャラは無視
	const int MAX_DISABLE = 2000;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// 莉翫ヵ繧ｩ繝ｼ繧ｫ繧ｹ縺励※縺�ｋ繧ｭ繝｣繝ｩ縺ｮ蠎ｧ讓吶↓蜷医ｏ縺帙ｋ
		if (m_focusId == m_characters[i]->getId()) {
			gx = m_characters[i]->getCenterX();
			gy = m_characters[i]->getCenterY();
		}
		// 繝輔か繝ｼ繧ｫ繧ｹ縺励※縺�ｋ繧ｭ繝｣繝ｩ莉･螟悶↑繧芽ｷ晞屬繧定ｪｿ縺ｹ繧
		else if (m_characters[i]->getHp() > 0) {
			int x = m_characters[i]->getX();
			if (m_camera->getX() < x) { x += m_characters[i]->getWide(); }
			int dx = abs(m_camera->getX() - x);
			if (dx < MAX_DISABLE) {
				max_dx = max(max_dx, dx);
				max_dy = max(max_dy, abs(m_camera->getY() - m_characters[i]->getY()) + m_characters[i]->getHeight());
			}
		}
	}

	// 繧ｫ繝｡繝ｩ縺ｮ諡｡螟ｧ繝ｻ邵ｮ蟆
	// 螟ｧ縺阪￥螟画峩縺吶ｋ蠢�ｦ√′縺ゅｋ蝣ｴ蜷医⊇縺ｩ縲∝､ｧ縺阪￥諡｡螟ｧ邇�ｒ螟画峩縺吶ｋ縲
	double nowEx = m_camera->getEx();
	double gex = nowEx;
	int shift = controlLeftShift() + controlRightShift();
	if (shift > 0) {
		if (nowEx > m_cameraMinEx) {
			gex = max(nowEx - 0.01, 0.1);
		}
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		max_dx = (int)(max_dx / m_exX);
		max_dy = (int)(max_dy / m_exY);
		if (nowEx > m_cameraMinEx && (max_dx > nowWide || max_dy > nowHeight)) {
			// 邵ｮ蟆
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			gex = nowEx - min(0.1, d / 100000);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// 諡｡螟ｧ
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			gex = nowEx + min(0.005, d / 100000);
		}
	}

	// 繧ｫ繝｡繝ｩ縺ｮ譖ｴ譁ｰ
	updateCamera(gx, gy, gex);
}

//  Battle�壹い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
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

//  Battle�壹く繝｣繝ｩ縺ｮ譖ｴ譁ｰ�域判謦�ｯｾ雎｡縺ｮ螟画峩�
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain縺ｮ隕∬ｫ九〒謾ｻ謦�ｯｾ雎｡螟画峩
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) {
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

//  Battle�壹く繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蜍輔″
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP縺0縺ｪ繧峨せ繧ｭ繝��
		if (controller->getAction()->getCharacter()->getHp() == 0) { continue; }

		// 陦悟虚蜑阪�蜃ｦ逅
		controller->init();

		// 繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ蠖薙◆繧雁愛螳
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);
		atariCharacterAndObject(controller, m_stageObjects); // 2蝗樒岼蜻ｼ縺ｶ縺ｮ縺ｯ螯･蜊疲｡医1蝗樒岼縺ｧ譁憺擇縺ｫ縺�ｋ縺九′繧上°繧翫√◎繧後↓繧医▲縺ｦ蜃ｦ逅�′螟峨ｏ繧九◆繧2蝗樒岼縺悟ｿ�ｦ
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// 謫堺ｽ original縺ｮ繝上�繝医�繝輔Μ繝ｼ繧ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// 蟆�茶謾ｻ謦
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// 譁ｬ謦�判謦
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// 蜿肴丐 original縺ｮ繝上�繝医�繝輔Μ繝ｼ繧ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ雋ｫ騾壼愛螳
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle�壹が繝悶ず繧ｧ繧ｯ繝医�蜍輔″
void World::controlObject() {
	// 螢√ｄ蠎翫�蜍輔″
	actionObject(m_stageObjects);

	// 謾ｻ謦�ｽ薙◆繧雁愛螳壹�蜍輔″
	actionObject(m_attackObjects);

	// 螢√ｄ蠎<->謾ｻ謦��蠖薙◆繧雁愛螳
	atariStageAndAttack();

	// 謾ｻ謦<->謾ｻ謦��蠖薙◆繧雁愛螳
	atariAttackAndAttack();
}

// Battle�壹い繧､繝�Β縺ｮ蜍輔″
void World::controlItem() {
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// 蜿門ｾ玲ｸ医∩
		if (m_itemVector[i]->getDeleteFlag()) {
			// 蜉ｹ譫憺浹縺悟�逕滉ｸｭ縺ｧ縺ｪ縺�↑繧牙炎髯､
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// 蛻晄悄蛹
		m_itemVector[i]->init();
		// 螢∝ｺ翫→縺ｮ蠖薙◆繧雁愛螳
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, m_itemVector[i]->getVx(), m_itemVector[i]->getVy())) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
		}
		// 繧ｭ繝｣繝ｩ縺ｨ縺ｮ蠖薙◆繧雁愛螳
		if (m_itemVector[i]->atariCharacter(m_player_p)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
		}
		// 蜍輔″
		m_itemVector[i]->action();
	}
}

//  Battle�壹く繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医�蠖薙◆繧雁愛螳
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// 螢√ｄ蠎翫が繝悶ず繧ｧ繧ｯ繝医�蜃ｦ逅 (蠖薙◆繧雁愛螳壹→蜍輔″)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 蠖薙◆繧雁愛螳壹ｒ縺薙％縺ｧ陦後≧
		if (objects[i]->atari(controller)) {
			const Character* character = controller->getAction()->getCharacter();
			// 繧ｨ繝輔ぉ繧ｯ繝井ｽ懈�
			int x = character->getCenterX();
			int y = character->getCenterY();
			m_animations.push_back(objects[i]->createAnimation(x, y, 3));
			// 蜉ｹ譫憺浹
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			// HP = 0縺ｫ縺ｪ縺｣縺溘→縺搾ｼ医ｄ繧峨ｌ縺溘→縺搾ｼ
			if (character->getHp() == 0) {
				m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
				m_camera->shakingStart(20, 20);
				m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
				if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && GetRand(100) < 20) {
					// 繧ｹ繧ｭ繝ｫ逋ｺ蜍穂ｸｭ縺ｧ縺ｪ縺代ｌ縺ｰ遒ｺ邇�〒繧｢繧､繝�Β縺瑚誠縺｡繧
					m_itemVector.push_back(new CureItem("cure", x, y, 50));
				}
			}
		}
		// deleteFlag縺荊rue縺ｪ繧牙炎髯､縺吶ｋ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 譛ｫ蟆ｾ繧貞炎髯､縺吶ｋ譁ｹ縺碁溘＞
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

//  Battle�壹く繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ謇峨が繝悶ず繧ｧ繧ｯ繝医�蠖薙◆繧雁愛螳
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// 繧ｹ繧ｭ繝ｫ逋ｺ蜍穂ｸｭ縺ｯ謇峨↓蜈･繧後↑縺
	if (m_skillFlag) { return; }

	// 螢√ｄ蠎翫が繝悶ず繧ｧ繧ｯ繝医�蜃ｦ逅 (蠖薙◆繧雁愛螳壹→蜍輔″)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// 蠖薙◆繧雁愛螳壹ｒ縺薙％縺ｧ陦後≧
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// 繝峨い縺倥ｃ縺ｪ縺
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// 蠖薙◆縺｣縺溷ｴ蜷 繧ｨ繝ｪ繧｢遘ｻ蜍輔′逋ｺ逕
				m_nextAreaNum = objects[i]->getAreaNum();
				// 逕ｻ髱｢繧呈囓霆｢
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle�壼｣√ｄ蠎<->謾ｻ謦��蠖薙◆繧雁愛螳
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 謾ｻ謦�′螢∝ｺ翫↓蠖薙◆縺｣縺ｦ縺�ｋ縺句愛螳
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// 蠖薙◆縺｣縺溷ｴ蜷 繧ｨ繝輔ぉ繧ｯ繝井ｽ懈�
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[i]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// 螢∝ｺ翫�deleteFlag縺荊rue縺ｪ繧牙炎髯､縺吶ｋ
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 謾ｻ謦��deleteFlag縺荊rue縺ｪ繧牙炎髯､縺吶ｋ
		if (m_attackObjects[i]->getDeleteFlag()) {
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle�壽判謦<->謾ｻ謦��蠖薙◆繧雁愛螳
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// 謾ｻ謦�′螢∝ｺ翫↓蠖薙◆縺｣縺ｦ縺�ｋ縺句愛螳
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// 蠖薙◆縺｣縺溷ｴ蜷 繧ｨ繝輔ぉ繧ｯ繝井ｽ懈�
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

// 蜷�く繝｣繝ｩ縺檎岼讓吝慍轤ｹ縺ｸ遘ｻ蜍輔☆繧九□縺 蜈ｨ蜩｡蛻ｰ驕斐＠縺溘ｉtrue繧定ｿ斐☆
bool World::moveGoalCharacter() {
	// deleteFlag縺荊rue縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜑企勁縺吶ｋ縲
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 繧ｭ繝｣繝ｩ縺ｮ譖ｴ譁ｰ�域判謦�ｯｾ雎｡縺ｮ螟画峩�
	// 荳翫〒繧ｭ繝｣繝ｩ繧貞炎髯､縺励◆縺九ｉ譖ｴ譁ｰ縺励◆縺九ｉ蠢�ｦ
	updateCharacter();

	// 繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蜍輔″
	bool allCharacterAlreadyGoal = true;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP縺0縺ｪ繧峨せ繧ｭ繝��
		if (controller->getAction()->getCharacter()->getHp() == 0) { continue; }

		// 陦悟虚蜑阪�蜃ｦ逅
		controller->init();

		// 繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ蠖薙◆繧雁愛螳
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);
		atariCharacterAndObject(controller, m_stageObjects); // 2蝗樒岼蜻ｼ縺ｶ縺ｮ縺ｯ螯･蜊疲｡医1蝗樒岼縺ｧ譁憺擇縺ｫ縺�ｋ縺九′繧上°繧翫√◎繧後↓繧医▲縺ｦ蜃ｦ逅�′螟峨ｏ繧九◆繧2蝗樒岼縺悟ｿ�ｦ

		// 逶ｮ讓吝慍轤ｹ縺ｸ遘ｻ蜍輔☆繧区桃菴 original縺ｮ繝上�繝医�繝輔Μ繝ｼ繧ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}

		// 蜿肴丐 original縺ｮ繝上�繝医�繝輔Μ繝ｼ繧ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ雋ｫ騾壼愛螳
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// 繧ｪ繝悶ず繧ｧ繧ｯ繝医�蜍輔″
	controlObject();

	// 繧｢繧､繝�Β縺ｮ蜍輔″
	controlItem();

	// 繧ｫ繝｡繝ｩ縺ｮ譖ｴ譁ｰ
	adjustBattleCamera();

	// 繧ｵ繧ｦ繝ｳ繝峨�繝ｬ繧､繝､繝ｼ縺ｮ繝代Φ險ｭ螳夂畑
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ譖ｴ譁ｰ
	updateAnimation();

	return allCharacterAlreadyGoal;
}

// 莨夊ｩｱ縺輔○繧
void World::talk() {
	if (m_conversation_p != nullptr) {
		m_conversation_p->play();
		// 莨夊ｩｱ邨ゆｺ
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// 繝繝ｼ繝薙�繧呈ｵ√☆
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// 繝繝ｼ繝薙�邨ゆｺ
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}