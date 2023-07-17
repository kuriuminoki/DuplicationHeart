#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED

class Character;
class Object;


// キャラクターの状態
enum class CHARACTER_STATE {
	STAND,	// 何もしていない
	DAMAGE,	// ダメージ受け中 着地で解除
	BULLET,	// 射撃中
	SLASH,	// 斬撃中
	PREJUMP,	// ジャンプ前
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

	// キャラが走っていないなら-1 そうでないなら走ったフレーム数
	int m_runCnt;

	// しゃがみ中
	bool m_squat;

	// ジャンプ前の動作
	int m_preJumpCnt;

	// ジャンプのため時間の最大
	const int PRE_JUMP_MAX = 10;

	// 着地モーションの残り時間
	int m_landCnt;

	// 着地モーションの総時間
	const int LAND_TIME = 10;

	int m_boostCnt;
	const int BOOST_TIME = 10;

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
	inline CHARACTER_STATE getState() const { return m_state; }
	inline bool getGrand() const { return m_grand; }
	inline void setGrand(bool grand) { 
		if (m_vy > 0) { // 着地モーションになる
			m_landCnt = LAND_TIME;
		}
		m_grand = grand;
	}
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline int getSlashCnt() { return m_slashCnt; }
	bool getRightLock() const { return m_rightLock; }
	bool getLeftLock() const { return m_leftLock; }
	bool getUpLock() const { return m_upLock; }
	bool getDownLock() const { return m_downLock; }
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	inline void setBoost() { m_boostCnt = BOOST_TIME; }
	inline const Character* getCharacter() const { return m_character; }

	// squat==trueならしゃがむ、falseなら立つ
	void setSquat(bool squat);

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

protected:
	// 画像のサイズ変更による位置調整
	void afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight);
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

	// ジャンプ cntフレーム目
	void jump(int cnt);

	// 射撃攻撃
	Object* bulletAttack(int gx, int gy);

	// 斬撃攻撃
	Object* slashAttack();
};

#endif