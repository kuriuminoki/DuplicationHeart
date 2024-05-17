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
* 鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｲ蟶ｷ�ｿ�ｫ�ｽ�･鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ
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
* controllers_p鬩搾ｽｵ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣搾ｽｱlayer_p鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｽ陞ｳ螟ｲ�ｽ�ｿ陞滂ｽｲ�ｽ�ｽ�ｽ�ｹ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｲ驕ｶ謫ｾ�ｽ�ｵ�ｽ�ｽ�ｽ�ｬ�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｲ蟶ｷ�ｿ�ｫ�ｽ�･鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｿ髫ｴ竏ｫ�ｵ�ｶ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｲ
* 鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬮ｯ�ｷ闔�･霑ｴ�ｾ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｯID鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬮ｮ謇具ｽｶ�｣�ｽ�ｽ�ｽ�ｺ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｾ鬩幢ｽ｢�ｽ�ｧ髣包ｽｵ
* 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｻplayer鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮBrain鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｲ驕ｯ�ｶ�ｽ�ｦontrollers_p鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮBrain鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬮ｯ讖ｸ�ｽ�ｳ髯溷桁�ｽ�｡�ｽ�ｽ�ｽ�ｺ
*/
const Character* PlayerChanger::play(SoundPlayer* soundPlayer_p, std::vector<CharacterController*> controllers_p, const Character* player_p) {
	// E鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ髫ｴ�ｴ�ｽ�ｧ髫ｹ�ｺ�ｽ�ｾ鬩搾ｽｵ�ｽ�ｺ鬮ｴ驛�ｽｲ�ｻ�ｽ�ｽ隶呵ｶ｣�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｹ�ｽ�ｧ髫ｰ譴ｧ豎夲ｿｽ�ｽ�ｽ�ｽ鬮ｴ驛�ｽｲ�ｻ�ｽ�ｽ郢ｧ莨夲ｽｽ�ｸ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ
	if (controlE() != 1) {
		return nullptr;
	}

	const Character* minCharacter = nullptr;
	const Character* nextPlayer = nullptr;
	for (unsigned int i = 0; i < controllers_p.size(); i++) {
		const Character* target = controllers_p[i]->getAction()->getCharacter();
		// 鬮ｯ讖ｸ�ｽ�ｻ髫ｶ謚ｵ�ｽ�ｭ髫ｨ蛛�ｽｿ�ｽ�ｽ�ｹ�ｽ�ｧ髣包ｽｵ隴擾ｽｴ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢
		int groupId = target->getGroupId();
		// ID
		int id = target->getId();
		if (id == player_p->getId()) { continue; }
		// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｿ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｿ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡
		if (groupId == player_p->getGroupId()) {
			// ID鬩搾ｽｵ�ｽ�ｺ髫ｴ�ｴ�ｽ�ｧ髫ｲ�､陷ｻ�ｵ雎鯉ｽ｣髣包ｽｳ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ
			if (minCharacter == nullptr || id < minCharacter->getId()) {
				minCharacter = target;
			}
			// ID鬩搾ｽｵ�ｽ�ｺ髯滓坩�ｯ莨夲ｽｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｹ�ｽ�ｺ�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｸ陞ゑｿｽ�ｽ�ｼ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ
			if (id > player_p->getId()) {
				if (nextPlayer == nullptr || id < nextPlayer->getId()) {
					nextPlayer = target;
				}
			}
		}
	}
	// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ髫ｴ�ｴ�ｽ�ｧ髫ｲ�､陷･謫ｾ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｽD鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬩搾ｽｵ�ｽ�ｺ鬯ｮ�ｦ�ｽ�ｪ�ｽ�ｽ隶捺慣�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ髮具ｿｽ�ｽ�ｷ�ｽ�ｽ�ｽ�ｴ鬮ｯ�ｷ�ｽ�ｷ�ｽ�ｽ�ｽ�ｽullptr鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧID鬩搾ｽｵ�ｽ�ｺ髫ｴ�ｴ�ｽ�ｧ髫ｲ�､陷･謫ｾ�ｽ�ｹ�ｽ�ｧ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ髣包ｽｳ陞ゑｿｽ�ｽ�ｼ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯晢ｽｶ隴擾ｽｶ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｶ
	if (nextPlayer == nullptr) {
		nextPlayer = minCharacter;
	}
	return nextPlayer;
}


// vector鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�･鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ髮具ｿｽ�ｽ�ｷ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�･鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ髮趣ｿｽ�ｽ�ｸeleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌雁ｯｰrue鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 鬮ｫ�ｴ陝ｷ�｢�ｽ�ｽ�ｽ�ｫ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ驍�私�ｽ�ｭ�ｽ�ｽ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ鬩髮具ｿｽ竏橸ｿｽ�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�･鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ髮具ｿｽ�ｽ�ｷ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竊鷹ｫｴ蝓滓蛤�ｽ�ｰ鬩搾ｽｵ�ｽ�ｺ
void actionObject(vector<Object*>& objects) {
	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ陞ｳ闌ｨ�ｽ�ｿ�ｽ�ｫ驍ｵ�ｺ隶呵ｶ｣�ｽ�ｹ隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ (鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ驕ｶ髮�ｽｮ螢ｽ竊鷹ｫｴ蝓溷繭�ｽ�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
		objects[i]->action();
		// deleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 鬮ｫ�ｴ陝ｷ�｢�ｽ�ｽ�ｽ�ｫ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ驍�私�ｽ�ｭ�ｽ�ｽ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ鬩髮具ｿｽ竏橸ｿｽ�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬯ｮ�ｮ陷茨ｽｷ�ｽ�ｽ�ｽ�ｫ鬯ｯ�ｨ�ｽ�ｾ髯橸ｽ｢�ｽ�ｽ髯ｷ�ｽ�ｽ�ｾ鬮ｯ蜈ｷ�ｽ�ｻ�ｽ�ｽ�ｽ�､鬮ｯ讖ｸ�ｽ�ｳ
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ陞ｳ闌ｨ�ｽ�ｿ�ｽ�ｫ驍ｵ�ｺ隶呵ｶ｣�ｽ�ｹ隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ (鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ驕ｶ髮�ｽｮ螢ｽ竊鷹ｫｴ蝓溷繭�ｽ�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// 鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ陜｣�､�ｽ�ｸ�ｽ�ｺ鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｼ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬯ｮ�ｯ�ｽ�ｦ髯溷供�ｨ�ｯ鬲假ｽｬ
		objects[i]->penetration(controller);
	}
}


/*
* 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢隴主�讓滂ｿｽ荳ｻ�ｸ�ｷ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ
*/
World::World() {
	m_duplicationFlag = false;

	m_brightValue = 255;

	m_resetBgmFlag = false;

	m_blindFlag = false;

	// 闔ｨ螟奇ｽｩ�ｱ郢ｧ�､郢晏生ﾎｦ郢
	m_conversation_p = nullptr;
	m_objectConversation = nullptr;

	// 鬩幢ｽ｢鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴寂或�ｾ�ｭ�ｽ�ｽ�ｽ�ｽ
	m_movie_p = nullptr;

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ陷･�ｲ�ｽ�ｩ郢ｧ謇假ｽｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�ｭ
	m_skillFlag = false;

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ蛹ｺ�ｺ�ｷ髫ｱ�ｿ鬩肴得�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｴ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�､鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｧ�ｽ�ｽ�ｽ�｣鬮ｯ�ｷ陷托ｿｽ�ｽ�ｸ隶厄ｽｸ�ｽ�ｽ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩搾ｽｵ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣�ｽｽ�ｽ�ｱ髮懶ｽ｣�ｽ�ｽ�ｽ�ｺ鬮ｯ讖ｸ�ｽ�ｳ
	getGameEx(m_exX, m_exY);
	m_cameraMaxEx *= m_exX;
	m_cameraMinEx *= m_exX;

	m_areaLock = false;

	m_date = 0;

}

/*
* 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｩ
*/
World::World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) :
	World()
{

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｵ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｦ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ
	m_soundPlayer_p = soundPlayer;

	// 鬮｣蛹�ｽｽ�ｳ�ｽ�ｽ�ｽ�ｻ鬮｣�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｬ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴乗��ｽ�ｺ�ｽ�･髫ｲ�ｷ陝雜｣�ｽ�ｽ�ｽ�､�ｽ�ｽ�ｽ�ｹ
	m_areaNum = toAreaNum;
	m_nextAreaNum = m_areaNum;

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ髯句ｹ｢�ｽ�ｵ�ｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢
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

	// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ髯句ｹ｢�ｽ�ｵ驍ｵ�ｺ隴惹ｼ夲ｽｽ�ｹ隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			m_player_p = m_characters[i];
			break;
		}
	}

	m_playerChanger = new PlayerChanger(m_characterControllers, m_player_p);

	// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｹ鬮ｯ�ｷ�ｽ�ｷ髣比ｼ夲ｽｽ�｣驕ｶ蝓弱Γ隲�ｺ髣比ｼ夲ｽｽ�｣�ｽ繧托ｽｽ�ｰ鬩搾ｽｵ�ｽ�ｺ髯晢ｽｶ陷ｻ�ｻ�ｽ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		m_characterControllers[i]->setPlayerDirection(m_player_p, true);
	}

	m_camera->setEx(m_cameraMaxEx);

	m_characterDeadGraph = new GraphHandles("picture/effect/dead", 5, 1.0, 0, true);
	m_bombGraph = new GraphHandles("picture/effect/bomb", 9, 1.0, 0, true);
	m_characterDeadSound = LoadSoundMem("sound/battle/dead.wav");
	m_bombSound = LoadSoundMem("sound/battle/bomb.wav");
	m_doorSound = LoadSoundMem("sound/battle/door.wav");
	m_cameraInSound = LoadSoundMem("sound/battle/cameraIn.mp3");
	m_cameraOutSound = LoadSoundMem("sound/battle/cameraOut.mp3");

}

