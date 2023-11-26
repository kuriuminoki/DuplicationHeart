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

// アニメの再生が終わったか
bool EventAnime::getFinishAnime() const {
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
* 会話イベント
*/
Conversation::Conversation(int textNum, World* world, SoundPlayer* soundPlayer) {

	m_finishCnt = 0;
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

	// 効果音
	m_displaySound = LoadSoundMem("sound/text/display.wav");
	m_nextSound = LoadSoundMem("sound/text/next.wav");

	// BGM
	m_originalBgmPath = m_soundPlayer_p->getBgmName();

	// 対象のファイルを開く
	ostringstream oss;
	oss << "data/text/text" << textNum << ".txt";
	m_fp = FileRead_open(oss.str().c_str());
	loadNextBlock();

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
}

// テキストを返す（描画用）
std::string Conversation::getText() const {
	return m_text.substr(0, m_textNow);
}

// 画像を返す（描画用）
GraphHandle* Conversation::getGraph() const {
	int size = (int)m_speakerGraph->getSize();
	int index = size - (m_textNow / 2 % size) - 1;
	index = m_textNow == (unsigned int)m_text.size() ? 0 : index;
	return m_speakerGraph->getGraphHandle(index);
}

int Conversation::getTextSize() const {
	return (int)m_text.size();
}

bool Conversation::play() {

	// 終了処理
	if (m_finishCnt > 0) {
		m_finishCnt++;
		if (m_finishCnt == FINISH_COUNT) {
			m_finishFlag = true;
			return true;
		}
		return false;
	}

	// アニメイベント
	if (m_eventAnime != nullptr && !m_eventAnime->getFinishAnime()) {
		m_eventAnime->play();
		if (m_eventAnime->getFinishAnime()) {
			// 次のテキストへ移る
			loadNextBlock();
		}
		return false;
	}

	// プレイヤーからのアクション（スペースキー入力）
	if (controlSpace() == 1 || leftClick() == 1) {
		if (m_textNow == m_text.size()) {
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
	m_cnt++;
	if (m_cnt % m_textSpeed == 0 && m_textNow < m_text.size()) {
		// 日本語表示は１文字がサイズ２分
		m_textNow = min(m_textNow + 2, (unsigned int)m_text.size());
		// 効果音
		m_soundPlayer_p->pushSoundQueue(m_displaySound);
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
	if (str == "@eventStart" || str == "@eventPic" || str == "@eventToDark" || str == "@eventToClear") { // 挿絵の始まり
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
	else if (str == "@eventEnd") { // 挿絵の終わり
		delete m_eventAnime;
		m_eventAnime = nullptr;
		loadNextBlock();
	}
	else if (str == "@same") { // セリフだけ更新
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
	else { // 発言
		if (str == "@null" || str == "???" || str == "ひとみ") { // ナレーション
			// 発言者
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
	m_speakerGraph = c->getFaceHandle()->getGraphHandle(faceName);
}