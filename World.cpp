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
* 隰ｫ蝣ｺ�ｽ諛翫￥郢晢ｽ｣郢晢ｽｩ陞溽判蟲ｩ陷�ｽｦ騾
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
* controllers_p邵ｺ荵晢ｽ英layer_p邵ｺ�ｮ闔会ｽｲ鬮｢阮呻ｽ定ｿ夲ｽｹ陞ｳ螢ｹ�邵ｲ竏ｵ�ｬ�｡邵ｺ�ｮ隰ｫ蝣ｺ�ｽ諛翫￥郢晢ｽ｣郢晢ｽｩ郢ｧ螳夲ｽｿ譁絶�邵ｲ
* 陞溽判蟲ｩ陷亥現�ｽ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｯID邵ｺ�ｧ雎趣ｽｺ邵ｺ�ｾ郢ｧ荵
* 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ郢ｧ�ｻ郢晢ｿｽ繝ｨ郢晢ｽｻplayer邵ｺ�ｮBrain陞溽判蟲ｩ邵ｲ窶ｦontrollers_p邵ｺ�ｮBrain陞溽判蟲ｩ邵ｺ�ｧ陞ｳ蠕｡�ｺ
*/
const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p, const Character* player_p) {
	// E郢ｧ�ｭ郢晢ｽｼ邵ｺ譴ｧ谺ｾ邵ｺ霈費ｽ檎ｸｺ�ｦ邵ｺ�ｽ竊醍ｸｺ�ｽ竊醍ｹｧ謌托ｽｽ霈費ｽらｸｺ蜉ｱ竊醍ｸｺ
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		// 陞ｻ讒ｭ笘�ｹｧ荵昴￡郢晢ｽｫ郢晢ｽｼ郢
		int groupId = target->getGroupId();
		// ID
		int id = target->getId();
		if (id == player_p->getId()) { continue; }
		// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ闔会ｽｲ鬮｢阮呻ｿｽ邵ｺ�ｿ陝�ｽｾ髮趣ｽ｡
		if (groupId == player_p->getGroupId()) {
			// ID邵ｺ譴ｧ諤呵氣荳奇ｿｽ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			// ID邵ｺ蠕鯉ｿｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ隹ｺ�｡邵ｺ�ｫ陝�ｸ奇ｼ�ｸｺ�ｽ縺冗ｹ晢ｽ｣郢晢ｽｩ
			if (id > player_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ譴ｧ諤咏ｹｧ�ｽD邵ｺ�ｮ陞滂ｽｧ邵ｺ髦ｪ�樒ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ邵ｺ�｣邵ｺ貅ｷ�ｴ陷ｷ�ｽullptr邵ｺ�ｪ邵ｺ�ｮ邵ｺ�ｧID邵ｺ譴ｧ諤咏ｹｧ繧�ｽｰ荳奇ｼ�ｸｺ�ｽ縺冗ｹ晢ｽ｣郢晢ｽｩ郢ｧ蟶昶�邵ｺ�ｶ
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}


// vector邵ｺ�ｫ陷茨ｽ･邵ｺ�｣邵ｺ貅ｷ�ｽ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恆莨∝求邵ｺ蜷ｶ�
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector邵ｺ�ｫ陷茨ｽ･邵ｺ�｣邵ｺ豁ｸeleteFlag邵ｺ闕蛎rue邵ｺ�ｮ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恆莨∝求邵ｺ蜷ｶ�
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag邵ｺ闕較ue邵ｺ�ｪ郢ｧ迚咏ｎ鬮ｯ�､邵ｺ蜷ｶ�
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 隴幢ｽｫ陝�ｽｾ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�玖ｭ�ｽｹ邵ｺ遒貅假ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector邵ｺ�ｫ陷茨ｽ･邵ｺ�｣邵ｺ貅ｷ�ｽ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恪霈板ｰ邵ｺ
void actionObject(vector<Object*>& objects) {
	// 陞｢竏夲ｽ�守ｿｫ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷�ｽｦ騾 (陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ竊定恪霈披ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷崎ｼ披ｳ
		objects[i]->action();
		// deleteFlag邵ｺ闕較ue邵ｺ�ｪ郢ｧ迚咏ｎ鬮ｯ�､邵ｺ蜷ｶ�
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 隴幢ｽｫ陝�ｽｾ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�玖ｭ�ｽｹ邵ｺ遒貅假ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｨ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ髮具ｽｫ鬨ｾ螢ｽ蜃ｾ陋ｻ�､陞ｳ
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 陞｢竏夲ｽ�守ｿｫ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷�ｽｦ騾 (陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ竊定恪霈披ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ�堤ｸｺ阮呻ｼ�ｸｺ�ｧ髯ｦ蠕娯鴬
		objects[i]->penetration(controller);
	}
}


/*
* 郢ｧ�ｳ郢晢ｽｳ郢ｧ�ｹ郢晏現ﾎ帷ｹｧ�ｯ郢ｧ�ｿ
*/
World::World() {
	m_duplicationFlag = false;

	m_brightValue = 255;

	m_resetBgmFlag = false;

	m_blindFlag = false;

	// 会話イベント
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// 郢郢晢ｽｼ郢晁侭�ｽ
	m_movie_p = nullptr;

	// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷咲ｩゑｽｸ�ｭ
	m_skillFlag = false;

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ陋溷調邏ｫ邵ｺ�ｮ隴陞滂ｽｧ郢晢ｽｻ隴陝�ｸ�､郢ｧ螳夲ｽｧ�｣陷剃ｸ橸ｽｺ�ｦ邵ｺ荵晢ｽ芽ｱ趣ｽｺ陞ｳ
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;

	m_date = 0;

}

/*
* 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ郢晢ｽｭ郢晢ｽｼ郢晏ｳｨ竊醍ｸｺ�ｩ
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// 郢ｧ�ｵ郢ｧ�ｦ郢晢ｽｳ郢晏ｳｨ�ｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ
	m_soundPlayer_p = soundPlayer;

	// 闕ｳ�ｻ闔�ｺ陷茨ｽｬ邵ｺ�ｮ郢ｧ�ｹ郢ｧ�ｿ郢晢ｽｼ郢昜ｺ･諷崎ｽ､�ｹ
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// 郢ｧ�ｨ郢晢ｽｪ郢ｧ�｢郢ｧ蛛ｵﾎ溽ｹ晢ｽｼ郢
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

	// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ郢ｧ蛛ｵ縺晉ｹ晢ｿｽ繝ｨ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ隴�ｽｹ陷ｷ莉｣竏郁惺莉｣ﾂｰ邵ｺ蟶呻ｽ
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

	// 郢ｧ�ｨ郢晢ｽｪ郢ｧ�｢郢ｧ蛛ｵ縺慕ｹ晄鱒�ｽ
	m_camera = new Camera(original->getCamera());
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_doorSound = original->getDoorSound();
	m_date = original->getDate();

	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ蛛ｵ縺慕ｹ晄鱒�ｽ
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player_p = copy; }
	}
	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);
	// 郢ｧ�ｳ郢晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ郢ｧ蛛ｵ縺慕ｹ晄鱒�ｽ
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// Brain邵ｺ�ｨAction郢ｧ�ｳ郢晄鱒�ｽ騾包ｽｨ邵ｺ�ｫCharacter邵ｺ�ｨ郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢ｧ蜻茨ｽｸ�｡邵ｺ
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
	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�
	delete m_camera;

	// 陷茨ｽｨ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恆莨∝求邵ｺ蜷ｶ�狗ｸｲ
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// 隰ｾ�ｻ隰ｦ�ｽ縺顔ｹ晁ｼ斐♂郢ｧ�ｯ郢昜ｺ･轤朱ｫｯ�､
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// 郢ｧ�｢郢ｧ�､郢晢ｿｽﾎ定恆莨∝求
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// 陷茨ｽｨ郢ｧ�ｳ郢晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�狗ｸｲ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 陷茨ｽｨ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�狗ｸｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// 髢ｭ譴ｧ蜍ｹ
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

// Drawer騾包ｽｨ�ｽ蜥ｾharacterAction邵ｺ�ｮvector郢ｧ螳夲ｽｿ譁絶�
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

// Drawer騾包ｽｨ�ｽ蜚ｹbject邵ｺ�ｮvector郢ｧ螳夲ｽｿ譁絶�
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer騾包ｽｨ�ｽ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ郢ｧ蛹ｻ�願募ｾ鯉ｽ咲ｸｺ�ｫ隰蜀怜愛邵ｺ蜷ｶ�軌bject邵ｺ�ｮvector郢ｧ螳夲ｽｿ譁絶�
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer騾包ｽｨ�ｽ蜩Ｏimation邵ｺ�ｮvector郢ｧ螳夲ｽｿ譁絶�
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// 郢ｧ�ｨ郢晁ｼ斐♂郢ｧ�ｯ郢
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// 郢ｧ�｢郢ｧ�､郢晢ｿｽﾎ
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			allAnimations.push_back(m_itemVector[i]->getAnimation());
		}
	}

	return allAnimations;
}

// 陷ｷ讎顔√邵ｺ�ｧ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ隶諛�ｽｴ�｢
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// 陷ｷ讎顔√邵ｺ�ｧ郢ｧ�ｳ郢晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ隶諛�ｽｴ�｢
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// ID邵ｺ�ｧ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ隶諛�ｽｴ�｢
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID隰厄ｿｽ�ｮ螢ｹ縲達rain陞溽判蟲ｩ
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// 郢ｧ�ｹ郢晏現�ｽ郢晢ｽｪ郢晢ｽｼ邵ｺ�ｫ郢ｧ蛹ｻ�矩恆�ｽ陷郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// 郢ｧ�ｳ郢晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// 繧ｹ繝医�繝ｪ繝ｼ縺ｫ繧医ｋ繧ｭ繝｣繝ｩ縺ｮ諤ｧ閭ｽ螟牙喧
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 陞｢竏夲ｽ�
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// 郢晏ｳｨ縺
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮHP邵ｺ0邵ｺ�ｪ郢ｧ豸ｲrue
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮHP郢ｧ讓ｽAX邵ｺ�ｫ邵ｺ蜷ｶ�
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
}


// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螢ｹ繝ｯ郢晢ｽｼ郢晏現�巽reeze邵ｺ�ｫ邵ｺ蜷ｶ�
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "郢昜ｸ奇ｿｽ郢") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螟奇ｽ､�ｽ�｣�ｽ邵ｺ�ｮ郢昜ｸ奇ｿｽ郢晞メ�ｿ�ｽ陷騾包ｽｨ
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螟奇ｽ､�ｽ�｣�ｽ邵ｺ�ｮ郢昜ｸ奇ｿｽ郢昜ｺ･轤朱ｫｯ�､騾包ｽｨ
void World::popCharacterController(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "郢昜ｸ奇ｿｽ郢") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	// Target邵ｺ�ｫ邵ｺ蜉ｱ窶ｻ邵ｺ�ｽ�帰I邵ｺ�ｯ陞滓じ�邵ｺ�ｦ郢ｧ繧�ｽ臥ｸｺ�ｽ笳�ｸｺ�ｽ�ｽ邵ｺ�ｧHP=0邵ｺ�ｫ邵ｺ蜷ｶ�狗ｸｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "郢昜ｸ奇ｿｽ郢") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螢ｹﾎ樒ｹｧ�ｳ郢晢ｽｼ郢郢ｧ蜑�ｽｽ諛茨ｿｽ邵ｺ蠍ｺ�ｽ�ｿ騾包ｽｨ郢ｧ蟶晏ｹ戊沂
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "郢昜ｸ奇ｿｽ郢") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螢ｹﾎ樒ｹｧ�ｳ郢晢ｽｼ郢邵ｺ�ｮ隴弱ｋ菫｣郢ｧ蜻域呵崕譏ｴ竊楢ｬ鯉ｽｻ邵ｺ
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "郢昜ｸ奇ｿｽ郢") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷榊桁�ｼ螢ｹﾎ樒ｹｧ�ｳ郢晢ｽｼ郢邵ｺ�ｮ闖ｴ�ｿ騾包ｽｨ郢ｧ蛛ｵ��ｹｧ竏壺ｻ陷台ｼ∝求邵ｺ蜷ｶ�
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "郢昜ｸ奇ｿｽ郢") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ邵ｺ�ｮ霑･�ｶ隲ｷ荵晢ｽ定棔逕ｻ蟲ｩ邵ｺ蜷ｶ� 邵ｺ�ｽ竊醍ｸｺ�ｽ竊醍ｹｧ謌托ｽｽ諛茨ｿｽ邵ｺ蜷ｶ�
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｮ髫ｪ�ｭ陞ｳ
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ蜻育悛髫穂ｸ茨ｽｽ諛茨ｿｽ邵ｺ蜷ｶ�玖撻�ｴ陷ｷ闌ｨ�ｼ蛹ｻ��ｸｺ�ｮ郢ｧ�ｨ郢晢ｽｪ郢ｧ�｢邵ｺ�ｫ邵ｺ�ｽ�狗ｸｺ�ｯ邵ｺ螢ｹ�ｽ郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ邵ｺ蠕娯穐邵ｺ邵ｺ�ｽ竊醍ｸｺ�ｽ�ｼ
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "郢昜ｸ奇ｿｽ郢")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// 郢ｧ�ｳ郢晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ邵ｲ竏壹＞郢ｧ�ｯ郢ｧ�ｷ郢晢ｽｧ郢晢ｽｳ邵ｲ縲Ｓain邵ｺ�ｮ髫ｪ�ｭ陞ｳ
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

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ邵ｺ�ｮ霑･�ｶ隲ｷ荵晢ｽ定ｬｨ蜷ｶ竏ｴ郢ｧ
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
				data->setY(c->getY() + c->getHeight()); // Y2陟趣ｽｧ隶灘生�定将譎擾ｽｭ 郢晢ｽｭ郢晢ｽｼ郢晉判蜃ｾ邵ｺ�ｯ髴�ｽｫ鬮滂ｽｷ邵ｺ�ｧ髯ｬ諛茨ｽｭ�｣
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

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ蜥ｼoor邵ｺ�ｮ霑･�ｶ隲ｷ荵晢ｽ定棔逕ｻ蟲ｩ邵ｺ蜷ｶ� 邵ｺ�ｽ竊醍ｸｺ�ｽ竊醍ｹｧ謌托ｽｽ諛茨ｿｽ邵ｺ蜷ｶ�
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

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ蜥ｼoor邵ｺ�ｮ霑･�ｶ隲ｷ荵晢ｽ定ｬｨ蜷ｶ竏ｴ郢ｧ
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// 郢晏ｳｨ縺�ｸｺ蛟･��ｸｺ�ｪ邵ｺ
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
		// 郢ｧ�ｻ郢晢ｽｼ郢晄じ繝ｧ郢晢ｽｼ郢ｧ�ｿ邵ｺ�ｫ郢晏ｳｨ縺�ｸｺ謔滂ｽｭ莨懈Β邵ｺ蜷ｶ�狗ｸｺ
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
		if (!flag) { // 隴�ｽｰ邵ｺ貅倪�郢晏ｳｨ縺�ｸｺ�ｪ郢ｧ蟲ｨ縺晉ｹ晢ｽｼ郢晄じ繝ｧ郢晢ｽｼ郢ｧ�ｿ邵ｺ�ｫ髴托ｽｽ陷
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ�ｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｨ邵ｺ譏ｴ�ｽ闔会ｽｲ鬮｢阮呻ｽ堤ｹ晏ｳｨ縺�ｸｺ�ｮ陷鷹亂竊馴§�ｻ陷
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// 闔会ｽｲ鬮｢阮呻ｽる§�ｻ陷
	setPlayerFollowerPoint();

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢晢ｽｪ郢ｧ�ｻ郢晢ｿｽ繝ｨ
	cameraPointInit();
}

// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ郢ｧ蝣､髻ｳ陞ｳ螢ｹ�ｽ陟趣ｽｧ隶灘生竏磯§�ｻ陷
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢晢ｽｪ郢ｧ�ｻ郢晢ｿｽ繝ｨ
	cameraPointInit();
}

// 闔会ｽｲ鬮｢阮呻ｽ堤ｹ晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ闖ｴ蜥ｲ�ｽ�ｮ邵ｺ�ｸ驕假ｽｻ陷
void World::setPlayerFollowerPoint() {
	// 郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ闔会ｽｲ鬮｢
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// 髴托ｽｽ髴搾ｽ｡陝�ｽｾ髮趣ｽ｡邵ｺ蠕鯉ｿｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｪ郢ｧ
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controller邵ｺ�ｫ陝�ｽｾ陟｢諛岩�郢ｧ豌揺aracter邵ｺ�ｫ陞溽判蟲ｩ郢ｧ雋槫�邵ｺ蛹ｻ�
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

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ縺咲ｹ晢ｽ｡郢晢ｽｩ邵ｺ�ｮ闖ｴ蜥ｲ�ｽ�ｮ郢ｧ蛛ｵﾎ懃ｹｧ�ｻ郢晢ｿｽ繝ｨ
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ1闖ｴ阮呻ｿｽ隲�ｽ�ｱ郢ｧ蜑�ｽｸ荵滄�邵ｺ�ｫ陷ｿ閧ｴ荳
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// 邵ｺ阮呻ｿｽ郢ｧ�ｲ郢晢ｽｼ郢邵ｺ�ｧ陋ｻ譎牙元陜｣�ｴ邵ｺ蛟･��ｸｺ�ｪ邵ｺ
		character->setHp(data->hp());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y陟趣ｽｧ隶灘生�ｽ髴�ｽｫ鬮滂ｽｷ邵ｺ�ｫ陷ｷ蛹ｻ�冗ｸｺ蟶吮ｻ髫ｱ�ｿ隰ｨ�ｴ
		character->setY(data->y() - character->getHeight());
	}
}

// 郢晢ｿｽ�ｽ郢ｧ�ｿ驍ゑｽ｡騾�ｿｽ�ｼ螢ｹ縺慕ｹ晢ｽｳ郢晏現ﾎ溽ｹ晢ｽｼ郢晢ｽｩ1陋滉ｹ晢ｿｽ隲�ｽ�ｱ郢ｧ蜑�ｽｸ荵滄�邵ｺ�ｫ陷ｿ閧ｴ荳
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Action郢ｧ蜑�ｽｽ諛茨ｿｽ
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brain郢ｧ蜑�ｽｽ諛茨ｿｽ
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controller郢ｧ蜑�ｽｽ諛茨ｿｽ
	return createController(data->controllerName(), brain, action);
}

/*
*  隰鯉ｽｦ郢ｧ荳岩雷郢ｧ
*/
void World::battle() {
	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	
	// 画面暗転処理
	if (dealBrightValue()) { return; }

	// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�帝坡�ｿ邵ｺ�ｹ邵ｺ貊灘�邵ｺ�ｮ郢晢ｿｽ縺冗ｹｧ�ｹ郢
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// 闔ｨ螟奇ｽｩ�ｱ驍ｨ繧�ｽｺ
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

	// deleteFlag邵ｺ闕較ue邵ｺ�ｮ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恆莨∝求邵ｺ蜷ｶ�狗ｸｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ�ｽ蝓溷愛隰ｦ�ｽ�ｯ�ｾ髮趣ｽ｡邵ｺ�ｮ陞溽判蟲ｩ�ｽ
	// 闕ｳ鄙ｫ縲堤ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜉ｱ笳�ｸｺ荵晢ｽ芽ｭ厄ｽｴ隴�ｽｰ邵ｺ蜉ｱ笳�ｸｺ荵晢ｽ芽｢�ｽ�ｦ
	updateCharacter();

	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｮ陷崎ｼ披ｳ
	controlCharacter();

	// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷崎ｼ披ｳ
	controlObject();

	// 郢ｧ�｢郢ｧ�､郢晢ｿｽﾎ堤ｸｺ�ｮ陷崎ｼ披ｳ
	controlItem();

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
	adjustBattleCamera();

	// 郢ｧ�ｵ郢ｧ�ｦ郢晢ｽｳ郢晏ｳｨ�ｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ郢昜ｻ｣ﾎｦ髫ｪ�ｭ陞ｳ螟ら舞
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 郢ｧ�｢郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ�ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
	updateAnimation();

	// 隰ｫ蝣ｺ�ｽ諛翫￥郢晢ｽ｣郢晢ｽｩ陞溽判蟲ｩ
	changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers, m_player_p));

}

