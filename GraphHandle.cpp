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

	wide = (int)(wide * m_ex);
	height = (int)(height * m_ex);

	int xi = (x2 - x1) / wide;
	int yi = (y2 - y1) / height;

	if (xi == 0 || yi == 0) {
		double ex = 1.0;
		camera->setCamera(&x1, &y1, &ex);
		camera->setCamera(&x2, &y2, &ex);
		this->extendDraw(x1, y1, x2, y2);
		return;
	}

	wide += ((x2 - x1) % wide) / xi + 1;
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
* �����蔻��̏��
*/
AtariArea::AtariArea(CsvReader* csvReader, const char* graphName, const char* prefix) {
	map<string, string> data = csvReader->findOne("name", graphName);

	m_defaultWide = false;
	m_wide = -1;
	m_x1 = 0, m_x2 = 0;
	m_x1none = true, m_x2none = true;

	m_defaultHeight = false;
	m_height = -1;
	m_y1 = 0, m_y2 = 0;
	m_y1none = true, m_y2none = true;

	if (data.size() == 0) {
		m_defaultWide = true;
		m_defaultHeight = true;
	}
	else {
		string p = prefix;

		string defaultWide = data[(p + "defaultWide")];
		string defaultHeight = data[(p + "defaultHeight").c_str()];
		string wide = data[(p + "wide").c_str()];
		string height = data[(p + "height").c_str()];
		string x1 = data[(p + "x1").c_str()];
		string y1 = data[(p + "y1").c_str()];
		string x2 = data[(p + "x2").c_str()];
		string y2 = data[(p + "y2").c_str()];

		// ��
		if (defaultWide == "1") {
			m_defaultWide = true;
		}
		else if (wide != "-1") {
			m_wide = stoi(wide);
		}
		else {
			if (x1 != "null") {
				m_x1 = stoi(x1);
				m_x1none = false;
			}
			if (x2 != "null") {
				m_x2 = stoi(x2);
				m_x2none = false;
			}
		}

		// �c
		if (defaultHeight == "1") {
			m_defaultHeight = true;
		}
		else if (height != "-1") {
			m_height = stoi(height);
		}
		else {
			if (y1 != "null") {
				m_y1 = stoi(y1);
				m_y1none = false;
			}
			if (y2 != "null") {
				m_y2 = stoi(y2);
				m_y2none = false;
			}
		}
	}

}

void AtariArea::getArea(int* x1, int* y1, int* x2, int* y2, int wide, int height) const {
	// ��
	if (m_defaultWide) {
		*x1 = 0;
		*x2 = wide;
	}
	else if (m_wide != -1) {
		*x2 = wide / 2 + m_wide / 2;
		*x1 = wide / 2 - m_wide / 2;
	}
	else {
		if (m_x1none) {
			*x1 = 0;
		}
		else {
			*x1 = m_x1;
		}
		if (m_x2none) {
			*x2 = wide;
		}
		else {
			*x2 = m_x2;
		}
	}
	// �c
	if (m_defaultHeight) {
		*y1 = 0;
		*y2 = height;
	}
	else if (m_height != -1) {
		*y2 = height / 2 + m_height / 2;
		*y1 = height / 2 - m_height / 2;
	}
	else {
		if (m_y1none) {
			*y1 = 0;
		}
		else {
			*y1 = m_y1;
		}
		if (m_y2none) {
			*y2 = height;
		}
		else {
			*y2 = m_y2;
		}
	}
}


/*
* �����蔻��̏��t����GraphHandles
*/
GraphHandlesWithAtari::GraphHandlesWithAtari(GraphHandles* graphHandles, const char* graphName, CsvReader* csvReader) {
	m_graphHandles = graphHandles;

	m_atariArea = new AtariArea(csvReader, graphName, "");
	m_damageArea = new AtariArea(csvReader, graphName, "damage_");

}

GraphHandlesWithAtari::~GraphHandlesWithAtari() {
	delete m_graphHandles;
	delete m_atariArea;
	delete m_damageArea;
}

void GraphHandlesWithAtari::getAtari(int* x1, int* y1, int* x2, int* y2, int index) const {
	int wide, height;
	GetGraphSize(m_graphHandles->getHandle(index), &wide, &height);
	m_atariArea->getArea(x1, y1, x2, y2, wide, height);
}


