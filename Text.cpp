#include "Text.h"
#include "Animation.h"
#include "World.h"
#include "GraphHandle.h"
#include "Control.h"
#include "Sound.h"
#include "Character.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>
#include <string>
#include <sstream>


using namespace std;



/*
* イベント中に挿入される画像
*/
EventAnime::EventAnime(const char* filePath, int sum, int speed) {

	// ゲームの解像度を考慮して拡大率決定
	double exX, exY;
	getGameEx(exX, exY);
	double ex = min(exX, exY);

	// 画像ロード
	string path = "picture/event/";
	m_handles = new GraphHandles((path + filePath).c_str(), sum, ex);

	// アニメのスピード
	m_speed = speed == -1 ? m_speed : speed;

	// アニメ
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, m_speed, m_handles);

	m_bright = 255;
	m_toDark = false;

	m_finishFlag = false;
}

EventAnime::~EventAnime() {
	delete m_handles;
	delete m_animation;
}

// アニメイベントが終わったか
bool EventAnime::getFinishAnimeEvent() const {
	if (m_bright > 0 && m_toDark) { return false; }
	if (m_bright < 255 && !m_toDark) { return false; }
	return m_finishFlag;
}

// falseの間は操作不可
void EventAnime::play() {
	if (m_bright < 255 && !m_toDark) { m_bright++; return; }
	else if (m_bright > 0 && m_toDark) { m_bright--; return; }
	m_animation->count();
	if (m_animation->getFinishFlag() && leftClick() == 1) {
		m_finishFlag = true;
	}
}



/*
* フキダシのアクション
*/
TextAction::TextAction() {
	init();
}

void TextAction::init() {
	m_jumpFlag = false;
	m_dx = 0;
	m_dy = 0;
	m_vy = 0;
	m_quakeCnt = 0;
	m_quakeDx = 0;
	m_quakeDy = 0;
}

void TextAction::play() {

	if (m_jumpFlag) {
		m_vy++;
		m_dy += m_vy;
		if (m_dy > 0 && m_vy > 0) { 
			init();
		}
	}

	if (m_quakeCnt > 0) { 
		m_quakeCnt--;
		m_quakeDx = GetRand(50) - 25;
		m_quakeDy = GetRand(50) - 25;
		if (m_vy == 0 && m_quakeCnt == 0) {
			init();
		}
	}

}



/*
* 会話イベント
*/
Conversation::Conversation(int textNum, World* world, SoundPlayer* soundPlayer) {

	m_finishCnt = 0;
	m_startCnt = FINISH_COUNT;
	m_finishFlag = false;
	m_world_p = world;
	m_soundPlayer_p = soundPlayer;
	m_speakerName = "ハート";
	setSpeakerGraph("通常");
	m_noFace = true;
	m_text = "";
	m_textNow = 0;
	m_cnt = 0;
	m_textSpeed = TEXT_SPEED;
	m_eventAnime = nullptr;
	m_sound = -1;

	// 効果音
	m_displaySound = LoadSoundMem("sound/text/display.wav");
	m_nextSound = LoadSoundMem("sound/text/next.wav");

	// BGM
	m_originalBgmPath = "";

	// 対象のファイルを開く
	ostringstream oss;
	oss << "data/text/text" << textNum << ".txt";
	m_fp = FileRead_open(oss.str().c_str());

	double exX = 0, exY = 0;
	getGameEx(exX, exY);

	// クリックエフェクト
	m_clickGraph = new GraphHandles("picture/system/clickEffect", 4, 0.5 * exX, 0, true);

	// セリフを最後まで表示したときの画像
	m_textFinishGraph = new GraphHandle("picture/system/textFinish.png", 0.5 * exX, 0, true);

}

Conversation::~Conversation() {
	// 効果音削除
	DeleteSoundMem(m_displaySound);
	DeleteSoundMem(m_nextSound);
	// ファイルを閉じる
	FileRead_close(m_fp);
	if (m_eventAnime != nullptr) { delete m_eventAnime; }
	// BGMを戻す
	m_soundPlayer_p->setBGM(m_originalBgmPath);
	// クリックエフェクト削除
	delete m_clickGraph;
	for (unsigned i = 0; i < m_animations.size(); i++) {
		delete m_animations[i];
	}
	delete m_textFinishGraph;
	if (m_sound != -1) {
		DeleteSoundMem(m_sound);
	}
}

