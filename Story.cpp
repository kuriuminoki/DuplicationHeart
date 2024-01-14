#include "Story.h"
#include "Event.h"
#include "CsvReader.h"
#include "World.h"
#include "Sound.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include <sstream>

using namespace std;


Story::Story(int storyNum, World* world, SoundPlayer* soundPlayer) {
	m_world_p = world;
	m_nowEvent = nullptr;
	m_storyNum = storyNum;

	ostringstream oss;
	oss << "data/story/story" << storyNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());

	// イベント生成
	vector<map<string, string> > eventData = csvReader2.getDomainData("EVENT:");
	for (unsigned int i = 0; i < eventData.size(); i++) {
		int eventNum = stoi(eventData[i]["num"]);
		bool mustFlag = (bool)stoi(eventData[i]["mustFlag"]);
		Event* eventOne = new Event(eventNum, world, soundPlayer);
		if (mustFlag) { m_mustEvent.push_back(eventOne); }
		else { m_subEvent.push_back(eventOne); }
	}

	// キャラクターを用意
	vector<map<string, string> > characterData = csvReader2.getDomainData("CHARACTER:");
	m_characterLoader = new CharacterLoader;
	for (unsigned int i = 0; i < characterData.size(); i++) {
		m_characterLoader->addCharacter(characterData[i]);
	}

	// オブジェクトを用意
	vector<map<string, string> > objectData = csvReader2.getDomainData("OBJECT:");
	m_objectLoader = new ObjectLoader;
	for (unsigned int i = 0; i < objectData.size(); i++) {
		m_objectLoader->addObject(objectData[i]);
	}

	// 時間帯を決定
	vector<map<string, string> > dateData = csvReader2.getDomainData("DATE:");
	if (dateData.size() > 0) {
		m_date = stoi(dateData[0]["num"]);
	}

	// イベントの発火確認
	checkFire();
}

Story::~Story() {
	for (unsigned int i = 0; i < m_mustEvent.size(); i++) {
		delete m_mustEvent[i];
	}
	for (unsigned int i = 0; i < m_subEvent.size(); i++) {
		delete m_subEvent[i];
	}
	if (m_nowEvent != nullptr) {
		delete m_nowEvent;
	}
	delete m_characterLoader;
	delete m_objectLoader;
}

bool Story::play() {
	if (m_nowEvent == nullptr) {
		// 普通に世界を動かす
		m_world_p->battle();
		// イベントの発火確認
		checkFire();
	}
	else {
		// イベント進行中
		EVENT_RESULT result = m_nowEvent->play();
		
		// イベント失敗
		if (result == EVENT_RESULT::FAILURE) {
			// mustのイベントならタイトルへ戻る
		}
		// 成功または失敗したのでイベント終了
		if (result != EVENT_RESULT::NOW) {
			delete m_nowEvent;
			m_nowEvent = nullptr;
		}
	}

	// 必須イベント全て終わり
	if (m_mustEvent.size() == 0 && m_nowEvent == nullptr) { 
		return true;
	}
	return false;
}

// イベントの発火確認
void Story::checkFire() {
	for (unsigned int i = 0; i < m_mustEvent.size(); i++) {
		if (m_mustEvent[i]->fire()) {
			m_nowEvent = m_mustEvent[i];
			m_mustEvent[i] = m_mustEvent.back();
			m_mustEvent.pop_back();
			i--;
		}
	}
	for (unsigned int i = 0; i < m_subEvent.size(); i++) {
		if (m_subEvent[i]->fire()) {
			m_nowEvent = m_subEvent[i];
			m_subEvent[i] = m_subEvent.back();
			m_subEvent.pop_back();
			i--;
		}
	}
}

// ハートのスキル発動が可能かどうか
bool Story::skillAble() {
	if (m_nowEvent == nullptr) {
		return true;
	}
	return m_nowEvent->skillAble();
}

// セッタ
void Story::setWorld(World* world) {
	m_world_p = world;
	m_world_p->setDate(m_date);
	if (m_nowEvent != nullptr) {
		m_nowEvent->setWorld(m_world_p);
	}
	for (unsigned int i = 0; i < m_mustEvent.size(); i++) {
		m_mustEvent[i]->setWorld(m_world_p);
	}
	for (unsigned int i = 0; i < m_subEvent.size(); i++) {
		m_subEvent[i]->setWorld(m_world_p);
	}
}

// 前のセーブポイントへ戻る必要があるか
bool Story::getBackPrevSaveFlag() const {
	return m_nowEvent != nullptr ? m_nowEvent->getBackPrevSaveFlag() : false;
}

// 前のセーブポイントへ戻ったことを教えてもらう
void Story::doneBackPrevSave() {
	m_nowEvent->doneBackPrevSave();
}