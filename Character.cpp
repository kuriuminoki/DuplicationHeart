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

	m_slashHandles = new GraphHandles("picture/stick/zangeki", 3, ex);
}

// デストラクタ
Heart::~Heart() {
	// 画像を削除
	delete m_standHandle;
	delete m_slashHandles;
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x, m_y, WHITE, gx, gy, m_attackInfo);
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(bool leftDirection, int cnt) {
	// 攻撃範囲を決定
	int x2 = m_x;
	int y2 = m_y + m_attackInfo->slashLenY();
	if (leftDirection) { // 左向きに攻撃
		x2 -= m_attackInfo->slashLenX();
	}
	else { // 右向きに攻撃
		x2 += m_attackInfo->slashLenX();
	}

	// 攻撃の画像と持続時間(cntを考慮して決定)
	int index = 0;
	int slashCountSum = 5;
	SlashObject* attackObject = NULL;
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	return attackObject;
}
