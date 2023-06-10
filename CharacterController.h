#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class CharacterAction;

/*
* �L�����N�^�[�𑀍삷��
*/
class CharacterController {
protected:
	CharacterAction* m_characterAction;
public:
	CharacterController(CharacterAction* characterAction);

	// �f�o�b�O
	virtual void debug(int x, int y, int color) = 0;

	// �L�����̑���
	virtual void control() = 0;
};


/*
* �L�����N�^�[�𑀍삷�邽�߂̃L�[�{�[�h�̃N���X
*/
class CharacterKeyboard {
public:
	void controlStick(int& right, int& left, int& up, int& down);
};

/*
* �L�[�{�[�h�ɂ��L��������
*/
class CharacterKeyboardController :
	public CharacterController {

private:
	CharacterKeyboard m_keyboard;
	int m_rightStick;
	int m_leftStick;
	int m_upStick;
	int m_downStick;
public:
	void debug(int x, int y, int color);

	// �L�����̑���
	void control();
};

#endif
