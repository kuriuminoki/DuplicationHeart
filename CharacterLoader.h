#ifndef CHARACTER_LOADER_H_INCLUDED
#define CHARACTER_LOADER_H_INCLUDED


#include <map>
#include <vector>
#include <string>
#include <utility>


class Character;
class CharacterController;
class Camera;
class SoundPlayer;
class CharacterData;


class CharacterLoader {
private:
	std::map<int, std::vector<std::map<std::string, std::string> > > m_characters;

	int m_focusId, m_playerId;

	Character* m_playerCharacter_p;

public:
	// csv�t�@�C������ǂݍ���CHARACTER:�h���C������I�u�W�F�N�g���쐬
	CharacterLoader();

	// �Q�b�^
	inline int getFocusId() const { return m_focusId; }
	inline int getPlayerId() const { return m_playerId; }
	inline Character* getPlayerCharacter() const { return m_playerCharacter_p; }

	void addCharacter(std::map<std::string, std::string> dataMap);

	// ����̃G���A�̒ǉ��L������vector���擾<Character, Controller>
	std::pair<std::vector<Character*>, std::vector<CharacterController*> > getCharacters(Camera* camera_p, SoundPlayer* soundPlayer_p, int areaNum = -1);

	// �L�����̃G���A�ƍ��W���Z�[�u����
	void saveCharacterData(CharacterData* characterData);
};


#endif