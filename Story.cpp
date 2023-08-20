#include "Story.h"
#include "Event.h"
#include "CsvReader.h"
#include "World.h"
#include "Sound.h"
#include "ObjectLoader.h"
#include <sstream>

using namespace std;


Story::Story(int storyNum, World* world, SoundPlayer* soundPlayer) {
	m_world_p = world;
	m_nowEvent = NULL;
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

	// オブジェクトを用意
	vector<map<string, string> > objectData = csvReader2.getDomainData("OBJECT:");
	m_objectLoader = new ObjectLoader;
	for (unsigned int i = 0; i < objectData.size(); i++) {
		m_objectLoader->addObject(objectData[i]);
	}
}

Story::~Story() {
	for (unsigned int i = 0; i < m_mustEvent.size(); i++) {
		delete m_mustEvent[i];
	}
	for (unsigned int i = 0; i < m_subEvent.size(); i++) {
		delete m_subEvent[i];
	}
	delete m_objectLoader;
}

bool Story::play() {
	if (m_nowEvent == NULL) {
		// 普通に世界を動かす
		m_world_p->battle();
		// イベントの発火確認
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
	else {
		// イベント進行中
		EVENT_RESULT result = m_nowEvent->play();
		// イベント終了
		if (result != EVENT_RESULT::NOW) {
			delete m_nowEvent;
			m_nowEvent = NULL;
		}
	}

	// 必須イベント全て終わり
	if (m_mustEvent.size() == 0 && m_nowEvent == NULL) { 
		return true;
	}
	return false;
}

// ハートのスキル発動が可能かどうか
bool Story::skillAble() {
	if (m_nowEvent == NULL) {
		return true;
	}
	return m_nowEvent->skillAble();
}

// セッタ
void Story::setWorld(World* world) {
	m_world_p = world;
	for (unsigned int i = 0; i < m_mustEvent.size(); i++) {
		m_mustEvent[i]->setWorld(m_world_p);
	}
	for (unsigned int i = 0; i < m_subEvent.size(); i++) {
		m_mustEvent[i]->setWorld(m_world_p);
	}
}