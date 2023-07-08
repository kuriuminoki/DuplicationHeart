#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

class CharacterController;
class AttackInfo;
class GraphHandle;
class GraphHandles;


/*
* オブジェクトの基底クラス
*/
class Object {
protected:
	// 左上の座標
	int m_x1, m_y1;

	// 右下の座標+1
	int m_x2, m_y2;

	// 削除フラグ trueならWorldに消してもらう
	bool m_deleteFlag;
	// 攻撃で削除可能なオブジェクトならtrue
	bool m_ableDelete;

public:
	Object();
	Object(int x1, int y1, int x2, int y2);

	void debugObject(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタ
	inline bool getDeleteFlag() { return m_deleteFlag; }
	inline bool getAbleDelete() { return m_ableDelete; }
	inline int getX1() const { return m_x1; }
	inline int getX2() const { return m_x2; }
	inline int getY1() const { return m_y1; }
	inline int getY2() const { return m_y2; }

	// 画像を返す　ないならNULL
	virtual GraphHandle* getHandle() const = 0;

	// オブジェクト描画（画像がないときに使う）
	virtual void drawObject(int x1, int y1, int x2, int y2) const = 0;

	// セッタ
	inline void setDeleteFlag(bool deleteFlag) { m_deleteFlag = deleteFlag; }

	// キャラとの当たり判定
	virtual void atari(CharacterController* character) = 0;

	// 他オブジェクトとの当たり判定
	virtual void atariObject(Object* object) = 0;

	// 動くオブジェクト用 毎フレーム行う
	virtual void action() = 0;
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
	BoxObject(int x1, int y1, int x2, int y2, int color);

	void debug(int x, int y, int color);

	// 画像を返す　ないならNULL
	GraphHandle* getHandle() const { return nullptr; }

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 他オブジェクトとの当たり判定
	void atariObject(Object* object);

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
	int getY(int x);
public:
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown);

	void debug(int x, int y, int color);

	// 画像を返す　ないならNULL
	GraphHandle* getHandle() const { return nullptr; }

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 他オブジェクトとの当たり判定
	void atariObject(Object* object);

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

	void debug(int x, int y, int color);

	// 画像を返す　ないならNULL
	GraphHandle* getHandle() const { return nullptr; }

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// セッタ
	// キャラクターIDを取得
	inline void setCharacterId(int id) { m_characterId = id; }

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 他オブジェクトとの当たり判定
	void atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();
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

	// オブジェクトの画像
	GraphHandle* m_handle;

	// ダメージ
	int m_damage;

	// 何フレーム目か
	int m_cnt;

	// 全体フレーム
	int m_slashCountSum;

public:
	// 座標、画像、生存時間、AttackInfo
	SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	// 大きさを指定しない場合。画像からサイズ取得。生存時間、AttackInfo
	SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	void debug(int x, int y, int color);

	// 画像を返す　ないならNULL
	GraphHandle* getHandle() const { return m_handle; }

	// オブジェクト描画（画像がないときに使う）
	void drawObject(int x1, int y1, int x2, int y2) const;

	// セッタ
	inline void setCharacterId(int id) { m_characterId = id; }

	// キャラとの当たり判定
	// 当たっているならキャラを操作する。
	void atari(CharacterController* character);

	// 他オブジェクトとの当たり判定
	void atariObject(Object* object);

	// 動くオブジェクト用 毎フレーム行う
	void action();
};

#endif