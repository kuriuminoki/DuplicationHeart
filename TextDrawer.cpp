#include "TextDrawer.h"
#include "Text.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <string>


using namespace std;


ConversationDrawer::ConversationDrawer(Conversation* conversation) {

	m_conversation = conversation;

	// フォントデータ
	m_textHandle = CreateFontToHandle(NULL, TEXT_SIZE, 3);
	m_nameHandle = CreateFontToHandle(NULL, NAME_SIZE, 5);

	// 画像
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
	int graphSize = 0;
	GetGraphSize(graph->getHandle(), &graphSize, &graphSize);

	// 端の余白
	static const int EDGE = 52;

	// フキダシのフチの幅
	static const int TEXT_GRAPH_EDGE = 32;

	// 上端
	static const int Y1 = 551;

	// 会話終了時
	int finishCnt = m_conversation->getFinishCnt() * 8;
	if (finishCnt > 0) {
		// フキダシ
		DrawExtendGraph(EDGE, Y1 + finishCnt, GAME_WIDE - EDGE, GAME_HEIGHT - EDGE - finishCnt, m_frameHandle, TRUE);
		return;
	}

	// フキダシ
	DrawExtendGraph(EDGE, Y1, GAME_WIDE - EDGE, GAME_HEIGHT - EDGE, m_frameHandle, TRUE);

	// 名前
	DrawStringToHandle(EDGE * 2 + graphSize, GAME_HEIGHT - EDGE - NAME_SIZE - TEXT_GRAPH_EDGE, name.c_str(), BLACK, m_nameHandle);

	// テキスト
	int now = 0;
	int i = 0;
	while (now < text.size()) {
		int next = now + min(MAX_TEXT_LEN, (int)text.size() - now);
		string disp = text.substr(now, next - now);
		DrawStringToHandle(EDGE * 3 + graphSize, Y1 + 5 + EDGE + (i * (TEXT_SIZE + 10)), disp.c_str(), BLACK, m_textHandle);
		now = next;
		i++;
	}

	// キャラの顔画像
	graph->draw(EDGE + TEXT_GRAPH_EDGE + graphSize / 2, Y1 + TEXT_GRAPH_EDGE + graphSize / 2, 1.0);

}