void World::changePlayer(const Character* nextPlayer) {
	if (nextPlayer == nullptr) { return; }
	// 闔蛾第｡�抄諛奇ｼ邵ｺ�ｦ邵ｺ�ｽ�狗ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ隱ｰPC邵ｺ�ｮBrain邵ｺ�ｫ邵ｺ蜷ｶ�
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_player_p->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// 隹ｺ�｡隰ｫ蝣ｺ�ｽ諛岩�郢ｧ荵昴￥郢晢ｽ｣郢晢ｽｩ郢ｧ迢ｸeyboardBrain邵ｺ�ｫ邵ｺ蜷ｶ�
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_playerChanger->setPrevBrainName(brainName);
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// 隹ｺ�｡隰ｫ蝣ｺ�ｽ諛岩�郢ｧ荵昴￥郢晢ｽ｣郢晢ｽｩ郢ｧ蜊詫ayer邵ｺ�ｨ邵ｺ蜉ｱ窶ｻ郢ｧ�ｻ郢晢ｿｽ繝ｨ
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

//  Battle�ｽ螢ｹ縺咲ｹ晢ｽ｡郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
void World::updateCamera(int gx, int gy, double gex) {

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢ｧ蜻域亊郢ｧ蟲ｨ笘
	m_camera->shaking();

	// 隲｡�｡陞滂ｽｧ驍�ｿｽ縺晉ｹ晢ｿｽ繝ｨ
	m_camera->setEx(gex);

	// 騾ｶ�ｮ隶灘姓�ｽ蜥ｲ�ｽ�ｮ郢ｧ�ｻ郢晢ｿｽ繝ｨ
	m_camera->setGPoint(gx, gy);
	
	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ郢ｧ蝣､蟯ｼ隶灘姓�ｽ蜥ｲ�ｽ�ｮ邵ｺ�ｸ髴台ｻ｣笆ｼ邵ｺ莉｣�
	m_camera->move();
}

// Battle�ｽ螢ｽ蟋ｶ鬮｣蛟�ｽｸ�ｭ邵ｺ�ｮ郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ隰ｫ蝣ｺ�ｽ
void World::adjustBattleCamera() {

	// 騾ｶ�ｮ隶灘雀�ｺ�ｧ隶
	int gx = 0, gy = 0;
	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｨ郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ髴肴辨螻ｬ邵ｺ�ｮ隴陞滂ｽｧ陋滂ｽ､郢ｧ螳夲ｽｪ�ｿ邵ｺ�ｹ郢ｧ
	int max_dx = 0, max_dy = 0;
	// 逕ｻ髱｢蜀�↓蜈･繧後ｈ縺�→縺吶ｋ霍晞屬縺ｮ譛螟ｧ縲縺薙ｌ繧医ｊ髮｢繧後◆繧ｭ繝｣繝ｩ縺ｯ辟｡隕
	const int MAX_DISABLE = 2000;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// 闔臥ｿｫ繝ｵ郢ｧ�ｩ郢晢ｽｼ郢ｧ�ｫ郢ｧ�ｹ邵ｺ蜉ｱ窶ｻ邵ｺ�ｽ�狗ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｮ陟趣ｽｧ隶灘生竊楢惺蛹ｻ�冗ｸｺ蟶呻ｽ
		if (m_focusId == m_characters[i]->getId()) {
			gx = m_characters[i]->getCenterX();
			gy = m_characters[i]->getCenterY();
		}
		// 郢晁ｼ斐°郢晢ｽｼ郢ｧ�ｫ郢ｧ�ｹ邵ｺ蜉ｱ窶ｻ邵ｺ�ｽ�狗ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ闔会ｽ･陞滓じ竊醍ｹｧ闃ｽ�ｷ譎槫ｱｬ郢ｧ螳夲ｽｪ�ｿ邵ｺ�ｹ郢ｧ
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

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ隲｡�｡陞滂ｽｧ郢晢ｽｻ驍ｵ�ｮ陝
	// 陞滂ｽｧ邵ｺ髦ｪ�･陞溽判蟲ｩ邵ｺ蜷ｶ�玖｢�ｽ�ｦ竏壺ｲ邵ｺ繧�ｽ玖撻�ｴ陷ｷ蛹ｻ竓�ｸｺ�ｩ邵ｲ竏晢ｽ､�ｧ邵ｺ髦ｪ�･隲｡�｡陞滂ｽｧ驍�ｿｽ�定棔逕ｻ蟲ｩ邵ｺ蜷ｶ�狗ｸｲ
	double nowEx = m_camera->getEx();
	double gex = nowEx;
	int shift = controlLeftShift() + controlRightShift();
	if (shift > 0) {
		if (nowEx > m_cameraMinEx) {
			m_camera->setEx(max(nowEx - 0.01 * m_exX, 0.1));
		}
	}
	else {
		int nowWide = (int)(GAME_WIDE / 2 / nowEx);
		int nowHeight = (int)(GAME_HEIGHT / 2 / nowEx);
		if (nowEx > m_cameraMinEx && (max_dx > nowWide || max_dy > nowHeight)) {
			// 驍ｵ�ｮ陝
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.1, d / 100000) * m_exX);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// 隲｡�｡陞滂ｽｧ
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.005, d / 100000) * m_exX);
		}
	}

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
	updateCamera(gx, gy, gex);
}

