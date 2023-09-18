#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include<string>

class Object;
class GraphHandle;
class GraphHandles;
class CharacterGraphHandle;
class FaceGraphHandle;
class SoundPlayer;


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

	// 射撃時に撃つ方向を向くか
	bool m_sameBulletDirection;

	// ジャンプ時の音
	int m_jumpSound;

	// 受け身時の音
	int m_passiveSound;

	// 着地時の音
	int m_landSound;

public:
	// デフォルト値で初期化
	CharacterInfo();
	// csvファイルを読み込み、キャラ名で検索しパラメータ取得
	CharacterInfo(const char* characterName);

	~CharacterInfo();

	// ゲッタのみを持つ
	inline std::string name() const { return m_name; }
	inline int maxHp() const { return m_maxHp; }
	inline double handleEx() const { return m_handleEx; }
	inline int moveSpeed() const { return m_moveSpeed; }
	inline int jumpHeight() const { return m_jumpHeight; }
	inline bool sameBulletDirection() const { return m_sameBulletDirection; }
	inline int jumpSound() const { return m_jumpSound; }
	inline int passiveSound() const { return m_passiveSound; }
	inline int landSound() const { return m_landSound; }
};


class AttackInfo {
private:
	// 弾丸のHP
	int m_bulletHp;

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

	// 斬撃のHP
	int m_slashHp;

	// 斬撃のダメージ
	int m_slashDamage;

	// 斬撃の大きさ(長方形の辺の長さ)
	int m_slashLenX, m_slashLenY;

	// 斬撃の全体フレーム
	int m_slashCountSum;

	// 斬撃の後隙
	int m_slashInterval;

	// 斬撃の吹っ飛び(X方向の初速)
	int m_slashImpactX;

	// 斬撃の吹っ飛び(Y方向の初速)
	int m_slashImpactY;

	// 射撃攻撃のエフェクト
	GraphHandles* m_bulletEffectHandles;

	// 斬撃攻撃のエフェクト
	GraphHandles* m_slashEffectHandles;

	// 射撃攻撃が当たったときのサウンド
	int m_bulletSoundHandle;

	// 弾発射音
	int m_bulletStartSoundHandle;

	// 斬撃攻撃が当たったときのサウンド
	int m_slashSoundHandle;

	// 斬撃開始サウンド
	int m_slashStartSoundHandle;

public:
	// デフォルト値で初期化
	AttackInfo();
	// csvファイルを読み込み、キャラ名で検索しパラメータ取得
	AttackInfo(const char* characterName, double drawEx);

	~AttackInfo();
	
	// ゲッタのみを持つ
	int bulletHp() const { return m_bulletHp; }
	int bulletDamage() const { return m_bulletDamage; }
	int bulletRx() const { return m_bulletRx; }
	int bulletRy() const { return m_bulletRy; }
	int bulletSpeed() const { return m_bulletSpeed; }
	int bulletRapid() { return m_bulletRapid; }
	int bulletDistance() const { return m_bulletDistance; }
	int bulletImpactX() const { return m_bulletImpactX; }
	int bulletImpactY() const { return m_bulletImpactY; }
	int slashHp() const { return m_slashHp; }
	int slashDamage() const { return m_slashDamage; }
	int slashLenX() const { return m_slashLenX; }
	int slashLenY() const { return m_slashLenY; }
	int slashCountSum() const { return m_slashCountSum; }
	int slashInterval() const { return m_slashInterval; }
	int slashImpactX() const { return m_slashImpactX; }
	int slashImpactY() const { return m_slashImpactY; }
	GraphHandles* bulletEffectHandle() const { return m_bulletEffectHandles; }
	GraphHandles* slashEffectHandle() const { return m_slashEffectHandles; }
	int bulletSoundeHandle() const { return m_bulletSoundHandle; }
	int slashSoundHandle() const { return m_slashSoundHandle; }
	int bulletStartSoundeHandle() const { return m_bulletStartSoundHandle; }
	int slashStartSoundHandle() const { return m_slashStartSoundHandle; }
};


/*
* プレイヤーやエネミーの基底クラス
*/
class Character {
protected:
	static int characterId;

	bool m_duplicationFlag;

	// ID
	int m_id;

	// グループID 味方識別用
	int m_groupId;

	// 残り体力
	int m_hp;

	// X座標、Y座標
	int m_x, m_y;

	// 左を向いている
	bool m_leftDirection;

	// キャラの情報
	CharacterInfo* m_characterInfo;

	// 攻撃の情報
	AttackInfo* m_attackInfo;

	// キャラ画像
	CharacterGraphHandle* m_graphHandle;

	// 顔画像
	FaceGraphHandle* m_faceHandle;

public:
	// コンストラクタ
	Character();
	Character(int hp, int x, int y, int groupId);
	~Character();

	virtual Character* createCopy() = 0;

