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
	double exX, exY;
	getGameEx(exX, exY);
	m_ex = min(exX, exY);
	m_finishFlag = false;
	m_cnt = 0;
	m_animation = nullptr;
	m_soundPlayer_p = soundPlayer_p;
	m_bgmPath = "";
	m_originalBgmPath = m_soundPlayer_p->getBgmName();
}

Movie::~Movie() {
	if (m_animation != nullptr) {
		delete m_animation;
	}
	m_soundPlayer_p->setBGM(m_originalBgmPath);
}

void Movie::play() {

	if (m_cnt == 0) {
		// 音楽開始
		m_soundPlayer_p->setBGM(m_bgmPath.c_str());
		m_soundPlayer_p->clearSoundQueue();
	}

	m_cnt++;

	// メイン画像
	m_animation->count();

	// サブ画像
	unsigned int size = (unsigned int)m_subAnimation.size();
	for (unsigned int i = 0; i < size; i++) {
		Animation* subAnimation = m_subAnimation.front();
		m_subAnimation.pop();
		subAnimation->count();
		if (!subAnimation->getFinishFlag()) {
			m_subAnimation.push(subAnimation);
		}
	}
}


// オープニング
OpMovie::OpMovie(SoundPlayer* soundPlayer_p):
	Movie(soundPlayer_p)
{
	string path = "picture/movie/op/";
	// タイトル
	m_titleH = new GraphHandles((path + "title/" + "h").c_str(), 4, m_ex);
	m_title = new GraphHandles((path + "title/" + "title").c_str(), 8, m_ex);
	m_titleChara = new GraphHandles((path + "title/" + "キャラ").c_str(), 5, m_ex);
	m_titleBlue = new GraphHandles((path + "title/" + "titleBlue").c_str(), 1, m_ex);
	m_titleOrange = new GraphHandles((path + "title/" + "titleOrange").c_str(), 1, m_ex);
	m_titleHeart = new GraphHandles((path + "title/" + "heart").c_str(), 1, m_ex);
	m_heartHide = new GraphHandles((path + "title/" + "ハート隠し").c_str(), 2, m_ex);
	// キャラ
	m_archive = new GraphHandles((path + "アーカイブ").c_str(), 1, m_ex);
	m_aigis = new GraphHandles((path + "アイギス").c_str(), 1, m_ex);
	m_assault = new GraphHandles((path + "アサルト03").c_str(), 1, m_ex);
	m_vermelia = new GraphHandles((path + "ヴェルメリア").c_str(), 1, m_ex);
	m_exlucina = new GraphHandles((path + "エクスルキナ").c_str(), 1, m_ex);
	m_msadi = new GraphHandles((path + "エムサディ").c_str(), 1, m_ex);
	m_elnino = new GraphHandles((path + "エルニーニョ").c_str(), 1, m_ex);
	m_onyx = new GraphHandles((path + "オニュクス").c_str(), 1, m_ex);
	m_courir = new GraphHandles((path + "クーリール").c_str(), 1, m_ex);
	m_cornein = new GraphHandles((path + "コーネイン").c_str(), 1, m_ex);
	m_koharu = new GraphHandles((path + "コハル").c_str(), 1, m_ex);
	m_siesta = new GraphHandles((path + "シエスタ").c_str(), 5, m_ex);
	m_chocola = new GraphHandles((path + "ショコラ").c_str(), 1, m_ex);
	m_titius = new GraphHandles((path + "ティティウス").c_str(), 1, m_ex);
	m_heart = new GraphHandles((path + "ハート").c_str(), 1, m_ex);
	m_fred = new GraphHandles((path + "フレッド").c_str(), 1, m_ex);
	m_french = new GraphHandles((path + "フレンチ").c_str(), 1, m_ex);
	m_mascara = new GraphHandles((path + "マスカーラ").c_str(), 1, m_ex);
	m_yuri = new GraphHandles((path + "ユーリ").c_str(), 1, m_ex);
	m_rabbi = new GraphHandles((path + "ラビ―").c_str(), 1, m_ex);
	m_tank = new GraphHandles((path + "棒タンク").c_str(), 1, m_ex);

	// 表示する順にpush
	const int CHARA_TIME = 32;
	characterQueue.push(make_pair(m_koharu, CHARA_TIME));
	characterQueue.push(make_pair(m_assault, CHARA_TIME));
	characterQueue.push(make_pair(m_msadi, CHARA_TIME));
	characterQueue.push(make_pair(m_exlucina, CHARA_TIME));
	characterQueue.push(make_pair(m_yuri, CHARA_TIME));
	characterQueue.push(make_pair(m_titius, CHARA_TIME));
	characterQueue.push(make_pair(m_tank, CHARA_TIME));
	characterQueue.push(make_pair(m_chocola, CHARA_TIME));
	characterQueue.push(make_pair(m_vermelia, CHARA_TIME));
	characterQueue.push(make_pair(m_french, CHARA_TIME));
	characterQueue.push(make_pair(m_courir, CHARA_TIME));
	characterQueue.push(make_pair(m_cornein, CHARA_TIME));
	characterQueue.push(make_pair(m_aigis, CHARA_TIME));
	characterQueue.push(make_pair(m_elnino, CHARA_TIME));
	characterQueue.push(make_pair(m_onyx, CHARA_TIME));
	characterQueue.push(make_pair(m_fred, CHARA_TIME));
	characterQueue.push(make_pair(m_mascara, CHARA_TIME));
	characterQueue.push(make_pair(m_rabbi, CHARA_TIME));
	characterQueue.push(make_pair(m_archive, CHARA_TIME));
	characterQueue.push(make_pair(m_siesta, CHARA_TIME));

	// 最初の画像
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, 120, m_titleH);

	// BGM
	m_bgmPath = "sound/movie/kobune.mp3";
}

