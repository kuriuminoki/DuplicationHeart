#include "CharacterLoader.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Brain.h"
#include "CsvReader.h"
#include "Sound.h"
#include "Camera.h"


using namespace std;


// csv�t�@�C������ǂݍ���CHARACTER:�h���C������I�u�W�F�N�g���쐬
CharacterLoader::CharacterLoader() {
	m_focusId = -1;
	m_playerId = -1;
	m_playerCharacter_p = nullptr;
}

void CharacterLoader::addCharacter(map<string, string> dataMap) {
	// area�J�������Ȃ��ꍇ��-1�Ƀ}�b�s���O���Ă����B
	int areaNum = -1;
	if (dataMap.find("area") != dataMap.end()) {
		areaNum = stoi(dataMap["area"]);
	}
	m_characters[areaNum].push_back(dataMap);
}

// ����̃G���A�̒ǉ��I�u�W�F�N�g��vector���擾
pair<vector<Character*>, vector<CharacterController*> > CharacterLoader::getCharacters(Camera* camera_p, SoundPlayer* soundPlayer_p, int areaNum) {

	pair<vector<Character*>, vector<CharacterController*> > res;

	for (unsigned int i = 0; i < m_characters[areaNum].size(); i++) {
		string name = m_characters[areaNum][i]["name"];
		int x = stoi(m_characters[areaNum][i]["x"]);
		int y = stoi(m_characters[areaNum][i]["y"]);
		bool sound = (bool)stoi(m_characters[areaNum][i]["sound"]);
		int groupId = stoi(m_characters[areaNum][i]["groupId"]);
		string actionName = m_characters[areaNum][i]["action"];
		string brainName = m_characters[areaNum][i]["brain"];
		string controllerName = m_characters[areaNum][i]["controller"];
		bool cameraFlag = (bool)stoi(m_characters[areaNum][i]["camera"]);
		bool playerFlag = (bool)stoi(m_characters[areaNum][i]["player"]);

		// �L�������쐬
		Character* character = createCharacter(name.c_str(), 100, x, y, groupId);

		// �J�������Z�b�g
		if (cameraFlag && character != NULL) {
			camera_p->setPoint(character->getCenterX(), character->getCenterY());
			m_focusId = character->getId();
		}

		// �v���C���[�����쒆�̃L�����Ƃ��ăZ�b�g
		if (playerFlag && m_playerId == -1 && character != NULL) {
			m_playerId = character->getId();
			m_playerCharacter_p = character;
		}

		// �A�N�V�������쐬
		SoundPlayer* soundPlayer = sound ? soundPlayer_p : NULL;
		CharacterAction* action = createAction(actionName, character, soundPlayer);

		// Brain���쐬
		Brain* brain = createBrain(brainName, camera_p);

		// �R���g���[�����쐬
		CharacterController* controller = NULL;
		if (action != NULL && brain != NULL) {
			controller = createController(controllerName, brain, action);
		}

		// ���������L�����ƃR���g���[����ۑ�
		if (character != NULL && controller != NULL) {
			res.first.push_back(character);
			res.second.push_back(controller);
		}
	}

	return res;
}