// テキストを返す（描画用）
std::string Conversation::getText() const {
	return m_text.substr(0, m_textNow);
}

// 画像を返す（描画用）
GraphHandle* Conversation::getGraph() const {
	int size = (int)m_speakerGraph_p->getSize();
	int index = size - (m_textNow / 2 % size) - 1;
	index = m_textNow == (unsigned int)m_text.size() ? 0 : index;
	return m_speakerGraph_p->getGraphHandle(index);
}

// セリフの長さ
int Conversation::getTextSize() const {
	return (int)m_text.size();
}

// セリフを最後まで表示したか
bool Conversation::finishText() const {
	return m_textNow == m_text.size();
}

// 会話イベントの処理
bool Conversation::play() {

	// 効果音の制御
	if (m_sound != -1 && CheckSoundMem(m_sound) == 0) {
		DeleteSoundMem(m_sound);
		m_sound = -1;
	}

	// クリックエフェクトの再生
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}

	// クリックのエフェクト
	if (leftClick() == 1) {
		int handX = 0, handY = 0;
		GetMousePoint(&handX, &handY);
		m_animations.push_back(new Animation(handX, handY, 4, m_clickGraph));
	}

	// クリック長押しで終了
	if (leftClick() == 60) { m_finishFlag = true; return true; }

	// 終了処理
	if (m_finishCnt > 0) {
		m_finishCnt++;
		if (m_finishCnt == FINISH_COUNT) {
			if (FileRead_eof(m_fp) != 0) {
				// ファイルを読み終えたから
				m_finishFlag = true;
				return true;
			}
			else {
				// ファイルを読み終えてないから
				m_finishCnt = 0;
				m_startCnt = FINISH_COUNT;
			}
		}
		return false;
	}

	// アニメイベント
	if (m_eventAnime != nullptr && !m_eventAnime->getFinishAnimeEvent()) {
		m_cnt++;
		m_eventAnime->play();
		if (m_eventAnime->getFinishAnimeEvent()) {
			// 次のテキストへ移る
			loadNextBlock();
		}
		return false;
	}

	// イベント開始前のBGM名をバックアップ
	if (m_text == "") {
		m_originalBgmPath = m_soundPlayer_p->getBgmName();
		loadNextBlock();
	}

	// プレイヤーからのアクション（スペースキー入力）
	if (leftClick() == 1) {
		m_textAction.init();
		if (finishText()) {
			// 全ての会話が終わった
			if (FileRead_eof(m_fp) != 0) {
				m_finishCnt++;
				return false;
			}
			// 次のテキストへ移る
			loadNextBlock();
			// 効果音
			m_soundPlayer_p->pushSoundQueue(m_nextSound);
		}
		else if(m_cnt > MOVE_FINAL_ABLE) {
			// 最後までテキストを飛ばす
			m_textNow = (unsigned int)m_text.size();
		}
	}

	// 表示文字を増やす
	if (m_startCnt > 0) {
		m_startCnt--;
	}
	else {
		m_cnt++;
		if (m_cnt % m_textSpeed == 0 && !finishText()) {
			// 日本語表示は１文字がサイズ２分
			m_textNow = min(m_textNow + 2, (unsigned int)m_text.size());
			// 効果音
			m_soundPlayer_p->pushSoundQueue(m_displaySound);
		}
	}

	// フキダシのアクション
	if (m_startCnt == 0 && m_finishCnt == 0) {
		m_textAction.play();
	}

	return false;
}

