#ifndef GRAPH_HANDLE_H_INCLUDED
#define GRAPH_HANDLE_H_INCLUDED

/*
* 画像データ(ハンドル、拡大率、向き)をまとめて扱うためのクラス
*/
class GraphHandle {
private:
	// 画像ハンドル
	int m_handle;

	// 拡大率
	double m_ex;

	// 向き
	double m_angle;

	// 透明度を有効にするか
	bool m_trans;

	// 反転するか
	bool m_reverseX;
	bool m_reverseY;

public:
	GraphHandle(const char* filePath, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandle();

	// ゲッタ
	inline int getHandle() { return m_handle; }

	// セッタ
	inline void setEx(double ex) { m_ex = ex; }
	inline void setAngle(double angle) { m_angle = angle; }
	inline void setTrans(bool trans) { m_trans = trans; }
	inline void setReverseX(bool reverse) { m_reverseX = reverse; }
	inline void setReverseY(bool reverse) { m_reverseY = reverse; }

	// 描画する
	void draw(int x, int y);
};


/*
* GraphHandleを配列としてまとめて扱うクラス
*/
class GraphHandles {
private:
	GraphHandle** m_handles;
	int m_handleSum;

public:
	//  filePathの末尾には".png"をつけないこと。
	GraphHandles(const char* filePath, int handleSum, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandles();

	// ゲッタ
	inline GraphHandle* getGraphHandle(int i) { return m_handles[i]; }
	inline int getHandle(int i) { return m_handles[i]->getHandle(); }

	// セッタ
	void setEx(double ex);
	void setAngle(double angle);
	void setTrans(bool trans);
	void setReverseX(bool reverse);
	void setReverseY(bool reverse);

	// 描画する
	void draw(int x, int y, int index);
};


#endif