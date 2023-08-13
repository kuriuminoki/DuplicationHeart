#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <vector>

class Event;
class World;
class SoundPlayer;

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

public:
	Story(int storyNum, World* world, SoundPlayer* soundPlayer);
	~Story();

	void debug(int x, int y, int color);

	bool play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble();
};


#endif