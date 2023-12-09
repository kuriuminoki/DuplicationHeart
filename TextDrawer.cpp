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

	// �A�j���݂̂�\�����ăe�L�X�g�͕\�����Ȃ��Ȃ�true
	bool animeOnly = false;

	// ���W���̏���
	string text = m_conversation->getText();
	string name = m_conversation->getSpeakerName();
	GraphHandle* graph = m_conversation->getGraph();
	// �L�����̊�摜�͐����`��z��
	int graphSize = 0;
	GetGraphSize(graph->getHandle(), &graphSize, &graphSize);
	graphSize = (int)(graphSize * m_exX);
	// �t�L�_�V�̃t�`�̕�
	const int TEXT_GRAPH_EDGE = (int)(35 * m_exX);
	// �[�̗]��
	const int EDGE_X = (int)(48 * m_exX);
	const int EDGE_DOWN = (int)(48 * m_exX);
	// ��[
	const int Y1 = GAME_HEIGHT - EDGE_DOWN - graphSize - (TEXT_GRAPH_EDGE * 2);

	// �A�j��
	const Animation* anime = m_conversation->getAnime();
	if (anime != nullptr) {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
		int bright = m_conversation->getAnimeBright();
		SetDrawBright(bright, bright, bright);
		m_animationDrawer->setAnimation(anime);
		m_animationDrawer->drawAnimation();
		SetDrawBright(255, 255, 255);
		if (m_conversation->animePlayNow() && m_conversation->getFinishCnt() == 0) { animeOnly = true; }
	}

	// �A�j���ȊO
	if (!animeOnly) {
		// ��b�I����
		if (m_conversation->getFinishCnt() > 0) {
			int finishCnt = m_conversation->getFinishCnt() * 8 * m_exY;
			if ((Y1 + finishCnt) <= (GAME_HEIGHT - EDGE_DOWN - finishCnt)) { 
				// �t�L�_�V
				DrawExtendGraph(EDGE_X, Y1 + finishCnt, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN - finishCnt, m_frameHandle, TRUE);
			}
		}
		// ��b�J�n��
		else if (m_conversation->getStartCnt() > 0) {
			int startCnt = m_conversation->getStartCnt() * 8 * m_exY;
			if ((Y1 + startCnt) <= (GAME_HEIGHT - EDGE_DOWN - startCnt)) { 
				// �t�L�_�V
				DrawExtendGraph(EDGE_X, Y1 + startCnt, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN - startCnt, m_frameHandle, TRUE);
			}
		}
		// ��b��
		else {
			// �t�L�_�V
			DrawExtendGraph(EDGE_X, Y1, GAME_WIDE - EDGE_X, GAME_HEIGHT - EDGE_DOWN, m_frameHandle, TRUE);

			// �����҂̖��O�A�Z���t��摜
			int now = 0;
			int i = 0;
			const int CHAR_EDGE = (int)(30 * m_exX);
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
	}
	
	bool textFinish = m_conversation->finishText() && m_conversation->getFinishCnt() == 0 && m_conversation->getStartCnt() == 0;
	bool eventFinish = !(m_conversation->animePlayNow()) || (m_conversation->getEventAnime()->getAnime()->getFinishFlag());
	if (textFinish &&eventFinish) {
		int dy = (int)(((m_conversation->getCnt() / 3) % 20 - 10) * m_exY);
		m_conversation->getTextFinishGraph()->draw(GAME_WIDE - EDGE_X - (int)(100 * m_exX), GAME_HEIGHT - EDGE_DOWN - (int)(50 * m_exY) + dy - TEXT_GRAPH_EDGE, m_conversation->getTextFinishGraph()->getEx());
	}

	// �N���b�N�G�t�F�N�g
	const vector<Animation*> animations = m_conversation->getAnimations();
	size_t size = animations.size();
	for (unsigned int i = 0; i < size; i++) {
		// Animation��Drawer�ɃZ�b�g
		m_animationDrawer->setAnimation(animations[i]);

		// �J�������g���ăL������`��
		m_animationDrawer->drawAnimation();
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