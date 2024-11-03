#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "Sound.h"
#include "Game.h"
#include "DxLib.h"
#include <cstdlib>
#include <sstream>


using namespace std;


Character* createCharacter(const char* characterName, int hp, int x, int y, int groupId) {
	Character* character = nullptr;
	string name = characterName;
	if (name == "テスト") {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "ハート") {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "シエスタ") {
		character = new Siesta(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "ヒエラルキー") {
		character = new Hierarchy(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "ヴァルキリア" || name == "フレンチ") {
		character = new Valkyria(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "トロイ") {
		character = new Troy(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "コハル") {
		character = new Koharu(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "棒人間" || name == "クロ人間") {
		character = new SlashOnly(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "緑人間" || name == "フェーレース") {
		character = new BulletOnly(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "大砲") {
		character = new ParabolaOnly(name.c_str(), hp, x, y, groupId);
	}
	else if (name == "サン") {
		character = new Sun(name.c_str(), hp, x, y, groupId);
	}
	else {
		character = new Heart(name.c_str(), hp, x, y, groupId);
	}
	return character;
}


CharacterInfo::CharacterInfo() :
	CharacterInfo("test")
{

}
// CSVファイルから読み込む
CharacterInfo::CharacterInfo(const char* characterName) {
	CsvReader reader("data/characterInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// パラメータを設定
	setParam(data);
	m_name = data["name"];

	// 効果音をロード
	string filePath = "sound/stick/";
	string fileName;
	fileName = filePath + data["jumpSound"];
	if (fileName != "null") {
		m_jumpSound = LoadSoundMem(fileName.c_str());
	}
	fileName = filePath + data["passiveSound"];
	if (fileName != "null") {
		m_passiveSound = LoadSoundMem(fileName.c_str());
	}
	fileName = filePath + data["landSound"];
	if (fileName != "null") {
		m_landSound = LoadSoundMem(fileName.c_str());
	}
}

CharacterInfo::~CharacterInfo() {
	// サウンドを削除
	DeleteSoundMem(m_jumpSound);
	DeleteSoundMem(m_passiveSound);
	DeleteSoundMem(m_landSound);
}

// バージョン変更
void CharacterInfo::changeVersion(int version) {
	ostringstream oss;
	oss << "v" << version << ":" << m_name;

	CsvReader reader("data/characterInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", oss.str().c_str());
	if (data.size() == 0) { data = reader.findOne("name", m_name.c_str()); }
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// パラメータを設定
	setParam(data);
}

void CharacterInfo::setParam(map<string, string>& data) {
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
	m_sameBulletDirection = (bool)stoi(data["sameBulletDirection"]);
}



AttackInfo::AttackInfo():
	AttackInfo("test", 1.0)
{

}
// CSVファイルから読み込む
AttackInfo::AttackInfo(const char* characterName, double drawEx) {
	CsvReader reader("data/attackInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// パラメータを設定
	setParam(data);

	// 攻撃のエフェクト
	string path = "picture/effect/";
	m_bulletEffectHandles = new GraphHandles((path + data["bulletEffect"]).c_str(), stoi(data["bulletEffectSum"]), drawEx, 0.0, true);
	m_slashEffectHandles = new GraphHandles((path + data["slashEffect"]).c_str(), stoi(data["slashEffectSum"]), drawEx, 0.0, true);
	// サウンド
	path = "sound/stick/";
	m_bulletSoundHandle = LoadSoundMem((path + data["bulletSound"]).c_str());
	m_slashSoundHandle = LoadSoundMem((path + data["slashSound"]).c_str());
	m_bulletStartSoundHandle = LoadSoundMem((path + data["bulletStartSound"]).c_str());
	m_slashStartSoundHandle = LoadSoundMem((path + data["slashStartSound"]).c_str());
}

AttackInfo::~AttackInfo() {
	// 画像を削除
	delete m_bulletEffectHandles;
	delete m_slashEffectHandles;
	// サウンドを削除
	DeleteSoundMem(m_bulletSoundHandle);
	DeleteSoundMem(m_slashSoundHandle);
	DeleteSoundMem(m_bulletStartSoundHandle);
	DeleteSoundMem(m_slashStartSoundHandle);
}

// バージョン変更
void AttackInfo::changeVersion(const char* characterName, int version) {
	ostringstream oss;
	oss << "v" << version << ":" << characterName;

	CsvReader reader("data/attackInfo.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", oss.str().c_str());
	if (data.size() == 0) { data = reader.findOne("name", characterName); }
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// パラメータを設定
	setParam(data);
}

void AttackInfo::setParam(map<string, string>& data) {
	m_bulletHp = stoi(data["bulletHp"]);
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_bulletBomb = (bool)stoi(data["bulletBomb"]);
	m_slashHp = stoi(data["slashHp"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashInterval = stoi(data["slashInterval"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);
}


/*
* Characterクラス
*/
int Character::characterId;

Character::Character() :
	Character(100, 0, 0, 0)
{

}

Character::Character(int hp, int x, int y, int groupId) {
	// IDを振る
	characterId++;
	m_id = characterId;

	m_groupId = groupId;

	m_version = 1;
	m_hp = hp;
	m_prevHp = m_hp;
	m_dispHpCnt = 0;
	m_skillGage = SKILL_MAX;
	m_invincible = false;
	m_x = x;
	m_y = y;

	m_leftDirection = true;
	m_freeze = false;
	m_bossFlag = false;

	m_characterInfo = nullptr;
	m_attackInfo = nullptr;
	m_graphHandle = nullptr;
	m_faceHandle = nullptr;
	m_duplicationFlag = false;
}

Character::~Character() {
	// CharacterInfoの削除
	if (m_characterInfo != nullptr) {
		delete m_characterInfo;
	}
	// AttackInfoの削除
	if (m_attackInfo != nullptr && !m_duplicationFlag) {
		delete m_attackInfo;
	}
	// GraphHandleの削除
	if (m_graphHandle != nullptr) {
		delete m_graphHandle;
	}
	// FaceHandleの削除
	if (m_faceHandle != nullptr) {
		delete m_faceHandle;
	}
}

void Character::setParam(Character* character) {
	character->changeInfoVersion(m_version);
	character->setY(m_y);
	character->setId(m_id);
	character->setLeftDirection(m_leftDirection);
	character->setHp(m_hp);
	character->setPrevHp(m_prevHp);
	character->setSkillGage(m_skillGage);
	character->setInvincible(m_invincible);
	character->setBossFlag(m_bossFlag);
	character->getCharacterGraphHandle()->setGraph(m_graphHandle->getDispGraphHandle(), m_graphHandle->getDispGraphIndex());
}

GraphHandle* Character::getGraphHandle() const {
	return m_graphHandle->getHandle();
}

void Character::getHandleSize(int& wide, int& height) const {
	// 今セットしている画像の縦幅と横幅を取得する。
	wide = getWide();
	height = getHeight();
}

// 当たり判定の範囲を取得
void Character::getAtariArea(int* x1, int* y1, int* x2, int* y2) const {
	m_graphHandle->getAtari(x1, y1, x2, y2);
	if (m_leftDirection) {
		int wide = getWide();
		*x1 = wide - *x1;
		*x2 = wide - *x2;
		int tmp = *x1;
		*x1 = *x2;
		*x2 = tmp;
	}
	*x1 = *x1 + m_x;
	*y1 = *y1 + m_y;
	*x2 = *x2 + m_x;
	*y2 = *y2 + m_y;
}

// 当たり判定の範囲を取得
void Character::getDamageArea(int* x1, int* y1, int* x2, int* y2) const {
	m_graphHandle->getDamage(x1, y1, x2, y2);
	if (m_leftDirection) {
		int wide = getWide();
		*x1 = wide - *x1;
		*x2 = wide - *x2;
		int tmp = *x1;
		*x1 = *x2;
		*x2 = tmp;
	}
	*x1 = *x1 + m_x;
	*y1 = *y1 + m_y;
	*x2 = *x2 + m_x;
	*y2 = *y2 + m_y;
}

// Infoのバージョンを変更する
void Character::changeInfoVersion(int version) {
	m_version = version;
	m_characterInfo->changeVersion(version);
	m_attackInfo->changeVersion(m_characterInfo->name().c_str(), version);
}

int Character::getCenterX() const { 
	return m_x + (getWide() / 2);
}
int Character::getCenterY() const { 
	return m_y + (getHeight() / 2);
}
int Character::getWide() const {
	return m_graphHandle->getWide();
}
int Character::getHeight() const {
	return m_graphHandle->getHeight();
}
int Character::getAtariCenterX() const {
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getAtariArea(&x1, &y1, &x2, &y2);
	return (x1 + x2) / 2;
}
int Character::getAtariCenterY() const {
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	getAtariArea(&x1, &y1, &x2, &y2);
	return (y1 + y2) / 2;
}

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->getHandle()->setReverseX(m_leftDirection);
}

// HP減少
void Character::damageHp(int value) {

	// 無敵
	if (m_invincible) { return; }
	
	// 無敵じゃないならHP現象
	m_hp = max(0, m_hp - value);
	m_dispHpCnt = 60;
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

bool Character::haveDeadGraph() const {
	return !(m_graphHandle->getDeadHandle() == nullptr);
}

// 立ち画像をセット
void Character::switchStand(int cnt) { m_graphHandle->switchStand(); }
// 立ち射撃画像をセット
void Character::switchBullet(int cnt) { m_graphHandle->switchBullet(); }
// 立ち斬撃画像をセット
void Character::switchSlash(int cnt) { m_graphHandle->switchSlash(); }
// しゃがみ画像をセット
void Character::switchSquat(int cnt) { m_graphHandle->switchSquat(); }
// しゃがみ画像をセット
void Character::switchSquatBullet(int cnt) { m_graphHandle->switchSquatBullet(); }
// 走り画像をセット
void Character::switchRun(int cnt) { m_graphHandle->switchRun(); }
// 走り射撃画像をセット
void Character::switchRunBullet(int cnt) { m_graphHandle->switchRunBullet(); }
// 着地画像をセット
void Character::switchLand(int cnt) { m_graphHandle->switchLand(); }
// 上昇画像をセット
void Character::switchJump(int cnt) { m_graphHandle->switchJump(); }
// 降下画像をセット
void Character::switchDown(int cnt) { m_graphHandle->switchDown(); }
// ジャンプ前画像をセット
void Character::switchPreJump(int cnt) { m_graphHandle->switchPreJump(); }
// ダメージ画像をセット
void Character::switchDamage(int cnt) { m_graphHandle->switchDamage(); }
// ブースト画像をセット
void Character::switchBoost(int cnt) { m_graphHandle->switchBoost(); }
// 空中射撃画像をセット
void Character::switchAirBullet(int cnt) { m_graphHandle->switchAirBullet(); }
// 空中斬撃画像をセット
void Character::switchAirSlash(int cnt) { m_graphHandle->switchAirSlash(); }
// やられ画像をセット
void Character::switchDead(int cnt) { m_graphHandle->switchDead(); }
// ボスの初期アニメーションをセット
void Character::switchInit(int cnt) { m_graphHandle->switchInit(); }
// 追加画像をセット
void Character::switchSpecial1(int cnt) { m_graphHandle->switchSpecial1(); }


/*
* ハート
*/
Heart::Heart(const char* name, int hp, int x, int y, int groupId) :
	Character(hp, x, y, groupId)
{
	// キャラ固有の情報設定
	m_characterInfo = new CharacterInfo(name);
	m_attackInfo = new AttackInfo(name, m_characterInfo->handleEx());

	m_hp = m_characterInfo->maxHp();
	m_prevHp = m_hp;

	// 各画像のロード
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);
	// 画像の反転
	setLeftDirection(m_leftDirection);

	m_bulletColor = WHITE;

	// とりあえず立ち画像でスタート
	switchStand();
	m_y -= getHeight();
}

Heart::Heart(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Character(hp, x, y, groupId)
{
	m_duplicationFlag = true;
	m_attackInfo = attackInfo;
	m_characterInfo = new CharacterInfo(name);
	m_hp = m_characterInfo->maxHp();
	m_prevHp = m_hp;
	// 各画像のロード
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);
	// 画像の反転
	setLeftDirection(m_leftDirection);

	m_bulletColor = WHITE;

}

// デストラクタ
Heart::~Heart() {

}

Character* Heart::createCopy() {
	Character* res = new Heart(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 走り画像をセット
void Heart::switchRun(int cnt) { 
	if (m_graphHandle->getRunHandle() == nullptr) { return; }
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchRun(index);
}

// 走り射撃画像をセット
void Heart::switchRunBullet(int cnt) {
	if (m_graphHandle->getRunBulletHandle() == nullptr) { 
		switchRun(cnt);
		return;
	}
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunBulletHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchRunBullet(index);
}

// ジャンプ前画像をセット
void Heart::switchPreJump(int cnt) { 
	if (m_graphHandle->getPreJumpHandle() == nullptr) { return; }
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_graphHandle->getPreJumpHandle()->getGraphHandles()->getSize());
	m_graphHandle->switchPreJump(index);
}

// 立ち斬撃画像をセット
void Heart::switchSlash(int cnt) {
	if (m_graphHandle->getStandSlashHandle() == nullptr) { return; }
	int index = (getSlashCountSum() + getSlashInterval() - cnt) / 3;
	if (cnt > 6) {
		index = min(m_graphHandle->getStandSlashHandle()->getGraphHandles()->getSize() - 2, index);
	}
	m_graphHandle->switchSlash(index);
}

// 立ち射撃画像をセット
void Heart::switchBullet(int cnt) {
	if (m_graphHandle->getStandBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getStandBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchBullet(index);
}

// 空中斬撃画像をセット
void Heart::switchAirSlash(int cnt) {
	if (m_graphHandle->getAirSlashHandle() == nullptr) { return; }
	int index = (getSlashCountSum() + getSlashInterval() - cnt) / 3;
	if (cnt > 6) {
		index = min(m_graphHandle->getAirSlashHandle()->getGraphHandles()->getSize() - 2, index);
	}
	m_graphHandle->switchAirSlash(index);
}

// 空中射撃画像をセット
void Heart::switchAirBullet(int cnt) {
	if (m_graphHandle->getAirBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getAirBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchAirBullet(index);
}

// しゃがみ射撃画像をセット
void Heart::switchSquatBullet(int cnt) {
	if (m_graphHandle->getSquatBulletHandle() == nullptr) { return; }
	int flame = getBulletRapid() / m_graphHandle->getSquatBulletHandle()->getGraphHandles()->getSize();
	int index = (getBulletRapid() - cnt) / flame;
	m_graphHandle->switchSquatBullet(index);
}

// 射撃攻撃をする
Object* Heart::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	// 弾の作成
	BulletObject* attackObject;
	if (m_graphHandle->getBulletHandle() != nullptr) {
		attackObject = new BulletObject(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	}
	else {
		attackObject = new BulletObject(getCenterX(), getCenterY(), m_bulletColor, gx, gy, m_attackInfo);
	}
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// 斬撃攻撃をする
Object* Heart::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	// 攻撃範囲を決定
	int centerX = getCenterX();
	int height = m_attackInfo->slashLenY() / 2;
	int centerY = getCenterY();
	int x2 = centerX;
	if (leftDirection) { // 左向きに攻撃
		x2 -= m_attackInfo->slashLenX();
	}
	else { // 右向きに攻撃
		x2 += m_attackInfo->slashLenX();
	}

	// 攻撃の画像と持続時間(cntを考慮して決定)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandlesWithAtari* slashHandles = m_graphHandle->getAirSlashEffectHandle();
	if (grand || slashHandles == nullptr) {
		// 地上にいる、もしくは空中斬撃画像がないなら地上用の画像を使う
		slashHandles = m_graphHandle->getSlashHandle();
	}
	// 攻撃の方向
	slashHandles->getGraphHandles()->setReverseX(m_leftDirection);
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
		// 効果音
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandles()->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// 自滅防止
		attackObject->setCharacterId(m_id);
		// チームキル防止
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* シエスタ
*/
Siesta::Siesta(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Siesta::Siesta(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo):
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Siesta::createCopy() {
	Character* res = new Siesta(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 射撃攻撃をする
Object* Siesta::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid() / 2) { return nullptr; }
	ParabolaBullet *attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// 斬撃攻撃をする
Object* Siesta::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	// 攻撃範囲を決定
	int centerX = getCenterX();
	int height = getHeight();
	int x1 = centerX;
	int x2 = x1;
	if (leftDirection) { // 左向きに攻撃
		x1 += 100;
		x2 = x1 - m_attackInfo->slashLenX();
	}
	else { // 右向きに攻撃
		x1 -= 100;
		x2 = x1 + m_attackInfo->slashLenX();
	}

	// 攻撃の画像と持続時間(cntを考慮して決定)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle()->getGraphHandles();
	// 攻撃の方向
	slashHandles->setReverseX(m_leftDirection);
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
		// 効果音
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// 自滅防止
		attackObject->setCharacterId(m_id);
		// チームキル防止
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* ヒエラルキー
*/
Hierarchy::Hierarchy(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Hierarchy::Hierarchy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Hierarchy::createCopy() {
	Character* res = new Hierarchy(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 射撃攻撃をする
Object* Hierarchy::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	//gx = GetRand(600) - 300 + getCenterX();
	//gy = getCenterY() - GetRand(300);
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// 斬撃攻撃をする
Object* Hierarchy::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	return nullptr;
}


/*
* ヴァルキリア
*/
Valkyria::Valkyria(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Valkyria::Valkyria(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Valkyria::createCopy() {
	Character* res = new Valkyria(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// ジャンプ前画像をセット
void Valkyria::switchPreJump(int cnt) {
	if (m_graphHandle->getPreJumpHandle() == nullptr) { return; }
	int index = 0;
	if (cnt >= 25) { index = 3; }
	else if (cnt >= 20) { index = 2; }
	else if (cnt >= 15) { index = 1; }
	m_graphHandle->switchPreJump(index);
}

// 斬撃攻撃をする
Object* Valkyria::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	// 攻撃範囲を決定
	int attackWide, attackHeight;
	GetGraphSize(m_graphHandle->getStandSlashHandle()->getGraphHandles()->getHandle(0), &attackWide, &attackHeight);
	attackWide = (int)(attackWide * m_graphHandle->getStandSlashHandle()->getGraphHandles()->getGraphHandle()->getEx());
	attackHeight = (int)(attackHeight * m_graphHandle->getStandSlashHandle()->getGraphHandles()->getGraphHandle()->getEx());
	int x1 = m_x;
	int x2 = m_x + attackWide;

	// 攻撃の画像と持続時間(cntを考慮して決定)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = nullptr;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle()->getGraphHandles();
	// 攻撃の方向
	slashHandles->setReverseX(m_leftDirection);
	// キャラの身長
	int height = attackHeight;
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum() - 1) {
		index = 0 % slashHandles->getSize();
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - 12, m_attackInfo);
		// 効果音
		if (soundPlayer != nullptr) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1 % slashHandles->getSize();
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - slashCountSum - 6, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2 % slashHandles->getSize();
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), m_attackInfo->slashCountSum() - 2 * slashCountSum, m_attackInfo);
	}
	if (attackObject != nullptr) {
		// 自滅防止
		attackObject->setCharacterId(m_id);
		// チームキル防止
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}


/*
* トロイ
*/
Troy::Troy(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{
	m_bulletColor = BLUE;
}
Troy::Troy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{
	m_bulletColor = BLUE;
}

Character* Troy::createCopy() {
	Character* res = new Troy(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 斬撃攻撃をする
Object* Troy::slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) {
	return nullptr;
}


/*
* コハル
*/
Koharu::Koharu(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Koharu::Koharu(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Koharu::createCopy() {
	Character* res = new Koharu(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 射撃攻撃をする
Object* Koharu::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	// バズーカの銃口から出るように見せる
	gy = getY() + getHeight() - 160;
	BulletObject* attackObject = new BulletObject(getCenterX(), gy, m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}


/*
* 普通の射撃のみをするキャラ
*/
BulletOnly::BulletOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
BulletOnly::BulletOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* BulletOnly::createCopy() {
	Character* res = new BulletOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

/*
* 普通の斬撃のみをするキャラ
*/
SlashOnly::SlashOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
SlashOnly::SlashOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* SlashOnly::createCopy() {
	Character* res = new SlashOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}


/*
* ParabolaBulletのみを撃つキャラ
*/
ParabolaOnly::ParabolaOnly(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{
	m_bulletColor = GRAY;
}
ParabolaOnly::ParabolaOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{
	m_bulletColor = GRAY;
}

Character* ParabolaOnly::createCopy() {
	Character* res = new ParabolaOnly(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// 射撃攻撃をする
Object* ParabolaOnly::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	ParabolaBullet* attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_bulletColor, gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}


/*
* Boss1: サン
*/
Sun::Sun(const char* name, int hp, int x, int y, int groupId) :
	Heart(name, hp, x, y, groupId)
{

}
Sun::Sun(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo) :
	Heart(name, hp, x, y, groupId, attackInfo)
{

}

Character* Sun::createCopy() {
	Character* res = new Sun(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	setParam(res);
	return res;
}

// ボスの初期アニメーションをセット
void Sun::switchInit(int cnt) { 
	if (m_graphHandle->getInitHandle() == nullptr) { return; }
	if (cnt < 0) {
		m_graphHandle->switchSpecial1();
		return;
	}
	int index = min(cnt / RUN_ANIME_SPEED, m_graphHandle->getInitHandle()->getGraphHandles()->getSize() - 1);
	m_graphHandle->switchInit(index);
}

Object* Sun::bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) {
	if (cnt != getBulletRapid()) { return nullptr; }
	int x = getCenterX() + GetRand(400) - 200;
	int y = getCenterY() + GetRand(400) - 200;
	ParabolaBullet* attackObject = new ParabolaBullet(x, y, m_graphHandle->getBulletHandle()->getGraphHandles()->getGraphHandle(), gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != nullptr) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}