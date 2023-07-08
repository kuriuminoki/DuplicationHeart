#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED

class Character;
class Object;


// キャラクターの状態
enum class CHARACTER_STATE {
	STAND,
	WALK,
	JUMP_UP,
	JUMP_DOWN,
	ATTACK,
	DAMAGE
};


/*
* キャラクターを動かすクラス
* Controllerが使用する。それ以外のインスタンスから使われることはない。
*/
class CharacterAction {
protected:
	// 動かすキャラクター
	Character* m_character;

	// キャラの状態
	CHARACTER_STATE m_state;

	// キャラが地面にいる
	bool m_grand;

	// 移動中
	bool m_moveRight;
	bool m_moveLeft;
	bool m_moveUp;
	bool m_moveDown;

	// 加速度
	int m_vx;
	int m_vy;

	// 移動のロック（オブジェクト等で動けない方向はtrue）
	bool m_rightLock;
	bool m_leftLock;
	bool m_upLock;
	bool m_downLock;

	// 射撃用カウント
	int m_bulletCnt;

	// 斬撃用カウント
	int m_slashCnt;

	// 攻撃する方向
	bool m_attackLeftDirection;

public:
	CharacterAction();
	CharacterAction(Character* character);

	// デバッグ
	void debugAction(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタとセッタ
	inline bool getGrand() { return m_grand; }
	inline void setGrand(bool grand) { m_grand = grand; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline int getSlashCnt() { return m_slashCnt; }
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	inline const Character* getCharacter() const { return m_character; }

	// キャラクターのセッタ
	void setCharacterX(int x);
	void setCharacterY(int y);
	void setCharacterLeftDirection(bool leftDirection);

	// 行動前の処理 毎フレーム行う
	virtual void init() = 0;

	// 物理演算 毎フレーム行う
	virtual void action() = 0;

	// キャラの画像を変更
	virtual void switchHandle() = 0;

	// 移動 引数は４方向分 キャラによっては斜めに移動できるため。
	virtual void move(bool right, bool left, bool up, bool down) = 0;

	// ジャンプ rate%の力で飛び上がる。
	virtual void jump(int rate) = 0;

	// 射撃攻撃
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// 斬撃攻撃
	virtual Object* slashAttack() = 0;
};


/*
* 空を飛ばない普通の棒人間
*/
class StickAction :
	public CharacterAction 
{
private:
	// 重力加速度
	const int G = 1;

private:
	// 横へ歩く 引数は右と左の２つ
	void walk(bool right, bool left);

	// キャラの画像を状態(state)に応じて変更
	void switchHandle();

public:
	StickAction(Character* character);

	void debug(int x, int y, int color);

	//行動前の処理 毎フレーム行う
	void init();

	// 物理演算 毎フレーム行う
	void action();

	// 移動 引数は４方向分
	void move(bool right, bool left, bool up, bool down);

	// ジャンプ rate%の力で飛び上がる。
	void jump(int rate);

	// 射撃攻撃
	Object* bulletAttack(int gx, int gy);

	// 斬撃攻撃
	Object* slashAttack();
};

#endif