OpMovie::~OpMovie() {
	// 画像
	// タイトル
	delete m_titleH;
	delete m_title;
	delete m_titleChara;
	delete m_titleBlue;
	delete m_titleOrange;
	delete m_titleHeart;
	delete m_heartHide;
	// キャラ
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

	// カウント
	Movie::play();

	// 画像を設定
	if (m_cnt == 45) {
		m_animation->changeGraph(m_titleChara, 12);
	}
	else if (m_cnt < 120 && m_animation->getFinishFlag()) {
		m_animation->changeGraph(m_titleH, 1000);
	}
	else if (m_cnt == 180) {
		m_animation->changeGraph(m_titleH, 5);
	}
	else if (m_cnt < 440 && m_animation->getFinishFlag()) {
		m_animation->changeGraph(m_title, 30);
	}
	else if (m_cnt == 440) {
		m_animation->changeGraph(m_heartHide, 60);
	}
	else if (m_cnt < 600 && m_cnt >= 560) {
		m_animation->changeGraph(m_titleHeart, 60);
		m_animation->setX(m_animation->getX() + (int)(15 * m_ex));
	}
	else if (m_cnt < 690 && m_cnt >= 600) {
		m_animation->setX(GAME_WIDE / 2);
		if (m_cnt / 5 % 2 == 0) {
			m_animation->changeGraph(m_titleBlue, 60);
		}
		else {
			m_animation->changeGraph(m_titleOrange, 60);
		}
	}
	else if (m_cnt < 700 && m_cnt >= 690) {
		m_animation->changeGraph(m_titleOrange, 60);
	}
	else if (m_cnt >= 2130 && !characterQueue.empty()) {
		if (m_animation->getFinishFlag() && !characterQueue.empty()) {
			GraphHandles* next = characterQueue.front().first;
			m_animation->changeGraph(next, characterQueue.front().second / next->getSize());
			characterQueue.pop();
		}
	}
	if (m_animation->getFinishFlag() && characterQueue.empty()) {
		m_animation->changeGraph(m_heart);
	}

	// 終了
	if (m_cnt == 5000) {
		m_finishFlag = true;
	}
}
