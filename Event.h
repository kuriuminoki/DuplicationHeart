#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>
#include <string>
#include <map>


class World;
class SoundPlayer;
class CharacterController;
class Character;
class Conversation;
class Movie;


enum class EVENT_RESULT {
	NOW,			// �i�s��
	SUCCESS,		// �N���A
	FAILURE			// ���s
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
	virtual ~EventFire() {}

	virtual bool fire() = 0;

	virtual void setWorld(World* world) { m_world_p = world; }
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
	virtual ~EventElement();

	// ������
	virtual void init(){}

	// �v���C
	virtual EVENT_RESULT play() = 0;

	// �n�[�g�̃X�L���������\���ǂ���
	virtual bool skillAble() { return true; }

	// �N���A���ɑO�̃Z�[�u�|�C���g�֖߂�K�v�����邩
	virtual bool needBackPrevSave() { return false; }

	// �Z�b�^
	virtual void setWorld(World* world) { m_world_p = world; }
};


/*
* �C�x���g
* EventElement���Ǘ�����
*/
class Event {
private:

	// ���Ό�ɂ�����g����Element����
	World* m_world_p;
	SoundPlayer* m_soundPlayer_p;

	// �C�x���g�ԍ�
	int m_eventNum;

	// �C�x���g�̔��Ώ���
	std::vector<EventFire*> m_eventFire;

	// �C�x���g�̗v�f
	EventElement* m_eventElement;

	// �C�x���g�̓��e
	std::vector<std::map<std::string, std::string> > m_elementsData;

	// �C�x���g�̐i��(EventElement�̃C���f�b�N�X)
	int m_nowElement;

	// �O�̃Z�[�u�|�C���g�֖߂�v��
	bool m_backPrevSaveFlag;

public:
	Event(int eventNum, World* world, SoundPlayer* soundPlayer);
	~Event();

	// �Q�b�^
	inline int getEventNum() { return m_eventNum; }

	// ����
	bool fire();

	// �C�x���g�i�s
	EVENT_RESULT play();

	// ���n�[�g�̃X�L�������\���ǂ���
	bool skillAble();

	// World��ݒ肵�Ȃ���
	void setWorld(World* world);

	bool getBackPrevSaveFlag() const { return m_backPrevSaveFlag; }

	// �O�̃Z�[�u�|�C���g�֖߂������Ƃ������Ă��炤
	void doneBackPrevSave() { m_backPrevSaveFlag = false; }

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
	// �p�����[�^
	std::vector<std::string> m_param;

	// �L����
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

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����̃L����������̃L�����̋߂��ɂ���
class CharacterNearFire :
	public	EventFire
{
private:
	// �p�����[�^
	std::vector<std::string> m_param;

	// �L����
	Character* m_character_p;

	// ������G���A�ԍ��@�G���A�ړ���m��p
	int m_areaNum;

	// �q�̃L�����̋߂��ɍs���̂�����
	Character* m_target_p;

	// ���W�̂���̋��e
	int m_dx, m_dy;

public:
	CharacterNearFire(World* world, std::vector<std::string> param);

	bool fire();

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����ɔ���
class AutoFire :
	public EventFire
{
public:

	AutoFire(World* world_p) :
		EventFire(world_p)
	{

	}

	bool fire() { return true; }

};

// ���΂��Ȃ��@�f�o�b�O�p
class NonFire :
	public EventFire
{
public:

	NonFire(World* world_p) :
		EventFire(world_p)
	{

	}

	bool fire() { return false; }
};


/*
* EventElement�̔h���N���X
*/
// �G���A�ړ����֎~����
class LockAreaEvent :
	public EventElement
{
private:

	// �֎~�Ȃ�true
	bool m_lock;

public:
	LockAreaEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

};
// �L�����𖳓G�ɂ���
class InvincinbleEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// true�Ȃ疳�G�ɂ���
	bool m_invincible;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	InvincinbleEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};
// �L�����̖ڕW�n�_��ݒ��ς���
class SetGoalPointEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// �ڕW�n�_
	int m_gx, m_gy;

	// �Ώۂ̃L����
	CharacterController* m_controller_p;

public:
	SetGoalPointEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};
// �S�L�������ڕW�n�_�ֈړ�����܂őҋ@
class MoveGoalEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

public:
	MoveGoalEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

};
// �L������AI��ς���
class ChangeBrainEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// Brain�̃N���X��
	std::string m_brainName;

	// �Ώۂ̃L����
	CharacterController* m_controller_p;

public:
	ChangeBrainEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// �L������GroupID��ς���
class ChangeGroupEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	int m_groupId;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	ChangeGroupEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// �L������version��ς���
class ChangeInfoVersionEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	int m_version;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	ChangeInfoVersionEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// �L�����̍��W��ς���
class ChangeCharacterPointEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	int m_x, m_y;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	ChangeCharacterPointEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// �L�����̌�����ς���
class ChangeCharacterDirectionEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// ���������Ȃ�true
	bool m_leftDirection;

	// �q�̃L��������������֌��� ���Ȃ��Ȃ�nullptr
	Character* m_targetCharacter_p;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	ChangeCharacterDirectionEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����̃L������HP��0�ɂȂ�܂Ő키
class DeadCharacterEvent :
	public EventElement
{
private:

	// �p�����[�^
	std::vector<std::string> m_param;

	// �Ώۂ̃L����
	Character* m_character_p;

public:
	DeadCharacterEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// ���E��ݒ肵�Ȃ���
	void setWorld(World* world);
};

// ����̃O���[�v���S�ł���܂Ő키
class DeadGroupEvent :
	public EventElement
{
private:

	// �Ώۂ̃O���[�v
	int m_groupId;

	// �G���A�ԍ�
	int m_areaNum;

public:
	DeadGroupEvent(World* world, std::vector<std::string> param);

	// �v���C
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

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }

	// �Z�b�^
	void setWorld(World* world);
};

// ���[�r�[�C�x���g
class MovieEvent :
	public EventElement
{
private:

	Movie* m_movie;

public:
	MovieEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param);
	~MovieEvent();

	// ������
	void init();

	// �v���C
	EVENT_RESULT play();

	// �n�[�g�̃X�L���������\���ǂ���
	bool skillAble() { return false; }
};

// ����̃G���A�Ńv���C���[�������C�x���g
class PlayerDeadEvent :
	public EventElement
{
private:
	
	int m_areaNum;

public:
	PlayerDeadEvent(World* world, std::vector<std::string> param);

	// �v���C
	EVENT_RESULT play();

	// �N���A���ɑO�̃Z�[�u�|�C���g�֖߂�K�v�����邩
	bool needBackPrevSave() { return true; }
};

#endif