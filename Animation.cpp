#include "Animation.h"
#include "GraphHandle.h"
#include "Sound.h"
#include "Define.h"

#include<string>

using namespace std;


/*
* アニメーションのクラス
*/
Animation::Animation(int x, int y, int flameCnt, GraphHandles* graphHandles) {
	m_x = x;
	m_y = y;
	m_handles = graphHandles;
	m_flameCnt = flameCnt;
	init();
}

Animation* Animation::createCopy() {
	Animation* res = new Animation(m_x, m_y, m_flameCnt, m_handles);
	res->setCnt(m_cnt);
	res->setFinishCnt(m_finishCnt);
	res->setFinishFlag(m_finishFlag);
	return res;
}

// 初期化
void Animation::init() {
	m_cnt = 0;
	m_finishCnt = m_flameCnt * m_handles->getSize();
	m_finishFlag = false;
}

// アニメーションの切り替え
void Animation::changeGraph(GraphHandles* nextGraph, int flameCnt) {
	m_handles = nextGraph;
	if (flameCnt > 0) { m_flameCnt = flameCnt; }
	init();
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
GraphHandle* Animation::getHandle() const {
	return m_handles->getGraphHandle((m_cnt - 1) / m_flameCnt);
}



/*
* 動画の基底クラス
*/
Movie::Movie(SoundPlayer* soundPlayer_p) {
	m_finishFlag = false;
	m_cnt = 0;
	m_animation = nullptr;
	m_soundPlayer_p = soundPlayer_p;
}

Movie::~Movie() {
	if (m_animation != nullptr) {
		delete m_animation;
	}
}


// オープニング
OpMovie::OpMovie(SoundPlayer* soundPlayer_p):
	Movie(soundPlayer_p)
{
	string path = "picture/movie/op/";
	m_title = new GraphHandles((path + "複製のHeartタイトル").c_str(), 1);
	m_archive = new GraphHandles((path + "アーカイブ").c_str(), 1);
	m_aigis = new GraphHandles((path + "アイギス").c_str(), 1);
	m_assault = new GraphHandles((path + "アサルト03").c_str(), 1);
	m_vermelia = new GraphHandles((path + "ヴェルメリア").c_str(), 1);
	m_exlucina = new GraphHandles((path + "エクスルキナ").c_str(), 1);
	m_msadi = new GraphHandles((path + "エムサディ").c_str(), 1);
	m_elnino = new GraphHandles((path + "エルニーニョ").c_str(), 1);
	m_onyx = new GraphHandles((path + "オニュクス").c_str(), 1);
	m_courir = new GraphHandles((path + "クーリール").c_str(), 1);
	m_cornein = new GraphHandles((path + "コーネイン").c_str(), 1);
	m_koharu = new GraphHandles((path + "コハル").c_str(), 1);
	m_siesta = new GraphHandles((path + "シエスタ").c_str(), 5);
	m_chocola = new GraphHandles((path + "ショコラ").c_str(), 1);
	m_titius = new GraphHandles((path + "ティティウス").c_str(), 1);
	m_heart = new GraphHandles((path + "ハート").c_str(), 1);
	m_fred = new GraphHandles((path + "フレッド").c_str(), 1);
	m_french = new GraphHandles((path + "フレンチ").c_str(), 1);
	m_mascara = new GraphHandles((path + "マスカーラ").c_str(), 1);
	m_yuri = new GraphHandles((path + "ユーリ").c_str(), 1);
	m_rabbi = new GraphHandles((path + "ラビ―").c_str(), 1);
	m_tank = new GraphHandles((path + "棒タンク").c_str(), 1);

	// 最初の画像
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, 100, m_title);

	// 音楽
	m_soundPlayer_p->setBGM("sound/movie/kobune_full.mp3");
}

OpMovie::~OpMovie() {
	// 画像
	delete m_title;
	delete m_archive;
	delete m_aigis;
	delete m_assault;
	delete m_vermelia;
	delete m_exlucina;
	delete m_msadi;
	delete m_elnino;
	delete m_onyx;
	delete m_courir;
	delete m_cornein;
	delete m_koharu;
	delete m_siesta;
	delete m_chocola;
	delete m_titius;
	delete m_heart;
	delete m_fred;
	delete m_french;
	delete m_mascara;
	delete m_yuri;
	delete m_rabbi;
	delete m_tank;

	// 音楽を止める
	m_soundPlayer_p->stopBGM();
}

void OpMovie::play() {
	m_cnt++;
	m_animation->count();
	if (m_cnt == 180) {
		m_animation->changeGraph(m_siesta, 10);
	}
	else if (m_cnt == 230) {
		m_animation->changeGraph(m_heart);
	}

	// 終了
	if (m_soundPlayer_p->checkBGMplay() == 0) {
		m_finishFlag = true;
	}
}
