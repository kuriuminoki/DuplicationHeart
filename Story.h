#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <vector>

class Event;
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

	// �i�s���̃C�x���g
	Event* m_nowEvent;
	
	// �N���A�K�{�C�x���g
	std::vector<Event*> m_mustEvent;
	
	// �N���A�C�ӃC�x���g
	std::vector<Event*> m_subEvent;

	// �L�����N�^�[�̃f�[�^
	CharacterLoader* m_characterLoader;

	// �I�u�W�F�N�g�̃f�[�^
	ObjectLoader* m_objectLoader;

public:
	Story(int storyNum, World* world, SoundPlayer* soundPlayer);
	~Story();

	void debug(int x, int y, int color);

	bool play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble();

	// �Q�b�^
	inline int getStoryNum() const { return m_storyNum; }
	inline CharacterLoader* getCharacterLoader() const { return m_characterLoader; }
	inline ObjectLoader* getObjectLoader() const { return m_objectLoader; }
	inline const World* getWorld() const { return m_world_p; }

	// �Z�b�^
	void setWorld(World* world);
};


#endif