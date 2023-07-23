#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <cstdlib>


using namespace std;


CharacterInfo::CharacterInfo() :
	CharacterInfo("test")
{

}
// CSVファイルから読み込む
CharacterInfo::CharacterInfo(const char* characterName) {
	CsvReader reader("data/characterInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);

	// パラメータを設定
	m_name = data["name"];
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
}


AttackInfo::AttackInfo():
	AttackInfo("test")
{

}
// CSVファイルから読み込む
AttackInfo::AttackInfo(const char* characterName) {
	CsvReader reader("data/attackInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);

	// パラメータを設定
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);
}


int Character::characterId;

Character::Character() :
	Character(100, 100, 0, 0)
{

}

Character::Character(int maxHp, int hp, int x, int y) {
	// IDを振る
	characterId++;
	m_id = characterId;

	m_groupId = 0;

	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_graphHandle = NULL;

	m_characterInfo = NULL;
	m_attackInfo = NULL;

	m_leftDirection = true;
}

Character::~Character() {
	// CharacterInfoの削除
	if (m_characterInfo != NULL) {
		delete m_characterInfo;
	}
	// AttackInfoの削除
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
}

void Character::setHandle(GraphHandle* handle) {

	m_graphHandle = handle;

	// 画像の縦幅と横幅を取得する。
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);

	// 画像の拡大率も考慮してサイズを決定
	m_wide = (int)(m_wide * m_characterInfo->handleEx());
	m_height = (int)(m_height * m_characterInfo->handleEx());
}

void Character::getHandleSize(int& wide, int& height) {
	// 今セットしている画像の縦幅と横幅を取得する。
	wide = m_wide;
	height = m_height;
}

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->setReverseX(m_leftDirection);
}

// HP減少
void Character::damageHp(int value) {
	m_hp = max(0, m_hp - value);
}

// 移動する（座標を動かす）
void Character::moveRight(int d) {
	m_x += d;
}
void Character::moveLeft(int d) {
	m_x -= d;
}
void Character::moveUp(int d) {
	m_y -= d;
}
void Character::moveDown(int d) {
	m_y += d;
}


/*
* ハート
*/
Heart::Heart(int maxHp, int hp, int x, int y) :
	Character(maxHp, hp, x, y)
{
	// キャラ固有の情報設定
	m_characterInfo = new CharacterInfo(NAME);
	m_attackInfo = new AttackInfo(NAME);

	// 各画像のロード
	double ex = m_characterInfo->handleEx();
	m_standHandle = new GraphHandle("picture/stick/stand.png", ex);
	m_slashHandles = new GraphHandles("picture/stick/slashEffect", 3, ex, 0.0, true);
	m_squatHandle = new GraphHandle("picture/stick/squat.png", ex);
	m_standBulletHandle = new GraphHandle("picture/stick/bullet.png", ex);
	m_standSlashHandle = new GraphHandle("picture/stick/slash.png", ex);
	m_runHandles = new GraphHandles("picture/stick/run", 6, ex);
	m_landHandle = new GraphHandle("picture/stick/land.png", ex);
	m_jumpHandle = new GraphHandle("picture/stick/jump.png", ex);
	m_downHandle = new GraphHandle("picture/stick/down.png", ex);
	m_preJumpHandles = new GraphHandles("picture/stick/preJump", 2, ex);
	m_damageHandle = new GraphHandle("picture/stick/damage.png", ex);
	m_boostHandle = new GraphHandle("picture/stick/boost.png", ex);
	m_airBulletHandle = new GraphHandle("picture/stick/airBullet.png", ex);
	m_airSlashHandle = new GraphHandle("picture/stick/airSlash.png", ex);
	// 攻撃のエフェクト
	m_bulletEffectHandles = new GraphHandles("picture/effect/オレンジ", 4, ex, 0.0, true);
	m_slashEffectHandles = m_bulletEffectHandles;

	// とりあえず立ち画像でスタート
	switchStand();
}

// デストラクタ
Heart::~Heart() {
	// 画像を削除
	delete m_standHandle;
	delete m_slashHandles;
	delete m_squatHandle;
	delete m_standBulletHandle;
	delete m_standSlashHandle;
	delete m_runHandles;
	delete m_landHandle;
	delete m_jumpHandle;
	delete m_downHandle;
	delete m_preJumpHandles;
	delete m_damageHandle;
	delete m_boostHandle;
	delete m_airBulletHandle;
	delete m_airSlashHandle;
	delete m_bulletEffectHandles;
}

// 走り画像をセット
void Heart::switchRun(int cnt) { 
	int index = (cnt / RUN_ANIME_SPEED) % (m_runHandles->getSize());
	setHandle(m_runHandles->getGraphHandle(index));
}
// ジャンプ前画像をセット
void Heart::switchPreJump(int cnt) { 
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_preJumpHandles->getSize());
	setHandle(m_preJumpHandles->getGraphHandle(index));
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x + m_wide / 2, m_y + m_height / 2, WHITE, gx, gy, m_attackInfo, m_bulletEffectHandles);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(bool leftDirection, int cnt) {
	// 攻撃範囲を決定
	int centerX = m_x + m_wide / 2;
	int height = m_attackInfo->slashLenY() / 2;
	int centerY = m_y + m_height / 2;
	int x2 = centerX;
	if (leftDirection) { // 左向きに攻撃
		x2 -= m_attackInfo->slashLenX();
	}
	else { // 右向きに攻撃
		x2 += m_attackInfo->slashLenX();
	}

	// 攻撃の画像と持続時間(cntを考慮して決定)
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = NULL;
	m_slashHandles->setReverseX(m_leftDirection);
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo, m_slashEffectHandles);
	}
	// 自滅防止
	if (attackObject != NULL) {
		attackObject->setCharacterId(m_id);
	}
	return attackObject;
}
