#include "Event.h"
#include "World.h"
#include "Sound.h"
#include "CsvReader.h"
#include <sstream>

using namespace std;


// csvから取得したparam1～paramNのmapをvectorに変換
vector<string> mapParam2vector(map<string, string> paramMap) {
	vector<string> res;
	int paramNum = 0;
	while (true) {
		ostringstream oss;
		oss << "param" << paramNum;
		if (paramMap.find(oss.str()) != paramMap.end()) {
			res.push_back(paramMap[oss.str()]);
			paramNum++;
		}
		else {
			break;
		}
	}
	return res;
}


/*
* イベント
*/
Event::Event(int eventNum, World* world, SoundPlayer* soundPlayer) {

	m_eventNum = eventNum;
	m_nowElement = 0;

	ostringstream oss;
	oss << "data/event/event" << eventNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());

	// 発火条件
	vector<map<string, string> > fireData = csvReader2.getDomainData("FIRE:");
	for (unsigned int i = 0; i < fireData.size(); i++) {
		m_eventFire.push_back(new CharacterPointFire(world, mapParam2vector(fireData[i])));
	}

	// イベントの内容
	vector<map<string, string> > elementsData = csvReader2.getDomainData("ELEMENT:");
	for (unsigned int i = 0; i < elementsData.size(); i++) {
		m_eventElement.push_back(new ChangeBrainEvent(world, mapParam2vector(elementsData[i])));
	}

}

// Fireの作成
void Event::createFire(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventFire* fire = NULL;

	if (param0 == "CharacterPoint") {
		fire = new CharacterPointFire(world, param);
	}

	if (fire != NULL) { m_eventFire.push_back(fire); }
}

// Elementの作成
void Event::createElement(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventElement* element = NULL;

	if (param0 == "ChangeBrain") {
		element = new ChangeBrainEvent(world, param);
	}

	if (element != NULL) { m_eventElement.push_back(element); }
}

Event::~Event() {
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		delete m_eventFire[i];
	}
	for (unsigned int i = 0; i < m_eventElement.size(); i++) {
		delete m_eventElement[i];
	}
}

bool Event::fire() {
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		if (!m_eventFire[i]->fire()) {
			return false;
		}
	}
	return true;
}

EVENT_RESULT Event::play() {

	EVENT_RESULT result = m_eventElement[m_nowElement]->play();

	// Elementが一つ成功した
	if (result == EVENT_RESULT::SUCCESS) {
		// 次のエレメントへ
		m_nowElement++;
		if (m_nowElement == m_eventElement.size()) { 
			// イベントおわり
			return EVENT_RESULT::SUCCESS;
		}
		else { 
			// まだイベント続く
			return EVENT_RESULT::NOW;
		}
	}

	return result;
}



/*
* イベントの発火条件
*/
EventFire::EventFire(World* world) {
	m_world_p = world;
}

// 特定のキャラが指定した座標にいる
CharacterPointFire::CharacterPointFire(World* world, std::vector<std::string> param) :
	EventFire(world)
{
	m_characterName = param[1];
	m_areaNum = stoi(param[2]);
	m_x = stoi(param[3]);
	m_y = stoi(param[4]);
	m_dx = stoi(param[5]);
	m_dy = stoi(param[6]);
}
bool CharacterPointFire::fire() {
	// 特定のキャラが指定した場所にいるか判定
	return false;
}



/*
* イベントの構成要素
*/
EventElement::EventElement(World* world) {
	m_world_p = world;
}


// キャラのBrainを変更する
ChangeBrainEvent::ChangeBrainEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_brainName = param[1];
	m_characterName = param[2];
}
EVENT_RESULT ChangeBrainEvent::play() {
	// 対象のキャラのBrainを変更する
	return EVENT_RESULT::SUCCESS;
}

// 特定のキャラのHPが0になるまで戦う
DeadCharacterEvent::DeadCharacterEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_characterName = param[0];
}
EVENT_RESULT DeadCharacterEvent::play() {
	m_world_p->battle();
	// 対象のキャラのHPをチェックする
	return EVENT_RESULT::SUCCESS;
}