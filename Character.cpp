#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "Sound.h"
#include "DxLib.h"
#include <cstdlib>


using namespace std;


Character* createCharacter(const char* characterName, int hp, int x, int y, int groupId) {
	Character* character = NULL;
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
	m_name = data["name"];
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);

	// 効果音をロード
	string filePath = "sound/stick/";
	string fileName;
	fileName = filePath + data["jumpSound"];
	m_jumpSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["passiveSound"];
	m_passiveSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["landSound"];
	m_landSound = LoadSoundMem(fileName.c_str());
	fileName = filePath + data["runSound"];
}

CharacterInfo::~CharacterInfo() {
	// サウンドを削除
	DeleteSoundMem(m_jumpSound);
	DeleteSoundMem(m_passiveSound);
	DeleteSoundMem(m_landSound);
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
	m_bulletHp = stoi(data["bulletHp"]);
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_slashHp = stoi(data["slashHp"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashInterval = stoi(data["slashInterval"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);

	// 攻撃のエフェクト
	string path = "picture/effect/";
	m_bulletEffectHandles = new GraphHandles((path + data["bulletEffect"]).c_str(), 4, drawEx, 0.0, true);
	m_slashEffectHandles = new GraphHandles((path + data["slashEffect"]).c_str(), 4, drawEx, 0.0, true);
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

	m_hp = hp;
	m_x = x;
	m_y = y;

	m_leftDirection = true;

	m_characterInfo = NULL;
	m_attackInfo = NULL;
	m_graphHandle = NULL;
	m_faceHandle = NULL;
	m_duplicationFlag = false;
}

Character::~Character() {
	// CharacterInfoの削除
	if (m_characterInfo != NULL) {
		delete m_characterInfo;
	}
	// AttackInfoの削除
	if (m_attackInfo != NULL && !m_duplicationFlag) {
		delete m_attackInfo;
	}
	// GraphHandleの削除
	if (m_graphHandle != NULL) {
		delete m_graphHandle;
	}
	// FaceHandleの削除
	if (m_faceHandle != NULL) {
		delete m_faceHandle;
	}
}

GraphHandle* Character::getGraphHandle() const {
	return m_graphHandle->getHandle();
}

void Character::getHandleSize(int& wide, int& height) const {
	// 今セットしている画像の縦幅と横幅を取得する。
	wide = getWide();
	height = getHeight();
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

void Character::setLeftDirection(bool leftDirection) { 
	m_leftDirection = leftDirection;
	m_graphHandle->getHandle()->setReverseX(m_leftDirection);
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

	// 各画像のロード
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);

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
	// 各画像のロード
	m_graphHandle = new CharacterGraphHandle(name, m_characterInfo->handleEx());
	m_faceHandle = new FaceGraphHandle(name, 1.0);
	// とりあえず立ち画像でスタート
	//switchStand();
	//m_y -= getHeight();
}

// デストラクタ
Heart::~Heart() {

}

Character* Heart::createCopy() {
	Character* res = new Heart(m_characterInfo->name().c_str(), m_hp, m_x, m_y, m_groupId, m_attackInfo);
	res->setY(m_y);
	res->setId(m_id);
	res->setLeftDirection(m_leftDirection);
	res->setHp(m_hp);
	res->getCharacterGraphHandle()->setGraph(getGraphHandle());
	return res;
}

// 走り画像をセット
void Heart::switchRun(int cnt) { 
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunHandle()->getSize());
	m_graphHandle->switchRun(index);
}

// 走り射撃画像をセット
void Heart::switchRunBullet(int cnt) {
	int index = (cnt / RUN_ANIME_SPEED) % (m_graphHandle->getRunBulletHandle()->getSize());
	m_graphHandle->switchRunBullet(index);
}

// ジャンプ前画像をセット
void Heart::switchPreJump(int cnt) { 
	int index = (cnt / RUN_PREJUMP_SPEED) % (m_graphHandle->getPreJumpHandle()->getSize());
	m_graphHandle->switchPreJump(index);
}

// 射撃攻撃をする(キャラごとに違う)
Object* Heart::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	BulletObject* attackObject = new BulletObject(getCenterX(), getCenterY(), WHITE, gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != NULL) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Heart::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
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
	SlashObject* attackObject = NULL;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// 攻撃の方向
	slashHandles->setReverseX(m_leftDirection);
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
		// 効果音
		if (soundPlayer != NULL) {
			soundPlayer->pushSoundQueue(m_attackInfo->slashStartSoundHandle(),
				adjustPanSound(getCenterX(),
					soundPlayer->getCameraX()));
		}
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		index = 1;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		index = 2;
		attackObject = new SlashObject(centerX, centerY - height, x2, centerY + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	if (attackObject != NULL) {
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
	res->setY(m_y);
	res->setId(m_id);
	res->setLeftDirection(m_leftDirection);
	res->setHp(m_hp);
	res->getCharacterGraphHandle()->setGraph(getGraphHandle());
	return res;
}

// 射撃攻撃をする(キャラごとに違う)
Object* Siesta::bulletAttack(int gx, int gy, SoundPlayer* soundPlayer) {
	ParabolaBullet *attackObject = new ParabolaBullet(getCenterX(), getCenterY(), m_graphHandle->getBulletHandle()->getGraphHandle(), gx, gy, m_attackInfo);
	// 自滅防止
	attackObject->setCharacterId(m_id);
	// チームキル防止
	attackObject->setGroupId(m_groupId);
	// 効果音
	if (soundPlayer != NULL) {
		soundPlayer->pushSoundQueue(m_attackInfo->bulletStartSoundeHandle(),
			adjustPanSound(getCenterX(),
				soundPlayer->getCameraX()));
	}
	return attackObject;
}

// 斬撃攻撃をする(キャラごとに違う)
Object* Siesta::slashAttack(bool leftDirection, int cnt, SoundPlayer* soundPlayer) {
	// 攻撃範囲を決定
	int centerX = getCenterX();
	int height = getHeight();
	int x1 = centerX;
	int x2 = centerX;
	if (leftDirection) { // 左向きに攻撃
		x1 += 50;
		x2 -= m_attackInfo->slashLenX();
	}
	else { // 右向きに攻撃
		x1 -= 50;
		x2 += m_attackInfo->slashLenX();
	}

	// 攻撃の画像と持続時間(cntを考慮して決定)
	cnt -= m_attackInfo->slashInterval();
	int index = 0;
	int slashCountSum = m_attackInfo->slashCountSum() / 3 + 1;
	SlashObject* attackObject = NULL;
	GraphHandles* slashHandles = m_graphHandle->getSlashHandle();
	// 攻撃の方向
	slashHandles->setReverseX(m_leftDirection);
	// cntが攻撃のタイミングならオブジェクト生成
	if (cnt == m_attackInfo->slashCountSum()) {
		index = 0;
		attackObject = new SlashObject(x1, m_y, x2, m_y + height,
			slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
		// 効果音
		if (soundPlayer != NULL) {
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
	if (attackObject != NULL) {
		// 自滅防止
		attackObject->setCharacterId(m_id);
		// チームキル防止
		attackObject->setGroupId(m_groupId);
	}
	return attackObject;
}