//  Battle�ｽ螢ｹ縺�ｹ昜ｹ斟鍋ｹ晢ｽｼ郢ｧ�ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
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

//  Battle�ｽ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ�ｽ蝓溷愛隰ｦ�ｽ�ｯ�ｾ髮趣ｽ｡邵ｺ�ｮ陞溽判蟲ｩ�ｽ
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain邵ｺ�ｮ髫補握�ｫ荵昴定ｬｾ�ｻ隰ｦ�ｽ�ｯ�ｾ髮趣ｽ｡陞溽判蟲ｩ
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) {
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

//  Battle�ｽ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｮ陷崎ｼ披ｳ
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP邵ｺ0邵ｺ�ｪ郢ｧ蟲ｨ縺帷ｹｧ�ｭ郢晢ｿｽ�ｽ
		if (controller->getAction()->getCharacter()->getHp() == 0) { continue; }

		// 髯ｦ謔溯劒陷鷹亂�ｽ陷�ｽｦ騾
		controller->init();

		// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現竊堤ｸｺ�ｮ陟冶侭笳�ｹｧ髮∵�陞ｳ
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);
		atariCharacterAndObject(controller, m_stageObjects); // 2陜玲ｨ貞ｲｼ陷ｻ�ｼ邵ｺ�ｶ邵ｺ�ｮ邵ｺ�ｯ陞ｯ�･陷顔夢�｡蛹ｻ1陜玲ｨ貞ｲｼ邵ｺ�ｧ隴∵�謫�ｸｺ�ｫ邵ｺ�ｽ�狗ｸｺ荵昶ｲ郢ｧ荳環ｰ郢ｧ鄙ｫ竏壺落郢ｧ蠕娯�郢ｧ蛹ｻ笆ｲ邵ｺ�ｦ陷�ｽｦ騾�ｿｽ窶ｲ陞溷ｳｨ�冗ｹｧ荵昶螺郢ｧ2陜玲ｨ貞ｲｼ邵ｺ謔滂ｽｿ�ｽ�ｦ
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// 隰ｫ蝣ｺ�ｽ original邵ｺ�ｮ郢昜ｸ奇ｿｽ郢晏現�ｽ郢晁ｼ釆懃ｹ晢ｽｼ郢ｧ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// 陝�ｿｽ闌ｶ隰ｾ�ｻ隰ｦ
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// 隴�ｽｬ隰ｦ�ｽ蛻､隰ｦ
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// 陷ｿ閧ｴ荳 original邵ｺ�ｮ郢昜ｸ奇ｿｽ郢晏現�ｽ郢晁ｼ釆懃ｹ晢ｽｼ郢ｧ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現竊堤ｸｺ�ｮ髮具ｽｫ鬨ｾ螢ｼ諢幄楜
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle�ｽ螢ｹ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷崎ｼ披ｳ
void World::controlObject() {
	// 陞｢竏夲ｽ�守ｿｫ�ｽ陷崎ｼ披ｳ
	actionObject(m_stageObjects);

	// 隰ｾ�ｻ隰ｦ�ｽ�ｽ阮吮螺郢ｧ髮∵�陞ｳ螢ｹ�ｽ陷崎ｼ披ｳ
	actionObject(m_attackObjects);

	// 陞｢竏夲ｽ�<->隰ｾ�ｻ隰ｦ�ｽ�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
	atariStageAndAttack();

	// 隰ｾ�ｻ隰ｦ<->隰ｾ�ｻ隰ｦ�ｽ�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
	atariAttackAndAttack();
}

