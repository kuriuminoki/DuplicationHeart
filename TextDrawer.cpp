#include "TextDrawer.h"
#include "Text.h"
#include "GraphHandle.h"
#include "Animation.h"
#include "AnimationDrawer.h"
#include "Define.h"
#include "DxLib.h"
#include <string>


using namespace std;


ConversationDrawer::ConversationDrawer(Conversation* conversation) {

	m_conversation = conversation;

	m_animationDrawer = new AnimationDrawer(nullptr);

	getGameEx(m_exX, m_exY);

	// �t�H���g�f�[�^
	m_textHandle = CreateFontToHandle(nullptr, (int)(TEXT_SIZE * m_exX), 3);
	m_nameHandle = CreateFontToHandle(nullptr, (int)(NAME_SIZE * m_exX), 5);

	// �����o���摜
	m_frameHandle = LoadGraph("picture/textMaterial/frame.png");
}

ConversationDrawer::~ConversationDrawer() {
	delete m_animationDrawer;
	// �t�H���g�f�[�^�폜
	DeleteFontToHandle(m_textHandle);
	DeleteFontToHandle(m_nameHandle);
	// �摜�f�[�^�폜
	DeleteGraph(m_frameHandle);
}

void ConversationDrawer::draw() {

	// �A�j��
	const Animation* anime = m_conversation->getAnime();
	if (anime != nullptr) {
		int bright = m_conversation->getAnimeBright();
		SetDrawBright(bright, bright, bright);
		m_animationDrawer->setAnimation(anime);
		m_animationDrawer->drawAnimation();
		if (m_conversation->animePlayNow()) { return; }
		SetDrawBright(255, 255, 255);
	}

	string text = m_conversation->getText();
	string name = m_conversation->getSpeakerName();
	GraphHandle* graph = m_conversation->getGraph();

	// �L�����̊�摜�͐����`��z��
	int graphSize = 0;
	GetGraphSize(graph->getHandle(), &graphSize, &graphSize);
	graphSize = (int)(graphSize * m_exX);

	// �t�L�_�V�̃t�`�̕�
	static const int TEXT_GRAPH_EDGE = (int)(35 * m_exX);

	// �[�̗]��
	static const int EDGE_X = (int)(48 * m_exX);
	static const int EDGE_DOWN = (int)(48 * m_exX);

	// ��[
	static const int Y1 = GAME_HEIGHT - EDGE_DOWN - graphSize - (TEXT_GRAPH_EDGE * 2);

	// ��b�I����
	int finishCnt = m_conversation->getFinishCnt() * 8;
	if ((Y1 + finishCnt) > (GAME_HEIGHT - EDGE_DOWN - finishCnt)) { return; }
	if (finishCnt > 0) {
		// �t�L�_�V
		DrawExtendGraph(EDGE_X, Y1 + finishCnt, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN - finishCnt, m_frameHandle, TRUE);
		return;
	}

	// �t�L�_�V
	DrawExtendGraph(EDGE_X, Y1, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN, m_frameHandle, TRUE);

	// �����҂̖��O�A�Z���t��摜
	int now = 0;
	int i = 0;
	static const int CHAR_EDGE = (int)(30 * m_exX);
	if (m_conversation->getNoFace()) { // ��摜���Ȃ��ꍇ
		int x = EDGE_X + TEXT_GRAPH_EDGE + graphSize / 2;
		// ���O
		DrawStringToHandle(x, GAME_HEIGHT - EDGE_DOWN - (int)(NAME_SIZE * m_exX) - TEXT_GRAPH_EDGE, name.c_str(), BLACK, m_nameHandle);
		// �Z���t
		drawText(x, Y1 + TEXT_GRAPH_EDGE, (int)(TEXT_SIZE * m_exX) + CHAR_EDGE, text, BLACK, m_textHandle);
	}
	else { // ��摜������ꍇ
		// ���O
		DrawStringToHandle(EDGE_X + TEXT_GRAPH_EDGE + graphSize + (int)(NAME_SIZE * m_exX), GAME_HEIGHT - EDGE_DOWN - (int)(NAME_SIZE * m_exX) - TEXT_GRAPH_EDGE, name.c_str(), BLACK, m_nameHandle);
		// �Z���t
		drawText(EDGE_X + TEXT_GRAPH_EDGE * 2 + graphSize, Y1 + TEXT_GRAPH_EDGE, (int)(TEXT_SIZE * m_exX) + CHAR_EDGE, text, BLACK, m_textHandle);
		// �L�����̊�摜
		graph->draw(EDGE_X + TEXT_GRAPH_EDGE + graphSize / 2, Y1 + TEXT_GRAPH_EDGE + graphSize / 2, m_exX);
	}

}

void ConversationDrawer::drawText(int x, int y,int height, std::string text, int color, int font) {
	int now = 0;
	int i = 0;
	int size = (int)(text.size());
	// �Z���t
	while (now < size) {

		// ���͉������ڂ܂ŕ\�����邩
		int next = now + min(MAX_TEXT_LEN, (int)text.size() - now);

		string disp = text.substr(now, next - now);
		size_t br = disp.find("�b");
		if (br != string::npos) {
			disp = disp.substr(0, br);
			now += (int)br + 2;
		}
		else {
			now = next;
		}

		// �Z���t��`��
		DrawStringToHandle(x, y + (i * height), disp.c_str(), BLACK, m_textHandle);

		// ���̍s
		i++;
	}
}