void GraphHandlesWithAtari::getDamage(int* x1, int* y1, int* x2, int* y2, int index) const {
	int wide, height;
	GetGraphSize(m_graphHandles->getHandle(index), &wide, &height);
	m_damageArea->getArea(x1, y1, x2, y2, wide, height);
}


/*
* �L�����N�^�[�̖ڂ̏u���̏���������N���X
*/
CharacterEyeClose::CharacterEyeClose() {
	m_cnt = 0;
}

// �u���X�^�[�g��true
bool CharacterEyeClose::closeFlag() {
	count();
	// �����Ă���
	if (closeNow()) {
		return true;
	}
	// �����n�߂�
	if (GetRand(100) < 1) {
		m_cnt = EYE_CLOSE_MIN_TIME + GetRand(EYE_CLOSE_MAX_TIME - EYE_CLOSE_MIN_TIME);
		return true;
	}
	// ����J���Ă���
	return false;
}

// �����鎞�Ԃ��J�E���g
void CharacterEyeClose::count() {
	m_cnt = m_cnt > 0 ? m_cnt - 1 : 0;
}


/*
* �L�����̉摜
*/
// �摜���[�h�p
void loadCharacterGraph(const char* dir, const char* characterName, GraphHandlesWithAtari*& handles, string stateName, map<string, string>& data, double ex, CsvReader* atariReader) {
	int n = stoi(data[stateName]);
	if (n > 0) {
		ostringstream oss;
		oss << dir << characterName << "/" << stateName;
		GraphHandles* graphHandles = new GraphHandles(oss.str().c_str(), n, ex, 0.0, true);
		handles = new GraphHandlesWithAtari(graphHandles, stateName.c_str(), atariReader);
	}
	else {
		handles = nullptr;
	}
}
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
	string path = "data/atari/";
	path += characterName;
	path += ".csv";
	CsvReader atariReader(path.c_str());

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);
	if (data.size() == 0) { data = reader.findOne("name", "�e�X�g"); }

	// ���[�h
	const char* dir = "picture/stick/";
	loadCharacterGraph(dir, characterName, m_standHandles, "stand", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_slashHandles, "slash", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_airSlashEffectHandles, "airSlashEffect", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_bulletHandles, "bullet", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_squatHandles, "squat", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_squatBulletHandles, "squatBullet", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_standBulletHandles, "standBullet", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_standSlashHandles, "standSlash", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_runHandles, "run", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_runBulletHandles, "runBullet", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_landHandles, "land", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_jumpHandles, "jump", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_downHandles, "down", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_preJumpHandles, "preJump", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_damageHandles, "damage", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_boostHandles, "boost", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_airBulletHandles, "airBullet", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_airSlashHandles, "airSlash", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_closeHandles, "close", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_deadHandles, "dead", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_initHandles, "init", data, m_ex, &atariReader);
	loadCharacterGraph(dir, characterName, m_special1Handles, "special1", data, m_ex, &atariReader);

	switchStand();
}
// �摜���폜
CharacterGraphHandle::~CharacterGraphHandle() {
	if (m_standHandles != nullptr) { delete m_standHandles; }
	if (m_slashHandles != nullptr) { delete m_slashHandles; }
	if (m_airSlashEffectHandles != nullptr) { delete m_airSlashEffectHandles; }
	if (m_bulletHandles != nullptr) { delete m_bulletHandles; }
	if (m_squatHandles != nullptr) { delete m_squatHandles; }
	if (m_squatBulletHandles != nullptr) { delete m_squatBulletHandles; }
	if (m_standBulletHandles != nullptr) { delete m_standBulletHandles; }
	if (m_standSlashHandles != nullptr) { delete m_standSlashHandles; }
	if (m_runHandles != nullptr) { delete m_runHandles; }
	if (m_runBulletHandles != nullptr) { delete m_runBulletHandles; }
	if (m_landHandles != nullptr) { delete m_landHandles; }
	if (m_jumpHandles != nullptr) { delete m_jumpHandles; }
	if (m_downHandles != nullptr) { delete m_downHandles; }
	if (m_preJumpHandles != nullptr) { delete m_preJumpHandles; }
	if (m_damageHandles != nullptr) { delete m_damageHandles; }
	if (m_boostHandles != nullptr) { delete m_boostHandles; }
	if (m_airBulletHandles != nullptr) { delete m_airBulletHandles; }
	if (m_airSlashHandles != nullptr) { delete m_airSlashHandles; }
	if (m_closeHandles != nullptr) { delete m_closeHandles; }
	if (m_deadHandles != nullptr) { delete m_deadHandles; }
	if (m_initHandles != nullptr) { delete m_initHandles; }
	if (m_special1Handles != nullptr) { delete m_special1Handles; }
}

