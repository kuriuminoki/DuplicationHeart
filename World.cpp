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
* 髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ隲帷ｿｫ�･驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ髯樊ｺｽ蛻､陝ｲ�ｩ髯ｷ�ｽ�ｽ�ｦ鬨ｾ
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
* controllers_p驍ｵ�ｺ闕ｵ譎｢�ｽ闍ｱlayer_p驍ｵ�ｺ�ｽ�ｮ髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢髦ｮ蜻ｻ�ｽ螳夲ｽｿ螟ｲ�ｽ�ｹ髯橸ｽｳ陞｢�ｹ�ｽ驍ｵ�ｲ遶擾ｽｵ�ｽ�ｬ�ｽ�｡驍ｵ�ｺ�ｽ�ｮ髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ隲帷ｿｫ�･驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ陞ｳ螟ｲ�ｽ�ｿ隴∫ｵｶ�ｽ驍ｵ�ｲ
* 髯樊ｺｽ蛻､陝ｲ�ｩ髯ｷ莠･迴ｾ�ｽ�ｽ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｯID驍ｵ�ｺ�ｽ�ｧ髮手ｶ｣�ｽ�ｺ驍ｵ�ｺ�ｽ�ｾ驛｢�ｧ闕ｵ
* 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ驛｢譎｢�ｽ�ｻplayer驍ｵ�ｺ�ｽ�ｮBrain髯樊ｺｽ蛻､陝ｲ�ｩ驍ｵ�ｲ遯ｶ�ｦontrollers_p驍ｵ�ｺ�ｽ�ｮBrain髯樊ｺｽ蛻､陝ｲ�ｩ驍ｵ�ｺ�ｽ�ｧ髯橸ｽｳ陟包ｽ｡�ｽ�ｺ
*/
const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p, const Character* player_p) {
	// E驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ隴ｴ�ｧ隹ｺ�ｾ驍ｵ�ｺ髴郁ｲｻ�ｽ讙趣ｽｸ�ｺ�ｽ�ｦ驍ｵ�ｺ�ｽ�ｽ遶企��ｸ�ｺ�ｽ�ｽ遶企��ｹ�ｧ隰梧汚�ｽ�ｽ髴郁ｲｻ�ｽ繧会ｽｸ�ｺ陷会ｽｱ遶企��ｸ�ｺ
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		// 髯橸ｽｻ隶抵ｽｭ隨假ｿｽ�ｹ�ｧ闕ｵ譏ｴ�｡驛｢譎｢�ｽ�ｫ驛｢譎｢�ｽ�ｼ驛｢
		int groupId = target->getGroupId();
		// ID
		int id = target->getId();
		if (id == player_p->getId()) { continue; }
		// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢髦ｮ蜻ｻ�ｿ�ｽ驍ｵ�ｺ�ｽ�ｿ髯晢ｿｽ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡
		if (groupId == player_p->getGroupId()) {
			// ID驍ｵ�ｺ隴ｴ�ｧ隲､蜻ｵ豌｣闕ｳ螂�ｽｿ�ｽ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			// ID驍ｵ�ｺ陟暮ｯ会ｽｿ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髫ｹ�ｺ�ｽ�｡驍ｵ�ｺ�ｽ�ｫ髯晢ｿｽ�ｸ螂�ｽｼ�ｽ�ｸ�ｺ�ｽ�ｽ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ
			if (id > player_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ隴ｴ�ｧ隲､蜥擾ｽｹ�ｧ�ｽ�ｽD驍ｵ�ｺ�ｽ�ｮ髯樊ｻゑｽｽ�ｧ驍ｵ�ｺ鬮ｦ�ｪ�ｽ讓抵ｽｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ驍ｵ�ｺ�ｽ�｣驍ｵ�ｺ雋�ｽｷ�ｽ�ｴ髯ｷ�ｷ�ｽ�ｽullptr驍ｵ�ｺ�ｽ�ｪ驍ｵ�ｺ�ｽ�ｮ驍ｵ�ｺ�ｽ�ｧID驍ｵ�ｺ隴ｴ�ｧ隲､蜥擾ｽｹ�ｧ郢ｧ�ｽ�ｽ�ｰ闕ｳ螂�ｽｼ�ｽ�ｸ�ｺ�ｽ�ｽ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ陝ｶ譏ｶ�ｽ驍ｵ�ｺ�ｽ�ｶ
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}


// vector驍ｵ�ｺ�ｽ�ｫ髯ｷ闌ｨ�ｽ�･驍ｵ�ｺ�ｽ�｣驍ｵ�ｺ雋�ｽｷ�ｽ�ｽ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽≧闔ｨ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector驍ｵ�ｺ�ｽ�ｫ髯ｷ闌ｨ�ｽ�･驍ｵ�ｺ�ｽ�｣驍ｵ�ｺ雎�ｽｸeleteFlag驍ｵ�ｺ髣戊寰rue驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽≧闔ｨ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 髫ｴ蟷｢�ｽ�ｫ髯晢ｿｽ�ｽ�ｾ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ邇厄ｽｭ�ｽ�ｽ�ｹ驍ｵ�ｺ驕雋�∞�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector驍ｵ�ｺ�ｽ�ｫ髯ｷ闌ｨ�ｽ�･驍ｵ�ｺ�ｽ�｣驍ｵ�ｺ雋�ｽｷ�ｽ�ｽ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽↑髴域攸�ｰ驍ｵ�ｺ
void actionObject(vector<Object*>& objects) {
	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ螳茨ｽｿ�ｫ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ (髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ遶雁ｮ壽↑髴域喚�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
		objects[i]->action();
		// deleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 髫ｴ蟷｢�ｽ�ｫ髯晢ｿｽ�ｽ�ｾ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ邇厄ｽｭ�ｽ�ｽ�ｹ驍ｵ�ｺ驕雋�∞�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｨ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ鬮ｮ蜈ｷ�ｽ�ｫ鬯ｨ�ｾ陞｢�ｽ陷�ｽｾ髯具ｽｻ�ｽ�､髯橸ｽｳ
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ螳茨ｽｿ�ｫ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ (髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ遶雁ｮ壽↑髴域喚�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ�ｽ蝣､�ｸ�ｺ髦ｮ蜻ｻ�ｼ�ｽ�ｸ�ｺ�ｽ�ｧ鬮ｯ�ｦ陟募ｨｯ魘ｬ
		objects[i]->penetration(controller);
	}
}


/*
* 驛｢�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｳ驛｢�ｧ�ｽ�ｹ驛｢譎冗樟�主ｸｷ�ｹ�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ
*/
World::World() {
	m_duplicationFlag = false;

	m_brightValue = 255;

	m_resetBgmFlag = false;

	m_blindFlag = false;

	// 莨夊ｩｱ繧､繝吶Φ繝
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// 驛｢驛｢譎｢�ｽ�ｼ驛｢譎∽ｾｭ�ｽ�ｽ
	m_movie_p = nullptr;

	// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ蜥ｲ�ｩ繧托ｽｽ�ｸ�ｽ�ｭ
	m_skillFlag = false;

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髯区ｺｷ隱ｿ驍擾ｽｫ驍ｵ�ｺ�ｽ�ｮ髫ｴ髯樊ｻゑｽｽ�ｧ驛｢譎｢�ｽ�ｻ髫ｴ髯晢ｿｽ�ｸ�ｽ�､驛｢�ｧ陞ｳ螟ｲ�ｽ�ｧ�ｽ�｣髯ｷ蜑�ｽｸ讖ｸ�ｽ�ｺ�ｽ�ｦ驍ｵ�ｺ闕ｵ譎｢�ｽ闃ｽ�ｱ雜｣�ｽ�ｺ髯橸ｽｳ
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;

	m_date = 0;

}

/*
* 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ驛｢譎｢�ｽ�ｭ驛｢譎｢�ｽ�ｼ驛｢譎擾ｽｳ�ｨ遶企��ｸ�ｺ�ｽ�ｩ
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// 驛｢�ｧ�ｽ�ｵ驛｢�ｧ�ｽ�ｦ驛｢譎｢�ｽ�ｳ驛｢譎擾ｽｳ�ｨ�ｽ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ
	m_soundPlayer_p = soundPlayer;

	// 髣包ｽｳ�ｽ�ｻ髣�ｽ�ｺ髯ｷ闌ｨ�ｽ�ｬ驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驛｢譏懶ｽｺ�･隲ｷ蟠趣ｽｽ�､�ｽ�ｹ
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// 驛｢�ｧ�ｽ�ｨ驛｢譎｢�ｽ�ｪ驛｢�ｧ�ｽ�｢驛｢�ｧ陋幢ｽｵ�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢
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

	// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驛｢�ｧ陋幢ｽｵ邵ｺ譎会ｽｹ譎｢�ｿ�ｽ郢晢ｽｨ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髫ｴ�ｽ�ｽ�ｹ髯ｷ�ｷ闔会ｽ｣遶城メ諠ｺ闔会ｽ｣�ゑｽｰ驍ｵ�ｺ陝ｶ蜻ｻ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		m_characterControllers[i]->setPlayerDirection(m_player_p, true);
	}

	m_camera->setEx(m_cameraMaxEx);

	m_characterDeadGraph = new GraphHandles("picture/effect/dead", 5, 1.0, 0, true);
	m_bombGraph = new GraphHandles("picture/effect/bomb", 9, 1.0, 0, true);
	m_characterDeadSound = LoadSoundMem("sound/battle/dead.wav");
	m_bombSound = LoadSoundMem("sound/battle/bomb.wav");
	m_doorSound = LoadSoundMem("sound/battle/door.wav");

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

	// エリアをコピー (コピー元と共有するもの)
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_bombGraph = original->getBombGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_bombSound = original->getBombSound();
	m_doorSound = original->getDoorSound();
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();

	// 新規作成するもの (ポインタが変わる)
	m_camera = new Camera(original->getCamera());
	for (unsigned int i = 0; i < original->getCharacters().size(); i++) {
		Character* copy;
		copy = original->getCharacters()[i]->createCopy();
		m_characters.push_back(copy);
		if (copy->getId() == m_playerId) { m_player_p = copy; }
	}
	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);
	for (unsigned int i = 0; i < original->getCharacterControllers().size(); i++) {
		CharacterController* copy;
		// Brain驍ｵ�ｺ�ｽ�ｨAction驛｢�ｧ�ｽ�ｳ驛｢譎�ｱ抵ｿｽ�ｽ鬨ｾ蛹�ｽｽ�ｨ驍ｵ�ｺ�ｽ�ｫCharacter驍ｵ�ｺ�ｽ�ｨ驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢�ｧ陷ｻ闌ｨ�ｽ�ｸ�ｽ�｡驍ｵ�ｺ
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
	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
	delete m_camera;

	// 髯ｷ闌ｨ�ｽ�ｨ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽≧闔ｨ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ邵ｺ鬘費ｽｹ譎�ｽｼ譁絶凾驛｢�ｧ�ｽ�ｯ驛｢譏懶ｽｺ�･霓､譛ｱ�ｫ�ｯ�ｽ�､
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// 驛｢�ｧ�ｽ�｢驛｢�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�主ｮ壽≧闔ｨ竏晄ｱ
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// 髯ｷ闌ｨ�ｽ�ｨ驛｢�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｳ驛｢譎冗樟�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢譎｢�ｽ�ｩ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 髯ｷ闌ｨ�ｽ�ｨ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// 鬮｢�ｭ隴ｴ�ｧ陷搾ｽｹ
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
		delete m_characterDeadGraph;
		delete m_bombGraph;
		DeleteSoundMem(m_characterDeadSound);
		DeleteSoundMem(m_bombSound);
		DeleteSoundMem(m_doorSound);
	}

	if (m_objectConversation != nullptr) {
		delete m_objectConversation;
	}
}

// Drawer鬨ｾ蛹�ｽｽ�ｨ�ｽ�ｽ陷･�ｾharacterAction驍ｵ�ｺ�ｽ�ｮvector驛｢�ｧ陞ｳ螟ｲ�ｽ�ｿ隴∫ｵｶ�ｽ
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() > 0 || m_characterControllers[i]->getAction()->getCharacter()->haveDeadGraph()) {
			actions.push_back(m_characterControllers[i]->getAction());
		}
	}
	return actions;
}

// Drawer鬨ｾ蛹�ｽｽ�ｨ�ｽ�ｽ陷夲ｽｹbject驍ｵ�ｺ�ｽ�ｮvector驛｢�ｧ陞ｳ螟ｲ�ｽ�ｿ隴∫ｵｶ�ｽ
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer鬨ｾ蛹�ｽｽ�ｨ�ｽ�ｽ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ陋ｹ�ｻ�ｽ鬘蜍滂ｽｾ魃会ｽｽ蜥ｲ�ｸ�ｺ�ｽ�ｫ髫ｰ陷諤懈�驍ｵ�ｺ陷ｷ�ｶ�ｽ霆恵ject驍ｵ�ｺ�ｽ�ｮvector驛｢�ｧ陞ｳ螟ｲ�ｽ�ｿ隴∫ｵｶ�ｽ
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer鬨ｾ蛹�ｽｽ�ｨ�ｽ�ｽ陷ｩ�ｯimation驍ｵ�ｺ�ｽ�ｮvector驛｢�ｧ陞ｳ螟ｲ�ｽ�ｿ隴∫ｵｶ�ｽ
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// 驛｢�ｧ�ｽ�ｨ驛｢譎�ｽｼ譁絶凾驛｢�ｧ�ｽ�ｯ驛｢
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// 驛｢�ｧ�ｽ�｢驛｢�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			allAnimations.push_back(m_itemVector[i]->getAnimation());
		}
	}

	return allAnimations;
}

// 髯ｷ�ｷ隶朱｡披�驍ｵ�ｺ�ｽ�ｧ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ髫ｶ隲幢ｿｽ�ｽ�ｴ�ｽ�｢
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// 髯ｷ�ｷ隶朱｡披�驍ｵ�ｺ�ｽ�ｧ驛｢�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｳ驛｢譎冗樟�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢譎｢�ｽ�ｩ髫ｶ隲幢ｿｽ�ｽ�ｴ�ｽ�｢
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// ID驍ｵ�ｺ�ｽ�ｧ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ髫ｶ隲幢ｿｽ�ｽ�ｴ�ｽ�｢
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID髫ｰ蜴�ｽｿ�ｽ�ｽ�ｮ陞｢�ｹ邵ｲ驕排ain髯樊ｺｽ蛻､陝ｲ�ｩ
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// 驛｢�ｧ�ｽ�ｹ驛｢譎冗樟�ｽ�ｽ驛｢譎｢�ｽ�ｪ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｫ驛｢�ｧ陋ｹ�ｻ�ｽ遏ｩ諱�ｿｽ�ｽ髯ｷ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// 驛｢�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｳ驛｢譎冗樟�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢譎｢�ｽ�ｩ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// 郢ｧ�ｹ郢晏現�ｽ郢晢ｽｪ郢晢ｽｼ邵ｺ�ｫ郢ｧ蛹ｻ�狗ｹｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｮ隲､�ｧ髢ｭ�ｽ陞溽甥蝟ｧ
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// 驛｢譎擾ｽｳ�ｨ邵ｺ
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮHP驍ｵ�ｺ0驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ雎ｸ�ｲrue
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮHP驛｢�ｧ隶難ｽｽAX驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷ｷ�ｶ�ｽ
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
}


// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞｢�ｹ郢晢ｽｯ驛｢譎｢�ｽ�ｼ驛｢譎冗樟�ｽ蟾ｽreeze驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷ｷ�ｶ�ｽ
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞溷･�ｽｽ�､�ｽ�ｽ�ｽ�｣�ｽ�ｽ驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譎槭Γ�ｽ�ｿ�ｽ�ｽ髯ｷ鬨ｾ蛹�ｽｽ�ｨ
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞溷･�ｽｽ�､�ｽ�ｽ�ｽ�｣�ｽ�ｽ驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譏懶ｽｺ�･霓､譛ｱ�ｫ�ｯ�ｽ�､鬨ｾ蛹�ｽｽ�ｨ
void World::popCharacterController(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	// Target驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷会ｽｱ遯ｶ�ｻ驍ｵ�ｺ�ｽ�ｽ�ｽ蟶ｰI驍ｵ�ｺ�ｽ�ｯ髯樊ｻ薙§�ｽ驍ｵ�ｺ�ｽ�ｦ驛｢�ｧ郢ｧ�ｽ�ｽ閾･�ｸ�ｺ�ｽ�ｽ隨ｳ�ｽ�ｸ�ｺ�ｽ�ｽ�ｽ�ｽ驍ｵ�ｺ�ｽ�ｧHP=0驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞｢�ｹ�取ｨ抵ｽｹ�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｼ驛｢驛｢�ｧ陷托ｿｽ�ｽ�ｽ隲幄肩�ｿ�ｽ驍ｵ�ｺ陟搾ｽｺ�ｽ�ｽ�ｽ�ｿ鬨ｾ蛹�ｽｽ�ｨ驛｢�ｧ陝ｶ譎擾ｽｹ謌頑ｲ
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞｢�ｹ�取ｨ抵ｽｹ�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｼ驛｢驍ｵ�ｺ�ｽ�ｮ髫ｴ蠑ｱ�玖将�｣驛｢�ｧ陷ｻ蝓蜻ｵ蟠戊ｭ擾ｽｴ遶頑･｢�ｬ魃会ｽｽ�ｻ驍ｵ�ｺ
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ讎頑｡�ｿｽ�ｼ陞｢�ｹ�取ｨ抵ｽｹ�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｼ驛｢驍ｵ�ｺ�ｽ�ｮ髣厄ｽｴ�ｽ�ｿ鬨ｾ蛹�ｽｽ�ｨ驛｢�ｧ陋幢ｽｵ�ｽ�ｽ�ｹ�ｧ遶丞｣ｺ�ｻ髯ｷ蜿ｰ�ｼ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髴托ｽ･�ｽ�ｶ髫ｲ�ｷ闕ｵ譎｢�ｽ螳壽｣秘包ｽｻ陝ｲ�ｩ驍ｵ�ｺ陷ｷ�ｶ�ｽ 驍ｵ�ｺ�ｽ�ｽ遶企��ｸ�ｺ�ｽ�ｽ遶企��ｹ�ｧ隰梧汚�ｽ�ｽ隲幄肩�ｿ�ｽ驍ｵ�ｺ陷ｷ�ｶ�ｽ
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ鬮ｫ�ｪ�ｽ�ｭ髯橸ｽｳ
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ陷ｻ閧ｲ謔幃ｫｫ遨ゑｽｸ闌ｨ�ｽ�ｽ隲幄肩�ｿ�ｽ驍ｵ�ｺ陷ｷ�ｶ�ｽ邇匁捗�ｽ�ｴ髯ｷ�ｷ髣鯉ｽｨ�ｽ�ｼ陋ｹ�ｻ�ｽ�ｽ�ｸ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｨ驛｢譎｢�ｽ�ｪ驛｢�ｧ�ｽ�｢驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｸ�ｺ�ｽ�ｯ驍ｵ�ｺ陞｢�ｹ�ｽ�ｽ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ驍ｵ�ｺ陟募ｨｯ遨宣し�ｺ驍ｵ�ｺ�ｽ�ｽ遶企��ｸ�ｺ�ｽ�ｽ�ｽ�ｼ
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// 驛｢�ｧ�ｽ�ｳ驛｢譎｢�ｽ�ｳ驛｢譎冗樟�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢譎｢�ｽ�ｩ驍ｵ�ｲ遶丞｣ｹ�樣Δ�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｷ驛｢譎｢�ｽ�ｧ驛｢譎｢�ｽ�ｳ驍ｵ�ｲ邵ｲ�ｳain驍ｵ�ｺ�ｽ�ｮ鬮ｫ�ｪ�ｽ�ｭ髯橸ｽｳ
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

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髴托ｽ･�ｽ�ｶ髫ｲ�ｷ闕ｵ譎｢�ｽ螳夲ｽｬ�ｨ陷ｷ�ｶ遶擾ｽｴ驛｢�ｧ
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
				data->setY(c->getY() + c->getHeight()); // Y2髯溯ｶ｣�ｽ�ｧ髫ｶ轣倡函�ｽ螳壼ｰ�ｭ取得�ｽ�ｭ 驛｢譎｢�ｽ�ｭ驛｢譎｢�ｽ�ｼ驛｢譎牙愛陷�ｽｾ驍ｵ�ｺ�ｽ�ｯ鬮ｴ�ｽ�ｽ�ｫ鬯ｮ貊ゑｽｽ�ｷ驍ｵ�ｺ�ｽ�ｧ鬮ｯ�ｬ隲幄肩�ｽ�ｭ�ｽ�｣
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

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陷･�ｼoor驍ｵ�ｺ�ｽ�ｮ髴托ｽ･�ｽ�ｶ髫ｲ�ｷ闕ｵ譎｢�ｽ螳壽｣秘包ｽｻ陝ｲ�ｩ驍ｵ�ｺ陷ｷ�ｶ�ｽ 驍ｵ�ｺ�ｽ�ｽ遶企��ｸ�ｺ�ｽ�ｽ遶企��ｹ�ｧ隰梧汚�ｽ�ｽ隲幄肩�ｿ�ｽ驍ｵ�ｺ陷ｷ�ｶ�ｽ
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

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陷･�ｼoor驍ｵ�ｺ�ｽ�ｮ髴托ｽ･�ｽ�ｶ髫ｲ�ｷ闕ｵ譎｢�ｽ螳夲ｽｬ�ｨ陷ｷ�ｶ遶擾ｽｴ驛｢�ｧ
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// 驛｢譎擾ｽｳ�ｨ邵ｺ�ｽ�ｸ�ｺ陋滂ｽ･�ｽ�ｽ�ｸ�ｺ�ｽ�ｪ驍ｵ�ｺ
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
		// 驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｽ�ｼ驛｢譎�§郢晢ｽｧ驛｢譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｿ驍ｵ�ｺ�ｽ�ｫ驛｢譎擾ｽｳ�ｨ邵ｺ�ｽ�ｸ�ｺ隰疲ｻゑｽｽ�ｭ闔ｨ諛夷帝し�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｺ
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
		if (!flag) { // 髫ｴ�ｽ�ｽ�ｰ驍ｵ�ｺ雋�ｪ�ｽ驛｢譎擾ｽｳ�ｨ邵ｺ�ｽ�ｸ�ｺ�ｽ�ｪ驛｢�ｧ陝ｲ�ｨ邵ｺ譎会ｽｹ譎｢�ｽ�ｼ驛｢譎�§郢晢ｽｧ驛｢譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｿ驍ｵ�ｺ�ｽ�ｫ鬮ｴ謇假ｽｽ�ｽ髯ｷ
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ�ｽ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｨ驍ｵ�ｺ隴擾ｽｴ�ｽ�ｽ髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢髦ｮ蜻ｻ�ｽ蝣､�ｹ譎擾ｽｳ�ｨ邵ｺ�ｽ�ｸ�ｺ�ｽ�ｮ髯ｷ鮃ｹ莠らｫ企ｦｴﾂｧ�ｽ�ｻ髯ｷ
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// 髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢髦ｮ蜻ｻ�ｽ繧仰ｧ�ｽ�ｻ髯ｷ
	setPlayerFollowerPoint();

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢譎｢�ｽ�ｪ驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ
	cameraPointInit();
}

// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驛｢�ｧ陜｣�､鬮ｻ�ｳ髯橸ｽｳ陞｢�ｹ�ｽ�ｽ髯溯ｶ｣�ｽ�ｧ髫ｶ轣倡函遶冗｣ｯﾂｧ�ｽ�ｻ髯ｷ
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢譎｢�ｽ�ｪ驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ
	cameraPointInit();
}

// 髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢髦ｮ蜻ｻ�ｽ蝣､�ｹ譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髣厄ｽｴ陷･�ｲ�ｽ�ｽ�ｽ�ｮ驍ｵ�ｺ�ｽ�ｸ鬩募∞�ｽ�ｻ髯ｷ
void World::setPlayerFollowerPoint() {
	// 驛｢譎丞ｹｲ�取ｨ抵ｽｹ�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髣比ｼ夲ｽｽ�ｲ鬯ｮ�｢
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// 鬮ｴ謇假ｽｽ�ｽ鬮ｴ謳ｾ�ｽ�｡髯晢ｿｽ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡驍ｵ�ｺ陟暮ｯ会ｽｿ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controller驍ｵ�ｺ�ｽ�ｫ髯晢ｿｽ�ｽ�ｾ髯滂ｽ｢隲帛ｲｩ�ｽ驛｢�ｧ雎梧昭aracter驍ｵ�ｺ�ｽ�ｫ髯樊ｺｽ蛻､陝ｲ�ｩ驛｢�ｧ髮区ｧｫ�ｽ驍ｵ�ｺ陋ｹ�ｻ�ｽ
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

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ邵ｺ蜥ｲ�ｹ譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髣厄ｽｴ陷･�ｲ�ｽ�ｽ�ｽ�ｮ驛｢�ｧ陋幢ｽｵ�取㏍�ｹ�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ1髣厄ｽｴ髦ｮ蜻ｻ�ｿ�ｽ髫ｲ�ｽ�ｽ�ｽ�ｱ驛｢�ｧ陷托ｿｽ�ｽ�ｸ闕ｵ貊�ｿｽ驍ｵ�ｺ�ｽ�ｫ髯ｷ�ｿ髢ｧ�ｴ闕ｳ
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// 驍ｵ�ｺ髦ｮ蜻ｻ�ｿ�ｽ驛｢�ｧ�ｽ�ｲ驛｢譎｢�ｽ�ｼ驛｢驍ｵ�ｺ�ｽ�ｧ髯具ｽｻ隴守甥蜈�劈�｣�ｽ�ｴ驍ｵ�ｺ陋滂ｽ･�ｽ�ｽ�ｸ�ｺ�ｽ�ｪ驍ｵ�ｺ
		character->setHp(data->hp());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y髯溯ｶ｣�ｽ�ｧ髫ｶ轣倡函�ｽ�ｽ鬮ｴ�ｽ�ｽ�ｫ鬯ｮ貊ゑｽｽ�ｷ驍ｵ�ｺ�ｽ�ｫ髯ｷ�ｷ陋ｹ�ｻ�ｽ蜀暦ｽｸ�ｺ陝ｶ蜷ｮ�ｻ鬮ｫ�ｱ�ｽ�ｿ髫ｰ�ｨ�ｽ�ｴ
		character->setY(data->y() - character->getHeight());
	}
}

// 驛｢譎｢�ｿ�ｽ�ｽ�ｽ驛｢�ｧ�ｽ�ｿ鬩阪ｑ�ｽ�｡鬨ｾ�ｽ�ｿ�ｽ�ｽ�ｼ陞｢�ｹ邵ｺ諷包ｽｹ譎｢�ｽ�ｳ驛｢譎冗樟�取ｺｽ�ｹ譎｢�ｽ�ｼ驛｢譎｢�ｽ�ｩ1髯区ｻ会ｽｹ譎｢�ｿ�ｽ髫ｲ�ｽ�ｽ�ｽ�ｱ驛｢�ｧ陷托ｿｽ�ｽ�ｸ闕ｵ貊�ｿｽ驍ｵ�ｺ�ｽ�ｫ髯ｷ�ｿ髢ｧ�ｴ闕ｳ
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Action驛｢�ｧ陷托ｿｽ�ｽ�ｽ隲幄肩�ｿ�ｽ
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brain驛｢�ｧ陷托ｿｽ�ｽ�ｽ隲幄肩�ｿ�ｽ
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controller驛｢�ｧ陷托ｿｽ�ｽ�ｽ隲幄肩�ｿ�ｽ
	return createController(data->controllerName(), brain, action);
}

/*
*  髫ｰ魃会ｽｽ�ｦ驛｢�ｧ闕ｳ蟯ｩ髮ｷ驛｢�ｧ
*/
void World::battle() {
	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	
	// 逕ｻ髱｢證苓ｻ｢蜃ｦ逅
	if (dealBrightValue()) { return; }

	// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ蟶晏擅�ｽ�ｿ驍ｵ�ｺ�ｽ�ｹ驍ｵ�ｺ雋顔§�ｽ驍ｵ�ｺ�ｽ�ｮ驛｢譎｢�ｿ�ｽ邵ｺ蜀暦ｽｹ�ｧ�ｽ�ｹ驛｢
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// 髣費ｽｨ陞溷･�ｽｽ�ｩ�ｽ�ｱ鬩搾ｽｨ郢ｧ�ｽ�ｽ�ｺ
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

	// deleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽≧闔ｨ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ�ｽ�ｽ陜捺ｺｷ諢幃垈�ｦ�ｽ�ｽ�ｽ�ｯ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡驍ｵ�ｺ�ｽ�ｮ髯樊ｺｽ蛻､陝ｲ�ｩ�ｽ�ｽ
	// 髣包ｽｳ驗呻ｽｫ邵ｲ蝣､�ｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷会ｽｱ隨ｳ�ｽ�ｸ�ｺ闕ｵ譎｢�ｽ闃ｽ�ｭ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ驍ｵ�ｺ陷会ｽｱ隨ｳ�ｽ�ｸ�ｺ闕ｵ譎｢�ｽ闃ｽ�｢�ｽ�ｽ�ｽ�ｦ
	updateCharacter();

	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	controlCharacter();

	// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	controlObject();

	// 驛｢�ｧ�ｽ�｢驛｢�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�主､�ｸ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	controlItem();

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
	adjustBattleCamera();

	// 驛｢�ｧ�ｽ�ｵ驛｢�ｧ�ｽ�ｦ驛｢譎｢�ｽ�ｳ驛｢譎擾ｽｳ�ｨ�ｽ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｻ�｣�趣ｽｦ鬮ｫ�ｪ�ｽ�ｭ髯橸ｽｳ陞溘ｉ闊
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 驛｢�ｧ�ｽ�｢驛｢譏懶ｽｹ譁滄豪�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｷ驛｢譎｢�ｽ�ｧ驛｢譎｢�ｽ�ｳ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
	updateAnimation();

	// 髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ隲帷ｿｫ�･驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ髯樊ｺｽ蛻､陝ｲ�ｩ
	changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers, m_player_p));

}

void World::changePlayer(const Character* nextPlayer) {
	if (nextPlayer == nullptr) { return; }
	// 髣碑崟隨ｬ�｡�ｽ謚�ｫ帛･�ｽｼ驍ｵ�ｺ�ｽ�ｦ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ髫ｱ�ｰPC驍ｵ�ｺ�ｽ�ｮBrain驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷ｷ�ｶ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_player_p->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// 髫ｹ�ｺ�ｽ�｡髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ隲帛ｲｩ�ｽ驛｢�ｧ闕ｵ譏ｴ�･驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ霑｢�ｸeyboardBrain驍ｵ�ｺ�ｽ�ｫ驍ｵ�ｺ陷ｷ�ｶ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_playerChanger->setPrevBrainName(brainName);
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// 髫ｹ�ｺ�ｽ�｡髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ隲帛ｲｩ�ｽ驛｢�ｧ闕ｵ譏ｴ�･驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ陷願ｩｫayer驍ｵ�ｺ�ｽ�ｨ驍ｵ�ｺ陷会ｽｱ遯ｶ�ｻ驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ
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

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ蜥ｲ�ｹ譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
void World::updateCamera(int gx, int gy, double gex) {

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢�ｧ陷ｻ蝓滉ｺ企Δ�ｧ陝ｲ�ｨ隨
	m_camera->shaking();

	// 髫ｲ�｡�ｽ�｡髯樊ｻゑｽｽ�ｧ鬩搾ｿｽ�ｿ�ｽ邵ｺ譎会ｽｹ譎｢�ｿ�ｽ郢晢ｽｨ
	m_camera->setEx(gex);

	// 鬨ｾ�ｶ�ｽ�ｮ髫ｶ轣伜ｧ難ｿｽ�ｽ陷･�ｲ�ｽ�ｽ�ｽ�ｮ驛｢�ｧ�ｽ�ｻ驛｢譎｢�ｿ�ｽ郢晢ｽｨ
	m_camera->setGPoint(gx, gy);
	
	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驛｢�ｧ陜｣�､陝ｯ�ｼ髫ｶ轣伜ｧ難ｿｽ�ｽ陷･�ｲ�ｽ�ｽ�ｽ�ｮ驍ｵ�ｺ�ｽ�ｸ鬮ｴ蜿ｰ�ｻ�｣隨�ｽｼ驍ｵ�ｺ闔会ｽ｣�ｽ
	m_camera->move();
}

// Battle�ｽ�ｽ陞｢�ｽ陝具ｽｶ鬯ｮ�｣陋滂ｿｽ�ｽ�ｸ�ｽ�ｭ驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ
void World::adjustBattleCamera() {

	// 鬨ｾ�ｶ�ｽ�ｮ髫ｶ轣倬它�ｽ�ｺ�ｽ�ｧ髫ｶ
	int gx = 0, gy = 0;
	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｨ驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ鬮ｴ閧ｴ霎ｨ陞ｻ�ｬ驍ｵ�ｺ�ｽ�ｮ髫ｴ髯樊ｻゑｽｽ�ｧ髯区ｻゑｽｽ�､驛｢�ｧ陞ｳ螟ｲ�ｽ�ｪ�ｽ�ｿ驍ｵ�ｺ�ｽ�ｹ驛｢�ｧ
	int max_dx = 0, max_dy = 0;
	// 騾包ｽｻ鬮ｱ�｢陷�ｽ竊楢怦�･郢ｧ蠕鯉ｽ育ｸｺ�ｽ竊堤ｸｺ蜷ｶ�矩恪譎槫ｱｬ邵ｺ�ｮ隴陞滂ｽｧ邵ｲ邵ｺ阮呻ｽ檎ｹｧ蛹ｻ�企ｫｮ�｢郢ｧ蠕娯螺郢ｧ�ｭ郢晢ｽ｣郢晢ｽｩ邵ｺ�ｯ霎滂ｽ｡髫
	const int MAX_DISABLE = 2000;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// 髣碑��ｿ�ｫ郢晢ｽｵ驛｢�ｧ�ｽ�ｩ驛｢譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｫ驛｢�ｧ�ｽ�ｹ驍ｵ�ｺ陷会ｽｱ遯ｶ�ｻ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髯溯ｶ｣�ｽ�ｧ髫ｶ轣倡函遶頑･｢諠ｺ陋ｹ�ｻ�ｽ蜀暦ｽｸ�ｺ陝ｶ蜻ｻ�ｽ
		if (m_focusId == m_characters[i]->getId()) {
			gx = m_characters[i]->getCenterX();
			gy = m_characters[i]->getCenterY();
		}
		// 驛｢譎�ｽｼ譁青ｰ驛｢譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｫ驛｢�ｧ�ｽ�ｹ驍ｵ�ｺ陷会ｽｱ遯ｶ�ｻ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ髣比ｼ夲ｽｽ�･髯樊ｻ薙§遶企��ｹ�ｧ髣�ｽｽ�ｽ�ｷ隴取ｧｫ�ｱ�ｬ驛｢�ｧ陞ｳ螟ｲ�ｽ�ｪ�ｽ�ｿ驍ｵ�ｺ�ｽ�ｹ驛｢�ｧ
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

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｲ�｡�ｽ�｡髯樊ｻゑｽｽ�ｧ驛｢譎｢�ｽ�ｻ鬩搾ｽｵ�ｽ�ｮ髯
	// 髯樊ｻゑｽｽ�ｧ驍ｵ�ｺ鬮ｦ�ｪ�ｽ�･髯樊ｺｽ蛻､陝ｲ�ｩ驍ｵ�ｺ陷ｷ�ｶ�ｽ邇�｢�ｽ�ｽ�ｽ�ｦ遶丞｣ｺ�ｲ驍ｵ�ｺ郢ｧ�ｽ�ｽ邇匁捗�ｽ�ｴ髯ｷ�ｷ陋ｹ�ｻ遶難ｿｽ�ｸ�ｺ�ｽ�ｩ驍ｵ�ｲ遶乗劼�ｽ�､�ｽ�ｧ驍ｵ�ｺ鬮ｦ�ｪ�ｽ�･髫ｲ�｡�ｽ�｡髯樊ｻゑｽｽ�ｧ鬩搾ｿｽ�ｿ�ｽ�ｽ螳壽｣秘包ｽｻ陝ｲ�ｩ驍ｵ�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	double nowEx = m_camera->getEx();
	double gex = nowEx;
	int shift = controlLeftShift() + controlRightShift();
	if (shift == 1) {
		m_camera->setZoomOutMode(!m_camera->getZoomOutMode());
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
			// 鬩搾ｽｵ�ｽ�ｮ髯
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.1, d / 100000) * m_exX);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// 髫ｲ�｡�ｽ�｡髯樊ｻゑｽｽ�ｧ
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.005, d / 100000) * m_exX);
		}
	}

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
	updateCamera(gx, gy, gex);
}

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ�ｽ�ｹ譏懶ｽｹ譁滄豪�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｷ驛｢譎｢�ｽ�ｧ驛｢譎｢�ｽ�ｳ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
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

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ�ｽ�ｽ陜捺ｺｷ諢幃垈�ｦ�ｽ�ｽ�ｽ�ｯ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡驍ｵ�ｺ�ｽ�ｮ髯樊ｺｽ蛻､陝ｲ�ｩ�ｽ�ｽ
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain驍ｵ�ｺ�ｽ�ｮ鬮ｫ陬懈升�ｽ�ｫ闕ｵ譏ｴ螳夲ｽｬ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｯ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡髯樊ｺｽ蛻､陝ｲ�ｩ
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) {
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) {
			continue;
		}

		// 鬮ｯ�ｦ隰疲ｺｯ蜉帝匚鮃ｹ莠ゑｿｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ
		controller->init();

		// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟遶雁､�ｸ�ｺ�ｽ�ｮ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
		atariCharacterAndObject(controller, m_stageObjects);
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects);
		}
		atariCharacterAndObject(controller, m_stageObjects); // 2回目呼ぶのは妥協案　1回目で斜面にいるかがわかり、それによって処理が変わるため2回目が必要
		if (controller->getAction()->getCharacter()->getId() == m_playerId) {
			atariCharacterAndDoor(controller, m_doorObjects);
		}

		// 髫ｰ�ｫ陜｣�ｺ�ｽ�ｽ original驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譎冗樟�ｽ�ｽ驛｢譎�ｽｼ驥�㏍�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// 髯晢ｿｽ�ｿ�ｽ髣鯉ｽｶ髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// 髫ｴ�ｽ�ｽ�ｬ髫ｰ�ｦ�ｽ�ｽ陋ｻ�､髫ｰ�ｦ
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// 髯ｷ�ｿ髢ｧ�ｴ闕ｳ original驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譎冗樟�ｽ�ｽ驛｢譎�ｽｼ驥�㏍�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟遶雁､�ｸ�ｺ�ｽ�ｮ鬮ｮ蜈ｷ�ｽ�ｫ鬯ｨ�ｾ陞｢�ｼ隲｢蟷�･
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
void World::controlObject() {
	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ螳茨ｽｿ�ｫ�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	actionObject(m_stageObjects);

	// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽ髦ｮ蜷ｮ陞ｺ驛｢�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	actionObject(m_attackObjects);

	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ<->髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
	atariStageAndAttack();

	// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ<->髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
	atariAttackAndAttack();
}

