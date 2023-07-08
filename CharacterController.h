#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class CharacterAction;
class Object;

/*
* �L�����N�^�[�𑀍삷��
*/
class CharacterController {
protected:
	CharacterAction* m_characterAction;
public:
	CharacterController();
	CharacterController(CharacterAction* characterAction);
	~CharacterController();

	// �f�o�b�O
	void debugController(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �L�����N�^�[�̏��擾
	int getCharacterX();
	int getCharacterY();
	int getCharacterWide();
	int getCharacterHeight();

	// �A�N�V�����̏��擾
	inline CharacterAction* getAction() { return m_characterAction; }
	int getCharacterVx();
	int getCharacterVy();

	// �A�N�V�����̃Z�b�^
	void setCharacterGrand(bool grand);
	void setActionRightLock(bool lock);
	void setActionLeftLock(bool lock);
	void setActionUpLock(bool lock);
	void setActionDownLock(bool lock);

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);

	// �s���O�̏��� ���t���[���s��
	void init();

	// �L�����̑���
	virtual void control() = 0;

	// ����ⓖ���蔻��̌��ʂ𔽉f�i���ۂɃL�����𓮂����j���t���[���s��
	void action();

	// �ˌ��U��
	virtual Object* bulletAttack() = 0;

	// �a���U��
	virtual Object* slashAttack() = 0;
};


/*
* �L�����N�^�[�𑀍삷�邽�߂̃L�[�{�[�h�̃N���X
*/
class CharacterKeyboard {
private:
	int m_nowSpaceKey;
	int m_preSpaceKey;
public:
	CharacterKeyboard();
	void controlStick(int& right, int& left, int& up, int& down);
	void controlJump(int& nowSpaceKey, int& preSpaceKey);
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
	// �W�����v�L�[�𒷉�������ő厞��
	const int JUMP_KEY_LONG = 10;
	int m_jumpKey;
public:
	CharacterKeyboardController(CharacterAction* characterAction);

	void debug(int x, int y, int color);

	// �L�����̑��� ���t���[���s��
	void control();

	// �ˌ��U��
	Object* bulletAttack();

	// �a���U��
	Object* slashAttack();
};

#endif
