#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED


#include <vector>
#include <string>


class Character;
class Object;
class SoundPlayer;


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
	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// キャラの状態
	CHARACTER_STATE m_state;

	// 動かすキャラクター
	Character* m_character_p;

	// キャラが地面にいる
	bool m_grand;

	// 右肩下がりの坂にいる
	bool m_grandRightSlope;
	
	// 左肩下がりの坂にいる
	bool m_grandLeftSlope;

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

	// ブーストアニメの残り時間 または受け身状態
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

	int m_damageCnt;

public:
	static const char* ACTION_NAME;
	virtual const char* getActionName() { return this->ACTION_NAME; }

	CharacterAction();
	CharacterAction(Character* character, SoundPlayer* soundPlayer_p);

	virtual CharacterAction* createCopy(std::vector<Character*> characters) = 0;
	void setParam(CharacterAction* action);

	// デバッグ
	void debugAction(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// ゲッタ
	inline const Character* getCharacter() const { return m_character_p; }
	inline CHARACTER_STATE getState() const { return m_state; }
	inline bool getGrand() const { return m_grand; }
	inline bool getGrandRightSlope() const { return m_grandRightSlope; }
	inline bool getGrandLeftSlope() const { return m_grandLeftSlope; }
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline int getSlashCnt() const { return m_slashCnt; }
	inline int getBulletCnt() const { return m_bulletCnt; }
	bool getRightLock() const { return m_rightLock; }
	bool getLeftLock() const { return m_leftLock; }
	bool getUpLock() const { return m_upLock; }
	bool getDownLock() const { return m_downLock; }

	// セッタ
	void setState(CHARACTER_STATE state);
	inline void setSimpleGrand(bool grand) { m_grand = grand; }
	void setGrand(bool grand);
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	inline void setBoost() { if(!m_grand) m_boostCnt = BOOST_TIME; }
	inline void setGrandRightSlope(bool grand) { m_grandRightSlope = grand; }
	inline void setGrandLeftSlope(bool grand) { m_grandLeftSlope = grand; }
	void setRunCnt(int runCnt) { m_runCnt = runCnt; }
	void setJumpCnt(int preJumpCnt) { m_preJumpCnt = preJumpCnt; }
	void setMoveRight(bool moveRight) { m_moveRight = moveRight; }
	void setMoveLeft(bool moveLeft) { m_moveLeft = moveLeft; }
	void setMoveUp(bool moveUp) { m_moveUp = moveUp; }
	void setMoveDown(bool moveDown) { m_moveDown = moveDown; }
	void setVx(int vx) { m_vx = vx; }
	void setVy(int vy) { m_vy = vy; }
	void setBulletCnt(int bulletCnt) { m_bulletCnt = bulletCnt; }
	void setSlashCnt(int slashCnt) { m_slashCnt = slashCnt; }
	void setAttackLeftDirection(bool attackLeftDirection) { m_attackLeftDirection = attackLeftDirection; }
	void setLandCnt(int landCnt) { m_landCnt = landCnt; }
	void setBoostCnt(int boostCnt) { m_boostCnt = boostCnt; }
	void setDamageCnt(int damageCnt) { m_damageCnt = damageCnt; }

	// 今ダメージを受けていて動けない
	inline bool damageFlag() const { return m_state == CHARACTER_STATE::DAMAGE; }

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
	virtual Object* slashAttack(int gx, int gy) = 0;

	// ダメージ
	virtual void damage(int vx, int vy, int damageValue) = 0;

	// 今無敵時間じゃない
	bool ableDamage() const;

	// 今攻撃可能状態
	bool ableAttack() const;

	// 歩くのをやめる
	void stopMoveLeft();
	void stopMoveRight();

protected:
	// 画像のサイズ変更による位置調整
	void afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight);
};


// クラス名からCharacterActionを作成する関数
CharacterAction* createAction(const std::string actionName, Character* character, SoundPlayer* soundPlayer_p);


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
	static const char* ACTION_NAME;
	const char* getActionName() { return this->ACTION_NAME; }

	StickAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

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
	Object* slashAttack(int gx, int gy);

	// ダメージ
	void damage(int vx, int vy, int damageValue);
};

#endif