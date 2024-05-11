#include "CsvReader.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Object.h"
#include "Camera.h"
#include "Brain.h"
#include "CharacterLoader.h"
#include "ObjectLoader.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// �F�̖��O����F�n���h�����擾
int str2color(string colorName) {
	if (colorName == "white") { return WHITE; }
	else if (colorName == "gray") { return GRAY; }
	else if (colorName == "lightBlue") { return LIGHT_BLUE; }
	return -1;
}


// ��s���̃e�L�X�g�����s���̃f�[�^�ɕϊ�
vector<string> csv2vector(string buff) {
	vector<string> data;
	string oneCell = "";

	// 1���������Ă���
	for (int i = 0; buff[i] != '\0'; i++) {
		// CSV�t�@�C���Ȃ̂ŃJ���}�ŋ�؂���oneData��push_back
		if (buff[i] == ',') {
			data.push_back(oneCell);
			oneCell = "";
		}
		else { // �J���}�ȊO�̕����Ȃ獇��
			oneCell += buff[i];
		}
	}
	// �Ō�̃Z���ɂ̓J���}���Ȃ�
	data.push_back(oneCell);

	return data;
}


// �t�@�C�������w�肵��CSV�t�@�C����ǂݍ���
CsvReader::CsvReader(const char* fileName) {

	// �t�@�C���|�C���^
	int fp;

	// �o�b�t�@
	const int size = 512;
	char buff[size];

	// �t�@�C�����J��
	fp = FileRead_open(fileName);

	// �o�b�t�@�Ɉ�s�ځi�J�������j�̃e�L�X�g������
	FileRead_gets(buff, size, fp);

	// �J�������̃��X�g���擾
	m_columnNames = csv2vector(buff);

	// �t�@�C���̏I�[�܂Ń��[�v
	while (FileRead_eof(fp) == 0) {
		// ��������o�b�t�@�Ɉ�s���̃e�L�X�g������
		FileRead_gets(buff, size, fp);

		// ��s���̃e�L�X�g���f�[�^�ɂ���Vector�ɕϊ�
		vector<string> oneDataVector;
		oneDataVector = csv2vector(buff);

		// vector�ƃJ���������g����map�𐶐�
		map<string, string> oneData;
		for (int i = 0; i < oneDataVector.size(); i++) {
			oneData[m_columnNames[i]] = oneDataVector[i];
		}

		// ��s����map�f�[�^��push_back
		m_data.push_back(oneData);
	}

	// �t�@�C�������
	FileRead_close(fp);
}


/*
* �J��������value�̃f�[�^���擾
* ��FfindOne("name", "�L������");
* ������Ȃ��������̃f�[�^��Ԃ�
*/
map<string, string> CsvReader::findOne(const char* columnName, const char* value) {

	// m_data[i][columnName] == value�ƂȂ�i�𒲂ׂ�
	map<string, string>::iterator ite;
	for (int i = 0; i < m_data.size(); i++) {

		// �J�������ɑΉ�����l���擾
		ite = m_data[i].find(columnName);

		// ���̃J�����������݂��Ȃ�
		if (ite == m_data[i].end()) { continue; }

		// �����Ɉ�v
		if (ite->second == value) {
			return m_data[i];
		}
	}

	// ��̃f�[�^��Ԃ�
	map<string, string> res;
	return res;
}


/*
* �S�f�[�^��Ԃ�
*/
vector<map<string, string>> CsvReader::getData() const {
	return m_data;
}


// �t�@�C�������w�肵��CSV�t�@�C����ǂݍ���
CsvReader2::CsvReader2(const char* fileName) {

	// �t�@�C���|�C���^
	int fp;

	// �o�b�t�@
	const int size = 512;
	char buff[size];

	// �t�@�C�����J��
	fp = FileRead_open(fileName);

	string domainName = "";
	// �t�@�C���̏I�[�܂Ń��[�v
	while (FileRead_eof(fp) == 0) {
		// ��������o�b�t�@�Ɉ�s���̃e�L�X�g������
		FileRead_gets(buff, size, fp);

		// ��s���̃e�L�X�g���f�[�^�ɂ���Vector�ɕϊ�
		vector<string> oneDataVector = csv2vector(buff);

		if ((oneDataVector[1] == "" && oneDataVector[0].find(":") != std::string::npos) || oneDataVector[0] == "") {
			domainName = oneDataVector[0];
			FileRead_gets(buff, size, fp);
			m_columnNames[domainName] = csv2vector(buff);
		}
		else {
			// vector�ƃJ���������g����map�𐶐�
			map<string, string> oneData;
			for (int i = 0; i < oneDataVector.size(); i++) {
				oneData[m_columnNames[domainName][i]] = oneDataVector[i];
			}

			// ��s����map�f�[�^��push_back
			m_data[domainName].push_back(oneData);
		}
	}

	// �t�@�C�������
	FileRead_close(fp);
}



