#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class CharacterAction;
class Object;

/*
* キャラクターを操作する
*/
class CharacterController {
protected:
	CharacterAction* m_characterAction;
public:
	CharacterController();
	CharacterController(CharacterAction* characterAction);
	~CharacterController();

	// デバッグ
	void debugController(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// キャラクターの情報取得
	int getCharacterX();
	int getCharacterY();
	int getCharacterWide();
	int getCharacterHeight();

	// アクションの情報取得
	inline CharacterAction* getAction() { return m_characterAction; }
	int getCharacterVx();
	int getCharacterVy();

	// アクションのセッタ
	void setCharacterGrand(bool grand);
	void setActionRightLock(bool lock);
	void setActionLeftLock(bool lock);
	void setActionUpLock(bool lock);
	void setActionDownLock(bool lock);

	// キャラクターのセッタ
	void setCharacterX(int x);
	void setCharacterY(int y);

	// 行動前の処理 毎フレーム行う
	void init();

	// キャラの操作
	virtual void control() = 0;

	// 操作や当たり判定の結果を反映（実際にキャラを動かす）毎フレーム行う
	void action();

	// 射撃攻撃
	virtual Object* bulletAttack() = 0;

	// 斬撃攻撃
	virtual Object* slashAttack() = 0;
};


/*
* キャラクターを操作するためのキーボードのクラス
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
	// ジャンプキーを長押しする最大時間
	const int JUMP_KEY_LONG = 10;
	int m_jumpKey;
public:
	CharacterKeyboardController(CharacterAction* characterAction);

	void debug(int x, int y, int color);

	// キャラの操作 毎フレーム行う
	void control();

	// 射撃攻撃
	Object* bulletAttack();

	// 斬撃攻撃
	Object* slashAttack();
};

#endif
