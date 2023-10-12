#include "ObjectLoader.h"
#include "Object.h"
#include "CsvReader.h"
#include "Game.h"


using namespace std;


// csv�t�@�C������ǂݍ���OBJECT:�h���C������I�u�W�F�N�g���쐬
ObjectLoader::ObjectLoader() {

}

void ObjectLoader::addObject(map<string, string> dataMap) {
	// area�J�������Ȃ��ꍇ��-1�Ƀ}�b�s���O���Ă����B
	int areaNum = -1;
	if (dataMap.find("area") != dataMap.end()) {
		areaNum = stoi(dataMap["area"]);
	}
	m_objects[areaNum].push_back(dataMap);
}

// ����̃G���A�̒ǉ��I�u�W�F�N�g��vector���擾
pair<vector<Object*>, vector<Object*> > ObjectLoader::getObjects(int areaNum) {

	pair<vector<Object*>, vector<Object*> > res;
	for (unsigned int i = 0; i < m_objects[areaNum].size(); i++) {
		string name = m_objects[areaNum][i]["name"];
		int x1 = stoi(m_objects[areaNum][i]["x1"]);
		int y1 = stoi(m_objects[areaNum][i]["y1"]);
		int x2 = stoi(m_objects[areaNum][i]["x2"]);
		int y2 = stoi(m_objects[areaNum][i]["y2"]);
		string graph = m_objects[areaNum][i]["graph"];
		string color = m_objects[areaNum][i]["color"];
		int hp = stoi(m_objects[areaNum][i]["hp"]);
		string other = m_objects[areaNum][i]["other"];

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
		if (object != NULL) { res.first.push_back(object); }

		// ���I�u�W�F�N�g�͕ʂɕ�����
		if (name == "Door") {
			res.second.push_back(new DoorObject(x1, y1, x2, y2, graph.c_str(), stoi(other)));
		}
	}
	
	return res;

}

// �L�����̃G���A�ƍ��W���Z�[�u����
void ObjectLoader::saveDoorData(vector<DoorData*>& doorData) {
	unsigned int initSize = doorData.size();
	for (auto it = m_objects.begin(); it != m_objects.end(); it++) {
		int areaNum = it->first;
		vector<map<string, string> > doors = it->second;
		for (unsigned int i = 0; i < doors.size(); i++) {
			if (doors[i]["name"] != "Door") {
				continue;
			}
			int x1 = stoi(m_objects[areaNum][i]["x1"]);
			int y1 = stoi(m_objects[areaNum][i]["y1"]);
			int x2 = stoi(m_objects[areaNum][i]["x2"]);
			int y2 = stoi(m_objects[areaNum][i]["y2"]);
			string graph = m_objects[areaNum][i]["graph"];
			int from = stoi(doors[i]["area"]);
			int to = stoi(doors[i]["other"]);
			bool exist = false;
			for (unsigned j = 0; j < initSize; j++) {
				// ���ɃZ�[�u�f�[�^�ɑ��݂���h�A
				if (from == doorData[j]->from() && to == doorData[j]->to()) {
					exist = true;
					doorData[j]->setX1(x1);
					doorData[j]->setY1(y1);
					doorData[j]->setX2(x2);
					doorData[j]->setY2(y2);
					doorData[j]->setFileName(graph.c_str());
					break;
				}
			}
			// �Z�[�u�f�[�^�ɑ��݂��Ȃ�����V�K�ǉ�����h�A
			if (!exist) {
				doorData.push_back(new DoorData(x1, y1, x2, y2, from, to, graph.c_str()));
			}
		}
	}
	
}