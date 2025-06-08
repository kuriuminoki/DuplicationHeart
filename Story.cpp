#include "Story.h"
#include "Game.h"
#include "Event.h"
#include "CsvReader.h"
#include "World.h"
#include "Sound.h"
#include "Timer.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include <sstream>

using namespace std;


vector<string> split(string str, char del) {
	vector<string> result;
	string subStr;

	for (const char c : str) {
		if (c == del) {
			result.push_back(subStr);
			subStr.clear();
		}
		else {
			subStr += c;
		}
	}

	result.push_back(subStr);
	return result;
}


Story::Story(int loop, int time, World* world, SoundPlayer* soundPlayer, EventData* eventData, int worldLifespan, int maxVersion) {
	m_worldEndFlag = false;
	m_timer = new Timer();
	m_timer->setTime(time);
	m_world_p = world;
	m_soundPlayer_p = soundPlayer;
	m_nowEvent = nullptr;
	m_loop = loop;
	m_eventData_p = eventData;

	m_needWorldUpdate = false;
	m_characterLoader = nullptr;
	m_objectLoader = nullptr;

	m_initDark = false;

	m_version = 1 + time / (worldLifespan / maxVersion);
	m_date = time / (worldLifespan / 3);
	if (time == 0) {
		updateWorldVersion();
	}
	m_world_p->setDate(m_date);

	// eventList.csvをロード
	ostringstream oss;
	oss << "data/newStory/" << "eventList.csv";
	loadEventCsvData(oss.str().c_str(), world, m_soundPlayer_p);

	// イベントの発火確認
	checkFire();
	m_soundPlayer_p->stopBGM();
}

Story::~Story() {
	delete m_timer;
	for (unsigned int i = 0; i < m_eventList.size(); i++) {
		delete m_eventList[i];
	}
	if (m_nowEvent != nullptr) {
		delete m_nowEvent;
	}
	delete m_characterLoader;
	delete m_objectLoader;
}

// csvファイルを読み込む
void Story::loadEventCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer) {
	CsvReader csvReader(fileName);
	vector<map<string, string> > data = csvReader.getData();
	for (int i = 0; i < data.size(); i++) {
		int eventNum = stoi(data[i].find("eventNum")->second);
		bool repeat = (bool)stoi(data[i].find("repeat")->second);
		string conditions_str = data[i].find("conditions")->second;
		vector<string> conditions = split(conditions_str, '|');
		int startTime = stoi(data[i].find("startTime")->second);
		int endTime = stoi(data[i].find("endTime")->second);
		bool checkConditions = true;
		for (unsigned int j = 0; j < conditions.size(); j++) {
			if (conditions[j] != "") {
				m_eventData_p->checkClearEvent(stoi(conditions[j]));
			}
		}
		if (checkConditions && (repeat || !m_eventData_p->checkClearEvent(eventNum))) {
			m_eventList.push_back(new Event(eventNum, startTime, endTime, world, soundPlayer, m_version));
		}
	}
}

void Story::loadVersionCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer) {
	delete m_characterLoader;
	delete m_objectLoader;
	m_characterLoader = new CharacterLoader;
	m_objectLoader = new ObjectLoader;
	CsvReader2 csvReader2(fileName);

	// キャラクターを用意
	vector<map<string, string> > characterData = csvReader2.getDomainData("CHARACTER:");
	for (unsigned int i = 0; i < characterData.size(); i++) {
		m_characterLoader->addCharacter(characterData[i]);
	}

	// オブジェクトを用意
	vector<map<string, string> > objectData = csvReader2.getDomainData("OBJECT:");
	for (unsigned int i = 0; i < objectData.size(); i++) {
		m_objectLoader->addObject(objectData[i]);
	}
}

bool Story::play(int worldLifespan, int maxVersion) {
	m_initDark = false;
	if (m_nowEvent == nullptr) {
		if (m_timer->getTime() == worldLifespan) {
			// 世界ループのイベントをセット
			m_eventList.push_back(new Event(9999, 0, 99999999, m_world_p, m_soundPlayer_p, m_version));
			m_worldEndFlag = true;
			m_loop++;
		}
		m_timer->advanceTime();
		m_world_p->battle();
		checkFire();
		if (m_timer->getTime() % (worldLifespan / maxVersion) == 0) {
			// 新バージョンをロードし世界を更新
			if (m_version < maxVersion) {
				m_version++;
				updateWorldVersion();
			}
		}
		if (m_timer->getTime() % (worldLifespan / 3) == 0) {
			m_date = min(2, m_date + 1);
			m_world_p->setDate(m_date);
		}
	}
	else {
		// イベント進行中
		EVENT_RESULT result = m_nowEvent->play();
		
		// イベントクリア
		if (result == EVENT_RESULT::SUCCESS) {
			m_eventData_p->setClearEvent(m_nowEvent->getEventNum());
		}
		// イベント失敗
		if (result == EVENT_RESULT::FAILURE) {
			// mustのイベントならタイトルへ戻る
		}
		// 成功または失敗したのでイベント終了
		if (result != EVENT_RESULT::NOW) {
			delete m_nowEvent;
			m_nowEvent = nullptr;
			return true;
		}
	}

	return false;
}

// イベントの発火確認
void Story::checkFire() {
	for (unsigned int i = 0; i < m_eventList.size(); i++) {
		if (!(m_timer->getTime() >= m_eventList[i]->getStartTime() && m_timer->getTime() < m_eventList[i]->getEndTime())) {
			continue;
		}
		if (m_eventList[i]->fire()) {
			m_nowEvent = m_eventList[i];
			m_eventList[i] = m_eventList.back();
			m_eventList.pop_back();
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

void Story::updateWorldVersion() {
	m_needWorldUpdate = true;
	ostringstream oss;
	oss << "data/story/template/version" << m_version << ".csv";
	loadVersionCsvData(oss.str().c_str(), m_world_p, m_soundPlayer_p);
	for (unsigned int i = 0; i < m_eventList.size(); i++) {
		m_eventList[i]->setVersion(m_version);
	}
	m_world_p->addCharacter(getCharacterLoader());
	m_world_p->addObject(getObjectLoader());
}

// セッタ
void Story::setWorld(World* world) {
	m_world_p = world;
	m_world_p->changeCharacterVersion(m_version);
	m_world_p->setDate(m_date);
	if (m_nowEvent != nullptr) {
		m_nowEvent->setWorld(m_world_p);
	}
	for (unsigned int i = 0; i < m_eventList.size(); i++) {
		m_eventList[i]->setWorld(m_world_p);
	}
}

// 前のセーブポイントへ戻る必要があるか 戻るならいくつ分戻るか返す(>0)
int Story::getBackPrevSave() const {
	return m_nowEvent != nullptr ? m_nowEvent->getBackPrevSave() : false;
}

// 前のセーブポイントへ戻ったことを教えてもらう
void Story::doneBackPrevSave() {
	m_nowEvent->doneBackPrevSave();
}