/*
* area/area?.csv����L�����N�^�[��I�u�W�F�N�g�����[�h����
* Character����new���邽�߁A���̃N���X��new�������get���č폜���邱�ƁB
* ���̃N���X��new���ꂽCharacter���͂��̃N���X�ō폜���Ȃ��B
*/
AreaReader::AreaReader(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) {
	m_fromAreaNum = fromAreaNum;
	m_soundPlayer_p = soundPlayer;

	m_camera_p = nullptr;
	m_focusId = -1;
	m_playerId = -1;
	m_backGroundGraph = -1;
	m_backGroundColor = -1;
	m_bgmName = "";

	// �t�@�C�����J��
	ostringstream fileName;
	fileName << "data/area/area" << toAreaNum << ".csv";

	CsvReader2 csvReader2(fileName.str().c_str());

	vector<map<string, string> > data;
	// BGM
	data = csvReader2.getDomainData("BGM:");
	for (unsigned int i = 0; i < data.size(); i++) {
		loadBGM(data[i]);
	}
	// CHARACTER
	data = csvReader2.getDomainData("CHARACTER:");
	CharacterLoader characterLoader;
	m_camera_p = new Camera(0, 0, 1.0);
	for (unsigned int i = 0; i < data.size(); i++) {
		characterLoader.addCharacter(data[i]);
	}
	pair<vector<Character*>, vector<CharacterController*> > cp = characterLoader.getCharacters(m_camera_p, soundPlayer);
	m_characters = cp.first;
	m_characterControllers = cp.second;
	m_focusId = characterLoader.getFocusId();
	m_playerId = characterLoader.getPlayerId();
	m_playerCharacter_p = characterLoader.getPlayerCharacter();
	// OBJECT
	data = csvReader2.getDomainData("OBJECT:");
	ObjectLoader objectLoader;
	for (unsigned int i = 0; i < data.size(); i++) {
		objectLoader.addObject(data[i]);
	}
	pair<vector<Object*>, vector<Object*> > op = objectLoader.getObjects();
	m_objects = op.first;
	m_doorObjects = op.second;
	// BACKGROUND
	data = csvReader2.getDomainData("BACKGROUND:");
	for (unsigned int i = 0; i < data.size(); i++) {
		loadBackGround(data[i]);
	}

	// �ǐՑΏۂ��v���C���[�ɂ���
	setFollow();
}

// BGM�̃��[�h
void AreaReader::loadBGM(std::map<std::string, std::string> dataMap) {
	ostringstream filePath;
	filePath << "sound/bgm/" << dataMap["name"];
	m_bgmName = filePath.str().c_str();
	m_bgmVolume = stoi(dataMap["volume"]);
}

// �w�i�̃��[�h
void AreaReader::loadBackGround(std::map<std::string, std::string> dataMap) {
	string graphName = dataMap["graph"];
	string color = dataMap["color"];

	// �w�i�摜
	if (graphName != "null") {
		ostringstream filePath;
		filePath << "picture/backGround/" << graphName;
		m_backGroundGraph = LoadGraph(filePath.str().c_str());
	}
	else {
		m_backGroundGraph = -1;
	}
	// �w�i�F
	m_backGroundColor = str2color(color);
}

// follow�Ώۂ��v���C���[�ɂ���
void AreaReader::setFollow() {
	for (int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getBrain()->needSearchFollow()) {
			m_characterControllers[i]->searchFollowCandidate(m_playerCharacter_p);
		}
	}
}