void Conversation::loadNextBlock() {
	// バッファ
	const int size = 512;
	char buff[size];
	// ブロックの1行目
	string str = "";
	while (FileRead_eof(m_fp) == 0) {
		// 空行以外が来るまでループ
		FileRead_gets(buff, size, m_fp);
		str = buff;
		if (str != "") { break; }
	}
	if (str == "@eventStart" || str == "@eventPic" || str == "@eventToDark" || str == "@eventToClear") {
		// 挿絵の始まり
		if (m_eventAnime != nullptr) { delete m_eventAnime; }
		FileRead_gets(buff, size, m_fp);
		string path = buff;
		FileRead_gets(buff, size, m_fp);
		string sum = buff;
		FileRead_gets(buff, size, m_fp);
		string speed = buff;
		if (speed == "") {
			m_eventAnime = new EventAnime(path.c_str(), stoi(sum));
		}
		else {
			m_eventAnime = new EventAnime(path.c_str(), stoi(sum), stoi(speed));
		}
		if (str == "@eventPic") {
			m_eventAnime->setFinishFlag(true);
			loadNextBlock();
		}
		else if (str == "@eventToDark") {
			m_eventAnime->setFinishFlag(true);
			m_eventAnime->setToDark(true);
		}
		else if (str == "@eventToClear") {
			m_eventAnime->setFinishFlag(true);
			m_eventAnime->setBright(0);
		}
	}
	else if (str == "@eventEnd") {
		// 挿絵の終わり
		delete m_eventAnime;
		m_eventAnime = nullptr;
		loadNextBlock();
	}
	else if (str == "@same") {
		// セリフだけ更新
		setNextText(size, buff);
	}
	else if (str == "@setBGM") {
		// BGMを変更
		FileRead_gets(buff, size, m_fp);
		string path = "sound/";
		path += buff;
		m_soundPlayer_p->setBGM(path);
		loadNextBlock();
	}
	else if (str == "@stopBGM") {
		// BGMを止める
		m_soundPlayer_p->stopBGM();
		loadNextBlock();
	}
	else if (str == "@resetBGM") {
		// BGMを戻す
		m_soundPlayer_p->setBGM(m_originalBgmPath);
		loadNextBlock();
	}
	else if (str == "@startCnt") {
		// startCnt = FINISH_COUNTに戻し、フキダシを大きくする
		m_startCnt = FINISH_COUNT;
		loadNextBlock();
	}
	else if (str == "@finishCnt") {
		// finishCntを加算していき、フキダシを小さくする
		m_finishCnt = 1;
		loadNextBlock();
	}
	else if (str == "@quake") {
		// フキダシを揺らす
		FileRead_gets(buff, size, m_fp);
		string s = buff; // 時間
		m_textAction.setQuakeCnt(stoi(s));
		loadNextBlock();
	}
	else if (str == "@jump") {
		// フキダシをジャンプさせる
		FileRead_gets(buff, size, m_fp);
		string s = buff; // 初速
		m_textAction.setVy(stoi(s));
		loadNextBlock();
	}
	else if (str == "@sound") {
		// 効果音を鳴らす
		if (m_sound != -1) { DeleteSoundMem(m_sound); }
		FileRead_gets(buff, size, m_fp);
		string path = "sound/";
		path += buff;
		m_sound = LoadSoundMem(path.c_str());
		m_soundPlayer_p->pushSoundQueue(m_sound);
		loadNextBlock();
	}
	else { // 発言
		if (str == "@null" || str == "???" || str == "ひとみ") {
			// ナレーション
			m_speakerName = str == "@null" ? "" : str;
			m_noFace = true;
		}
		else {
			// 発言者
			m_speakerName = buff;
			// 画像
			FileRead_gets(buff, size, m_fp);
			setSpeakerGraph(buff);
			m_noFace = false;
		}
		setNextText(size, buff);
	}
}


void Conversation::setNextText(const int size, char* buff) {
	m_cnt = 0;
	m_textNow = 0;

	FileRead_gets(buff, size, m_fp);
	m_text = buff;

	if (FileRead_eof(m_fp) == 0) {
		FileRead_gets(buff, size, m_fp);
		string s = buff;
		if (s == "") {
			m_textSpeed = TEXT_SPEED;
		}
		else {
			m_textSpeed = stoi(s);
			FileRead_gets(buff, size, m_fp);
		}
	}
	else {
		m_textSpeed = TEXT_SPEED;
	}
}


void Conversation::setSpeakerGraph(const char* faceName) {
	Character* c = m_world_p->getCharacterWithName(m_speakerName);
	m_speakerGraph_p = c->getFaceHandle()->getGraphHandle(faceName);
}

// セッタ
void Conversation::setWorld(World* world) {
	m_world_p = world;
	m_speakerName = "ハート";
	setSpeakerGraph("通常");
}