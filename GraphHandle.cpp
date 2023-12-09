#include "GraphHandle.h"
#include "Camera.h"
#include "CsvReader.h"
#include "Define.h"
#include "DxLib.h"
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;


GraphHandle::GraphHandle(const char* filePath, double ex, double angle, bool trans, bool reverseX, bool reverseY) {
	m_handle = LoadGraph(filePath);
	m_ex = ex;
	m_angle = angle;
	m_trans = trans;
	m_reverseX = reverseX;
	m_reverseY = reverseY;
}

// 画像のデリート
GraphHandle::~GraphHandle() {
	DeleteGraph(m_handle);
}

// 描画する
void GraphHandle::draw(int x, int y, double ex) const {
	DrawRotaGraph(x, y, ex, m_angle, m_handle, m_trans, m_reverseX, m_reverseY);
}

// 範囲を指定して変形描画する
void GraphHandle::extendDraw(int x1, int y1, int x2, int y2) const {
	if (m_reverseX) {
		swap(x1, x2);
	}
	if (m_reverseY) {
		swap(y1, y2);
	}
	DrawExtendGraph(x1, y1, x2, y2, m_handle, m_trans);
}

// 範囲を指定して敷き詰めるように描画する
void GraphHandle::lineUpDraw(int x1, int y1, int x2, int y2, const Camera* camera) const {

	if (x1 > x2) { swap(x1, x2); }
	if (y1 > y2) { swap(y1, y2); }

	int wide = 0, height = 0;
	GetGraphSize(m_handle, &wide, &height);

	wide = (int)(wide * m_ex) + 1;
	height = (int)(height * m_ex) + 1;

	int xi = (x2 - x1) / wide;
	if (xi == 0) { xi = 1; }
	int yi = (y2 - y1) / height;
	height += ((y2 - y1) % height) / yi + 1;
	
	int x = x1;
	for (int i = 0; i < xi; i++, x += wide) {
		int nextX = x + wide > x2 ? x2 : x + wide;
		int y = y1;
		for (int j = 0; j < yi; j++, y += height) {
			int nextY = y + height > y2 ? y2 : y + height;
			int drawX1 = x, drawY1 = y, drawX2 = nextX, drawY2 = nextY;
			double ex = 0;
			camera->setCamera(&drawX1, &drawY1, &ex);
			camera->setCamera(&drawX2, &drawY2, &ex);
			if (drawX1 > GAME_WIDE || drawX2 < 0) { continue; }
			if (drawY1 > GAME_HEIGHT || drawY2 < 0) { continue; }
			DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, m_handle, m_trans);
		}
	}

}


/*
* 複数の画像をまとめて扱うクラス
*/
GraphHandles::GraphHandles(const char* filePath, int handleSum, double ex, double angle, bool trans, bool reverseX, bool reverseY) {
	m_handleSum = handleSum;
	m_handles = new GraphHandle* [m_handleSum];
	if (m_handleSum == 1) {
		ostringstream oss;
		oss << filePath << ".png";
		m_handles[0] = new GraphHandle(oss.str().c_str(), ex, angle, trans, reverseX, reverseY);
	}
	else {
		for (int i = 0; i < m_handleSum; i++) {
			ostringstream oss;
			oss << filePath << i + 1 << ".png";
			m_handles[i] = new GraphHandle(oss.str().c_str(), ex, angle, trans, reverseX, reverseY);
		}
	}
}

// 画像のデリート
GraphHandles::~GraphHandles() {
	for (int i = 0; i < m_handleSum; i++) {
		delete m_handles[i];
	}
	delete m_handles;
}

// セッタ
void GraphHandles::setEx(double ex) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setEx(ex);
	}
}
void GraphHandles::setAngle(double angle) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setAngle(angle);
	}
}
void GraphHandles::setTrans(bool trans) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setTrans(trans);
	}
}
void GraphHandles::setReverseX(bool reverse) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setReverseX(reverse);
	}
}
void GraphHandles::setReverseY(bool reverse) {
	for (int i = 0; i < m_handleSum; i++) {
		m_handles[i]->setReverseY(reverse);
	}
}

