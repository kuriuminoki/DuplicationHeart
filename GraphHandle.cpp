#include "GraphHandle.h"
#include "DxLib.h"
#include <string>
#include <sstream>

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
void GraphHandle::draw(int x, int y, double ex = 1.0) const {
	DrawRotaGraph(x, y, x, m_angle, m_handle, m_trans, m_reverseX, m_reverseY);
}


/*
* 複数の画像をまとめて扱うクラス
*/
GraphHandles::GraphHandles(const char* filePath, int handleSum, double ex, double angle, bool trans, bool reverseX, bool reverseY) {
	m_handleSum = handleSum;
	m_handles = new GraphHandle* [m_handleSum];
	for (int i = 0; i < m_handleSum; i++) {
		ostringstream oss;
		oss << filePath << i << ".png";
		m_handles[i] = new GraphHandle(oss.str().c_str(), ex, angle, trans, reverseX, reverseY);
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