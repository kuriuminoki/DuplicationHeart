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
	const CharacterAction* m_characterAction;

public:
	Brain();

	// セッタ
	void setCharacterAction(const CharacterAction* characterAction) { m_characterAction = characterAction; }
	
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
	void searchTarget(Character* character) {  }
};

/*
* キーボードでキャラの操作を命令するクラス
*/
class KeyboardBrain :
	public Brain
{
private:
	// カメラ
	const Camera* m_camera;

public:
	KeyboardBrain(const Camera* camera);
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};

/*
*  普通に敵と戦うよう命令するＡＩのクラス
*/
class NormalAI :
	public Brain
{
private:
	// 攻撃対象
	Character* m_target;
public:
	NormalAI();
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();

	// 攻撃対象を決める(targetのままか、characterに変更するか)
	void searchTarget(Character* character);
};



/*
* コントローラの基底クラス（キャラクターを操作するクラス）
*/
class CharacterController {
protected:
	// こいつが操作を命令してくる
	Brain* m_brain;

	// 操作対象
	CharacterAction* m_characterAction;

public:
	CharacterController();
	CharacterController(Brain* brain, CharacterAction* characterAction);
	~CharacterController();

	// デバッグ
	void debugController(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// アクションの情報取得
	inline const CharacterAction* getAction() const { return m_characterAction; }

	// アクションのセッタ
	void setCharacterGrand(bool grand);
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
* 普通のコントローラ
*/
class NormalController :
	public CharacterController {

private:
	// ジャンプキーを長押しする最大時間
	const int JUMP_KEY_LONG = 10;
public:
	NormalController(Brain* brain, CharacterAction* characterAction);

	void debug(int x, int y, int color);

	// キャラの移動やしゃがみ(;攻撃以外の)操作 毎フレーム行う
	void control();

	// 射撃攻撃
	Object* bulletAttack();

	// 斬撃攻撃
	Object* slashAttack();
};

#endif
