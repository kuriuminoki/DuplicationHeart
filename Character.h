#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include<string>

class Object;
class GraphHandle;
class GraphHandles;


class CharacterInfo {
private:
	// キャラの名前
	std::string m_name;

	// キャラの初期体力
	int m_maxHp;

	// 画像の拡大率
	double m_handleEx;

	// 走るスピード
	int m_moveSpeed;

	// ジャンプ時のY方向の初速
	int m_jumpHeight;

public:
	// デフォルト値で初期化
	CharacterInfo();
	// csvファイルを読み込み、キャラ名で検索しパラメータ取得
	CharacterInfo(const char* characterName);

	// ゲッタのみを持つ
	inline std::string name() { return m_name; }
	inline int maxHp() { return m_maxHp; }
	inline double handleEx() { return m_handleEx; }
	inline int moveSpeed() { return m_moveSpeed; }
	inline int jumpHeight() { return m_jumpHeight; }
};


class AttackInfo {
private:
	// 弾丸のダメージ
	int m_bulletDamage;

	// 弾丸の大きさ(半径)
	int m_bulletRx, m_bulletRy;

	// 弾丸のスピード
	int m_bulletSpeed;

	// 弾丸の連射力
	int m_bulletRapid;

	// 弾丸の飛距離
	int m_bulletDistance;

	// 弾丸の吹っ飛び(X方向の初速)
	int m_bulletImpactX;

	// 弾丸の吹っ飛び(Y方向の初速)
	int m_bulletImpactY;

	// 斬撃のダメージ
	int m_slashDamage;

	// 斬撃の大きさ(長方形の辺の長さ)
	int m_slashLenX, m_slashLenY;

	// 斬撃の全体フレーム
	int m_slashCountSum;

	// 斬撃の吹っ飛び(X方向の初速)
	int m_slashImpactX;

	// 斬撃の吹っ飛び(Y方向の初速)
	int m_slashImpactY;

public:
	// デフォルト値で初期化
	AttackInfo();
	// csvファイルを読み込み、キャラ名で検索しパラメータ取得
	AttackInfo(const char* characterName);
	
	// ゲッタのみを持つ
	int bulletDamage() { return m_bulletDamage; }
	int bulletRx() { return m_bulletRx; }
	int bulletRy() { return m_bulletRy; }
	int bulletSpeed() { return m_bulletSpeed; }
	int bulletRapid() { return m_bulletRapid; }
	int bulletDistance() { return m_bulletDistance; }
	int bulletImpactX() { return m_bulletImpactX; }
	int bulletImpactY() { return m_bulletImpactY; }
	int slashDamage() { return m_slashDamage; }
	int slashLenX() { return m_slashLenX; }
	int slashLenY() { return m_slashLenY; }
	int slashCountSum() { return m_slashCountSum; }
	int slashImpactX() { return m_slashImpactX; }
	int slashImpactY() { return m_slashImpactY; }
};


/*
* プレイヤーやエネミーの基底クラス
*/
class Character {
protected:
	static int characterId;

	// ID
	int m_id;

	// グループID 味方識別用
	int m_groupId;

	// 最大体力
	int m_maxHp;

	// 残り体力
	int m_hp;

	// X座標、Y座標
	int m_x, m_y;

	// 横幅と縦幅
	int m_wide, m_height;

	// 表示される画像
	GraphHandle* m_graphHandle;

	// キャラの情報
	CharacterInfo* m_characterInfo;

	// 攻撃の情報
	AttackInfo* m_attackInfo;

	// 左を向いている
	bool m_leftDirection;

public:
	// コンストラクタ
	Character();
	Character(int maxHp, int hp, int x, int y);
	~Character();