// Battle�ｽ�ｽ陞｢�ｹ邵ｺ�ｽ�ｹ�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�主､�ｸ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
void World::controlItem() {
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// 髯ｷ�ｿ鬮｢�ｽ�ｾ驍�ｽｲ�ｽ�ｸ陋ｹ�ｻ遶擾ｽｩ
		if (m_itemVector[i]->getDeleteFlag()) {
			// 髯ｷ莨夲ｽｽ�ｹ髫ｴ�ｫ隲橸ｽｺ雎ｬ�ｹ驍ｵ�ｺ隰疲ｻゑｽｿ�ｽ鬨ｾ蠅難ｽｻ莨夲ｽｽ�ｸ�ｽ�ｭ驍ｵ�ｺ�ｽ�ｧ驍ｵ�ｺ�ｽ�ｪ驍ｵ�ｺ�ｽ�ｽ遶企��ｹ�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// 髯具ｽｻ隴趣ｿｽ��刹�ｹ
		m_itemVector[i]->init();
		// 髯橸ｽ｢遶乗劼�ｽ�ｺ驗呻ｽｫ遶雁､�ｸ�ｺ�ｽ�ｮ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, m_itemVector[i]->getVx(), m_itemVector[i]->getVy())) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
		}
		// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｮ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
		if (m_itemVector[i]->atariCharacter(m_player_p)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
		}
		// 髯ｷ蟠趣ｽｼ謚ｫ�ｳ
		m_itemVector[i]->action();
	}
}

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｨ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ螳茨ｽｿ�ｫ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ (髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ遶雁ｮ壽↑髴域喚�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ�ｽ蝣､�ｸ�ｺ髦ｮ蜻ｻ�ｼ�ｽ�ｸ�ｺ�ｽ�ｧ鬮ｯ�ｦ陟募ｨｯ魘ｬ
		if (objects[i]->atari(controller)) {
			const Character* character = controller->getAction()->getCharacter();
			// 驛｢�ｧ�ｽ�ｨ驛｢譎�ｽｼ譁絶凾驛｢�ｧ�ｽ�ｯ驛｢譏懶ｽｺ蛹�ｽｽ�ｽ隲幄肩�ｿ�ｽ
			int x = character->getCenterX();
			int y = character->getCenterY();
			Animation* atariAnimation = objects[i]->createAnimation(x, y, 3);
			if (atariAnimation != nullptr) {
				m_animations.push_back(atariAnimation);
			}
			// 効果音
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			// HP = 0になったとき（やられたとき）
			if (!character->haveDeadGraph() && character->getHp() == 0) {
				m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
				m_camera->shakingStart(20, 20);
				m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
				if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && GetRand(100) < 20) {
					// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ蜥ｲ�ｩ繧托ｽｽ�ｸ�ｽ�ｭ驍ｵ�ｺ�ｽ�ｧ驍ｵ�ｺ�ｽ�ｪ驍ｵ�ｺ闔会ｽ｣�ｽ讙趣ｽｸ�ｺ�ｽ�ｰ鬩墓慣�ｽ�ｺ鬩搾ｿｽ�ｿ�ｽ邵ｲ蝣､�ｹ�ｧ�ｽ�｢驛｢�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�主､�ｸ�ｺ霑ｹ螟奇ｽｪ驍ｵ�ｺ�ｽ�｡驛｢�ｧ
					m_itemVector.push_back(new CureItem("cure", x, y, 50));
				}
			}
		}
		// deleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 髫ｴ蟷｢�ｽ�ｫ髯晢ｿｽ�ｽ�ｾ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ邇厄ｽｭ�ｽ�ｽ�ｹ驍ｵ�ｺ驕雋�∞�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ�ｽ陞｢�ｹ邵ｺ蜀暦ｽｹ譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｨ髫ｰ�ｽ�ｳ�ｨ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// 驛｢�ｧ�ｽ�ｹ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�ｫ鬨ｾ蜈ｷ�ｽ�ｺ髯ｷ蜥ｲ�ｩ繧托ｽｽ�ｸ�ｽ�ｭ驍ｵ�ｺ�ｽ�ｯ髫ｰ�ｽ�ｳ�ｨ遶頑･｢諤ｦ�ｽ�･驛｢�ｧ陟募ｨｯ�ｽ驍ｵ�ｺ
	if (m_skillFlag) { return; }

	// 髯橸ｽ｢遶丞､ｲ�ｽ�ｽ螳茨ｽｿ�ｫ邵ｺ讙趣ｽｹ譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ (髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ遶雁ｮ壽↑髴域喚�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// 髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ陞｢�ｹ�ｽ蝣､�ｸ�ｺ髦ｮ蜻ｻ�ｼ�ｽ�ｸ�ｺ�ｽ�ｧ鬮ｯ�ｦ陟募ｨｯ魘ｬ
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// 驛｢譎擾ｽｳ�ｨ邵ｺ�ｽ�ｸ�ｺ陋滂ｽ･�ｽ�ｽ�ｸ�ｺ�ｽ�ｪ驍ｵ�ｺ
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// 髯溷�萓ｭ隨ｳ�ｽ�ｸ�ｺ�ｽ�｣驍ｵ�ｺ雋�ｽｷ�ｽ�ｴ髯ｷ�ｷ 驛｢�ｧ�ｽ�ｨ驛｢譎｢�ｽ�ｪ驛｢�ｧ�ｽ�｢鬩募∞�ｽ�ｻ髯ｷ蟠趣ｽｼ謚ｫ�ｲ鬨ｾ蜈ｷ�ｽ�ｺ鬨ｾ
				m_nextAreaNum = objects[i]->getAreaNum();
				// 鬨ｾ蛹�ｽｽ�ｻ鬯ｮ�ｱ�ｽ�｢驛｢�ｧ陷ｻ莠･蟆磯ｫｴ�ｽ�ｽ�｢
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle�ｽ�ｽ陞｢�ｼ�ｽ�｣遶丞､ｲ�ｽ�ｽ<->髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ遯ｶ�ｲ髯橸ｽ｢遶乗劼�ｽ�ｺ驗呻ｽｫ遶頑･｢蜀ｶ萓ｭ隨ｳ�ｽ�ｸ�ｺ�ｽ�｣驍ｵ�ｺ�ｽ�ｦ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｸ�ｺ陷ｿ�･隲｢蟷�･
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// エフェクト作成
				Animation* atariAnimation = m_attackObjects[i]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// 髯橸ｽ｢遶乗劼�ｽ�ｺ驗呻ｽｫ�ｽ�ｽdeleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽdeleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｪ驛｢�ｧ霑壼衷�朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷ｷ�ｶ�ｽ
		if (m_attackObjects[i]->getDeleteFlag()) {
			createBomb(x, y, m_attackObjects[i]);
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ�ｽ陞｢�ｽ陋ｻ�､髫ｰ�ｦ<->髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ�ｽ�ｽ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// 髫ｰ�ｾ�ｽ�ｻ髫ｰ�ｦ�ｽ�ｽ遯ｶ�ｲ髯橸ｽ｢遶乗劼�ｽ�ｺ驗呻ｽｫ遶頑･｢蜀ｶ萓ｭ隨ｳ�ｽ�ｸ�ｺ�ｽ�｣驍ｵ�ｺ�ｽ�ｦ驍ｵ�ｺ�ｽ�ｽ�ｽ迢暦ｽｸ�ｺ陷ｿ�･隲｢蟷�･
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// エフェクト作成
				Animation* atariAnimation = m_attackObjects[j]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				createBomb(x, y, m_attackObjects[i]);
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
		}
	}
}

