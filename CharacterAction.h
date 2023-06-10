#pragma once
#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED

class Character;


// �L�����N�^�[�̏��
enum CHARACTER_STATE {
	STAND,
	WARK,
	JUMP_UP,
	JUMP_DOWN,
	DAMAGE
};


/*
* �L�����N�^�[�𓮂����N���X
*/
class CharacterAction {
protected:
	// �������L�����N�^�[
	Character* m_character;

	// �L�����̏��
	CHARACTER_STATE m_state;

	// �L�������n�ʂɂ���
	bool m_grand;

	// �����x
	int m_vx;
	int m_vy;

public:
	CharacterAction(Character* character);

	// �f�o�b�O
	void debugAction(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �Q�b�^�ƃZ�b�^
	bool getGrand() { return m_grand; }
	bool getVx() { return m_vx; }
	bool getVy() { return m_vy; }

	// �������Z ���t���[���s��
	virtual void action() = 0;

	// �L�����̉摜��ύX
	virtual void switchHandle() = 0;

	// �ړ� �����͂S������ �L�����ɂ���Ă͎΂߂Ɉړ��ł��邽�߁B
	virtual void move(bool right, bool left, bool up, bool down) = 0;
};


/*
* ����΂Ȃ����ʂ̖_�l��
*/
class StickAction :
	public CharacterAction 
{
private:
	// �d�͉����x
	const int G = 1;

private:
	// ���֕��� �����͉E�ƍ��̂Q��
	void walk(bool right, bool left);

public:
	void debug(int x, int y, int color);

	// �������Z ���t���[���s��
	void action();

	// �L�����̉摜��ύX
	void switchHandle();

	// �ړ� �����͂S������
	void move(bool right, bool left, bool up, bool down);
};

#endif