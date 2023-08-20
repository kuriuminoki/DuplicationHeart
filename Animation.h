#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class GraphHandle;
class GraphHandles;

class Animation {
private:
	// 画像ハンドル
	GraphHandles* m_handles;

	// 座標
	int m_x, m_y;
	
	// カウント
	int m_cnt;

	// 終了までのカウント
	int m_finishCnt;

	// 画像１枚の表示時間
	int m_flameCnt;

	// アニメーションが終了
	bool m_finishFlag;

public:
	Animation(int x, int y, int flameCnt, GraphHandles* graphHandles);

	Animation* createCopy();

	// ゲッタ
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getFinishFlag() const { return m_finishFlag; }

	// セッタ
	inline void setCnt(int cnt) { m_cnt = cnt; }
	inline void setFinishCnt(int finishCnt) { m_finishCnt = finishCnt; }
	inline void setFinishFlag(int finishFlag) { m_finishFlag = finishFlag; }

	// カウント
	void count();

	// 描画用
	GraphHandle* getHandle() const;
};

#endif