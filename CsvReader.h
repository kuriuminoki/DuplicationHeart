#ifndef CSV_READER_H_INCLUDED
#define CSV_READER_H_INCLUDED

#include <vector>
#include <string>
#include <map>

class CsvReader {
private:
	/*
	* �f�[�^
	* m_data[�s�ԍ�]<�J������, �f�[�^>
	*/
	std::vector<std::map<std::string, std::string> > m_data;

	/*
	* �J�������̃��X�g
	*/
	std::vector<std::string> m_columnNames;

public:
	// �t�@�C�������w�肵��CSV�t�@�C����ǂݍ���
	CsvReader(const char* fileName);

	/*
	* �J��������value�̃f�[�^���擾
	* ��FfindOne("Name", "�L������");
	*/ 
	std::map<std::string, std::string> findOne(const char* columnName, const char* value);

	/*
	* �S�f�[�^��Ԃ�
	*/
	std::vector<std::map<std::string, std::string> > getData();
};

#endif