#pragma once
#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED

class Character;


// キャラクターの状態
enum CHARACTER_STATE {
	STAND,
	WARK,
	JUMP_UP,
	JUMP_DOWN,
	DAMAGE
};


/*
* キャラクターを動かすクラス
*/
class CharacterAction {
protected:
	// 動かすキャラクター
	Character* m_character;

	// キャラの状態
	CHARACTER_STATE m_state;

	// キャラが地面にいる
	bool m_grand;

	// 加速度
	int m_vx;
	int m_vy;

public:
	CharacterAction(Character* character);

	// デバッグ
	void debugAction(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタとセッタ
	bool getGrand() { return m_grand; }
	bool getVx() { return m_vx; }
	bool getVy() { return m_vy; }

	// 物理演算 毎フレーム行う
	virtual void action() = 0;

	// キャラの画像を変更
	virtual void switchHandle() = 0;

	// 移動 引数は４方向分 キャラによっては斜めに移動できるため。
	virtual void move(bool right, bool left, bool up, bool down) = 0;
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

public:
	void debug(int x, int y, int color);

	// 物理演算 毎フレーム行う
	void action();

	// キャラの画像を変更
	void switchHandle();

	// 移動 引数は４方向分
	void move(bool right, bool left, bool up, bool down);
};

#endif