// 描画する
void GraphHandles::draw(int x, int y, int index) {
	m_handles[index]->draw(x, y);
}


/*
* キャラの画像
*/
// 画像ロード用
void loadCharacterGraph(const char* dir, const char* characterName, GraphHandles*& handles, string stateName, map<string, string>& data, double ex) {
	int n = stoi(data[stateName]);
	if (n > 0) {
		ostringstream oss;
		oss << dir << characterName << "/" << stateName;
		handles = new GraphHandles(oss.str().c_str(), n, ex, 0.0, true);
	}
	else {
		handles = nullptr;
	}
}
// デフォルト値で初期化
CharacterGraphHandle::CharacterGraphHandle() :
	CharacterGraphHandle("test", 1.0)
{

}
// csvファイルを読み込み、キャラ名で検索しパラメータ取得
CharacterGraphHandle::CharacterGraphHandle(const char* characterName, double drawEx) {	
	m_ex = drawEx;

	CsvReader reader("data/characterGraph.csv");

	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "テスト"); }

	// ロード
	const char* dir = "picture/stick/";
	loadCharacterGraph(dir, characterName, m_standHandles, "stand", data, m_ex);
	loadCharacterGraph(dir, characterName, m_slashHandles, "slash", data, m_ex);
	loadCharacterGraph(dir, characterName, m_bulletHandles, "bullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_squatHandles, "squat", data, m_ex);
	loadCharacterGraph(dir, characterName, m_squatBulletHandles, "squatBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_standBulletHandles, "standBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_standSlashHandles, "standSlash", data, m_ex);
	loadCharacterGraph(dir, characterName, m_runHandles, "run", data, m_ex);
	loadCharacterGraph(dir, characterName, m_runBulletHandles, "runBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_landHandles, "land", data, m_ex);
	loadCharacterGraph(dir, characterName, m_jumpHandles, "jump", data, m_ex);
	loadCharacterGraph(dir, characterName, m_downHandles, "down", data, m_ex);
	loadCharacterGraph(dir, characterName, m_preJumpHandles, "preJump", data, m_ex);
	loadCharacterGraph(dir, characterName, m_damageHandles, "damage", data, m_ex);
	loadCharacterGraph(dir, characterName, m_boostHandles, "boost", data, m_ex);
	loadCharacterGraph(dir, characterName, m_airBulletHandles, "airBullet", data, m_ex);
	loadCharacterGraph(dir, characterName, m_airSlashHandles, "airSlash", data, m_ex);

	switchStand();
}
// 画像を削除
CharacterGraphHandle::~CharacterGraphHandle() {
	if (m_standHandles != nullptr) { delete m_standHandles; }
	if (m_standHandles != nullptr) { delete m_slashHandles; }
	if (m_standHandles != nullptr) { delete m_bulletHandles; }
	if (m_standHandles != nullptr) { delete m_squatHandles; }
	if (m_standHandles != nullptr) { delete m_squatBulletHandles; }
	if (m_standHandles != nullptr) { delete m_standBulletHandles; }
	if (m_standHandles != nullptr) { delete m_standSlashHandles; }
	if (m_standHandles != nullptr) { delete m_runHandles; }
	if (m_standHandles != nullptr) { delete m_runBulletHandles; }
	if (m_standHandles != nullptr) { delete m_landHandles; }
	if (m_standHandles != nullptr) { delete m_jumpHandles; }
	if (m_standHandles != nullptr) { delete m_downHandles; }
	if (m_standHandles != nullptr) { delete m_preJumpHandles; }
	if (m_standHandles != nullptr) { delete m_damageHandles; }
	if (m_standHandles != nullptr) { delete m_boostHandles; }
	if (m_standHandles != nullptr) { delete m_airBulletHandles; }
	if (m_standHandles != nullptr) { delete m_airSlashHandles; }
}

// 画像のサイズをセット
void CharacterGraphHandle::setGraphSize() {
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);
	// 画像の拡大率も考慮してサイズを決定
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

// 画像をセットする 存在しないならそのまま
void CharacterGraphHandle::setGraph(const GraphHandles* graphHandles, int index) {
	if (graphHandles == nullptr) { return; }
	if (index >= graphHandles->getSize() || index < 0) { return; }
	m_graphHandle = graphHandles->getGraphHandle(index);
	setGraphSize();
}
void CharacterGraphHandle::setGraph(GraphHandle* graphHandle) {
	m_graphHandle = graphHandle;
	setGraphSize();
}

// 立ち画像をセット
void CharacterGraphHandle::switchStand(int index){
	setGraph(m_standHandles, index);
}
// 立ち射撃画像をセット
void CharacterGraphHandle::switchBullet(int index){
	if (m_standBulletHandles == nullptr) { switchStand(index); }
	setGraph(m_standBulletHandles, index);
}
// 立ち斬撃画像をセット
void CharacterGraphHandle::switchSlash(int index){
	if (m_standSlashHandles == nullptr) { switchStand(index); }
	setGraph(m_standSlashHandles, index);
}
// しゃがみ画像をセット
void CharacterGraphHandle::switchSquat(int index){
	if (m_squatHandles == nullptr) { switchStand(index); }
	setGraph(m_squatHandles, index);
}
// しゃがみ射撃画像をセット
void CharacterGraphHandle::switchSquatBullet(int index) {
	if (m_squatBulletHandles == nullptr) { switchBullet(index); }
	setGraph(m_squatBulletHandles, index);
}
// 走り画像をセット
void CharacterGraphHandle::switchRun(int index){
	setGraph(m_runHandles, index);
}
// 走り射撃画像をセット
void CharacterGraphHandle::switchRunBullet(int index) {
	if (m_runBulletHandles == nullptr) { switchRun(index); }
	setGraph(m_runBulletHandles, index);
}
// 着地画像をセット
void CharacterGraphHandle::switchLand(int index){
	setGraph(m_landHandles, index);
}
// 上昇画像をセット
void CharacterGraphHandle::switchJump(int index){
	setGraph(m_jumpHandles, index);
}
// 降下画像をセット
void CharacterGraphHandle::switchDown(int index){
	setGraph(m_downHandles, index);
}
// ジャンプ前画像をセット
void CharacterGraphHandle::switchPreJump(int index){
	setGraph(m_preJumpHandles, index);
}
// ダメージ画像をセット
void CharacterGraphHandle::switchDamage(int index){
	setGraph(m_damageHandles, index);
}
// ブースト画像をセット
void CharacterGraphHandle::switchBoost(int index){
	setGraph(m_boostHandles, index);
}
// 空中射撃画像をセット
void CharacterGraphHandle::switchAirBullet(int index){
	setGraph(m_airBulletHandles, index);
}
// 空中斬撃画像をセット
void CharacterGraphHandle::switchAirSlash(int index){
	setGraph(m_airSlashHandles, index);
}



FaceGraphHandle::FaceGraphHandle():
	FaceGraphHandle("テスト", 1.0)
{

}
FaceGraphHandle::FaceGraphHandle(const char* characterName, double drawEx) {
	m_ex = drawEx;
	
	CsvReader reader("data/faceGraph.csv");
	// キャラ名でデータを検索
	map<string, string> data = reader.findOne("name", characterName);
	// 見つからなかったらテストで再検索
	if (data.size() == 0) { 
		characterName = "テスト";
		data = reader.findOne("name", characterName);
	}

	// ロード
	const char* dir = "picture/face/";
	loadCharacterGraph(dir, characterName, m_normalHandles, "通常", data, m_ex);

}
FaceGraphHandle::~FaceGraphHandle() {
	delete m_normalHandles;
}
GraphHandles* FaceGraphHandle::getGraphHandle(const char* faceName) {
	return m_normalHandles;
}