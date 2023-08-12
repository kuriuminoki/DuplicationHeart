#include "Text.h"
#include "World.h"
#include "GraphHandle.h"
#include "Control.h"
#include "Sound.h"
#include "Character.h"
#include "DxLib.h"
#include <algorithm>
#include <string>
#include <sstream>


using namespace std;


Conversation::Conversation(int textNum, World* world, SoundPlayer* soundPlayer) {

	m_finishFlag = false;
	m_world_p = world;
	m_soundPlayer_p = soundPlayer;
	m_speakerName = "";
	m_speakerGraph = NULL;
	m_text = "";
	m_textNow = 0;
	m_cnt = 0;

	// ���ʉ�
	m_displaySound = LoadSoundMem("sound/text/display.wav");
	m_nextSound = LoadSoundMem("sound/text/next.wav");

	// �Ώۂ̃t�@�C�����J��
	ostringstream oss;
	oss << "data/text/text" << textNum << ".txt";
	m_fp = FileRead_open(oss.str().c_str());
	setNextText();

}

Conversation::~Conversation() {
	// ���ʉ��폜
	DeleteSoundMem(m_displaySound);
	DeleteSoundMem(m_nextSound);
	// �t�@�C�������
	FileRead_close(m_fp);
}

// �e�L�X�g��Ԃ��i�`��p�j
std::string Conversation::getText() const {
	return m_text.substr(0, m_textNow);
}

// �摜��Ԃ��i�`��p�j
GraphHandle* Conversation::getGraph() {
	//int size = (int)m_speakerGraph->getSize();
	//int index = m_textNow / 2 % size;
	return m_speakerGraph->getGraphHandle(0);
}

int Conversation::getTextSize() const {
	return (int)m_text.size();
}

bool Conversation::play() {

	// �v���C���[����̃A�N�V�����i�X�y�[�X�L�[���́j
	if (controlSpace() == 1 || leftClick() == 1) {
		if (m_textNow == m_text.size()) {
			// �S�Ẳ�b���I�����
			if (FileRead_eof(m_fp) != 0) {
				m_finishFlag = true;
				return true;
			}
			// ���̃e�L�X�g�ֈڂ�
			setNextText();
			// ���ʉ�
			m_soundPlayer_p->pushSoundQueue(m_nextSound);
		}
		else if(m_cnt > MOVE_FINAL_ABLE) {
			// �Ō�܂Ńe�L�X�g���΂�
			m_textNow = (unsigned int)m_text.size();
		}
	}

	// �\�������𑝂₷
	m_cnt++;
	if (m_cnt % TEXT_SPEED == 0 && m_textNow < m_text.size()) {
		// ���{��\���͂P�������T�C�Y�Q��
		m_textNow = min(m_textNow + 2, (unsigned int)m_text.size());
		// ���ʉ�
		m_soundPlayer_p->pushSoundQueue(m_displaySound);
	}

	return false;
}

void Conversation::setNextText() {
	m_cnt = 0;
	m_textNow = 0;
	// �o�b�t�@
	const int size = 512;
	char buff[size];
	// ������
	FileRead_gets(buff, size, m_fp);
	m_speakerName = buff;
	// �摜
	FileRead_gets(buff, size, m_fp);
	setSpeakerGraph(buff);
	// �e�L�X�g
	FileRead_gets(buff, size, m_fp);
	m_text = buff;

	if (FileRead_eof(m_fp) == 0) {
		FileRead_gets(buff, size, m_fp);
	}
}

void Conversation::setSpeakerGraph(const char* faceName) {
	Character* c = m_world_p->getCharacterWithName(m_speakerName);
	m_speakerGraph = c->getFaceHandle()->getGraphHandle(faceName);
}