World::World(const World* original) :
	World()
{
	m_duplicationFlag = true;

	// 繧ｨ繝ｪ繧｢繧偵さ繝斐� (繝励Μ繝溘ユ繧｣繝門梛)
	m_areaNum = original->getAreaNum();
	m_focusId = original->getFocusId();
	m_playerId = original->getPlayerId();
	m_date = original->getDate();

	// 繧ｨ繝ｪ繧｢繧偵さ繝斐� (繧ｳ繝斐�蜈�→蜈ｱ譛峨☆繧九ｂ縺ｮ)
	m_soundPlayer_p = original->getSoundPlayer();
	m_characterDeadGraph = original->getCharacterDeadGraph();
	m_bombGraph = original->getBombGraph();
	m_characterDeadSound = original->getCharacterDeadSound();
	m_bombSound = original->getBombSound();
	m_doorSound = original->getDoorSound();
	m_backGroundGraph = original->getBackGroundGraph();
	m_backGroundColor = original->getBackGroundColor();

	// 譁ｰ隕丈ｽ懈�縺吶ｋ繧ゅ� (繝昴う繝ｳ繧ｿ縺悟､峨ｏ繧)
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
		// Brain鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨAction鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｿｽ�ｱ謚ｵ�ｿ�ｽ�ｽ�ｽ鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫCharacter鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ
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

	// 蛻晄悄險ｭ螳
	m_camera->setEx(m_cameraMaxEx);

}

World::~World() {
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
	delete m_camera;

	// 鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);
	deleteAllObject(m_doorObjects);

	// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ驍ｵ�ｺ鬯倩ｲｻ�ｽ�ｹ隴趣ｿｽ�ｽ�ｼ隴∫ｵｶ蜃ｾ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴乗��ｽ�ｺ�ｽ�･髴難ｽ､隴幢ｽｱ�ｽ�ｫ�ｽ�ｯ�ｽ�ｽ�ｽ�､
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ荳ｻ�ｮ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ
	for (unsigned i = 0; i < m_itemVector.size(); i++) {
		delete m_itemVector[i];
	}

	// 鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴主�讓滂ｿｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}

	// 鬮ｯ�ｷ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}

	delete m_playerChanger;

	// 鬯ｮ�｢�ｽ�ｭ髫ｴ�ｴ�ｽ�ｧ髯ｷ謳ｾ�ｽ�ｹ
	if (!m_duplicationFlag) {
		DeleteGraph(m_backGroundGraph);
		delete m_characterDeadGraph;
		delete m_bombGraph;
		DeleteSoundMem(m_characterDeadSound);
		DeleteSoundMem(m_bombSound);
		DeleteSoundMem(m_doorSound);
		DeleteSoundMem(m_cameraInSound);
		DeleteSoundMem(m_cameraOutSound);
	}

	if (m_objectConversation != nullptr) {
		delete m_objectConversation;
	}
}

// Drawer鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ髯ｷ�･�ｽ�ｾharacterAction鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮvector鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｿ髫ｴ竏ｫ�ｵ�ｶ�ｽ�ｽ
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

// Drawer鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ髯ｷ螟ｲ�ｽ�ｹbject鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮvector鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｿ髫ｴ竏ｫ�ｵ�ｶ�ｽ�ｽ
vector<const Object*> World::getFrontObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// Drawer鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ鬯陷肴ｻゑｽｽ�ｾ鬲�ｼ夲ｽｽ�ｽ陷･�ｲ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｫ�ｰ髯ｷ隲､諛茨ｿｽ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ髴��ject鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮvector鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｿ髫ｴ竏ｫ�ｵ�ｶ�ｽ�ｽ
vector<const Object*> World::getBackObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_doorObjects.begin(), m_doorObjects.end());

	return allObjects;
}

// Drawer鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ髯ｷ�ｩ�ｽ�ｯimation鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮvector鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｿ髫ｴ竏ｫ�ｵ�ｶ�ｽ�ｽ
vector<const Animation*> World::getConstAnimations() const {

	vector<const Animation*> allAnimations;

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ隴∫ｵｶ蜃ｾ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢
	allAnimations.insert(allAnimations.end(), m_animations.begin(), m_animations.end());

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		if (!m_itemVector[i]->getDeleteFlag()) {
			allAnimations.push_back(m_itemVector[i]->getAnimation());
		}
	}

	return allAnimations;
}

// 鬮ｯ�ｷ�ｽ�ｷ髫ｶ譛ｱ�｡謚ｫ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｫ�ｶ髫ｲ蟷｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｴ�ｽ�ｽ�ｽ�｢
Character* World::getCharacterWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == characterName) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// 鬮ｯ�ｷ�ｽ�ｷ髫ｶ譛ｱ�｡謚ｫ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴主�讓滂ｿｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｫ�ｶ髫ｲ蟷｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｴ�ｽ�ｽ�ｽ�｢
CharacterController* World::getControllerWithName(string characterName) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == characterName) {
			return m_characterControllers[i];
		}
	}
	return nullptr;
}

// ID鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｫ�ｶ髫ｲ蟷｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｴ�ｽ�ｽ�ｽ�｢
Character* World::getCharacterWithId(int id) const {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == id) {
			return m_characters[i];
		}
	}
	return nullptr;
}

// ID鬮ｫ�ｰ陷ｴ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｮ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｲ鬩墓賜ain鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ
void World::setBrainWithId(int id, Brain* brain) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			m_characterControllers[i]->setBrain(brain);
		}
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢�ｽ�ｧ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ驕擾ｽｩ隲ｱ�ｽ�ｿ�ｽ�ｽ�ｽ鬮ｯ�ｷ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ
void World::addCharacter(CharacterLoader* characterLoader) {
	pair<vector<Character*>, vector<CharacterController*> > p = characterLoader->getCharacters(m_camera, m_soundPlayer_p, m_areaNum);
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ
	m_characters.insert(m_characters.end(), p.first.begin(), p.first.end());
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴主�讓滂ｿｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ
	m_characterControllers.insert(m_characterControllers.end(), p.second.begin(), p.second.end());
}

// 驛｢�ｧ�ｽ�ｹ驛｢譎冗樟�ｽ�ｽ驛｢譎｢�ｽ�ｪ驛｢譎｢�ｽ�ｼ驍ｵ�ｺ�ｽ�ｫ驛｢�ｧ陋ｹ�ｻ�ｽ迢暦ｽｹ�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｮ髫ｲ�､�ｽ�ｧ鬮｢�ｭ�ｽ�ｽ髯樊ｺｽ逕･陜滂ｽｧ
void World::changeCharacterVersion(int version) {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->changeInfoVersion(version);
	}
}

