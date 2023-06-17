#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
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
	m_slashRx = stoi(data["slashRx"]);
	m_slashRy = stoi(data["slashRy"]);
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
	m_handle = -1;

	m_characterInfo = NULL;
	m_attackInfo = NULL;

	m_leftDirection = true;
}

Character::~Character() {
	if (m_characterInfo != NULL) {
		delete m_characterInfo;
	}
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
}

void Character::setHandle(int handle) {

	m_handle = handle;

	// 画像の縦幅と横幅を取得する。
	GetGraphSize(m_handle, &m_wide, &m_height);

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
	m_standHandle = LoadGraph("picture/stick/stand.png");

	m_slashHandleSum = 3;
	m_slashHandles = new int[m_slashHandleSum];

	m_attackInfo = new AttackInfo();
}

// デストラクタ
Heart::~Heart() {
	DeleteGraph(m_standHandle);
	for (int i = 0; i < m_slashHandleSum; i++) {
		DeleteGraph(m_slashHandles[i]);
	}
	delete m_slashHandles;
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x, m_y,
		m_x + m_attackInfo->bulletRx(), m_y + m_attackInfo->bulletRy(),
		WHITE, gx, gy,
		m_attackInfo->bulletDamage(), m_attackInfo->bulletSpeed(), m_attackInfo->bulletDistance());
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(bool leftDirection) {
	int x2 = m_x;
	int y2 = m_y;
	if (leftDirection) { // 左向きに攻撃
		x2 -= m_attackInfo->slashRx();
		y2 -= m_attackInfo->slashRy();
	}
	else { // 右向きに攻撃
		x2 += m_attackInfo->slashRx();
		y2 += m_attackInfo->slashRy();
	}
	SlashObject* attackObject = new SlashObject(m_x, m_y, 
		m_x + m_attackInfo->slashRx(), m_y + m_attackInfo->slashRy(),
		m_slashHandles, m_slashHandleSum, 
		m_attackInfo->slashDamage(), m_attackInfo->slashCountSum());
	return attackObject;
}