void CharacterGraphHandle::getAtari(int* x1, int* y1, int* x2, int* y2) const {
	m_dispGraphHandle_p->getAtari(x1, y1, x2, y2, m_dispGraphIndex);
	*x1 = (int)(*x1 * m_ex);
	*y1 = (int)(*y1 * m_ex);
	*x2 = (int)(*x2 * m_ex);
	*y2 = (int)(*y2 * m_ex);
}

void CharacterGraphHandle::getDamage(int* x1, int* y1, int* x2, int* y2) const {
	m_dispGraphHandle_p->getDamage(x1, y1, x2, y2, m_dispGraphIndex);
	*x1 = (int)(*x1 * m_ex);
	*y1 = (int)(*y1 * m_ex);
	*x2 = (int)(*x2 * m_ex);
	*y2 = (int)(*y2 * m_ex);
}

// �摜�̃T�C�Y���Z�b�g
void CharacterGraphHandle::setGraphSize() {
	GetGraphSize(m_dispGraphHandle_p->getGraphHandles()->getGraphHandle(m_dispGraphIndex)->getHandle(), &m_wide, &m_height);
	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_ex);
	m_height = (int)(m_height * m_ex);
}

void CharacterGraphHandle::setAtari() {
	m_dispGraphHandle_p->getAtari(&m_atariX1, &m_atariY1, &m_atariX2, &m_atariY2, m_dispGraphIndex);
}

// �摜���Z�b�g���� ���݂��Ȃ��Ȃ炻�̂܂�
void CharacterGraphHandle::setGraph(GraphHandlesWithAtari* graphHandles, int index) {
	if (graphHandles == nullptr) { return; }
	if (index >= graphHandles->getGraphHandles()->getSize() || index < 0) { return; }
	m_dispGraphHandle_p = graphHandles;
	m_dispGraphIndex = index;
	setGraphSize();
	setAtari();
}

// �����摜���Z�b�g
void CharacterGraphHandle::switchStand(int index){
	if (m_closeHandles != nullptr && m_characterEyeClose.closeFlag()) {
		setGraph(m_closeHandles, index);
	}
	else {
		setGraph(m_standHandles, index);
	}
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
// �u���摜���Z�b�g
void CharacterGraphHandle::switchClose(int index) {
	setGraph(m_closeHandles, index);
}
// ����摜���Z�b�g
void CharacterGraphHandle::switchDead(int index) {
	setGraph(m_deadHandles, index);
}
// �{�X�̏����A�j���[�V�������Z�b�g
void CharacterGraphHandle::switchInit(int index) {
	setGraph(m_initHandles, index);
}
// �ǉ��摜���Z�b�g
void CharacterGraphHandle::switchSpecial1(int index) {
	setGraph(m_special1Handles, index);
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
	auto ite = data.begin();
	while (ite != data.end()) {
		string faceName = ite->first;
		if (faceName != "name") {
			loadCharacterGraph(dir, characterName, m_faceHandles[faceName], faceName, data, m_ex);
		}
		ite++;
	}

}
FaceGraphHandle::~FaceGraphHandle() {
	auto ite = m_faceHandles.begin();
	while (ite != m_faceHandles.end()) {
		string faceName = ite->first;
		if (m_faceHandles[faceName] != nullptr) {
			delete m_faceHandles[faceName];
		}
		ite++;
	}
}
GraphHandles* FaceGraphHandle::getGraphHandle(const char* faceName) {
	return m_faceHandles[faceName];
}