void World::addObject(ObjectLoader* objectLoader) {
	pair<vector<Object*>, vector<Object*> > p = objectLoader->getObjects(m_areaNum);
	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ
	m_stageObjects.insert(m_stageObjects.end(), p.first.begin(), p.first.end());
	// 鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ
	m_doorObjects.insert(m_doorObjects.end(), p.second.begin(), p.second.end());
}

// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮHP鬩搾ｽｵ�ｽ�ｺ0鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髮趣ｽｸ�ｽ�ｲrue
bool World::playerDead() {
	return m_player_p->getHp() <= 0;
}

// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮHP鬩幢ｽ｢�ｽ�ｧ髫ｶ髮｣�ｽ�ｽAX鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void World::playerHpReset() {
	m_player_p->setHp(m_player_p->getMaxHp());
}


// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驛｢譎｢�ｽ�ｯ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陝ｾ�ｽreeze鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void World::setSkillFlag(bool skillFlag) { 
	m_skillFlag = skillFlag;
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") {
			m_characterControllers[i]->setCharacterFreeze(skillFlag);
		}
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯樊ｺｷ�･�ｽ�ｽ�ｽ�ｽ�､�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�｣�ｽ�ｽ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴取ｧｭﾎ難ｿｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ
void World::pushCharacter(Character* character, CharacterController* controller) {
	m_characters.push_back(character);
	m_characterControllers.push_back(controller);
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯樊ｺｷ�･�ｽ�ｽ�ｽ�ｽ�､�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�｣�ｽ�ｽ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴乗��ｽ�ｺ�ｽ�･髴難ｽ､隴幢ｽｱ�ｽ�ｫ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ
void World::popCharacterController(int id) {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") {
			continue;
		}
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == id) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
	// Target鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ驕ｯ�ｶ�ｽ�ｻ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ陝ｶ�ｰI鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｯ鬮ｯ讓奇ｽｻ阮卍ｧ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩幢ｽ｢�ｽ�ｧ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ髢ｾ�･�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧHP=0鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") {
			continue;
		}
		if (m_characters[i]->getId() == id) {
			m_characters[i]->setHp(0);
		}
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ髯滓誓�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｿ鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ髯晢ｽｶ隴取得�ｽ�ｹ隰碁托ｽｲ
void World::createRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") { continue; }
		m_characterControllers[i]->setStickRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setJumpRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSquatRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setSlashRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setBulletRecorder(new ControllerRecorder(0));
		m_characterControllers[i]->setDamageRecorder(new ControllerRecorder(0));
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陟托ｽｱ�ｽ邇門ｰ�ｿｽ�｣鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ陜陷ｻ�ｵ陝謌奇ｽｭ謫ｾ�ｽ�ｴ驕ｶ鬆托ｽ･�｢�ｽ�ｬ鬲�ｼ夲ｽｽ�ｽ�ｽ�ｻ鬩搾ｽｵ�ｽ�ｺ
void World::initRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") { continue; }
		m_characterControllers[i]->initRecorder();
	}
}

// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ隶朱托ｽ｡�ｽ�ｿ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣蜴�ｽｽ�ｴ�ｽ�ｽ�ｽ�ｿ鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ髯句ｹ｢�ｽ�ｵ�ｽ�ｽ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ驕ｶ荳橸ｽ｣�ｺ�ｽ�ｻ鬮ｯ�ｷ陷ｿ�ｰ�ｽ�ｼ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void World::eraseRecorder() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢") { continue; }
		m_characterControllers[i]->eraseRecorder();
	}
}

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｴ謇假ｽｽ�･�ｽ�ｽ�ｽ�ｶ鬮ｫ�ｲ�ｽ�ｷ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ陞ｳ螢ｽ�｣遘蛹�ｽｽ�ｻ髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ 鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｹ�ｽ�ｧ髫ｰ譴ｧ豎夲ｿｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
void World::asignedCharacterData(const char* name, CharacterData* data) {
	if (data->areaNum() == -1) { return; }
	size_t size = m_characters.size();
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｫ�ｽ�ｪ�ｽ�ｽ�ｽ�ｭ鬮ｯ讖ｸ�ｽ�ｳ
	bool flag = false;
	for (unsigned i = 0; i < size; i++) {
		if (name == m_characters[i]->getName()) {
			asignedCharacter(m_characters[i], data, data->areaNum() == m_areaNum);
			flag = true;
		}
	}
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ髢ｧ�ｲ隰泌ｹ�ｽｫ�ｫ驕ｨ繧托ｽｽ�ｸ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ驍�戟謐暦ｿｽ�ｽ�ｽ�ｴ鬮ｯ�ｷ�ｽ�ｷ鬮｣魃会ｽｽ�ｨ�ｽ�ｽ�ｽ�ｼ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｯ鬩搾ｽｵ�ｽ�ｺ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ鬩搾ｽｵ�ｽ�ｺ髯溷供�ｨ�ｯ驕ｨ螳｣縺暦ｿｽ�ｺ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ
	if (!flag && (data->areaNum() == m_areaNum || data->followName() == "鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢")) {
		Character* character = createCharacter(name);
		asignedCharacter(character, data, true);
		m_characters.push_back(character);
		CharacterController* controller = createControllerWithData(character, data);
		controller->setPlayerDirection(m_player_p, true);
		m_characterControllers.push_back(controller);
		return;
	}

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴主�讓滂ｿｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｲ驕ｶ荳橸ｽ｣�ｹ�ｽ讓｣ﾎ費ｿｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｷ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩搾ｽｵ�ｽ�ｲ驍ｵ�ｲ�ｽ�ｳain鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｫ�ｽ�ｪ�ｽ�ｽ�ｽ�ｭ鬮ｯ讖ｸ�ｽ�ｳ
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

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｴ謇假ｽｽ�･�ｽ�ｽ�ｽ�ｶ鬮ｫ�ｲ�ｽ�ｷ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ陞ｳ螟ｲ�ｽ�ｬ�ｽ�ｨ髯ｷ�ｷ�ｽ�ｶ驕ｶ謫ｾ�ｽ�ｴ鬩幢ｽ｢�ｽ�ｧ
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
				data->setY(c->getY() + c->getHeight()); // Y2鬮ｯ貅ｯ�ｶ�｣�ｽ�ｽ�ｽ�ｧ鬮ｫ�ｶ霓｣蛟｡蜃ｽ�ｽ�ｽ陞ｳ螢ｼ�ｰ�ｽ�ｭ蜿門ｾ暦ｿｽ�ｽ�ｽ�ｭ 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴守甥諢幃匚�ｽ�ｽ�ｾ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｯ鬯ｮ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｫ鬯ｯ�ｮ雋翫ｑ�ｽ�ｽ�ｽ�ｷ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬯ｮ�ｯ�ｽ�ｬ髫ｲ蟷�か�ｽ�ｽ�ｽ�ｭ�ｽ�ｽ�ｽ�｣
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

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯ｷ�･�ｽ�ｼoor鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｴ謇假ｽｽ�･�ｽ�ｽ�ｽ�ｶ鬮ｫ�ｲ�ｽ�ｷ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ陞ｳ螢ｽ�｣遘蛹�ｽｽ�ｻ髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ 鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｹ�ｽ�ｧ髫ｰ譴ｧ豎夲ｿｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
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

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯ｷ�･�ｽ�ｼoor鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｴ謇假ｽｽ�･�ｽ�ｽ�ｽ�ｶ鬮ｫ�ｲ�ｽ�ｷ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ陞ｳ螟ｲ�ｽ�ｬ�ｽ�ｨ髯ｷ�ｷ�ｽ�ｶ驕ｶ謫ｾ�ｽ�ｴ鬩幢ｽ｢�ｽ�ｧ
void World::asignDoorData(vector<DoorData*>& data, int fromAreaNum) const {
	size_t size = data.size();
	for (unsigned i = 0; i < m_doorObjects.size(); i++) {
		// 鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髯区ｻゑｽｽ�･�ｽ�ｽ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ
		if (m_doorObjects[i]->getAreaNum() == -1) { continue; }
		// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｿｽﾂｧ驛｢譎｢�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髫ｰ逍ｲ�ｻ繧托ｽｽ�ｽ�ｽ�ｭ髣費ｽｨ隲帛､ｷ蟶昴＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｺ
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
		if (!flag) { // 鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ鬩搾ｽｵ�ｽ�ｺ髮具ｿｽ�ｪ�ｽ�ｽ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髯晢ｽｲ�ｽ�ｨ驍ｵ�ｺ隴惹ｼ夲ｽｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｿｽﾂｧ驛｢譎｢�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬯ｮ�ｴ隰�∞�ｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ
			data.push_back(new DoorData(m_doorObjects[i]->getX1(), m_doorObjects[i]->getY1(),
				m_doorObjects[i]->getX2(), m_doorObjects[i]->getY2(),
				fromAreaNum, m_doorObjects[i]->getAreaNum(), m_doorObjects[i]->getFileName()));
		}
	}
}

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩搾ｽｵ�ｽ�ｺ髫ｴ謫ｾ�ｽ�ｴ�ｽ�ｽ�ｽ�ｽ鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｽ陜｣�､�ｽ�ｹ隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ魄�ｽｹ闔繧会ｽｫ莨�ｽｦ�ｴ�ゑｽｧ�ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ
void World::setPlayerOnDoor(int from) {
	int doorX1 = m_player_p->getX(), doorY2 = m_player_p->getY() + m_player_p->getHeight();
	for (unsigned int i = 0; i < m_doorObjects.size(); i++) {
		if (m_doorObjects[i]->getAreaNum() == from) {
			doorX1 = m_doorObjects[i]->getX1();
			doorY2 = m_doorObjects[i]->getY2();
		}
	}
	// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ
	m_player_p->setX(doorX1);
	m_player_p->setY(doorY2 - m_player_p->getHeight());

	// 鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｽ郢ｧ莉ｰ�ｧ�ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ
	setPlayerFollowerPoint();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
	cameraPointInit();
}

// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ髯懶ｽ｣�ｽ�､鬯ｮ�ｻ�ｽ�ｳ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ�ｽ�ｽ鬮ｯ貅ｯ�ｶ�｣�ｽ�ｽ�ｽ�ｧ鬮ｫ�ｶ霓｣蛟｡蜃ｽ驕ｶ蜀暦ｽ｣�ｯ�ゑｽｧ�ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ
void World::setPlayerPoint(CharacterData* characterData) {
	m_player_p->setX(characterData->x());
	m_player_p->setY(characterData->y() - m_player_p->getHeight());
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
	cameraPointInit();
}

// 鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｽ陜｣�､�ｽ�ｹ隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣蜴�ｽｽ�ｴ髯ｷ�･�ｽ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｮ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｸ鬯ｩ蜍溪��ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ
void World::setPlayerFollowerPoint() {
	// 鬩幢ｽ｢隴惹ｸ橸ｽｹ�ｲ�ｽ蜿厄ｽｨ謚ｵ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�ｲ鬯ｯ�ｮ�ｽ�｢
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		const Character* follow = m_characterControllers[i]->getBrain()->getFollow();
		// 鬯ｮ�ｴ隰�∞�ｽ�ｽ�ｽ�ｽ鬯ｮ�ｴ隰ｳ�ｾ�ｽ�ｽ�ｽ�｡鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ髯滓坩�ｯ莨夲ｽｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ
		if (follow != nullptr && m_playerId == follow->getId()) {
			// Controller鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬮ｯ貊ゑｽｽ�｢髫ｲ蟶幢ｽｲ�ｩ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ髮取｢ｧ譏ｭaracter鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｧ�ｫ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ
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

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷･�ｲ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮｣蜴�ｽｽ�ｴ髯ｷ�･�ｽ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ髯句ｹ｢�ｽ�ｵ�ｽ蜿悶渚�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
void World::cameraPointInit() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		if (m_characters[i]->getId() == m_focusId) {
			m_camera->setPoint(m_characters[i]->getCenterX(), m_characters[i]->getCenterY());
			break;
		}
	}
}

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ1鬮｣蜴�ｽｽ�ｴ鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｿ�ｽ�ｽ鬮ｫ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｱ鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｸ髣包ｽｵ雋奇ｿｽ�ｿ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ�ｽ�ｿ鬮｢�ｧ�ｽ�ｴ髣包ｽｳ
void World::asignedCharacter(Character* character, CharacterData* data, bool changePosition) {
	character->changeInfoVersion(data->version());
	if (data->id() != -1) {
		// 鬩搾ｽｵ�ｽ�ｺ鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｲ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬮ｯ蜈ｷ�ｽ�ｻ髫ｴ螳育櫨陷茨ｿｽ蜉茨ｿｽ�｣�ｽ�ｽ�ｽ�ｴ鬩搾ｽｵ�ｽ�ｺ髯区ｻゑｽｽ�･�ｽ�ｽ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ
		character->setHp(data->hp());
	}
	character->setInvincible(data->invincible());
	character->setGroupId(data->groupId());
	if (changePosition) {
		character->setX(data->x());
		// Y鬮ｯ貅ｯ�ｶ�｣�ｽ�ｽ�ｽ�ｧ鬮ｫ�ｶ霓｣蛟｡蜃ｽ�ｽ�ｽ�ｽ�ｽ鬯ｮ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｫ鬯ｯ�ｮ雋翫ｑ�ｽ�ｽ�ｽ�ｷ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ�ｽ�ｷ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ陷證ｦ�ｽ�ｸ�ｽ�ｺ髯晢ｽｶ陷ｷ�ｮ�ｽ�ｻ鬯ｮ�ｫ�ｽ�ｱ�ｽ�ｽ�ｽ�ｿ鬮ｫ�ｰ�ｽ�ｨ�ｽ�ｽ�ｽ�ｴ
		character->setY(data->y() - character->getHeight());
	}
}

// 鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬯ｩ髦ｪ�托ｿｽ�ｽ�ｽ�｡鬯ｨ�ｾ�ｽ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ�ｽ�ｼ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ隲ｷ蛹�ｽｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴主�讓滂ｿｽ蜿厄ｽｺ�ｽ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ1鬮ｯ蛹ｺ�ｻ莨夲ｽｽ�ｹ隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ鬮ｫ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｱ鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｸ髣包ｽｵ雋奇ｿｽ�ｿ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬮ｯ�ｷ�ｽ�ｿ鬮｢�ｧ�ｽ�ｴ髣包ｽｳ
CharacterController* World::createControllerWithData(const Character* character, CharacterData* data) {
	size_t size = m_characters.size();
	// Action鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ
	CharacterAction* action = nullptr;
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == character->getName()) {
			action = createAction(data->actionName(), m_characters[j], data->soundFlag() ? m_soundPlayer_p : nullptr);
			break;
		}
	}
	// Brain鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ
	Brain* brain = createBrain(data->brainName(), m_camera);
	brain->setCharacterAction(action);
	string follow = data->followName();
	for (unsigned int j = 0; j < size; j++) {
		if (m_characters[j]->getName() == follow) {
			brain->searchFollow(m_characters[j]);
			break;
		}
	}
	// Controller鬩幢ｽ｢�ｽ�ｧ髯ｷ謇假ｽｿ�ｽ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ
	return createController(data->controllerName(), brain, action);
}