// Battle�ｽ螢ｹ縺�ｹｧ�､郢晢ｿｽﾎ堤ｸｺ�ｮ陷崎ｼ披ｳ
void World::controlItem() {
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// 陷ｿ髢�ｾ邇ｲ�ｸ蛹ｻ竏ｩ
		if (m_itemVector[i]->getDeleteFlag()) {
			// 陷会ｽｹ隴ｫ諞ｺ豬ｹ邵ｺ謔滂ｿｽ騾墓ｻ会ｽｸ�ｭ邵ｺ�ｧ邵ｺ�ｪ邵ｺ�ｽ竊醍ｹｧ迚咏ｎ鬮ｯ�､
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// 陋ｻ譎�ｄ陋ｹ
		m_itemVector[i]->init();
		// 陞｢竏晢ｽｺ鄙ｫ竊堤ｸｺ�ｮ陟冶侭笳�ｹｧ髮∵�陞ｳ
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, m_itemVector[i]->getVx(), m_itemVector[i]->getVy())) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
		}
		// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｨ邵ｺ�ｮ陟冶侭笳�ｹｧ髮∵�陞ｳ
		if (m_itemVector[i]->atariCharacter(m_player_p)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
		}
		// 陷崎ｼ披ｳ
		m_itemVector[i]->action();
	}
}

