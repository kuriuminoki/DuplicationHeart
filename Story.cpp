#include "Story.h"
#include "Game.h"
#include "Event.h"
#include "CsvReader.h"
#include "World.h"
#include "Sound.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include <sstream>

using namespace std;


Story::Story(int storyNum, World* world, SoundPlayer* soundPlayer, EventData* eventData) {
	m_world_p = world;
	m_nowEvent = nullptr;
	m_storyNum = storyNum;
	m_eventData_p = eventData;

	m_characterLoader = new CharacterLoader;
	m_objectLoader = new ObjectLoader;

	m_date = 0;
	m_version = 0;
	m_initDark = false;

	// story����.csv�����[�h
	ostringstream oss;
	oss << "data/story/story" << storyNum << ".csv";
	loadCsvData(oss.str().c_str(), world, soundPlayer);

	// �C�x���g�̔��Ίm�F
	checkFire();
	soundPlayer->stopBGM();
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

// csv�t�@�C����ǂݍ���
void Story::loadCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer) {
	CsvReader2 csvReader2(fileName);

	// �C�x���g����
	vector<map<string, string> > eventData = csvReader2.getDomainData("EVENT:");
	for (unsigned int i = 0; i < eventData.size(); i++) {
		int eventNum = stoi(eventData[i]["num"]);
		bool mustFlag = (bool)stoi(eventData[i]["mustFlag"]);
		string condition = eventData[i]["condition"];
		string secondNum = eventData[i]["secondNum"];
		// condition���N���A�ς݂Ȃ�secondNum�̃C�x���g�����ɃZ�b�g
		if (condition != "" && m_eventData_p->checkClearEvent(stoi(eventData[i]["condition"]))) {
			if (secondNum != "") {
				// ����̃C�x���g���Z�b�g
				eventNum = stoi(eventData[i]["secondNum"]);
			}
			else {
				// ����̃C�x���g�͂Ȃ�
				eventNum = -1;
			}
		}
		if (eventNum != -1) {
			Event* eventOne = new Event(eventNum, world, soundPlayer);
			if (mustFlag) { m_mustEvent.push_back(eventOne); }
			else { m_subEvent.push_back(eventOne); }
		}
	}

	// �L�����N�^�[��p��
	vector<map<string, string> > characterData = csvReader2.getDomainData("CHARACTER:");
	for (unsigned int i = 0; i < characterData.size(); i++) {
		m_characterLoader->addCharacter(characterData[i]);
	}

	// �I�u�W�F�N�g��p��
	vector<map<string, string> > objectData = csvReader2.getDomainData("OBJECT:");
	for (unsigned int i = 0; i < objectData.size(); i++) {
		m_objectLoader->addObject(objectData[i]);
	}

	// ���ԑт�����
	vector<map<string, string> > dateData = csvReader2.getDomainData("DATE:");
	if (dateData.size() > 0) {
		m_date = stoi(dateData[0]["num"]);
	}

	// ���E�̃o�[�W�������擾�����[�h���� �ω��Ȃ�(update��0)�Ȃ烍�[�h���Ȃ�
	vector<map<string, string> > versionData = csvReader2.getDomainData("VERSION:");
	if (versionData.size() > 0) {
		m_version = stoi(versionData[0]["num"]);
		if (m_version > 0 && (bool)stoi(versionData[0]["update"])) {
			ostringstream oss;
			oss << "data/story/version" << m_version << ".csv";
			loadCsvData(oss.str().c_str(), world, soundPlayer);
		}
	}

	// Story�̏������
	vector<map<string, string> > initData = csvReader2.getDomainData("INIT:");
	if (initData.size() > 0) {
		m_initDark = (bool)stoi(initData[0]["dark"]);
	}
}

bool Story::play() {
	m_initDark = false;
	if (m_nowEvent == nullptr) {
		// ���ʂɐ��E�𓮂���
		m_world_p->battle();
		// �C�x���g�̔��Ίm�F
		checkFire();
	}
	else {
		// �C�x���g�i�s��
		EVENT_RESULT result = m_nowEvent->play();
		
		// �C�x���g�N���A
		if (result == EVENT_RESULT::SUCCESS) {
			m_eventData_p->setClearEvent(m_nowEvent->getEventNum());
		}
		// �C�x���g���s
		if (result == EVENT_RESULT::FAILURE) {
			// must�̃C�x���g�Ȃ�^�C�g���֖߂�
		}
		// �����܂��͎��s�����̂ŃC�x���g�I��
		if (result != EVENT_RESULT::NOW) {
			delete m_nowEvent;
			m_nowEvent = nullptr;
		}
	}

	// �K�{�C�x���g�S�ďI���
	if (m_mustEvent.size() == 0 && m_nowEvent == nullptr) { 
		return true;
	}
	return false;
}

// �C�x���g�̔��Ίm�F
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

// �n�[�g�̃X�L���������\���ǂ���
bool Story::skillAble() {
	if (m_nowEvent == nullptr) {
		return true;
	}
	return m_nowEvent->skillAble();
}

// �Z�b�^
void Story::setWorld(World* world) {
	m_world_p = world;
	m_world_p->changeCharacterVersion(m_version);
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

// �O�̃Z�[�u�|�C���g�֖߂�K�v�����邩
bool Story::getBackPrevSaveFlag() const {
	return m_nowEvent != nullptr ? m_nowEvent->getBackPrevSaveFlag() : false;
}

// �O�̃Z�[�u�|�C���g�֖߂������Ƃ������Ă��炤
void Story::doneBackPrevSave() {
	m_nowEvent->doneBackPrevSave();
}