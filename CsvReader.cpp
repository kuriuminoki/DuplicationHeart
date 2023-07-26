#include "CsvReader.h"
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
	char buff[256];

	// �t�@�C�����J��
	fp = FileRead_open(fileName);

	// �o�b�t�@�Ɉ�s�ځi�J�������j�̃e�L�X�g������
	FileRead_gets(buff, 256, fp);

	// �J�������̃��X�g���擾
	m_columnNames = csv2vector(buff);

	// �t�@�C���̏I�[�܂Ń��[�v
	while (FileRead_eof(fp) == 0) {
		// ��������o�b�t�@�Ɉ�s���̃e�L�X�g������
		FileRead_gets(buff, 256, fp);

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