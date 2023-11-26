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
* �C�x���g���ɑ}�������摜
*/
EventAnime::EventAnime(const char* filePath, int sum, int speed) {

	// �Q�[���̉𑜓x���l�����Ċg�嗦����
	double exX, exY;
	getGameEx(exX, exY);
	double ex = min(exX, exY);

	// �摜���[�h
	string path = "picture/event/";
	m_handles = new GraphHandles((path + filePath).c_str(), sum, ex);

	// �A�j���̃X�s�[�h
	m_speed = speed == -1 ? m_speed : speed;

	// �A�j��
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, m_speed, m_handles);

	m_bright = 255;
	m_toDark = false;

	m_finishFlag = false;
}

EventAnime::~EventAnime() {
	delete m_handles;
	delete m_animation;
}

// �A�j���̍Đ����I�������
bool EventAnime::getFinishAnime() const {
	if (m_bright > 0 && m_toDark) { return false; }
	if (m_bright < 255 && !m_toDark) { return false; }
	return m_finishFlag;
}

// false�̊Ԃ͑���s��
void EventAnime::play() {
	if (m_bright < 255 && !m_toDark) { m_bright++; return; }
	else if (m_bright > 0 && m_toDark) { m_bright--; return; }
	m_animation->count();
	if (m_animation->getFinishFlag() && leftClick() == 1) {
		m_finishFlag = true;
	}
}



/*
* ��b�C�x���g
*/
Conversation::Conversation(int textNum, World* world, SoundPlayer* soundPlayer) {

	m_finishCnt = 0;
	m_finishFlag = false;
	m_world_p = world;
	m_soundPlayer_p = soundPlayer;
	m_speakerName = "�n�[�g";
	setSpeakerGraph("�ʏ�");
	m_noFace = true;
	m_text = "";
	m_textNow = 0;
	m_cnt = 0;
	m_textSpeed = TEXT_SPEED;
	m_eventAnime = nullptr;

	// ���ʉ�
	m_displaySound = LoadSoundMem("sound/text/display.wav");
	m_nextSound = LoadSoundMem("sound/text/next.wav");

	// BGM
	m_originalBgmPath = m_soundPlayer_p->getBgmName();

	// �Ώۂ̃t�@�C�����J��
	ostringstream oss;
	oss << "data/text/text" << textNum << ".txt";
	m_fp = FileRead_open(oss.str().c_str());
	loadNextBlock();

}

Conversation::~Conversation() {
	// ���ʉ��폜
	DeleteSoundMem(m_displaySound);
	DeleteSoundMem(m_nextSound);
	// �t�@�C�������
	FileRead_close(m_fp);
	if (m_eventAnime != nullptr) { delete m_eventAnime; }
	// BGM��߂�
	m_soundPlayer_p->setBGM(m_originalBgmPath);
}

// �e�L�X�g��Ԃ��i�`��p�j
std::string Conversation::getText() const {
	return m_text.substr(0, m_textNow);
}

// �摜��Ԃ��i�`��p�j
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

	// �I������
	if (m_finishCnt > 0) {
		m_finishCnt++;
		if (m_finishCnt == FINISH_COUNT) {
			m_finishFlag = true;
			return true;
		}
		return false;
	}

	// �A�j���C�x���g
	if (m_eventAnime != nullptr && !m_eventAnime->getFinishAnime()) {
		m_eventAnime->play();
		if (m_eventAnime->getFinishAnime()) {
			// ���̃e�L�X�g�ֈڂ�
			loadNextBlock();
		}
		return false;
	}

	// �v���C���[����̃A�N�V�����i�X�y�[�X�L�[���́j
	if (controlSpace() == 1 || leftClick() == 1) {
		if (m_textNow == m_text.size()) {
			// �S�Ẳ�b���I�����
			if (FileRead_eof(m_fp) != 0) {
				m_finishCnt++;
				return false;
			}
			// ���̃e�L�X�g�ֈڂ�
			loadNextBlock();
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
	if (m_cnt % m_textSpeed == 0 && m_textNow < m_text.size()) {
		// ���{��\���͂P�������T�C�Y�Q��
		m_textNow = min(m_textNow + 2, (unsigned int)m_text.size());
		// ���ʉ�
		m_soundPlayer_p->pushSoundQueue(m_displaySound);
	}

	return false;
}

void Conversation::loadNextBlock() {
	// �o�b�t�@
	const int size = 512;
	char buff[size];
	// �u���b�N��1�s��
	string str = "";
	while (FileRead_eof(m_fp) == 0) {
		// ��s�ȊO������܂Ń��[�v
		FileRead_gets(buff, size, m_fp);
		str = buff;
		if (str != "") { break; }
	}
	if (str == "@eventStart" || str == "@eventPic" || str == "@eventToDark" || str == "@eventToClear") { // �}�G�̎n�܂�
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
	else if (str == "@eventEnd") { // �}�G�̏I���
		delete m_eventAnime;
		m_eventAnime = nullptr;
		loadNextBlock();
	}
	else if (str == "@same") { // �Z���t�����X�V
		setNextText(size, buff);
	}
	else if (str == "@setBGM") {
		// BGM��ύX
		FileRead_gets(buff, size, m_fp);
		string path = "sound/";
		path += buff;
		m_soundPlayer_p->setBGM(path);
		loadNextBlock();
	}
	else if (str == "@stopBGM") {
		// BGM���~�߂�
		m_soundPlayer_p->stopBGM();
		loadNextBlock();
	}
	else if (str == "@resetBGM") {
		// BGM��߂�
		m_soundPlayer_p->setBGM(m_originalBgmPath);
		loadNextBlock();
	}
	else { // ����
		if (str == "@null" || str == "???" || str == "�ЂƂ�") { // �i���[�V����
			// ������
			m_speakerName = str == "@null" ? "" : str;
			m_noFace = true;
		}
		else {
			// ������
			m_speakerName = buff;
			// �摜
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