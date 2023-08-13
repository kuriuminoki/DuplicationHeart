#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED


#include <vector>


class Character;
class CharacterAction;
class Object;
class Camera;


// Controllerに命令するクラス（キーボード＆マウスやＡＩ）
class Brain {
protected:
	// 今のキャラの状態を考慮して射撃の目標座標や次の行動を決めるため必要
	const CharacterAction* m_characterAction_p;

public:
	Brain();

	virtual Brain* createCopy(std::vector<Character*> characters, const Camera* camera) = 0;

	virtual void debug(int x, int y, int color) const = 0;

	// 話しかけたり扉入ったり
	virtual bool actionOrder() { return false; }

	// セッタ
	virtual void setCharacterAction(const CharacterAction* characterAction) = 0;
	
	// 遠距離攻撃の目標座標
	virtual void bulletTargetPoint(int& x, int& y) = 0;

	// 移動（上下左右の入力）
	virtual void moveOrder(int& right, int& left, int& up, int& down) = 0;

	// ジャンプの制御
	virtual int jumpOrder() = 0;

	// しゃがみの制御
	virtual int squatOrder() = 0;

	// 近距離攻撃
	virtual int slashOrder() = 0;

	// 遠距離攻撃
	virtual int bulletOrder() = 0;

	// 攻撃対象を決める(AIクラスでオーバライドする。)
	virtual void searchTarget(const Character* character) { };

	// 攻撃対象を変更する必要があるならtrueでアピールする(AIクラスでオーバライドする)。
	virtual bool needSearchTarget() const { return false; }

	// 追跡対象を決める(AIクラスでオーバライドする。)
	virtual void searchFollow(const Character* character) { };

	// 追跡対象を変更する必要があるならtrueでアピールする(AIクラスでオーバライドする)。
	virtual bool needSearchFollow() const { return false; }
};

/*
* キーボードでキャラの操作を命令するクラス
*/
class KeyboardBrain :
	public Brain
{
private:
	// カメラ
	const Camera* m_camera_p;

public:
	KeyboardBrain(const Camera* camera);
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera){ return new KeyboardBrain(camera); }
	void debug(int x, int y, int color) const;
	inline void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }
	void bulletTargetPoint(int& x, int& y);
	bool actionOrder();
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};

/*
* 全く動かないAI
*/
class Freeze :
	public Brain
{
public:
	Freeze() { }
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera) { return new Freeze(); }
	void debug(int x, int y, int color) const { }
	bool actionOrder() { return false; }
	void setCharacterAction(const CharacterAction* characterAction) {  }
	void bulletTargetPoint(int& x, int& y) {  }
	void moveOrder(int& right, int& left, int& up, int& down) { right = 0; left = 0; up = 0; down = 0; }
	int jumpOrder() { return 0; }
	int squatOrder() { return 0; }
	int slashOrder() { return 0; }
	int bulletOrder() { return 0; }
};

/*
*  普通に敵と戦うよう命令するＡＩのクラス
*/
class NormalAI :
	public Brain
{
private:

	// 攻撃対象を認知する距離
	const int TARGET_DISTANCE = 2000;

	// 移動用
	int m_rightKey, m_leftKey, m_upKey, m_downKey;

	// ジャンプの長さ
	int m_jumpCnt;

	// しゃがむ長さ
	int m_squatCnt;

protected:
	// 攻撃対象
	const Character* m_target_p;

	// 射撃の精度
	const int BULLET_ERROR = 400;

	// 特に意味のない移動をする確率
	const int MOVE_RAND = 59;

	// 移動目標
	int m_gx, m_gy;

	// 移動目標達成とみなす誤差 ±GX_ERROR
	const int GX_ERROR = 100;

	// 移動時間
	int m_moveCnt;

	// 移動を諦めるまでの時間
	const int GIVE_UP_MOVE_CNT = 300;

public:
	NormalAI();
	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);
	void setParam(NormalAI* brain);

	void setRightKey(int rightKey) { m_rightKey = rightKey; }
	void setLeftKey(int leftKey) { m_leftKey = leftKey; }
	void setUpKey(int upKey) { m_upKey = upKey; }
	void setDownKey(int downKey) { m_downKey = downKey; }
	void setJumpCnt(int cnt) { m_jumpCnt = cnt; }
	void setSquatCnt(int cnt) { m_squatCnt = cnt; }
	void setGx(int gx) { m_gx = gx; }
	void setGy(int gy) { m_gy = gy; }
	void setMoveCnt(int cnt) { m_moveCnt = cnt; }
	void setTarget(Character* character) { m_target_p = character; }

	void debug(int x, int y, int color) const;
	void setCharacterAction(const CharacterAction* characterAction);
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();

	// 攻撃対象を決める(targetのままか、characterに変更するか)
	void searchTarget(const Character* character);

	// 攻撃対象を変更する必要があるならtrueでアピールする。
	bool needSearchTarget() const;

