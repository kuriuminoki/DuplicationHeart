#include "Animation.h"
#include "GraphHandle.h"


Animation::Animation(int x, int y, int flameCnt, GraphHandles* graphHandles) {
	m_x = x;
	m_y = y;
	m_handles = graphHandles;
	m_flameCnt = flameCnt;
	m_cnt = 0;
	m_finishCnt = m_flameCnt * m_handles->getSize();
	m_finishFlag = false;
}

Animation::~Animation() {
	// delete m_handles;
}

// カウント
void Animation::count() { 
	if (m_cnt == m_finishCnt) {
		m_finishFlag = true;
	}
	else {
		m_cnt++;
	}
};

// 描画用
GraphHandle* Animation::getHandle() {
	return m_handles->getGraphHandle((m_cnt - 1) / m_flameCnt);
}