//  Battle�ｽ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｨ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// 陞｢竏夲ｽ�守ｿｫ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷�ｽｦ騾 (陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ竊定恪霈披ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ�堤ｸｺ阮呻ｼ�ｸｺ�ｧ髯ｦ蠕娯鴬
		if (objects[i]->atari(controller)) {
			const Character* character = controller->getAction()->getCharacter();
			// 郢ｧ�ｨ郢晁ｼ斐♂郢ｧ�ｯ郢昜ｺ包ｽｽ諛茨ｿｽ
			int x = character->getCenterX();
			int y = character->getCenterY();
			m_animations.push_back(objects[i]->createAnimation(x, y, 3));
			// 陷会ｽｹ隴ｫ諞ｺ豬ｹ
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			// HP = 0邵ｺ�ｫ邵ｺ�ｪ邵ｺ�｣邵ｺ貅倪�邵ｺ謳ｾ�ｼ蛹ｻ��ｹｧ蟲ｨ�檎ｸｺ貅倪�邵ｺ謳ｾ�ｼ
			if (character->getHp() == 0) {
				m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
				m_camera->shakingStart(20, 20);
				m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
				if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && GetRand(100) < 20) {
					// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷咲ｩゑｽｸ�ｭ邵ｺ�ｧ邵ｺ�ｪ邵ｺ莉｣�檎ｸｺ�ｰ驕抵ｽｺ驍�ｿｽ縲堤ｹｧ�｢郢ｧ�､郢晢ｿｽﾎ堤ｸｺ迹夊ｪ邵ｺ�｡郢ｧ
					m_itemVector.push_back(new CureItem("cure", x, y, 50));
				}
			}
		}
		// deleteFlag邵ｺ闕較ue邵ｺ�ｪ郢ｧ迚咏ｎ鬮ｯ�､邵ｺ蜷ｶ�
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 隴幢ｽｫ陝�ｽｾ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜷ｶ�玖ｭ�ｽｹ邵ｺ遒貅假ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ螢ｹ縺冗ｹ晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｨ隰�ｳｨ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// 郢ｧ�ｹ郢ｧ�ｭ郢晢ｽｫ騾具ｽｺ陷咲ｩゑｽｸ�ｭ邵ｺ�ｯ隰�ｳｨ竊楢怦�･郢ｧ蠕娯�邵ｺ
	if (m_skillFlag) { return; }

	// 陞｢竏夲ｽ�守ｿｫ縺檎ｹ晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷�ｽｦ騾 (陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ竊定恪霈披ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// 陟冶侭笳�ｹｧ髮∵�陞ｳ螢ｹ�堤ｸｺ阮呻ｼ�ｸｺ�ｧ髯ｦ蠕娯鴬
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// 郢晏ｳｨ縺�ｸｺ蛟･��ｸｺ�ｪ邵ｺ
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// 陟冶侭笳�ｸｺ�｣邵ｺ貅ｷ�ｴ陷ｷ 郢ｧ�ｨ郢晢ｽｪ郢ｧ�｢驕假ｽｻ陷崎ｼ披ｲ騾具ｽｺ騾
				m_nextAreaNum = objects[i]->getAreaNum();
				// 騾包ｽｻ鬮ｱ�｢郢ｧ蜻亥專髴�ｽ｢
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle�ｽ螢ｼ�｣竏夲ｽ�<->隰ｾ�ｻ隰ｦ�ｽ�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 隰ｾ�ｻ隰ｦ�ｽ窶ｲ陞｢竏晢ｽｺ鄙ｫ竊楢冶侭笳�ｸｺ�｣邵ｺ�ｦ邵ｺ�ｽ�狗ｸｺ蜿･諢幄楜
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// 陟冶侭笳�ｸｺ�｣邵ｺ貅ｷ�ｴ陷ｷ 郢ｧ�ｨ郢晁ｼ斐♂郢ｧ�ｯ郢昜ｺ包ｽｽ諛茨ｿｽ
				int x = m_attackObjects[i]->getCenterX();
				int y = m_attackObjects[i]->getCenterY();
				m_animations.push_back(m_attackObjects[i]->createAnimation(x, y, 3));
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// 陞｢竏晢ｽｺ鄙ｫ�ｽdeleteFlag邵ｺ闕較ue邵ｺ�ｪ郢ｧ迚咏ｎ鬮ｯ�､邵ｺ蜷ｶ�
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 隰ｾ�ｻ隰ｦ�ｽ�ｽdeleteFlag邵ｺ闕較ue邵ｺ�ｪ郢ｧ迚咏ｎ鬮ｯ�､邵ｺ蜷ｶ�
		if (m_attackObjects[i]->getDeleteFlag()) {
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ螢ｽ蛻､隰ｦ<->隰ｾ�ｻ隰ｦ�ｽ�ｽ陟冶侭笳�ｹｧ髮∵�陞ｳ
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// 隰ｾ�ｻ隰ｦ�ｽ窶ｲ陞｢竏晢ｽｺ鄙ｫ竊楢冶侭笳�ｸｺ�｣邵ｺ�ｦ邵ｺ�ｽ�狗ｸｺ蜿･諢幄楜
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// 陟冶侭笳�ｸｺ�｣邵ｺ貅ｷ�ｴ陷ｷ 郢ｧ�ｨ郢晁ｼ斐♂郢ｧ�ｯ郢昜ｺ包ｽｽ諛茨ｿｽ
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

// 陷ｷ�ｽ縺冗ｹ晢ｽ｣郢晢ｽｩ邵ｺ讙主ｲｼ隶灘雀諷崎ｽ､�ｹ邵ｺ�ｸ驕假ｽｻ陷崎ｼ披�郢ｧ荵昶味邵ｺ 陷茨ｽｨ陷ｩ�｡陋ｻ�ｰ鬩墓鱒�邵ｺ貅假ｽ液rue郢ｧ螳夲ｽｿ譁絶�
bool World::moveGoalCharacter() {
	// deleteFlag邵ｺ闕較ue邵ｺ�ｮ郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�定恆莨∝求邵ｺ蜷ｶ�狗ｸｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ�ｽ蝓溷愛隰ｦ�ｽ�ｯ�ｾ髮趣ｽ｡邵ｺ�ｮ陞溽判蟲ｩ�ｽ
	// 闕ｳ鄙ｫ縲堤ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ雋樒ｎ鬮ｯ�､邵ｺ蜉ｱ笳�ｸｺ荵晢ｽ芽ｭ厄ｽｴ隴�ｽｰ邵ｺ蜉ｱ笳�ｸｺ荵晢ｽ芽｢�ｽ�ｦ
	updateCharacter();

	// 郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ郢ｧ�ｯ郢ｧ�ｿ郢晢ｽｼ邵ｺ�ｮ陷崎ｼ披ｳ
	bool allCharacterAlreadyGoal = true;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP邵ｺ0邵ｺ�ｪ郢ｧ蟲ｨ縺帷ｹｧ�ｭ郢晢ｿｽ�ｽ
		if (controller->getAction()->getCharacter()->getHp() == 0) { continue; }

		// 髯ｦ謔溯劒陷鷹亂�ｽ陷�ｽｦ騾
		controller->init();

		// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現竊堤ｸｺ�ｮ陟冶侭笳�ｹｧ髮∵�陞ｳ
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);
		atariCharacterAndObject(controller, m_stageObjects); // 2陜玲ｨ貞ｲｼ陷ｻ�ｼ邵ｺ�ｶ邵ｺ�ｮ邵ｺ�ｯ陞ｯ�･陷顔夢�｡蛹ｻ1陜玲ｨ貞ｲｼ邵ｺ�ｧ隴∵�謫�ｸｺ�ｫ邵ｺ�ｽ�狗ｸｺ荵昶ｲ郢ｧ荳環ｰ郢ｧ鄙ｫ竏壺落郢ｧ蠕娯�郢ｧ蛹ｻ笆ｲ邵ｺ�ｦ陷�ｽｦ騾�ｿｽ窶ｲ陞溷ｳｨ�冗ｹｧ荵昶螺郢ｧ2陜玲ｨ貞ｲｼ邵ｺ謔滂ｽｿ�ｽ�ｦ

		// 騾ｶ�ｮ隶灘雀諷崎ｽ､�ｹ邵ｺ�ｸ驕假ｽｻ陷崎ｼ披�郢ｧ蛹ｺ譯�抄 original邵ｺ�ｮ郢昜ｸ奇ｿｽ郢晏現�ｽ郢晁ｼ釆懃ｹ晢ｽｼ郢ｧ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}

		// 陷ｿ閧ｴ荳 original邵ｺ�ｮ郢昜ｸ奇ｿｽ郢晏現�ｽ郢晁ｼ釆懃ｹ晢ｽｼ郢ｧ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現竊堤ｸｺ�ｮ髮具ｽｫ鬨ｾ螢ｼ諢幄楜
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// 郢ｧ�ｪ郢晄じ縺夂ｹｧ�ｧ郢ｧ�ｯ郢晏現�ｽ陷崎ｼ披ｳ
	controlObject();

	// 郢ｧ�｢郢ｧ�､郢晢ｿｽﾎ堤ｸｺ�ｮ陷崎ｼ披ｳ
	controlItem();

	// 郢ｧ�ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
	adjustBattleCamera();

	// 郢ｧ�ｵ郢ｧ�ｦ郢晢ｽｳ郢晏ｳｨ�ｽ郢晢ｽｬ郢ｧ�､郢晢ｽ､郢晢ｽｼ邵ｺ�ｮ郢昜ｻ｣ﾎｦ髫ｪ�ｭ陞ｳ螟ら舞
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 郢ｧ�｢郢昜ｹ斟鍋ｹ晢ｽｼ郢ｧ�ｷ郢晢ｽｧ郢晢ｽｳ邵ｺ�ｮ隴厄ｽｴ隴�ｽｰ
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
	if (m_conversation_p != nullptr) {
		m_conversation_p->play();
		// 闔ｨ螟奇ｽｩ�ｱ驍ｨ繧�ｽｺ
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// 郢郢晢ｽｼ郢晁侭�ｽ郢ｧ蜻茨ｽｵ竏壺�
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// 郢郢晢ｽｼ郢晁侭�ｽ驍ｨ繧�ｽｺ
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}