protected:
	// スティック操作
	void stickOrder(int& right, int& left, int& up, int& down);
};


class ParabolaAI :
	public NormalAI
{
	void bulletTargetPoint(int& x, int& y);
};


class FollowNormalAI :
	public NormalAI 
{
private:
	// ついていくキャラ
	const Character* m_follow_p;

	// 追跡対象の近くにいるとみなす誤差 ±GX_ERROR
	const int FOLLOW_X_ERROR = 500;

public:
	FollowNormalAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void debug(int x, int y, int color) const;

	int getFollowId() const;

	void setFollow(Character* character) { m_follow_p = character; }

	// 移動の目標地点設定
	void moveOrder(int& right, int& left, int& up, int& down);

	// 追跡対象を決める(AIクラスでオーバライドする。)
	void searchFollow(const Character* character);

	// 追跡対象を変更する必要があるならtrueでアピールする(AIクラスでオーバライドする)。
	bool needSearchFollow() const;
};



/*
* コントローラの基底クラス（キャラクターを操作するクラス）
*/
class CharacterController {
protected:
	// こいつが操作を命令してくる Controllerがデリートする
	Brain* m_brain;

	// 操作対象 Controllerがデリートする
	CharacterAction* m_characterAction;

public:
	CharacterController();
	CharacterController(Brain* brain, CharacterAction* characterAction);
	~CharacterController();

	virtual CharacterController* createCopy(std::vector<Character*> characters, const Camera* camera) = 0;

	// デバッグ
	void debugController(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// ゲッタ
	inline const CharacterAction* getAction() const { return m_characterAction; }
	inline const Brain* getBrain() const { return m_brain; }

	// セッタ
	void setAction(CharacterAction* action);
	void setBrain(Brain* brain);

	// 話しかけたり扉に入ったりするボタンがtrueか
	virtual bool getActionKey() const { return m_brain->actionOrder(); }

	// アクションのセッタ
	void setCharacterGrand(bool grand);
	void setCharacterGrandRightSlope(bool grand);
	void setCharacterGrandLeftSlope(bool grand);
	void setActionRightLock(bool lock);
	void setActionLeftLock(bool lock);
	void setActionUpLock(bool lock);
	void setActionDownLock(bool lock);
	void setActionBoost();

	// キャラクターのセッタ
	void setCharacterX(int x);
	void setCharacterY(int y);

	// 行動前の処理 毎フレーム行う
	void init();

	// 攻撃対象を変更
	void searchTargetCandidate(Character* character);

	// 追跡対象を変更
	void searchFollowCandidate(Character* character);

	// 操作や当たり判定の結果を反映（実際にキャラを動かす）毎フレーム行う
	void action();

	// キャラの操作
	virtual void control() = 0;

	// 射撃攻撃
	virtual Object* bulletAttack() = 0;

	// 斬撃攻撃
	virtual Object* slashAttack() = 0;

	// ダメージ
	virtual void damage(int vx, int vy, int damageValue) = 0;
};

/*
* 普通のコントローラ
*/
class NormalController :
	public CharacterController {

private:
	// ジャンプキーを長押しする最大時間
	const int JUMP_KEY_LONG = 10;
public:
	NormalController(Brain* brain, CharacterAction* characterAction);

	CharacterController* createCopy(std::vector<Character*> characters, const Camera* camera);

	void debug(int x, int y, int color) const;

	// キャラの移動やしゃがみ(;攻撃以外の)操作 毎フレーム行う
	void control();

	// 射撃攻撃
	Object* bulletAttack();

	// 斬撃攻撃
	Object* slashAttack();

	// ダメ―ジ
	void damage(int vx, int vy, int damageValue);
};

#endif
