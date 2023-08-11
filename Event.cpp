#include "Event.h"
#include "World.h"
#include "Sound.h"
#include "CsvReader.h"
#include <sstream>

using namespace std;


// csv����擾����param1�`paramN��map��vector�ɕϊ�
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
* �C�x���g
*/
Event::Event(int eventNum, World* world, SoundPlayer* soundPlayer) {

	m_eventNum = eventNum;
	m_nowElement = 0;

	ostringstream oss;
	oss << "data/event/event" << eventNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());

	// ���Ώ���
	vector<map<string, string> > fireData = csvReader2.getDomainData("FIRE:");
	for (unsigned int i = 0; i < fireData.size(); i++) {
		m_eventFire.push_back(new CharacterPointFire(world, mapParam2vector(fireData[i])));
	}

	// �C�x���g�̓��e
	vector<map<string, string> > elementsData = csvReader2.getDomainData("ELEMENT:");
	for (unsigned int i = 0; i < elementsData.size(); i++) {
		m_eventElement.push_back(new ChangeBrainEvent(world, mapParam2vector(elementsData[i])));
	}

}

// Fire�̍쐬
void Event::createFire(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventFire* fire = NULL;

	if (param0 == "CharacterPoint") {
		fire = new CharacterPointFire(world, param);
	}

	if (fire != NULL) { m_eventFire.push_back(fire); }
}

// Element�̍쐬
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

	// Element�����������
	if (result == EVENT_RESULT::SUCCESS) {
		// ���̃G�������g��
		m_nowElement++;
		if (m_nowElement == m_eventElement.size()) { 
			// �C�x���g�����
			return EVENT_RESULT::SUCCESS;
		}
		else { 
			// �܂��C�x���g����
			return EVENT_RESULT::NOW;
		}
	}

	return result;
}



/*
* �C�x���g�̔��Ώ���
*/
EventFire::EventFire(World* world) {
	m_world_p = world;
}

// ����̃L�������w�肵�����W�ɂ���
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
	// ����̃L�������w�肵���ꏊ�ɂ��邩����
	return false;
}



/*
* �C�x���g�̍\���v�f
*/
EventElement::EventElement(World* world) {
	m_world_p = world;
}


// �L������Brain��ύX����
ChangeBrainEvent::ChangeBrainEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_brainName = param[1];
	m_characterName = param[2];
}
EVENT_RESULT ChangeBrainEvent::play() {
	// �Ώۂ̃L������Brain��ύX����
	return EVENT_RESULT::SUCCESS;
}

// ����̃L������HP��0�ɂȂ�܂Ő키
DeadCharacterEvent::DeadCharacterEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_characterName = param[0];
}
EVENT_RESULT DeadCharacterEvent::play() {
	m_world_p->battle();
	// �Ώۂ̃L������HP���`�F�b�N����
	return EVENT_RESULT::SUCCESS;
}