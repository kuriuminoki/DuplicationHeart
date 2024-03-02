#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <queue>
#include <string>
#include <vector>

class GraphHandle;
class GraphHandles;
class SoundPlayer;
class AnimationDrawer;

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

	// ループ再生
	bool m_loopFlag;

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
	inline void setFinishFlag(bool finishFlag) { m_finishFlag = finishFlag; }
	inline void setLoopFlag(bool loopFlag) { m_loopFlag = loopFlag; }

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

	int m_flameWide, m_flameHeight;

	// 終了したらtrue
	bool m_finishFlag;

	// 開始からの経過時間
	int m_cnt;

	// 画像を入れて動かすメイン画像
	int m_centerX, m_centerY;
	Animation* m_animation;
	AnimationDrawer* m_animationDrawer;

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

	// 描画
	virtual void draw();

protected:
	void drawFlame();
};


// オープニング用
class PartOneCharacter {
private:

	// 画像
	GraphHandle* m_character;

	// 座標
	int m_initX, m_initY;
	int m_x, m_y;

	// 速度
	int m_vx, m_vy;

	double m_ex;

public:
	PartOneCharacter(GraphHandle* character, int initX, int initY, int vx, double ex);

	void play();
	void draw();
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
	GraphHandles* m_heartHide2;

	// part1
	GraphHandles* m_darkHeart;
	GraphHandles* m_heartEye;
	std::vector<PartOneCharacter*> m_partOneCharacters;
	GraphHandle* m_archive1;
	GraphHandle* m_aigis1;
	GraphHandle* m_assault1;
	GraphHandle* m_vermelia1;
	GraphHandle* m_exlucina1;
	GraphHandle* m_msadi1;
	GraphHandle* m_elnino1;
	GraphHandle* m_onyx1;
	GraphHandle* m_courir1;
	GraphHandle* m_cornein1;
	GraphHandle* m_koharu1;
	GraphHandle* m_siesta1;
	GraphHandle* m_hierarchy1;
	GraphHandle* m_troy1;
	GraphHandle* m_ancient1;
	GraphHandle* m_valkiria1;
	GraphHandle* m_chocola1;
	GraphHandle* m_titius1;
	GraphHandle* m_fred1;
	GraphHandle* m_french1;
	GraphHandle* m_mascara1;
	GraphHandle* m_yuri1;
	GraphHandle* m_rabbi1;

	// part2
	GraphHandles* m_heartAndMem;
	GraphHandles* m_heartCry1;
	GraphHandles* m_heartCry2;
	GraphHandles* m_memSad1;
	GraphHandles* m_memSad2;
	GraphHandles* m_eyeFocus;

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
	GraphHandles* m_memoryA;
	GraphHandles* m_memoryB;
	GraphHandles* m_yuri;
	GraphHandles* m_rabbi;
	GraphHandles* m_tank;

	// サビ
	GraphHandles* m_orange;
	GraphHandles* m_duplications;
	Animation* m_orangeAnime;
	Animation* m_duplicationsAnime;
	GraphHandles* m_heartFlame;
	GraphHandles* m_rmem;
	GraphHandles* m_heartSabi;
	GraphHandles* m_tvSiesta;
	GraphHandles* m_tvHierarchy;
	GraphHandles* m_tvValkiria;
	GraphHandles* m_tvTroy;
	GraphHandles* m_tvHeart;
	GraphHandles* m_tvShine;
	GraphHandles* m_tvRshine;
	GraphHandles* m_titleFinal;

	// キャラを順に表示する用 (graph, cntSum)
	std::queue<std::pair<GraphHandles*, int> > characterQueue;

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// 再生
	void play();

	// 描画
	void draw();

private:
	void pushPartOneCharacter(int index, bool front, GraphHandle* character);
};


#endif