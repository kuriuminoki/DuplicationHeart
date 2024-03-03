#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <vector>

class Event;
class EventData;
class World;
class SoundPlayer;
class CharacterLoader;
class ObjectLoader;


// �X�g�[���|
class Story {
private:

	// �Ώۂ̐��E
	World* m_world_p;

	// �X�g�[���[�ԍ�
	int m_storyNum;

	// ���ԑ�
	int m_date;

	// Emote Online��version
	int m_version;

	// �i�s���̃C�x���g
	Event* m_nowEvent;
	
	// �N���A�K�{�C�x���g
	std::vector<Event*> m_mustEvent;
	
	// �N���A�C�ӃC�x���g
	std::vector<Event*> m_subEvent;

	// �C�x���g�̃N���A�� Game�N���X������炤
	EventData* m_eventData_p;

	// �L�����N�^�[�̃f�[�^
	CharacterLoader* m_characterLoader;

	// �I�u�W�F�N�g�̃f�[�^
	ObjectLoader* m_objectLoader;

public:
	Story(int storyNum, World* world, SoundPlayer* soundPlayer, EventData* eventData);
	~Story();

	// csv�t�@�C����ǂݍ���
	void loadCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer);

	void debug(int x, int y, int color);

	bool play();

	// �C�x���g�̔��Ίm�F
	void checkFire();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble();

	// �Q�b�^
	inline int getStoryNum() const { return m_storyNum; }
	inline int getDate() const { return m_date; }
	inline int getVersion() const { return m_version; }
	inline CharacterLoader* getCharacterLoader() const { return m_characterLoader; }
	inline ObjectLoader* getObjectLoader() const { return m_objectLoader; }
	inline const World* getWorld() const { return m_world_p; }
	bool getBackPrevSaveFlag() const;

	// �Z�b�^
	void setWorld(World* world);

	// �O�̃Z�[�u�|�C���g�֖߂������Ƃ������Ă��炤
	void doneBackPrevSave();
};


#endif