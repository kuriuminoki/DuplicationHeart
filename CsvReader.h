#ifndef CSV_READER_H_INCLUDED
#define CSV_READER_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <sstream>

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
	std::vector<std::map<std::string, std::string> > getData() const;
};


class Character;
class CharacterController;
class Object;
class DoorObject;
class SoundPlayer;
class Camera;

class AreaReader {
private:
	// �ړ���
	int m_fromAreaNum;

	// �A�N�V�����쐬����p
	SoundPlayer* m_soundPlayer_p;

	// �J�����͂��̃N���X�ō쐬����
	Camera* m_camera_p;

	// ���J�����Ō��Ă���L����
	int m_focusId;

	// �v���C���[�����삷��L����
	int m_playerId;

	// BGM
	std::string m_bgmName;
	int m_bgmVolume;

	// �L�����N�^�[
	std::vector<Character*> m_characters;

	// �R���g���[��
	std::vector<CharacterController*> m_characterControllers;

	// �I�u�W�F�N�g
	std::vector<Object*> m_objects;

	// ���I�u�W�F�N�g
	std::vector<DoorObject*> m_doorObjects;

	// �w�i�摜�ƐF
	int m_backGroundGraph, m_backGroundColor;

	enum class LOAD_AREA {
		BGM,
		CHARACTER,
		OBJECT,
		BACKGROUND
	};

public:
	AreaReader(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer);

	inline Camera* getCamera() const { return m_camera_p; }

	inline int getFocusId() const { return m_focusId; }

	inline int getPlayerId() const { return m_playerId; }

	inline std::string getBgm() const { return m_bgmName; }

	inline int getBgmVolume() const { return m_bgmVolume; }

	inline std::vector<Character*> getCharacters() const { return m_characters; }

	inline std::vector<CharacterController*> getCharacterControllers() const { return m_characterControllers; }

	inline std::vector<Object*> getObjects() const { return m_objects; }

	inline std::vector<DoorObject*> getDoorObjects() const { return m_doorObjects; }

	inline void getBackGround(int& graphHandle, int& colorHandle) const {
		graphHandle = m_backGroundGraph;
		colorHandle = m_backGroundColor;
	}

private:
	void map2instance(std::map<std::string, std::string> dataMap, LOAD_AREA now);
	void loadBGM(std::map<std::string, std::string> dataMap);
	void loadCharacter(std::map<std::string, std::string> dataMap);
	void loadObject(std::map<std::string, std::string> dataMap);
	void loadBackGround(std::map<std::string, std::string> dataMap);
	void setPlayer();
};

#endif