/*
*  鬮ｫ�ｰ鬲�ｼ夲ｽｽ�ｽ�ｽ�ｦ鬩幢ｽ｢�ｽ�ｧ髣包ｽｳ陝ｯ�ｩ鬮ｮ�ｷ鬩幢ｽ｢�ｽ�ｧ
*/
void World::battle() {
	if (!m_soundPlayer_p->checkBGMplay()) {
		m_soundPlayer_p->playBGM();
	}
	
	// 騾包ｽｻ鬮ｱ�｢隴芽挙�ｻ�｢陷�ｽｦ騾
	if (dealBrightValue()) { return; }

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陝ｶ譎乗套�ｽ�ｽ�ｽ�ｿ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ髮矩｡板ｧ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驍ｵ�ｺ陷證ｦ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢
	if (m_objectConversation != nullptr) {
		m_objectConversation->play();
		// 鬮｣雋ｻ�ｽ�ｨ髯樊ｺｷ�･�ｽ�ｽ�ｽ�ｽ�ｩ�ｽ�ｽ�ｽ�ｱ鬯ｩ謳ｾ�ｽ�ｨ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ�ｽ�ｺ
		if (m_objectConversation->getFinishFlag()) {
			delete m_objectConversation;
			m_objectConversation = nullptr;
		}
		return;
	}

	// deleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ�ｽ�ｽ�ｽ�ｽ髯懈瑳�ｺ�ｷ隲｢蟷�梭�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｯ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ�ｽ�ｽ�ｽ�ｽ
	// 鬮｣蛹�ｽｽ�ｳ鬩怜遜�ｽ�ｫ驍ｵ�ｲ陜｣�､�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣�ｽｽ�ｽ�ｭ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣�ｽｽ�ｽ�｢�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ
	updateCharacter();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	controlCharacter();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	controlObject();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ荳ｻ�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	controlItem();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
	adjustBattleCamera();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｵ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｦ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｻ�ｽ�｣�ｽ雜｣�ｽ�ｦ鬯ｮ�ｫ�ｽ�ｪ�ｽ�ｽ�ｽ�ｭ鬮ｯ讖ｸ�ｽ�ｳ髯樊ｺ假ｽ蛾頼
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢隴乗��ｽ�ｹ隴∵ｻ�ｱｪ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｷ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
	updateAnimation();

	// 鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｲ蟶ｷ�ｿ�ｫ�ｽ�･鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ
	changePlayer(m_playerChanger->play(m_soundPlayer_p, m_characterControllers, m_player_p));

}

void World::changePlayer(const Character* nextPlayer) {
	if (nextPlayer == nullptr) { return; }
	// 鬮｣遒大ｴ髫ｨ�ｬ�ｽ�｡�ｽ�ｽ隰夲ｿｽ�ｫ蟶幢ｽ･�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｫ�ｱ�ｽ�ｰPC鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮBrain鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getId() == m_player_p->getId()) {
			m_characterControllers[i]->setBrain(createBrain(m_playerChanger->getPrevBrainName(), m_camera));
			m_characterControllers[i]->setActionSound(nullptr);
			break;
		}
	}
	// 鬮ｫ�ｹ�ｽ�ｺ�ｽ�ｽ�ｽ�｡鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｲ蟶幢ｽｲ�ｩ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ髣包ｽｵ隴擾ｽｴ�ｽ�･鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髴托ｽ｢�ｽ�ｸeyboardBrain鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｫ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (nextPlayer->getId() == m_characterControllers[i]->getAction()->getCharacter()->getId()) {
			string brainName = m_characterControllers[i]->getBrain()->getBrainName();
			m_characterControllers[i]->setBrain(new KeyboardBrain(m_camera));
			m_playerChanger->setPrevBrainName(brainName);
			m_characterControllers[i]->setActionSound(m_soundPlayer_p);
			break;
		}
	}
	// 鬮ｫ�ｹ�ｽ�ｺ�ｽ�ｽ�ｽ�｡鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ髫ｲ蟶幢ｽｲ�ｩ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ髣包ｽｵ隴擾ｽｴ�ｽ�･鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯ｷ鬘假ｽｩ�ｫayer鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ驕ｯ�ｶ�ｽ�ｻ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
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

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷･�ｲ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
void World::updateCamera(int gx, int gy, double gex) {

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ陜捺ｻ会ｽｺ莨∃費ｿｽ�ｧ髯晢ｽｲ�ｽ�ｨ髫ｨ
	m_camera->shaking();

	// 鬮ｫ�ｲ�ｽ�｡�ｽ�ｽ�ｽ�｡鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬯ｩ謳ｾ�ｿ�ｽ�ｽ�ｿ�ｽ�ｽ驍ｵ�ｺ隴惹ｼ夲ｽｽ�ｹ隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
	m_camera->setEx(gex);

	// 鬯ｨ�ｾ�ｽ�ｶ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｶ霓｣莨懶ｽｧ髮｣�ｿ�ｽ�ｽ�ｽ髯ｷ�･�ｽ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｻ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ驛｢譎｢�ｽ�ｨ
	m_camera->setGPoint(gx, gy);
	
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ髯懶ｽ｣�ｽ�､髯晢ｽｯ�ｽ�ｼ鬮ｫ�ｶ霓｣莨懶ｽｧ髮｣�ｿ�ｽ�ｽ�ｽ髯ｷ�･�ｽ�ｲ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｮ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｸ鬯ｮ�ｴ陷ｿ�ｰ�ｽ�ｻ�ｽ�｣髫ｨ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ髣比ｼ夲ｽｽ�｣�ｽ�ｽ
	m_camera->move();
}

// Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｽ髯晏��ｽ�ｶ鬯ｯ�ｮ�ｽ�｣髯区ｻゑｽｿ�ｽ�ｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�ｭ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ
void World::adjustBattleCamera() {

	// 鬯ｨ�ｾ�ｽ�ｶ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｶ霓｣蛟ｬ螳�ｿｽ�ｽ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬮ｫ�ｶ
	int gx = 0, gy = 0;
	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｴ髢ｧ�ｴ髴趣ｽｨ髯橸ｽｻ�ｽ�ｬ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬮ｯ蛹ｺ�ｻ繧托ｽｽ�ｽ�ｽ�､鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｪ�ｽ�ｽ�ｽ�ｿ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ
	int max_dx = 0, max_dy = 0;
	// 鬨ｾ蛹�ｽｽ�ｻ鬯ｮ�ｱ�ｽ�｢髯ｷ�ｽ�ｽ遶頑･｢諤ｦ�ｽ�･驛｢�ｧ陟暮ｯ会ｽｽ閧ｲ�ｸ�ｺ�ｽ�ｽ遶雁､�ｸ�ｺ陷ｷ�ｶ�ｽ遏ｩ諱ｪ隴取ｧｫ�ｱ�ｬ驍ｵ�ｺ�ｽ�ｮ髫ｴ髯樊ｻゑｽｽ�ｧ驍ｵ�ｲ驍ｵ�ｺ髦ｮ蜻ｻ�ｽ讙趣ｽｹ�ｧ陋ｹ�ｻ�ｽ莨�ｽｫ�ｮ�ｽ�｢驛｢�ｧ陟募ｨｯ陞ｺ驛｢�ｧ�ｽ�ｭ驛｢譎｢�ｽ�｣驛｢譎｢�ｽ�ｩ驍ｵ�ｺ�ｽ�ｯ髴取ｻゑｽｽ�｡鬮ｫ
	const int MAX_DISABLE = 2000;
	size_t size = m_characters.size();
	for (unsigned int i = 0; i < size; i++) {
		// 鬮｣遒托ｿｽ�ｽ�ｿ�ｽ�ｫ驛｢譎｢�ｽ�ｵ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ驕ｯ�ｶ�ｽ�ｻ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ貅ｯ�ｶ�｣�ｽ�ｽ�ｽ�ｧ鬮ｫ�ｶ霓｣蛟｡蜃ｽ驕ｶ鬆托ｽ･�｢隲�ｺ髯具ｽｹ�ｽ�ｻ�ｽ�ｽ陷證ｦ�ｽ�ｸ�ｽ�ｺ髯晢ｽｶ陷ｻ�ｻ�ｽ�ｽ
		if (m_focusId == m_characters[i]->getId()) {
			m_camera->setGPoint(m_characters[i]->getAtariCenterX(), m_characters[i]->getAtariCenterY());
		}
		// 鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ隴�搨�ｰ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ驕ｯ�ｶ�ｽ�ｻ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬮｣豈費ｽｼ螟ｲ�ｽ�ｽ�ｽ�･鬮ｯ讓奇ｽｻ阮卍ｧ驕ｶ莨�ｿｽ�ｽ�ｹ�ｽ�ｧ鬮｣�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｷ髫ｴ蜿厄ｽｧ�ｫ�ｽ�ｱ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ髯橸ｽｳ陞滂ｽｲ�ｽ�ｽ�ｽ�ｪ�ｽ�ｽ�ｽ�ｿ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ
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

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｲ�ｽ�｡�ｽ�ｽ�ｽ�｡鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｻ鬯ｩ謳ｾ�ｽ�ｵ�ｽ�ｽ�ｽ�ｮ鬮ｯ
	// 鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬩搾ｽｵ�ｽ�ｺ鬯ｮ�ｦ�ｽ�ｪ�ｽ�ｽ�ｽ�･鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ驍�ｽ�｢�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ驕ｶ荳橸ｽ｣�ｺ�ｽ�ｲ鬩搾ｽｵ�ｽ�ｺ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ驍�戟謐暦ｿｽ�ｽ�ｽ�ｴ鬮ｯ�ｷ�ｽ�ｷ髯具ｽｹ�ｽ�ｻ驕ｶ髮｣�ｿ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｲ驕ｶ荵怜款�ｽ�ｽ�ｽ�､�ｽ�ｽ�ｽ�ｧ鬩搾ｽｵ�ｽ�ｺ鬯ｮ�ｦ�ｽ�ｪ�ｽ�ｽ�ｽ�･鬮ｫ�ｲ�ｽ�｡�ｽ�ｽ�ｽ�｡鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ鬯ｩ謳ｾ�ｿ�ｽ�ｽ�ｿ�ｽ�ｽ�ｽ�ｽ陞ｳ螢ｽ�｣遘蛹�ｽｽ�ｻ髯晢ｽｲ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	double nowEx = m_camera->getEx();
	double gex = nowEx;
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
			// 鬯ｩ謳ｾ�ｽ�ｵ�ｽ�ｽ�ｽ�ｮ鬮ｯ
			double d = double(max(max_dx - nowWide, max_dy - nowHeight));
			m_camera->setEx(nowEx - min(0.1, d / 100000) * m_exX);
		}
		else if (nowEx < m_cameraMaxEx && (max_dx < nowWide && max_dy < nowHeight)) {
			// 鬮ｫ�ｲ�ｽ�｡�ｽ�ｽ�ｽ�｡鬮ｯ讓奇ｽｻ繧托ｽｽ�ｽ�ｽ�ｧ
			double d = double(max(nowWide - max_dx, nowHeight - max_dy));
			m_camera->setEx(nowEx + min(0.005, d / 100000) * m_exX);
		}
	}

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
	updateCamera(gx, gy, gex);
}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｹ隴乗��ｽ�ｹ隴∵ｻ�ｱｪ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｷ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
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

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ�ｽ�ｽ�ｽ�ｽ髯懈瑳�ｺ�ｷ隲｢蟷�梭�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｯ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ�ｽ�ｽ�ｽ�ｽ
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｫ髯ｬ諛亥合�ｽ�ｽ�ｽ�ｫ髣包ｽｵ隴擾ｽｴ陞ｳ螟ｲ�ｽ�ｬ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｯ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) {
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP縺0縺ｪ繧峨せ繧ｭ繝��
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) {
			continue;
		}

		// 鬯ｮ�ｯ�ｽ�ｦ髫ｰ逍ｲ�ｺ�ｯ陷牙ｸ晏建魄�ｽｹ闔繧托ｽｿ�ｽ�ｽ�ｽ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ
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

		// 鬮ｫ�ｰ�ｽ�ｫ髯懶ｽ｣�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ original鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ鬩･�ｽ繽搾ｿｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->control();
			controller->setPlayerDirection(m_player_p);
		}

		// 鬮ｯ譎｢�ｿ�ｽ�ｽ�ｿ�ｽ�ｽ鬮｣魃会ｽｽ�ｶ鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != nullptr) { m_attackObjects.push_back(bulletAttack); }

		// 鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｬ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ髯具ｽｻ�ｽ�､鬮ｫ�ｰ�ｽ�ｦ
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != nullptr) { m_attackObjects.push_back(slashAttack); }

		// 鬮ｯ�ｷ�ｽ�ｿ鬮｢�ｧ�ｽ�ｴ髣包ｽｳ original鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ鬩･�ｽ繽搾ｿｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滄�髮�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｮ陷茨ｽｷ�ｽ�ｽ�ｽ�ｫ鬯ｯ�ｨ�ｽ�ｾ髯橸ｽ｢�ｽ�ｼ髫ｲ�｢陝ｷ�ｽ�･
		penetrationCharacterAndObject(controller, m_stageObjects);
	}
}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ隶呵ｶ｣�ｽ�ｹ隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
void World::controlObject() {
	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ陞ｳ闌ｨ�ｽ�ｿ�ｽ�ｫ�ｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	actionObject(m_stageObjects);

	// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬮ｦ�ｮ陷ｷ�ｮ髯橸ｽｺ鬩幢ｽ｢�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	actionObject(m_attackObjects);

	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ<->鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
	atariStageAndAttack();

	// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ<->鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
	atariAttackAndAttack();
}

// Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ荳ｻ�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
void World::controlItem() {
	for (unsigned int i = 0; i < m_itemVector.size(); i++) {
		// 鬮ｯ�ｷ�ｽ�ｿ鬯ｮ�｢�ｽ�ｽ�ｽ�ｾ鬩搾ｿｽ�ｽ�ｲ�ｽ�ｽ�ｽ�ｸ髯具ｽｹ�ｽ�ｻ驕ｶ謫ｾ�ｽ�ｩ
		if (m_itemVector[i]->getDeleteFlag()) {
			// 鬮ｯ�ｷ闔ｨ螟ｲ�ｽ�ｽ�ｽ�ｹ鬮ｫ�ｴ�ｽ�ｫ髫ｲ讖ｸ�ｽ�ｺ髮趣ｽｬ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ髫ｰ逍ｲ�ｻ繧托ｽｽ�ｿ�ｽ�ｽ鬯ｨ�ｾ陟�屮�ｽ�ｻ闔ｨ螟ｲ�ｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�ｭ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ驕ｶ莨�ｿｽ�ｽ�ｹ�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､
			if (CheckSoundMem(m_itemVector[i]->getSound()) == 0) {
				delete m_itemVector[i];
				m_itemVector[i] = m_itemVector.back();
				m_itemVector.pop_back();
				i--;
			}
			continue;
		}
		// 鬮ｯ蜈ｷ�ｽ�ｻ髫ｴ雜｣�ｿ�ｽ�ｽ�ｽ蛻ｹ�ｽ�ｹ
		m_itemVector[i]->init();
		// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荵怜款�ｽ�ｽ�ｽ�ｺ鬩怜遜�ｽ�ｫ驕ｶ髮�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
			m_itemVector[i]->getPoint(&x1, &y1, &x2, &y2);
			if (m_stageObjects[j]->atariDropBox(x1, y1, x2, y2, m_itemVector[i]->getVx(), m_itemVector[i]->getVy())) {
				m_itemVector[i]->setGrand(true);
				m_itemVector[i]->setY(m_stageObjects[j]->getY(m_itemVector[i]->getX()));
			}
		}
		// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
		if (m_itemVector[i]->atariCharacter(m_player_p)) {
			m_soundPlayer_p->pushSoundQueue(m_itemVector[i]->getSound());
		}
		// 鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
		m_itemVector[i]->action();
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
			// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ隴∫ｵｶ蜃ｾ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴乗��ｽ�ｺ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｽ髫ｲ蟷�か�ｽ�ｿ�ｽ�ｽ
			int x = character->getCenterX();
			int y = character->getCenterY();
			Animation* atariAnimation = objects[i]->createAnimation(x, y, 3);
			if (atariAnimation != nullptr) {
				m_animations.push_back(atariAnimation);
			}
			// 蜉ｹ譫憺浹
			int soundHandle = objects[i]->getSoundHandle();
			int panPal = adjustPanSound(x, m_camera->getX());
			m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			// HP = 0縺ｫ縺ｪ縺｣縺溘→縺搾ｼ医ｄ繧峨ｌ縺溘→縺搾ｼ
			if (!character->haveDeadGraph() && character->getHp() == 0) {
				m_animations.push_back(new Animation(x, y, 3, m_characterDeadGraph));
				m_camera->shakingStart(20, 20);
				m_soundPlayer_p->pushSoundQueue(m_characterDeadSound, panPal);
				if (!m_duplicationFlag && character->getGroupId() != m_player_p->getGroupId() && GetRand(100) < 20) {
					// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ陷･�ｲ�ｽ�ｩ郢ｧ謇假ｽｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�ｭ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ髣比ｼ夲ｽｽ�｣�ｽ�ｽ隶呵ｶ｣�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｰ鬯ｩ蠅捺��ｽ�ｽ�ｽ�ｺ鬯ｩ謳ｾ�ｿ�ｽ�ｽ�ｿ�ｽ�ｽ驍ｵ�ｲ陜｣�､�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ荳ｻ�､�ｽ�ｸ�ｽ�ｺ髴托ｽｹ陞溷･�ｽｽ�ｪ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�｡鬩幢ｽ｢�ｽ�ｧ
					m_itemVector.push_back(new CureItem("cure", x, y, 50));
				}
			}
		}
		// deleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// 鬮ｫ�ｴ陝ｷ�｢�ｽ�ｽ�ｽ�ｫ鬮ｯ譎｢�ｿ�ｽ�ｽ�ｽ�ｽ�ｾ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ驍�私�ｽ�ｭ�ｽ�ｽ�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ鬩髮具ｿｽ竏橸ｿｽ�ｼ
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｹ驍ｵ�ｺ陷證ｦ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｨ鬮ｫ�ｰ�ｽ�ｽ�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ隶呵ｶ｣�ｽ�ｹ隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
void World::atariCharacterAndDoor(CharacterController* controller, vector<Object*>& objects) {

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｹ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｫ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬮ｯ�ｷ陷･�ｲ�ｽ�ｩ郢ｧ謇假ｽｽ�ｽ�ｽ�ｸ�ｽ�ｽ�ｽ�ｭ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｯ鬮ｫ�ｰ�ｽ�ｽ�ｽ�ｳ�ｽ�ｨ驕ｶ鬆托ｽ･�｢隲､�ｦ�ｽ�ｽ�ｽ�･鬩幢ｽ｢�ｽ�ｧ髯溷供�ｨ�ｯ�ｽ�ｽ鬩搾ｽｵ�ｽ�ｺ
	if (m_skillFlag) { return; }

	// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ陞ｳ闌ｨ�ｽ�ｿ�ｽ�ｫ驍ｵ�ｺ隶呵ｶ｣�ｽ�ｹ隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ (鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ驕ｶ髮�ｽｮ螢ｽ竊鷹ｫｴ蝓溷繭�ｽ�ｳ)
	for (unsigned int i = 0; i < objects.size(); i++) {
		if (m_areaLock) {
			objects[i]->setTextDisp(false);
			continue;
		}
		// 鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ髯橸ｽ｢�ｽ�ｹ�ｽ�ｽ陜｣�､�ｽ�ｸ�ｽ�ｺ鬮ｦ�ｮ陷ｻ�ｻ�ｽ�ｼ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｧ鬯ｮ�ｯ�ｽ�ｦ髯溷供�ｨ�ｯ鬲假ｽｬ
		if (objects[i]->atari(controller) && controller->getActionKey()) {
			if (objects[i]->getAreaNum() == -1) {
				// 鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ驍ｵ�ｺ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髯区ｻゑｽｽ�･�ｽ�ｽ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩搾ｽｵ�ｽ�ｺ
				if (objects[i]->getTextNum() != -1) {
					m_objectConversation = new Conversation(objects[i]->getTextNum(), this, m_soundPlayer_p);
				}
			}
			else {
				// 鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ髮具ｿｽ�ｽ�ｷ�ｽ�ｽ�ｽ�ｴ鬮ｯ�ｷ�ｽ�ｷ 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｨ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬯ｩ蜍溪��ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｲ鬯ｨ�ｾ陷茨ｽｷ�ｽ�ｽ�ｽ�ｺ鬯ｨ�ｾ
				m_nextAreaNum = objects[i]->getAreaNum();
				// 鬯ｨ�ｾ陋ｹ�ｽ�ｽ�ｽ�ｽ�ｻ鬯ｯ�ｮ�ｽ�ｱ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ闔�･陝�｣ｯ�ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�｢
				m_brightValue--;
				m_soundPlayer_p->pushSoundQueue(m_doorSound);
			}
		}
	}

}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｼ�ｽ�ｽ�ｽ�｣驕ｶ荳橸ｽ､�ｲ�ｽ�ｽ�ｽ�ｽ<->鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
void World::atariStageAndAttack() {
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ驕ｯ�ｶ�ｽ�ｲ鬮ｯ讖ｸ�ｽ�｢驕ｶ荵怜款�ｽ�ｽ�ｽ�ｺ鬩怜遜�ｽ�ｫ驕ｶ鬆托ｽ･�｢陷�ｶ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｺ髯ｷ�ｿ�ｽ�･髫ｲ�｢陝ｷ�ｽ�･
			if (m_stageObjects[j]->atariObject(m_attackObjects[i])) {
				// 繧ｨ繝輔ぉ繧ｯ繝井ｽ懈�
				Animation* atariAnimation = m_attackObjects[i]->createAnimation(x, y, 3);
				if (atariAnimation != nullptr) {
					m_animations.push_back(atariAnimation);
				}
				int soundHandle = m_attackObjects[i]->getSoundHandle();
				int panPal = adjustPanSound(x, m_camera->getX());
				m_soundPlayer_p->pushSoundQueue(soundHandle, panPal);
			}
			// 鬮ｯ讖ｸ�ｽ�｢驕ｶ荵怜款�ｽ�ｽ�ｽ�ｺ鬩怜遜�ｽ�ｫ�ｽ�ｽ�ｽ�ｽdeleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽdeleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢�ｽ�ｧ髴大｣ｼ陦ｷ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ
		if (m_attackObjects[i]->getDeleteFlag()) {
			createBomb(x, y, m_attackObjects[i]);
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

//  Battle�ｽ�ｽ�ｽ�ｽ髯橸ｽ｢�ｽ�ｽ髯具ｽｻ�ｽ�､鬮ｫ�ｰ�ｽ�ｦ<->鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ鬮ｯ貅ｷ�ｽ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｹ�ｽ�ｧ鬯ｮ�ｮ遶擾ｽｵ�ｽ�ｽ鬮ｯ讖ｸ�ｽ�ｳ
void World::atariAttackAndAttack() {
	if (m_attackObjects.size() == 0) { return; }
	for (unsigned int i = 0; i < m_attackObjects.size() - 1; i++) {
		int x = m_attackObjects[i]->getCenterX();
		int y = m_attackObjects[i]->getCenterY();
		for (unsigned int j = i + 1; j < m_attackObjects.size(); j++) {
			// 鬮ｫ�ｰ�ｽ�ｾ�ｽ�ｽ�ｽ�ｻ鬮ｫ�ｰ�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ驕ｯ�ｶ�ｽ�ｲ鬮ｯ讖ｸ�ｽ�｢驕ｶ荵怜款�ｽ�ｽ�ｽ�ｺ鬩怜遜�ｽ�ｫ驕ｶ鬆托ｽ･�｢陷�ｶ關難ｽｭ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�｣鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｦ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｺ髯ｷ�ｿ�ｽ�･髫ｲ�｢陝ｷ�ｽ�･
			if (m_attackObjects[j]->atariObject(m_attackObjects[i])) {
				// 繧ｨ繝輔ぉ繧ｯ繝井ｽ懈�
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

// Battle: 辷�匱繧定ｵｷ縺薙☆
void World::createBomb(int x, int y, Object* attackObject) {
	if (attackObject->getBomb()) {
		// 辷�匱
		BombObject* bomb = new BombObject(x, y, 500, 500, attackObject->getDamage(), new Animation(x, y, 3, m_bombGraph));
		bomb->setCharacterId(attackObject->getCharacterId());
		bomb->setGroupId(attackObject->getGroupId());
		m_attackObjects.push_back(bomb);
		// 蜉ｹ譫憺浹
		m_soundPlayer_p->pushSoundQueue(m_bombSound, adjustPanSound(x, m_camera->getX()));
		// 逕ｻ髱｢縺ｮ謠ｺ繧
		m_camera->shakingStart(20, 20);
	}
}

// 蜷�く繝｣繝ｩ縺檎岼讓吝慍轤ｹ縺ｸ遘ｻ蜍輔☆繧九□縺 蜈ｨ蜩｡蛻ｰ驕斐＠縺溘ｉtrue繧定ｿ斐☆
bool World::moveGoalCharacter() {
	// deleteFlag鬩搾ｽｵ�ｽ�ｺ鬮｣謌奇ｽｼ繝貌鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ陞ｳ螢ｽ竕ｧ髣費ｽｨ遶乗刋�ｱ繧九＠�ｽ�ｺ髯ｷ�ｷ�ｽ�ｶ�ｽ�ｽ霑｢證ｦ�ｽ�ｸ�ｽ�ｲ
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ�ｽ�ｽ�ｽ�ｽ髯懈瑳�ｺ�ｷ隲｢蟷�梭�ｽ�ｦ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｯ�ｽ�ｽ�ｽ�ｾ鬯ｮ�ｮ髮懶ｽ｣�ｽ�ｽ�ｽ�｡鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ讓奇ｽｺ�ｽ陋ｻ�､髯晢ｽｲ�ｽ�ｩ�ｽ�ｽ�ｽ�ｽ
	// 鬮｣蛹�ｽｽ�ｳ鬩怜遜�ｽ�ｫ驍ｵ�ｲ陜｣�､�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ鬮ｮ蛹ｺ�ｨ謚ｵ�ｽ譛ｱ�ｬ�ｮ�ｽ�ｯ�ｽ�ｽ�ｽ�､鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣�ｽｽ�ｽ�ｭ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ鬩搾ｽｵ�ｽ�ｺ髯ｷ莨夲ｽｽ�ｱ髫ｨ�ｳ�ｽ�ｽ�ｽ�ｸ�ｽ�ｺ髣包ｽｵ隴趣ｽ｢�ｽ�ｽ髣�ｽｽ�ｽ�｢�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ
	updateCharacter();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｭ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｣鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｿ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	bool allCharacterAlreadyGoal = true;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];

		// HP縺0縺ｪ繧峨せ繧ｭ繝��
		if (controller->getAction()->getCharacter()->getHp() == 0 && !controller->getAction()->getCharacter()->haveDeadGraph()) { 
			continue;
		}

		// 鬯ｮ�ｯ�ｽ�ｦ髫ｰ逍ｲ�ｺ�ｯ陷牙ｸ晏建魄�ｽｹ闔繧托ｽｿ�ｽ�ｽ�ｽ鬮ｯ�ｷ�ｽ�ｽ�ｽ�ｽ�ｽ�ｦ鬯ｨ�ｾ
		controller->init();

		// オブジェクトとの当たり判定
		atariCharacterAndObject(controller, m_stageObjects, true);
		atariCharacterAndObject(controller, m_stageObjects, false); // 1回目で斜面にいるかがわかり、それによって処理が変わるため2回目が必要

		if (controller->getAction()->getCharacter()->getHp() > 0) {
			atariCharacterAndObject(controller, m_attackObjects, false);
		}

		// 鬯ｨ�ｾ�ｽ�ｶ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｶ霓｣蛟ｬ螳�坿�ｷ陝雜｣�ｽ�ｽ�ｽ�､�ｽ�ｽ�ｽ�ｹ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｸ鬯ｩ蜍溪��ｽ�ｽ�ｽ�ｻ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ髯具ｽｹ�ｽ�ｺ髫ｴ�ｯ�ｽ�ｽ隰 original鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ鬩･�ｽ繽搾ｿｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			allCharacterAlreadyGoal &= controller->moveGoal();
			controller->setPlayerDirection(m_player_p);
		}

		// 鬮ｯ�ｷ�ｽ�ｿ鬮｢�ｧ�ｽ�ｴ髣包ｽｳ original鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｸ陞ゑｿｽ�ｽ�ｿ�ｽ�ｽ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｿｽ�ｽ�ｼ鬩･�ｽ繽搾ｿｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｺ
		if (!m_duplicationFlag || m_characterControllers[i]->getAction()->getCharacter()->getId() != m_playerId) {
			controller->action();
		}

		// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滄�髮�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬯ｮ�ｮ陷茨ｽｷ�ｽ�ｽ�ｽ�ｫ鬯ｯ�ｨ�ｽ�ｾ髯橸ｽ｢�ｽ�ｼ髫ｲ�｢陝ｷ�ｽ�･
		penetrationCharacterAndObject(controller, m_stageObjects);
	}

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｪ鬩幢ｽ｢隴趣ｿｽﾂｧ驍ｵ�ｺ陞溘ｑ�ｽ�ｹ�ｽ�ｧ�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｯ鬩幢ｽ｢隴主�讓滂ｿｽ�ｽ�ｽ�ｽ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	controlObject();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｿ�ｽ�ｽ�ｽ荳ｻ�､�ｽ�ｸ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｯ�ｷ陝雜｣�ｽ�ｼ隰夲ｽｫ�ｽ�ｳ
	controlItem();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｫ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�｡鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｩ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
	adjustBattleCamera();

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｵ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｦ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩幢ｽ｢隴取得�ｽ�ｳ�ｽ�ｨ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｬ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�､鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬩幢ｽ｢隴乗��ｽ�ｻ�ｽ�｣�ｽ雜｣�ｽ�ｦ鬯ｮ�ｫ�ｽ�ｪ�ｽ�ｽ�ｽ�ｭ鬮ｯ讖ｸ�ｽ�ｳ髯樊ｺ假ｽ蛾頼
	m_soundPlayer_p->setCameraX(m_camera->getX());

	// 鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�｢鬩幢ｽ｢隴乗��ｽ�ｹ隴∵ｻ�ｱｪ�ｽ�ｹ隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢�ｽ�ｧ�ｽ�ｽ�ｽ�ｷ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｧ鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｳ鬩搾ｽｵ�ｽ�ｺ�ｽ�ｽ�ｽ�ｮ鬮ｫ�ｴ陷ｴ�ｽ�ｽ�ｽ�ｽ�ｴ鬮ｫ�ｴ�ｽ�ｽ�ｽ�ｽ�ｽ�ｰ
	updateAnimation();

	return allCharacterAlreadyGoal;
}

bool World::dealBrightValue() {
	// 騾包ｽｻ鬮ｱ�｢隴芽挙�ｻ�｢闕ｳ�ｭ 郢ｧ�ｨ郢晢ｽｪ郢ｧ�｢驕假ｽｻ陷崎ｼ板ｰ郢晏干ﾎ樒ｹｧ�､郢晢ｽ､郢晢ｽｼ郢ｧ�ｽ�臥ｹｧ譴ｧ蜃ｾ
	if (m_brightValue != 255 || playerDead()) {
		m_brightValue = max(0, m_brightValue - 10);
		if (!playerDead()) { return true; }
	}
	return false;
}

// 闔ｨ螟奇ｽｩ�ｱ邵ｺ霈披雷郢ｧ
void World::talk() {
	if (m_conversation_p != nullptr) {
		updateCamera();
		m_conversation_p->play();
		// 鬮｣雋ｻ�ｽ�ｨ髯樊ｺｷ�･�ｽ�ｽ�ｽ�ｽ�ｩ�ｽ�ｽ�ｽ�ｱ鬯ｩ謳ｾ�ｽ�ｨ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ�ｽ�ｺ
		if (m_conversation_p->getFinishFlag()) {
			m_conversation_p = nullptr;
		}
	}
}

// 鬩幢ｽ｢鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴寂或�ｾ�ｭ�ｽ�ｽ�ｽ�ｽ鬩幢ｽ｢�ｽ�ｧ髯ｷ�ｻ髣鯉ｽｨ�ｽ�ｽ�ｽ�ｵ驕ｶ荳橸ｽ｣�ｺ�ｽ�ｽ
void World::moviePlay() {
	if (m_movie_p != nullptr) {
		m_movie_p->play();
		// 鬩幢ｽ｢鬩幢ｽ｢隴趣ｽ｢�ｽ�ｽ�ｽ�ｼ鬩幢ｽ｢隴寂或�ｾ�ｭ�ｽ�ｽ�ｽ�ｽ鬯ｩ謳ｾ�ｽ�ｨ驛｢�ｧ�ｽ�ｽ�ｽ�ｽ�ｽ�ｺ
		if (m_movie_p->getFinishFlag()) {
			m_movie_p = nullptr;
		}
	}
}