	// デバッグ
	void debugCharacter(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// ゲッタとセッタ
	inline int getId() { return m_id; }

	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	inline int getX() const { return m_x; }

	inline void setX(int x) { m_x = x; }

	inline int getCenterX() const{ return m_x + (m_wide / 2); }
	inline int getCenterY() const { return m_y + (m_height / 2); }

	inline int getY() const { return m_y; }

	inline void setY(int y) { m_y = y; }

	inline int getWide() const { return m_wide; }

	inline int getHeight() const { return m_height; }

	inline int getMoveSpeed() { return m_characterInfo->moveSpeed(); }

	inline int getJumpHeight() { return m_characterInfo->jumpHeight(); }

	inline std::string getName() { return m_characterInfo->name(); }

	inline bool getLeftDirection() { return m_leftDirection; }

	void setLeftDirection(bool leftDirection);

	inline const GraphHandle* getGraphHandle() const { return m_graphHandle; }

	// AttackInfoのセッタとゲッタ
	inline int getBulletRapid() { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() { return m_attackInfo->slashCountSum(); }

	// 画像のセッタ。画像の横幅(wide)と縦幅(height)もセットする。
	void setHandle(GraphHandle* handle);
	void getHandleSize(int& wide, int& height);

	// 立ち画像をセット
	virtual void switchStand(int cnt = 0) = 0;
	// 立ち射撃画像をセット
	virtual void switchBullet(int cnt = 0) = 0;
	// 立ち斬撃画像をセット
	virtual void switchSlash(int cnt = 0) = 0;
	// しゃがみ画像をセット
	virtual void switchSquat(int cnt = 0) = 0;
	// 走り画像をセット
	virtual void switchRun(int cnt = 0) = 0;
	// 着地画像をセット
	virtual void switchLand(int cnt = 0) = 0;
	// 上昇画像をセット
	virtual void switchJump(int cnt = 0) = 0;
	// 降下画像をセット
	virtual void switchDown(int cnt = 0) = 0;
	// ジャンプ前画像をセット
	virtual void switchPreJump(int cnt = 0) = 0;
	// ダメージ画像をセット
	virtual void switchDamage(int cnt = 0) = 0;
	// ブースト画像をセット
	virtual void switchBoost(int cnt = 0) = 0;
	// 空中射撃画像をセット
	virtual void switchAirBullet(int cnt = 0) = 0;
	// 空中斬撃画像をセット
	virtual void switchAirSlash(int cnt = 0) = 0;


	// 移動する（座標を動かす）
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// 射撃攻撃をする(キャラごとに違う)
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// 斬撃攻撃をする(キャラごとに違う) 左を向いているか、今何カウントか
	virtual Object* slashAttack(bool leftDirection, int cnt) = 0;
};


/*
* ハート（主人公）
*/
class Heart :
	public Character
{
private:
	// キャラの名前
	const char* const NAME = "ハート";

	// 立ち画像
	GraphHandle* m_standHandle;

	// 斬撃攻撃画像
	GraphHandles* m_slashHandles;

	// しゃがみ
	GraphHandle* m_squatHandle;

	// 立ち射撃
	GraphHandle* m_standBulletHandle;

	// 立ち斬撃
	GraphHandle* m_standSlashHandle;

	// 走り
	const int RUN_ANIME_SPEED = 6;
	GraphHandles* m_runHandles;

	// 着地
	GraphHandle* m_landHandle;

	// 上昇
	GraphHandle* m_jumpHandle;

	// 下降
	GraphHandle* m_downHandle;

	// ジャンプ前
	const int RUN_PREJUMP_SPEED = 6;
	GraphHandles* m_preJumpHandles;

	// ダメージ
	GraphHandle* m_damageHandle;

	// ブースト
	GraphHandle* m_boostHandle;

	// 空中射撃
	GraphHandle* m_airBulletHandle;

	// 空中斬撃
	GraphHandle* m_airSlashHandle;
	
public:
	// コンストラクタ
	Heart(int maxHp, int hp, int x, int y);

	// デストラクタ
	~Heart();

	// デバッグ
	void debug(int x, int y, int color);

	// 立ち画像をセット
	inline void switchStand(int cnt = 0) { setHandle(m_standHandle); }
	// 立ち射撃画像をセット
	inline void switchBullet(int cnt = 0) { setHandle(m_standBulletHandle); }
	// 立ち斬撃画像をセット
	inline void switchSlash(int cnt = 0) { setHandle(m_standSlashHandle); }
	// しゃがみ画像をセット
	inline void switchSquat(int cnt = 0) { setHandle(m_squatHandle); }
	// 走り画像をセット
	void switchRun(int cnt = 0);
	// 着地画像をセット
	inline void switchLand(int cnt = 0) { setHandle(m_landHandle); }
	// 上昇画像をセット
	inline void switchJump(int cnt = 0) { setHandle(m_jumpHandle); }
	// 降下画像をセット
	inline void switchDown(int cnt = 0) { setHandle(m_downHandle); }
	// ジャンプ前画像をセット
	void switchPreJump(int cnt = 0);
	// ダメージ画像をセット
	inline void switchDamage(int cnt = 0) { setHandle(m_damageHandle); }
	// ブースト画像をセット
	inline void switchBoost(int cnt = 0) { setHandle(m_boostHandle); }
	// 空中射撃画像をセット
	inline void switchAirBullet(int cnt = 0) { setHandle(m_airBulletHandle); }
	// 空中斬撃画像をセット
	inline void switchAirSlash(int cnt = 0) { setHandle(m_airSlashHandle); }

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt);
};


#endif