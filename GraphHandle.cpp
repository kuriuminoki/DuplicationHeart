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

// �摜�̃f���[�g
GraphHandle::~GraphHandle() {
	DeleteGraph(m_handle);
}

// �`�悷��
void GraphHandle::draw(int x, int y, double ex) const {
	DrawRotaGraph(x, y, ex, m_angle, m_handle, m_trans, m_reverseX, m_reverseY);
}

// �͈͂��w�肵�ĕό`�`�悷��
void GraphHandle::extendDraw(int x1, int y1, int x2, int y2) const {
	if (m_reverseX) {
		swap(x1, x2);
	}
	if (m_reverseY) {
		swap(y1, y2);
	}
	DrawExtendGraph(x1, y1, x2, y2, m_handle, m_trans);
}

// �͈͂��w�肵�ĕ~���l�߂�悤�ɕ`�悷��
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
* �����̉摜���܂Ƃ߂Ĉ����N���X
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

// �摜�̃f���[�g
GraphHandles::~GraphHandles() {
	for (int i = 0; i < m_handleSum; i++) {
		delete m_handles[i];
	}
	delete m_handles;
}

// �Z�b�^
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

// �`�悷��
void GraphHandles::draw(int x, int y, int index) {
	m_handles[index]->draw(x, y);
}


/*
* �L�����̉摜
*/
// �摜���[�h�p
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
// �f�t�H���g�l�ŏ�����
CharacterGraphHandle::CharacterGraphHandle() :
	CharacterGraphHandle("test", 1.0)
{

}
// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
CharacterGraphHandle::CharacterGraphHandle(const char* characterName, double drawEx) {	
	m_ex = drawEx;

	CsvReader reader("data/characterGraph.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// ���[�h
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
// �摜���폜
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

// �摜�̃T�C�Y���Z�b�g
void CharacterGraphHandle::setGraphSize() {
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);
	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

// �摜���Z�b�g���� ���݂��Ȃ��Ȃ炻�̂܂�
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

// �����摜���Z�b�g
void CharacterGraphHandle::switchStand(int index){
	setGraph(m_standHandles, index);
}
// �����ˌ��摜���Z�b�g
void CharacterGraphHandle::switchBullet(int index){
	if (m_standBulletHandles == nullptr) { switchStand(index); }
	setGraph(m_standBulletHandles, index);
}
// �����a���摜���Z�b�g
void CharacterGraphHandle::switchSlash(int index){
	if (m_standSlashHandles == nullptr) { switchStand(index); }
	setGraph(m_standSlashHandles, index);
}
// ���Ⴊ�݉摜���Z�b�g
void CharacterGraphHandle::switchSquat(int index){
	if (m_squatHandles == nullptr) { switchStand(index); }
	setGraph(m_squatHandles, index);
}
// ���Ⴊ�ݎˌ��摜���Z�b�g
void CharacterGraphHandle::switchSquatBullet(int index) {
	if (m_squatBulletHandles == nullptr) { switchBullet(index); }
	setGraph(m_squatBulletHandles, index);
}
// ����摜���Z�b�g
void CharacterGraphHandle::switchRun(int index){
	setGraph(m_runHandles, index);
}
// ����ˌ��摜���Z�b�g
void CharacterGraphHandle::switchRunBullet(int index) {
	if (m_runBulletHandles == nullptr) { switchRun(index); }
	setGraph(m_runBulletHandles, index);
}
// ���n�摜���Z�b�g
void CharacterGraphHandle::switchLand(int index){
	setGraph(m_landHandles, index);
}
// �㏸�摜���Z�b�g
void CharacterGraphHandle::switchJump(int index){
	setGraph(m_jumpHandles, index);
}
// �~���摜���Z�b�g
void CharacterGraphHandle::switchDown(int index){
	setGraph(m_downHandles, index);
}
// �W�����v�O�摜���Z�b�g
void CharacterGraphHandle::switchPreJump(int index){
	setGraph(m_preJumpHandles, index);
}
// �_���[�W�摜���Z�b�g
void CharacterGraphHandle::switchDamage(int index){
	setGraph(m_damageHandles, index);
}
// �u�[�X�g�摜���Z�b�g
void CharacterGraphHandle::switchBoost(int index){
	setGraph(m_boostHandles, index);
}
// �󒆎ˌ��摜���Z�b�g
void CharacterGraphHandle::switchAirBullet(int index){
	setGraph(m_airBulletHandles, index);
}
// �󒆎a���摜���Z�b�g
void CharacterGraphHandle::switchAirSlash(int index){
	setGraph(m_airSlashHandles, index);
}



FaceGraphHandle::FaceGraphHandle():
	FaceGraphHandle("�e�X�g", 1.0)
{

}
FaceGraphHandle::FaceGraphHandle(const char* characterName, double drawEx) {
	m_ex = drawEx;
	
	CsvReader reader("data/faceGraph.csv");
	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	// ������Ȃ�������e�X�g�ōČ���
	if (data.size() == 0) { 
		characterName = "�e�X�g";
		data = reader.findOne("name", characterName);
	}

	// ���[�h
	const char* dir = "picture/face/";
	loadCharacterGraph(dir, characterName, m_normalHandles, "�ʏ�", data, m_ex);

}
FaceGraphHandle::~FaceGraphHandle() {
	delete m_normalHandles;
}
GraphHandles* FaceGraphHandle::getGraphHandle(const char* faceName) {
	return m_normalHandles;
}