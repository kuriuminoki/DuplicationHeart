#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class CharacterAction;

/*
* キャラクターを操作する
*/
class CharacterController {
protected:
	CharacterAction* m_characterAction;
public:
	CharacterController(CharacterAction* characterAction);

	// デバッグ
	virtual void debug(int x, int y, int color) = 0;

	// キャラの操作
	virtual void control() = 0;
};


/*
* キャラクターを操作するためのキーボードのクラス
*/
class CharacterKeyboard {
public:
	void controlStick(int& right, int& left, int& up, int& down);
};

/*
* キーボードによるキャラ操作
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

	// キャラの操作
	void control();
};

#endif
