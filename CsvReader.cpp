#include "CsvReader.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Object.h"
#include "Camera.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


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


// �F�̖��O����F�n���h�����擾
int str2color(string colorName) {
	if (colorName == "white") { return WHITE; }
	else if (colorName == "gray") { return GRAY; }
	else if (colorName == "lightBlue") { return LIGHT_BLUE; }
	return -1;
}

/*
* area/area?.csv����L�����N�^�[��I�u�W�F�N�g�����[�h����
* Character����new���邽�߁A���̃N���X��new�������get���č폜���邱�ƁB
* ���̃N���X��new���ꂽCharacter���͂��̃N���X�ō폜���Ȃ��B
*/
AreaReader::AreaReader(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer) {
	m_fromAreaNum = fromAreaNum;
	m_soundPlayer_p = soundPlayer;

	m_camera_p = NULL;
	m_focusId = -1;
	m_playerId = -1;
	m_backGroundGraph = -1;
	m_backGroundColor = -1;
	m_bgmName = "";

	// �t�@�C���|�C���^
	int fp;

	// �o�b�t�@
	char buff[256];

	// �t�@�C�����J��
	ostringstream fileName;
	fileName << "data/area/area" << toAreaNum << ".csv";
	fp = FileRead_open(fileName.str().c_str());

	LOAD_AREA now = LOAD_AREA::BGM;
	vector<string> columnNames;

	while (FileRead_eof(fp) == 0) {
		FileRead_gets(buff, 256, fp);
		vector<string> oneData = csv2vector(buff);
		if (oneData[0] == "") { break; }

		if (oneData[0] == "BGM:") {
			now = LOAD_AREA::BGM;
			FileRead_gets(buff, 256, fp);
			columnNames = csv2vector(buff);
		}
		else if (oneData[0] == "CHARACTER:") {
			now = LOAD_AREA::CHARACTER;
			FileRead_gets(buff, 256, fp);
			columnNames = csv2vector(buff);
		}
		else if (oneData[0] == "OBJECT:") {
			now = LOAD_AREA::OBJECT;
			FileRead_gets(buff, 256, fp);
			columnNames = csv2vector(buff);
		}
		else if (oneData[0] == "BACKGROUND:") {
			now = LOAD_AREA::BACKGROUND;
			FileRead_gets(buff, 256, fp);
			columnNames = csv2vector(buff);
		}
		else {
			// �f�[�^��1�s���ǂݍ��񂾏ꍇ
			map<string, string> dataMap;
			for (int i = 0; i < oneData.size(); i++) {
				dataMap[columnNames[i]] = oneData[i];
			}
			map2instance(dataMap, now);
		}
	}

	// �t�@�C�������
	FileRead_close(fp);

	setPlayer();
}

void AreaReader::map2instance(map<string, string> dataMap, LOAD_AREA now) {
	switch (now) {
	case LOAD_AREA::BGM:
		loadBGM(dataMap);
		break;
	case LOAD_AREA::CHARACTER:
		loadCharacter(dataMap);
		break;
	case LOAD_AREA::OBJECT:
		loadObject(dataMap);
		break;
	case LOAD_AREA::BACKGROUND:
		loadBackGround(dataMap);
		break;
	}
}

// BGM�̃��[�h
void AreaReader::loadBGM(std::map<std::string, std::string> dataMap) {
	ostringstream filePath;
	filePath << "sound/bgm/" << dataMap["name"];
	m_bgmName = filePath.str().c_str();
	m_bgmVolume = stoi(dataMap["volume"]);
}

// �L�����N�^�[�̃��[�h
void AreaReader::loadCharacter(std::map<std::string, std::string> dataMap) {
	string name = dataMap["name"];
	int x = stoi(dataMap["x"]);
	int y = stoi(dataMap["y"]);
	bool sound = (bool)stoi(dataMap["sound"]);
	int groupId = stoi(dataMap["groupId"]);
	string actionName = dataMap["action"];
	string brainName = dataMap["brain"];
	string controllerName = dataMap["controller"];
	bool cameraFlag = (bool)stoi(dataMap["camera"]);
	bool playerFlag = (bool)stoi(dataMap["player"]);

	// �L�������쐬
	Character* character = NULL;
	if (name == "�e�X�g") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else if (name == "�n�[�g") {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}
	else {
		character = new Heart(name.c_str(), 100, x, y, groupId);
	}

	// �J�������Z�b�g
	if (cameraFlag && m_camera_p == NULL && character != NULL) {
		m_camera_p = new Camera(0, 0, 1.0);
		m_camera_p->setPoint(character->getCenterX(), character->getCenterY());
		m_focusId = character->getId();
	}

	// �v���C���[�����쒆�̃L�����Ƃ��ăZ�b�g
	if (playerFlag && m_playerId == -1 && character != NULL) {
		m_playerId = character->getId();
	}

	// �A�N�V�������쐬
	CharacterAction* action = NULL;
	SoundPlayer* soundPlayer = sound ? m_soundPlayer_p : NULL;
	if (actionName == "stick") {
		action = new StickAction(character, soundPlayer);
	}

	if (action == NULL) { return; }

	// Brain���쐬
	Brain* brain = NULL;
	if (brainName == "keyboard") {
		brain = new KeyboardBrain(m_camera_p);
	}
	else if (brainName == "normalAI") {
		brain = new NormalAI();
	}

	if (brain == NULL) { return; }

	// �R���g���[�����쐬
	CharacterController* controller = NULL;
	if (controllerName == "normal") {
		controller = new NormalController(brain, action);
	}

	if (character != NULL && controller != NULL) { 
		m_characters.push_back(character);
		m_characterControllers.push_back(controller);
	}
}

// �I�u�W�F�N�g�̃��[�h
void AreaReader::loadObject(std::map<std::string, std::string> dataMap) {
	string name = dataMap["name"];
	int x1 = stoi(dataMap["x1"]);
	int y1 = stoi(dataMap["y1"]);
	int x2 = stoi(dataMap["x2"]);
	int y2 = stoi(dataMap["y2"]);
	string graph = dataMap["graph"];
	string color = dataMap["color"];
	int hp = stoi(dataMap["hp"]);
	string other = dataMap["other"];

	int colorHandle = str2color(color);
	Object* object = NULL;
	if (name == "Box") {
		object = new BoxObject(x1, y1, x2, y2, colorHandle, hp);
	}
	else if (name == "Triangle") {
		bool leftDown = false;
		if (other == "leftDown") { leftDown = true; }
		object = new TriangleObject(x1, y1, x2, y2, colorHandle, leftDown, hp);
	}
	if (object != NULL) { m_objects.push_back(object); }

	// ���I�u�W�F�N�g�͕ʂɕ�����
	if (name == "Door") {
		graph = "picture/stageMaterial/" + graph;
		m_doorObjects.push_back(new DoorObject(x1, y1, x2, y2, graph.c_str(), stoi(other)));
	}
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

// �v���C���[�̏����ʒu���A�O�����G���A�����ɐݒ�
void AreaReader::setPlayer() {
	for (int i = 0; i < m_characters.size(); i++) {
		if (m_playerId == m_characters[i]->getId()) {
			for (int j = 0; j < m_doorObjects.size(); j++) {
				if (m_doorObjects[j]->getAreaNum() == m_fromAreaNum) {
					m_characters[i]->setX(m_doorObjects[j]->getX1());
					m_characters[i]->setY(m_doorObjects[j]->getY2() - m_characters[i]->getHeight());
				}
			}
			break;
		}
	}
}