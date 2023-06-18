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
	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	inline int getX() { return m_x; }

	inline void setX(int x) { m_x = x; }

	inline int getY() { return m_y; }

	inline void setY(int y) { m_y = y; }

	inline int getWide() { return m_wide; }

	inline int getHeight() { return m_height; }

	inline int getMoveSpeed() { return m_characterInfo->moveSpeed(); }

	inline int getJumpHeight() { return m_characterInfo->jumpHeight(); }

	inline std::string getName() { return m_characterInfo->name(); }

	inline bool getLeftDirection() { return m_leftDirection; }

	inline void setLeftDirection(bool leftDirection) { m_leftDirection = leftDirection; }

	// AttackInfoのセッタとゲッタ
	inline int getBulletRapid() { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() { return m_attackInfo->slashCountSum(); }

	// 画像のセッタ。画像の横幅(wide)と縦幅(height)もセットする。
	void setHandle(GraphHandle* handle);

	// 立ち画像をセット
	virtual void switchStand() = 0;

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

	//立ち画像
	GraphHandle* m_standHandle;

	// 斬撃攻撃画像
	GraphHandles* m_slashHandles;
	
public:
	// コンストラクタ
	Heart(int maxHp, int hp, int x, int y);

	// デストラクタ
	~Heart();

	// デバッグ
	void debug(int x, int y, int color);

	// 立ち画像をセット
	inline void switchStand() { setHandle(m_standHandle); }

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt);
};


#endif