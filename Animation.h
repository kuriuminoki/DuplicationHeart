#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class GraphHandle;
class GraphHandles;
class SoundPlayer;

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

	// 初期化
	void init();

	// アニメーションの切り替え
	void changeGraph(GraphHandles* nextGraph, int flameCnt = -1);

	// カウント
	void count();

	// 描画用
	GraphHandle* getHandle() const;
};


// 動画の基底クラス
class Movie {
protected:
	// 終了したらtrue
	bool m_finishFlag;

	// 開始からの経過時間
	int m_cnt;

	// 画像を入れて動かす
	Animation* m_animation;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

public:
	Movie(SoundPlayer* soundPlayer_p);
	~Movie();

	// ゲッタ
	bool getFinishFlag() const { return m_finishFlag; }
	Animation* getAnimation() const { return m_animation; }
	inline int getCnt() const { return m_cnt; }

	// 再生
	virtual void play() = 0;
};


// オープニング
class OpMovie:
	public Movie
{
private:

	// 画像
	GraphHandles* m_title;
	GraphHandles* m_archive;
	GraphHandles* m_aigis;
	GraphHandles* m_assault;
	GraphHandles* m_vermelia;
	GraphHandles* m_exlucina;
	GraphHandles* m_msadi;
	GraphHandles* m_elnino;
	GraphHandles* m_onyx;
	GraphHandles* m_courir;
	GraphHandles* m_cornein;
	GraphHandles* m_koharu;
	GraphHandles* m_siesta;
	GraphHandles* m_chocola;
	GraphHandles* m_titius;
	GraphHandles* m_heart;
	GraphHandles* m_fred;
	GraphHandles* m_french;
	GraphHandles* m_mascara;
	GraphHandles* m_yuri;
	GraphHandles* m_rabbi;
	GraphHandles* m_tank;

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// 再生
	void play();
};


#endif