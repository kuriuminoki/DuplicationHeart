#include "Animation.h"
#include "GraphHandle.h"
#include "Sound.h"
#include "Define.h"

#include<string>

using namespace std;


/*
* �A�j���[�V�����̃N���X
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

// ������
void Animation::init() {
	m_cnt = 0;
	m_finishCnt = m_flameCnt * m_handles->getSize();
	m_finishFlag = false;
}

// �A�j���[�V�����̐؂�ւ�
void Animation::changeGraph(GraphHandles* nextGraph, int flameCnt) {
	m_handles = nextGraph;
	if (flameCnt > 0) { m_flameCnt = flameCnt; }
	init();
}


// �J�E���g
void Animation::count() { 
	if (m_cnt == m_finishCnt) {
		m_finishFlag = true;
	}
	else {
		m_cnt++;
	}
};

// �`��p
GraphHandle* Animation::getHandle() const {
	return m_handles->getGraphHandle((m_cnt - 1) / m_flameCnt);
}



/*
* ����̊��N���X
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


// �I�[�v�j���O
OpMovie::OpMovie(SoundPlayer* soundPlayer_p):
	Movie(soundPlayer_p)
{
	string path = "picture/movie/op/";
	// �^�C�g��
	m_titleH = new GraphHandles((path + "title/" + "h").c_str(), 4);
	m_title = new GraphHandles((path + "title/" + "title").c_str(), 8);
	m_titleChara = new GraphHandles((path + "title/" + "�L����").c_str(), 5);
	m_titleBlue = new GraphHandles((path + "title/" + "titleBlue").c_str(), 1);
	m_titleOrange = new GraphHandles((path + "title/" + "titleOrange").c_str(), 1);
	m_titleHeart = new GraphHandles((path + "title/" + "heart").c_str(), 1);
	// �L����
	m_archive = new GraphHandles((path + "�A�[�J�C�u").c_str(), 1);
	m_aigis = new GraphHandles((path + "�A�C�M�X").c_str(), 1);
	m_assault = new GraphHandles((path + "�A�T���g03").c_str(), 1);
	m_vermelia = new GraphHandles((path + "���F�������A").c_str(), 1);
	m_exlucina = new GraphHandles((path + "�G�N�X���L�i").c_str(), 1);
	m_msadi = new GraphHandles((path + "�G���T�f�B").c_str(), 1);
	m_elnino = new GraphHandles((path + "�G���j�[�j��").c_str(), 1);
	m_onyx = new GraphHandles((path + "�I�j���N�X").c_str(), 1);
	m_courir = new GraphHandles((path + "�N�[���[��").c_str(), 1);
	m_cornein = new GraphHandles((path + "�R�[�l�C��").c_str(), 1);
	m_koharu = new GraphHandles((path + "�R�n��").c_str(), 1);
	m_siesta = new GraphHandles((path + "�V�G�X�^").c_str(), 5);
	m_chocola = new GraphHandles((path + "�V���R��").c_str(), 1);
	m_titius = new GraphHandles((path + "�e�B�e�B�E�X").c_str(), 1);
	m_heart = new GraphHandles((path + "�n�[�g").c_str(), 1);
	m_fred = new GraphHandles((path + "�t���b�h").c_str(), 1);
	m_french = new GraphHandles((path + "�t�����`").c_str(), 1);
	m_mascara = new GraphHandles((path + "�}�X�J�[��").c_str(), 1);
	m_yuri = new GraphHandles((path + "���[��").c_str(), 1);
	m_rabbi = new GraphHandles((path + "���r�\").c_str(), 1);
	m_tank = new GraphHandles((path + "�_�^���N").c_str(), 1);

	// �\�����鏇��push
	characterQueue.push(make_pair(m_koharu, 30));
	characterQueue.push(make_pair(m_assault, 30));
	characterQueue.push(make_pair(m_msadi, 30));
	characterQueue.push(make_pair(m_exlucina, 30));
	characterQueue.push(make_pair(m_yuri, 30));
	characterQueue.push(make_pair(m_titius, 30));
	characterQueue.push(make_pair(m_tank, 30));
	characterQueue.push(make_pair(m_chocola, 30));
	characterQueue.push(make_pair(m_vermelia, 30));
	characterQueue.push(make_pair(m_french, 30));
	characterQueue.push(make_pair(m_courir, 30));
	characterQueue.push(make_pair(m_cornein, 30));
	characterQueue.push(make_pair(m_aigis, 30));
	characterQueue.push(make_pair(m_elnino, 30));
	characterQueue.push(make_pair(m_onyx, 30));
	characterQueue.push(make_pair(m_fred, 30));
	characterQueue.push(make_pair(m_mascara, 30));
	characterQueue.push(make_pair(m_rabbi, 30));
	characterQueue.push(make_pair(m_archive, 30));
	characterQueue.push(make_pair(m_siesta, 30));

	// �ŏ��̉摜
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, 120, m_titleH);

	// ���y
	m_soundPlayer_p->setBGM("sound/movie/kobune.mp3");
	m_soundPlayer_p->clearSoundQueue();
}

OpMovie::~OpMovie() {
	// �摜
	// �^�C�g��
	delete m_titleH;
	delete m_title;
	delete m_titleChara;
	delete m_titleBlue;
	delete m_titleOrange;
	delete m_titleHeart;
	// �L����
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

	// ���y���~�߂�
	m_soundPlayer_p->stopBGM();
}

void OpMovie::play() {
	m_cnt++;
	m_animation->count();

	// �摜��ݒ�
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
	else if (m_cnt < 600 && m_cnt >= 440) {
		m_animation->changeGraph(m_titleHeart, 60);
		m_animation->setX(m_animation->getX() + 8);
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
	else if (m_cnt >= 2130 && m_cnt < 2740) {
		if (m_animation->getFinishFlag() && !characterQueue.empty()) {
			GraphHandles* next = characterQueue.front().first;
			m_animation->changeGraph(next, characterQueue.front().second / next->getSize());
			characterQueue.pop();
		}
	}
	else if (m_cnt == 2760) {
		m_animation->changeGraph(m_heart);
	}

	// �I��
	if (m_cnt == 5000) {
		m_finishFlag = true;
	}
}
