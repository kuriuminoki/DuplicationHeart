#include "TextDrawer.h"
#include "Text.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <string>


using namespace std;


ConversationDrawer::ConversationDrawer(Conversation* conversation) {

	m_conversation = conversation;

	m_ex = GAME_WIDE / 1920.0;

	// フォントデータ
	m_textHandle = CreateFontToHandle(nullptr, (int)(TEXT_SIZE * m_ex), 3);
	m_nameHandle = CreateFontToHandle(nullptr, (int)(NAME_SIZE * m_ex), 5);

	// 吹き出し画像
	m_frameHandle = LoadGraph("picture/textMaterial/frame.png");
}

ConversationDrawer::~ConversationDrawer() {
	// フォントデータ削除
	DeleteFontToHandle(m_textHandle);
	DeleteFontToHandle(m_nameHandle);
	// 画像データ削除
	DeleteGraph(m_frameHandle);
}

void ConversationDrawer::draw() {
	string text = m_conversation->getText();
	string name = m_conversation->getSpeakerName();
	GraphHandle* graph = m_conversation->getGraph();

	// キャラの顔画像は正方形を想定
	int graphSize = 0;
	GetGraphSize(graph->getHandle(), &graphSize, &graphSize);
	graphSize = (int)(graphSize * m_ex);

	// フキダシのフチの幅
	static const int TEXT_GRAPH_EDGE = (int)(35 * m_ex);

	// 端の余白
	static const int EDGE_X = (int)(48 * m_ex);
	static const int EDGE_DOWN = (int)(48 * m_ex);

	// 上端
	static const int Y1 = GAME_HEIGHT - EDGE_DOWN - graphSize - (TEXT_GRAPH_EDGE * 2);

	// 会話終了時
	int finishCnt = m_conversation->getFinishCnt() * 8;
	if ((Y1 + finishCnt) > (GAME_HEIGHT - EDGE_DOWN - finishCnt)) { return; }
	if (finishCnt > 0) {
		// フキダシ
		DrawExtendGraph(EDGE_X, Y1 + finishCnt, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN - finishCnt, m_frameHandle, TRUE);
		return;
	}

	// フキダシ
	DrawExtendGraph(EDGE_X, Y1, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN, m_frameHandle, TRUE);

	// 名前
	DrawStringToHandle(EDGE_X + TEXT_GRAPH_EDGE + graphSize + (int)(NAME_SIZE * m_ex), GAME_HEIGHT - EDGE_DOWN - (int)(NAME_SIZE * m_ex) - TEXT_GRAPH_EDGE, name.c_str(), BLACK, m_nameHandle);

	// テキスト
	int now = 0;
	int i = 0;
	static const int CHAR_EDGE = (int)(30 * m_ex);
	while (now < text.size()) {
		int next = now + min(MAX_TEXT_LEN, (int)text.size() - now);
		string disp = text.substr(now, next - now);
		DrawStringToHandle(EDGE_X + TEXT_GRAPH_EDGE * 2 + graphSize, Y1  + TEXT_GRAPH_EDGE + (i * ((int)(TEXT_SIZE * m_ex) + CHAR_EDGE)), disp.c_str(), BLACK, m_textHandle);
		now = next;
		i++;
	}

	// キャラの顔画像
	graph->draw(EDGE_X + TEXT_GRAPH_EDGE + graphSize / 2, Y1 + TEXT_GRAPH_EDGE + graphSize / 2, m_ex);

}