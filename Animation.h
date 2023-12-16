#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <queue>
#include <string>

class GraphHandle;
class GraphHandles;
class SoundPlayer;

class Animation {
private:
	// 画像ハンドル
	GraphHandles* m_handles_p;

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
	inline int getCnt() const { return m_cnt; }

	// セッタ
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
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

	// 解像度の変更に対応
	double m_ex;

	// 終了したらtrue
	bool m_finishFlag;

	// 開始からの経過時間
	int m_cnt;

	// 画像を入れて動かすメイン画像
	Animation* m_animation;

	// サブ画像 cntが0になったものはpopしていく
	std::queue<Animation*> m_subAnimation;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// BGMのパス
	std::string m_bgmPath;

	// もともと流していたBGM
	std::string m_originalBgmPath;

public:
	Movie(SoundPlayer* soundPlayer_p);
	virtual ~Movie();

	// ゲッタ
	bool getFinishFlag() const { return m_finishFlag; }
	Animation* getAnimation() const { return m_animation; }
	std::queue<Animation*> getSubAnimation() const { return m_subAnimation; }
	inline int getCnt() const { return m_cnt; }

	// 再生
	virtual void play();
};


// オープニング
class OpMovie:
	public Movie
{
private:

	// 画像
	// タイトル
	GraphHandles* m_titleH;
	GraphHandles* m_title;
	GraphHandles* m_titleChara;
	GraphHandles* m_titleBlue;
	GraphHandles* m_titleOrange;
	GraphHandles* m_titleHeart;
	GraphHandles* m_heartHide;
	// キャラ
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

	// キャラを順に表示する用 (graph, cntSum)
	std::queue<std::pair<GraphHandles*, int> > characterQueue;

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// 再生
	void play();
};


#endif