// Battle: 爆発を起こす
void World::createBomb(int x, int y, Object* attackObject) {
	if (attackObject->getBomb()) {
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

// 各キャラが目標地点へ移動するだけ 全員到達したらtrueを返す
bool World::moveGoalCharacter() {
	// deleteFlag驍ｵ�ｺ髣戊ｼブe驍ｵ�ｺ�ｽ�ｮ驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ螳壽≧闔ｨ竏晄ｱるし�ｺ陷ｷ�ｶ�ｽ迢暦ｽｸ�ｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ�ｽ�ｽ陜捺ｺｷ諢幃垈�ｦ�ｽ�ｽ�ｽ�ｯ�ｽ�ｾ鬮ｮ雜｣�ｽ�｡驍ｵ�ｺ�ｽ�ｮ髯樊ｺｽ蛻､陝ｲ�ｩ�ｽ�ｽ
	// 髣包ｽｳ驗呻ｽｫ邵ｲ蝣､�ｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ髮区ｨ抵ｽ朱ｬｮ�ｯ�ｽ�､驍ｵ�ｺ陷会ｽｱ隨ｳ�ｽ�ｸ�ｺ闕ｵ譎｢�ｽ闃ｽ�ｭ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ驍ｵ�ｺ陷会ｽｱ隨ｳ�ｽ�ｸ�ｺ闕ｵ譎｢�ｽ闃ｽ�｢�ｽ�ｽ�ｽ�ｦ
	updateCharacter();

	// 驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驛｢�ｧ�ｽ�ｯ驛｢�ｧ�ｽ�ｿ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	bool allCharacterAlreadyGoal = true;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HPが0ならスキップ
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) { 
			continue;
		}

		// 鬮ｯ�ｦ隰疲ｺｯ蜉帝匚鮃ｹ莠ゑｿｽ�ｽ髯ｷ�ｽ�ｽ�ｦ鬨ｾ
		controller->init();

		// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟遶雁､�ｸ�ｺ�ｽ�ｮ髯溷�萓ｭ隨ｳ�ｽ�ｹ�ｧ鬮ｮ竏ｵ�ｽ髯橸ｽｳ
		atariCharacterAndObject(controller, m_stageObjects);
		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects);
		}
		atariCharacterAndObject(controller, m_stageObjects); // 2回目呼ぶのは妥協案　1回目で斜面にいるかがわかり、それによって処理が変わるため2回目が必要

		// 鬨ｾ�ｶ�ｽ�ｮ髫ｶ轣倬它隲ｷ蟠趣ｽｽ�､�ｽ�ｹ驍ｵ�ｺ�ｽ�ｸ鬩募∞�ｽ�ｻ髯ｷ蟠趣ｽｼ謚ｫ�ｽ驛｢�ｧ陋ｹ�ｺ隴ｯ�ｽ謚 original驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譎冗樟�ｽ�ｽ驛｢譎�ｽｼ驥�㏍�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}

		// 髯ｷ�ｿ髢ｧ�ｴ闕ｳ original驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｸ螂�ｽｿ�ｽ驛｢譎冗樟�ｽ�ｽ驛｢譎�ｽｼ驥�㏍�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟遶雁､�ｸ�ｺ�ｽ�ｮ鬮ｮ蜈ｷ�ｽ�ｫ鬯ｨ�ｾ陞｢�ｼ隲｢蟷�･
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// 驛｢�ｧ�ｽ�ｪ驛｢譎�§邵ｺ螟ゑｽｹ�ｧ�ｽ�ｧ驛｢�ｧ�ｽ�ｯ驛｢譎冗樟�ｽ�ｽ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	controlObject();

	// 驛｢�ｧ�ｽ�｢驛｢�ｧ�ｽ�､驛｢譎｢�ｿ�ｽ�主､�ｸ�ｺ�ｽ�ｮ髯ｷ蟠趣ｽｼ謚ｫ�ｳ
	controlItem();

	// 驛｢�ｧ�ｽ�ｫ驛｢譎｢�ｽ�｡驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
	adjustBattleCamera();

	// 驛｢�ｧ�ｽ�ｵ驛｢�ｧ�ｽ�ｦ驛｢譎｢�ｽ�ｳ驛｢譎擾ｽｳ�ｨ�ｽ�ｽ驛｢譎｢�ｽ�ｬ驛｢�ｧ�ｽ�､驛｢譎｢�ｽ�､驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｮ驛｢譏懶ｽｻ�｣�趣ｽｦ鬮ｫ�ｪ�ｽ�ｭ髯橸ｽｳ陞溘ｉ闊
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 驛｢�ｧ�ｽ�｢驛｢譏懶ｽｹ譁滄豪�ｹ譎｢�ｽ�ｼ驛｢�ｧ�ｽ�ｷ驛｢譎｢�ｽ�ｧ驛｢譎｢�ｽ�ｳ驍ｵ�ｺ�ｽ�ｮ髫ｴ蜴�ｽｽ�ｴ髫ｴ�ｽ�ｽ�ｰ
	updateAnimation();

	return allCharacterAlreadyGoal;
}

bool World::dealBrightValue() {
	// 逕ｻ髱｢證苓ｻ｢荳ｭ 繧ｨ繝ｪ繧｢遘ｻ蜍輔°繝励Ξ繧､繝､繝ｼ繧�ｉ繧梧凾
	if (m_brightValue != 255 || playerDead()) {
		m_brightValue = max(0, m_brightValue - 10);
		if (!playerDead()) { return true; }
	}
	return false;
}

// 莨夊ｩｱ縺輔○繧
void World::talk() {
	if (m_conversation_p != nullptr) {
		updateCamera();
		m_conversation_p->play();
		// 髣費ｽｨ陞溷･�ｽｽ�ｩ�ｽ�ｱ鬩搾ｽｨ郢ｧ�ｽ�ｽ�ｺ
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// 驛｢驛｢譎｢�ｽ�ｼ驛｢譎∽ｾｭ�ｽ�ｽ驛｢�ｧ陷ｻ闌ｨ�ｽ�ｵ遶丞｣ｺ�ｽ
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// 驛｢驛｢譎｢�ｽ�ｼ驛｢譎∽ｾｭ�ｽ�ｽ鬩搾ｽｨ郢ｧ�ｽ�ｽ�ｺ
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}