#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <string>

class Character;
class CharacterController;
class AttackInfo;
class GraphHandle;
class GraphHandles;
class Animation;


/*
* オブジェクトの基底クラス
*/
class Object {
protected:
	// 左上の座標
	int m_x1, m_y1;

	// 右下の座標+1
	int m_x2, m_y2;

	// HP -1なら無敵
	int m_hp;

	// ダメージ状態（描画用）
	int m_damageCnt;
	const int DAMAGE_CNT_SUM = 5;

	// 削除フラグ trueならWorldに消してもらう
	bool m_deleteFlag;

	// エフェクト
	GraphHandles* m_effectHandles_p;

	// サウンド
	int m_soundHandle_p;

public:
	Object();
	Object(int x1, int y1, int x2, int y2, int hp = -1);

	void debugObject(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// ゲッタ
	inline bool getDeleteFlag() const { return m_deleteFlag; }
	inline bool getAbleDelete() const { return m_hp != -1 ? true : false; }
	inline int getX1() const { return m_x1; }
	inline int getX2() const { return m_x2; }
	inline int getY1() const { return m_y1; }
	inline int getY2() const { return m_y2; }
	inline int getCenterX() const { return (m_x1 + m_x2) / 2; }
	inline int getCenterY() const { return (m_y1 + m_y2) / 2; }
	inline int getSoundHandle() const { return m_soundHandle_p; }
	inline int getHp() const { return m_hp; }
	inline int getDamageCnt() const { return m_damageCnt; }

	// セッタ
	inline void setDeleteFlag(bool deleteFlag) { m_deleteFlag = deleteFlag; }

	// HPを減らす
	void decreaseHp(int damageValue);

	// グループIDのゲッタ
	virtual inline int getGroupId() const { return -1; }

	// 攻撃力 攻撃オブジェクト用
	virtual inline int getDamage() const { return 0; }

	// 扉用
	virtual inline int getAreaNum() const { return -1; }

	// 画像を返す　ないならNULL
	virtual GraphHandle* getHandle() const { return nullptr; }

	// テキストを返す ないならNULL
	virtual inline std::string getText() const { return ""; }

	// オブジェクト描画（画像がないときに使う）
	virtual void drawObject(int x1, int y1, int x2, int y2) const {};

	// キャラとの当たり判定
	virtual bool atari(CharacterController* characterController) = 0;

	// キャラがオブジェクトに入り込んでいるときの処理
	virtual void penetration(CharacterController* characterController) {};

	// 攻撃オブジェクトとの当たり判定
	virtual bool atariObject(Object* object) { return false; }

	// 動くオブジェクト用 毎フレーム行う
	virtual void action() {};

	// アニメーション作成
	virtual Animation* createAnimation(int x, int y, int flameCnt) { return nullptr; };
};


/*
* 四角形のオブジェクト　床や壁
*/
class BoxObject :
	public Object 
{
private:
	// 段差とみなして自動で乗り越えられる高さ
	const int STAIR_HEIGHT = 50;

	// オブジェクトの色
	int m_color;

public:
	BoxObject(int x1, int y1, int x2, int y2, int color, int hp = -1);

	void debug(int x, int y, int color) const;

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	bool atari(CharacterController* characterController);

	// キャラがオブジェクトに入り込んでいるときの処理
	void penetration(CharacterController* characterController);

	// 攻撃オブジェクトとの当たり判定
	bool atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

/*
* 直角三角形のオブジェクトつまり坂
*/
class TriangleObject :
	public Object
{
private:
	// オブジェクトの色
	int m_color;

	// 左向きに下がっている坂
	bool m_leftDown;

	// 座標XにおけるY座標（傾きから算出する）
	int getY(int x) const;
public:
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown, int hp = -1);

	void debug(int x, int y, int color) const;

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	bool atari(CharacterController* characterController);

	// キャラがオブジェクトに入り込んでいるときの処理
	void penetration(CharacterController* characterController);

	// 攻撃オブジェクトとの当たり判定
	bool atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

/*
* 直進する弾のオブジェクト
* gx, gyに向かって飛んでいき、一定距離移動したら消滅
*/
class BulletObject :
	public Object
{
private:
	// この攻撃を出したキャラのＩＤ 自滅防止用
	int m_characterId;

	// この攻撃が当たらないグループのID チームキル防止用
	int m_groupId;

	// オブジェクトの色
	int m_color;

	// 半径
	int m_rx;
	int m_ry;

	// スピード
	int m_v;
	int m_vx;
	int m_vy;

	// 目標地点
	int m_gx;
	int m_gy;

	// 残りの飛距離
	int m_d;

	// ダメージ
	int m_damage;

public:
	// x, y, gx, gyは弾の中心座標
	BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo);

	void debug(int x, int y, int color) const;

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// ゲッタ
	inline int getGroupId() const { return m_groupId; }

	// セッタ
	// キャラクターIDをセット
	inline void setCharacterId(int id) { m_characterId = id; }
	inline void setGroupId(int id) { m_groupId = id; }

	// 攻撃力 攻撃オブジェクト用
	inline int getDamage() const { return m_damage; }

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	bool atari(CharacterController* characterController);

	// 攻撃オブジェクトとの当たり判定
	bool atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();

	// アニメーション作成
	Animation* createAnimation(int x, int y, int flameCnt);
};


/*
* 近距離攻撃のオブジェクト
* 一定時間経過したら消滅
*/
class SlashObject :
	public Object
{
private:
	// この攻撃を出したキャラのＩＤ 自滅やチームキル防止用
	int m_characterId;

	// この攻撃が当たらないグループのID チームキル防止用
	int m_groupId;

	// オブジェクトの画像
	GraphHandle* m_handle;

	// ダメージ
	int m_damage;

	// 何フレーム目か
	int m_cnt;

	// 全体フレーム
	int m_slashCountSum;

	// 斬撃の吹っ飛び(X方向の初速)
	int m_slashImpactX;

	// 斬撃の吹っ飛び(Y方向の初速)
	int m_slashImpactY;

public:
	// 座標、画像、生存時間、AttackInfo
	SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	// 大きさを指定しない場合。画像からサイズ取得。生存時間、AttackInfo
	SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	void debug(int x, int y, int color) const;

	// 画像を返す　ないならNULL
	GraphHandle* getHandle() const { return m_handle; }

	// ゲッタ
	inline int getGroupId() const { return m_groupId; }

	// セッタ
	inline void setCharacterId(int id) { m_characterId = id; }
	inline void setGroupId(int id) { m_groupId = id; }

	// 攻撃力 攻撃オブジェクト用
	inline int getDamage() const { return m_damage; }

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	bool atari(CharacterController* characterController);

	// 攻撃オブジェクトとの当たり判定
	bool atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();

	// アニメーション作成
	Animation* createAnimation(int x, int y, int flameCnt);
};


// 扉オブジェクト
class DoorObject :
	public Object 
{
private:
	// 画像
	GraphHandle* m_graph;

	// 行先のエリア番号
	int m_areaNum;

	// チュートリアルのテキスト
	std::string m_text;

public:
	DoorObject(int x1, int y1, int x2, int y2, const char* fileName, int areaNum);
	~DoorObject();

	void debug(int x, int y, int color) const;

	// ゲッタ
	GraphHandle* getHandle() const { return m_graph; }
	inline int getAreaNum() const { return m_areaNum; }
	inline std::string getText() const { return m_text; }

	// キャラとの当たり判定
	virtual bool atari(CharacterController* characterController);
};

#endif