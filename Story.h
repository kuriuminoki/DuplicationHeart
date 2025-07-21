#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <string>
#include <vector>

class Event;
class EventData;
class World;
class SoundPlayer;
class Timer;
class CharacterLoader;
class ObjectLoader;


// �X�g�[���|
class Story {
private:

	// �o�ߎ��ԁi���[�v�̓x���Z�b�g�j
	Timer* m_timer;

	// ���T�ڂ̐��E��
	int m_loop;

	// �Ώۂ̐��E
	World* m_world_p;

	SoundPlayer* m_soundPlayer_p;

	// ���ԑ�
	int m_date;

	// Emote Online��version
	int m_version;

	// version�A�b�v��Game�ɒm�点��
	bool m_needWorldUpdate;

	// Story����ʈÓ]�̏�Ԃ���X�^�[�g�Ȃ�true
	bool m_initDark;

	// ���E�̎�����������true
	bool m_worldEndFlag;

	// �i�s���̃C�x���g
	Event* m_nowEvent;
	
	// �N���A�K�{�C�x���g
	std::vector<Event*> m_eventList;

	// �C�x���g�̃N���A�� Game�N���X������炤
	EventData* m_eventData_p;

	// �L�����N�^�[�̃f�[�^
	CharacterLoader* m_characterLoader;

	// �I�u�W�F�N�g�̃f�[�^
	ObjectLoader* m_objectLoader;

public:
	Story(int loop, int time, World* world, SoundPlayer* soundPlayer, EventData* eventData, int worldLifespan, int maxVersion);
	~Story();

	// csv�t�@�C����ǂݍ���
	void loadEventCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer, int versionTimeSpan);
	void loadVersionCsvData(const char* fileName, World* world, SoundPlayer* soundPlayer);

	void debug(int x, int y, int color);

	bool play(int worldLifespan, int maxVersion, int timeSpeed);

	// �C�x���g�̔��Ίm�F
	void checkFire();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble();

	void updateWorldVersion();

	// ���[�v���s���̏������i���s�����1F�̕`��̂��߂Ɏg���B�j
	void loopInit();

	// �Q�b�^
	inline const Timer* getTimer() const { return m_timer; }
	inline int getDate() const { return m_date; }
	inline int getVersion() const { return m_version; }
	inline bool getNeedWorldUpdate() const { return m_needWorldUpdate; }
	inline bool getInitDark() const { return m_initDark; }
	inline CharacterLoader* getCharacterLoader() const { return m_characterLoader; }
	inline ObjectLoader* getObjectLoader() const { return m_objectLoader; }
	inline const World* getWorld() const { return m_world_p; }
	int getBackPrevSave() const;
	int getLoop() const { return m_loop; }

	// �Z�b�^
	void setWorld(World* world);
	inline void doneWorldUpdate() { m_needWorldUpdate = false; }

	// ���C�x���g����
	bool eventNow() const { return m_nowEvent != nullptr; }

	// �O�̃Z�[�u�|�C���g�֖߂������Ƃ������Ă��炤
	void doneBackPrevSave();

private:
	int calcVersion(int time, int worldLifespan, int maxVersion);
	int calcDate(int time, int worldLifespan);

};


#endif