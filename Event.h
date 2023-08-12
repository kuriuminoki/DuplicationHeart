#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>
#include <string>


class World;
class SoundPlayer;
class CharacterController;
class Character;
class Conversation;


enum class EVENT_RESULT {
	NOW,
	SUCCESS,
	FAILURE
};


/*
* �C�x���g�̔��Ώ��� World�ɕω��������Ȃ�
*/
class EventFire {
protected:

	// �C�x���g���N����World
	const World* m_world_p;

public:
	EventFire(World* world);

	virtual bool fire() = 0;
};


/*
* �C�x���g���\������v�f
*/
class EventElement {
protected:

	// �C�x���g���N���Ă���World
	World* m_world_p;

public:
	EventElement(World* world);
	virtual EVENT_RESULT play() = 0;
};


/*
* �C�x���g
* EventElement���Ǘ�����
*/
class Event {
private:

	// �C�x���g�ԍ�
	int m_eventNum;

	// �C�x���g�̔��Ώ���
	std::vector<EventFire*> m_eventFire;

	// �C�x���g�̗v�f
	std::vector<EventElement*> m_eventElement;

	// �C�x���g�̐i��(EventElement�̃C���f�b�N�X)
	int m_nowElement;

public:
	Event(int eventNum, World* world, SoundPlayer* soundPlayer);
	~Event();

	// ����
	bool fire();

	// �C�x���g�i�s
	EVENT_RESULT play();

private:
	void createFire(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
	void createElement(std::vector<std::string> param, World* world, SoundPlayer* soundPlayer);
};


/*
* EventFire�̔h���N���X
*/
// ����̃L�������w�肵�����W�ɂ���
class CharacterPointFire :
	public	EventFire
{
private:
	// �L�����̖��O
	Character* m_character_p;

	// �G���A�ԍ�
	int m_areaNum;

	// �ڕW���W
	int m_x, m_y;

	// ���W�̂���̋��e
	int m_dx, m_dy;

public:
	CharacterPointFire(World* world, std::vector<std::string> param);

	bool fire();
};


/*
* EventElement�̔h���N���X
*/
// �L������AI��ς���
class ChangeBrainEvent :
	public EventElement
{
private:

	// Brain�̃N���X��
	std::string m_brainName;

	// �Ώۂ̃L����
	CharacterController* m_controller_p;

public:
	ChangeBrainEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();
};

// ����̃L������HP��0�ɂȂ�܂Ő키
class DeadCharacterEvent :
	public EventElement
{
private:

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	DeadCharacterEvent(World* world, std::vector<std::string> param);

	EVENT_RESULT play();
};

// ��b�C�x���g
class TalkEvent :
	public EventElement
{
private:

	Conversation* m_conversation;

public:
	TalkEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param);
	~TalkEvent();

	EVENT_RESULT play();
};

#endif