	// デバッグ
	void debugCharacter(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// ゲッタ
	inline int getId() const { return m_id; }
	inline int getGroupId() const { return m_groupId; }
	inline int getHp() const { return m_hp; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getLeftDirection() const { return m_leftDirection; }
	FaceGraphHandle* getFaceHandle() const { return m_faceHandle; }
	inline CharacterGraphHandle* getCharacterGraphHandle() const { return m_graphHandle; }
	inline AttackInfo* getAttackInfo() const { return m_attackInfo; }
	inline CharacterInfo* getCharacterInfo() const { return m_characterInfo; }

	// セッタ
	inline void setHp(int hp) { m_hp = (hp > m_characterInfo->maxHp()) ? m_characterInfo->maxHp() : hp; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setId(int id) { m_id = id; }
	inline void setGroupId(int id) { m_groupId = id; }
	// キャラの向き変更は、画像の反転も行う
	void setLeftDirection(bool leftDirection);
	inline void setDuplicationFlag(bool flag) { m_duplicationFlag = flag; }
	inline void setAttackInfo(AttackInfo* attackInfo) { m_attackInfo = attackInfo; }
	inline void setCharacterInfo(CharacterInfo* characterInfo) { m_characterInfo = characterInfo; }

	// CharacterInfoからキャラのスペックを取得
	inline std::string getName() const { return m_characterInfo->name(); }
	inline int getMaxHp() const { return m_characterInfo->maxHp(); }
	inline int getMoveSpeed() const { return m_characterInfo->moveSpeed(); }
	inline int getJumpHeight() const { return m_characterInfo->jumpHeight(); }
	inline int getJumpSound() const { return m_characterInfo->jumpSound(); }
	inline int getPassiveSound() const { return m_characterInfo->passiveSound(); }
	inline int getLandSound() const { return m_characterInfo->landSound(); }

	// AttackInfoから攻撃のスペックを取得
	inline int getBulletRapid() const { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() const { return m_attackInfo->slashCountSum(); }
	inline int getSlashInterval() const { return m_attackInfo->slashInterval(); }

	// 画像の情報を取得
	int getCenterX() const;
	int getCenterY() const;
	int getWide() const;
	int getHeight() const;
	// 今描画する画像を取得
	GraphHandle* getGraphHandle() const;
	void getHandleSize(int& wide, int& height) const;

	// 立ち画像をセット
	virtual void switchStand(int cnt = 0);
	// 立ち射撃画像をセット
	virtual void switchBullet(int cnt = 0);
	// 立ち斬撃画像をセット
	virtual void switchSlash(int cnt = 0);
	// しゃがみ画像をセット
	virtual void switchSquat(int cnt = 0);
	// しゃがみ射撃画像をセット
	virtual void switchSquatBullet(int cnt = 0);
	// 走り画像をセット
	virtual void switchRun(int cnt = 0);
	// 走り射撃画像をセット
	virtual void switchRunBullet(int cnt = 0);
	// 着地画像をセット
	virtual void switchLand(int cnt = 0);
	// 上昇画像をセット
	virtual void switchJump(int cnt = 0);
	// 降下画像をセット
	virtual void switchDown(int cnt = 0);
	// ジャンプ前画像をセット
	virtual void switchPreJump(int cnt = 0);
	// ダメージ画像をセット
	virtual void switchDamage(int cnt = 0);
	// ブースト画像をセット
	virtual void switchBoost(int cnt = 0);
	// 空中射撃画像をセット
	virtual void switchAirBullet(int cnt = 0);
	// 空中斬撃画像をセット
	virtual void switchAirSlash(int cnt = 0);

	// HP減少
	void damageHp(int value);

	// 移動する（座標を動かす）
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// 射撃攻撃をする(キャラごとに違う)
	virtual Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) { return NULL; }

	// 斬撃攻撃をする(キャラごとに違う) 左を向いているか、今何カウントか
	virtual Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) { return NULL; }
};


Character* createCharacter(const char* characterName, int hp = 100, int x = 0, int y = 0, int groupId = 0);


/*
* ハート（主人公）
*/
class Heart :
	public Character
{
private:
	//// 走りアニメのスピード
	const int RUN_ANIME_SPEED = 6;
	
	//// ジャンプ前アニメのスピード
	const int RUN_PREJUMP_SPEED = 6;
	
public:
	// コンストラクタ
	Heart(const char* name, int hp, int x, int y, int groupId);
	Heart(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	// デストラクタ
	~Heart();

	Character* createCopy();

	// デバッグ
	void debug(int x, int y, int color) const;

	// 画像変更関数のオーバーライド
	// 走り画像をセット
	void switchRun(int cnt = 0);

	// 走り射撃画像をセット
	void switchRunBullet(int cnt = 0);

	// ジャンプ前画像をセット
	void switchPreJump(int cnt = 0);

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


/*
* シエスタ
*/
class Siesta :
	public Heart
{
public:
	// コンストラクタ
	Siesta(const char* name, int hp, int x, int y, int groupId);
	Siesta(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


/*
* ヒエラルキー
*/
class Hierarchy :
	public Heart
{
public:
	// コンストラクタ
	Hierarchy(const char* name, int hp, int x, int y, int groupId);
	Hierarchy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


/*
* ヴァルキリア
*/
class Valkyria :
	public Heart
{
public:
	// コンストラクタ
	Valkyria(const char* name, int hp, int x, int y, int groupId);
	Valkyria(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// 射撃攻撃をする(キャラごとに違う)
	Object* bulletAttack(int gx, int gy, SoundPlayer* soundPlayer);

	// 斬撃攻撃をする(キャラごとに違う)
	Object* slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer);
};


#endif