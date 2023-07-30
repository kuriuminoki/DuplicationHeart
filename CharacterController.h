#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

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
	virtual void searchTarget(const Character* character) = 0;

	// 攻撃対象を変更する必要があるならtrueでアピールする(AIクラスでオーバライドする)。
	virtual bool needSearchTarget() const = 0;
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
	inline void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }
	void bulletTargetPoint(int& x, int& y);
	bool actionOrder();
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
	void searchTarget(const Character* character) {  }
	bool needSearchTarget() const { return false; }
};

/*
*  普通に敵と戦うよう命令するＡＩのクラス
*/
class NormalAI :
	public Brain
{
private:
	// 攻撃対象
	const Character* m_target_p;

	// 射撃の精度
	const int BULLET_ERROR = 400;

	// 移動目標
	int m_gx, m_gy;

	// 移動目標達成とみなす誤差 ±GX_ERROR
	const int GX_ERROR = 100;

	// 移動時間
	int m_moveCnt;

	// 移動を諦めるまでの時間
	const int GIVE_UP_MOVE_CNT = 300;

	// 移動用
	int m_rightKey, m_leftKey, m_upKey, m_downKey;

	// ジャンプの長さ
	int m_jumpCnt;

	// しゃがむ長さ
	int m_squatCnt;

public:
	NormalAI();
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

	// デバッグ
	void debugController(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// ゲッタ
	inline const CharacterAction* getAction() const { return m_characterAction; }
	inline const Brain* getBrain() const { return m_brain; }

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

	// 攻撃対象を変更（するかも）
	void searchTargetCandidate(Character* character);

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
