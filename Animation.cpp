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
		// ���y�J�n
		m_soundPlayer_p->setBGM(m_bgmPath.c_str());
		m_soundPlayer_p->clearSoundQueue();
	}

	m_cnt++;

	// ���C���摜
	m_animation->count();

	// �T�u�摜
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


// �I�[�v�j���O
OpMovie::OpMovie(SoundPlayer* soundPlayer_p):
	Movie(soundPlayer_p)
{
	string path = "picture/movie/op/";
	// �^�C�g��
	m_titleH = new GraphHandles((path + "title/" + "h").c_str(), 4, m_ex);
	m_title = new GraphHandles((path + "title/" + "title").c_str(), 8, m_ex);
	m_titleChara = new GraphHandles((path + "title/" + "�L����").c_str(), 5, m_ex);
	m_titleBlue = new GraphHandles((path + "title/" + "titleBlue").c_str(), 1, m_ex);
	m_titleOrange = new GraphHandles((path + "title/" + "titleOrange").c_str(), 1, m_ex);
	m_titleHeart = new GraphHandles((path + "title/" + "heart").c_str(), 1, m_ex);
	m_heartHide = new GraphHandles((path + "title/" + "�n�[�g�B��").c_str(), 2, m_ex);
	// �L����
	m_archive = new GraphHandles((path + "�A�[�J�C�u").c_str(), 1, m_ex);
	m_aigis = new GraphHandles((path + "�A�C�M�X").c_str(), 1, m_ex);
	m_assault = new GraphHandles((path + "�A�T���g03").c_str(), 1, m_ex);
	m_vermelia = new GraphHandles((path + "���F�������A").c_str(), 1, m_ex);
	m_exlucina = new GraphHandles((path + "�G�N�X���L�i").c_str(), 1, m_ex);
	m_msadi = new GraphHandles((path + "�G���T�f�B").c_str(), 1, m_ex);
	m_elnino = new GraphHandles((path + "�G���j�[�j��").c_str(), 1, m_ex);
	m_onyx = new GraphHandles((path + "�I�j���N�X").c_str(), 1, m_ex);
	m_courir = new GraphHandles((path + "�N�[���[��").c_str(), 1, m_ex);
	m_cornein = new GraphHandles((path + "�R�[�l�C��").c_str(), 1, m_ex);
	m_koharu = new GraphHandles((path + "�R�n��").c_str(), 1, m_ex);
	m_siesta = new GraphHandles((path + "�V�G�X�^").c_str(), 5, m_ex);
	m_chocola = new GraphHandles((path + "�V���R��").c_str(), 1, m_ex);
	m_titius = new GraphHandles((path + "�e�B�e�B�E�X").c_str(), 1, m_ex);
	m_heart = new GraphHandles((path + "�n�[�g").c_str(), 1, m_ex);
	m_fred = new GraphHandles((path + "�t���b�h").c_str(), 1, m_ex);
	m_french = new GraphHandles((path + "�t�����`").c_str(), 1, m_ex);
	m_mascara = new GraphHandles((path + "�}�X�J�[��").c_str(), 1, m_ex);
	m_yuri = new GraphHandles((path + "���[��").c_str(), 1, m_ex);
	m_rabbi = new GraphHandles((path + "���r�\").c_str(), 1, m_ex);
	m_tank = new GraphHandles((path + "�_�^���N").c_str(), 1, m_ex);

	// �\�����鏇��push
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

	// �ŏ��̉摜
	m_animation = new Animation(GAME_WIDE / 2, GAME_HEIGHT / 2, 120, m_titleH);

	// BGM
	m_bgmPath = "sound/movie/kobune.mp3";
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
	delete m_heartHide;
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

	// �J�E���g
	Movie::play();

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

	// �I��
	if (m_cnt == 5000) {
		m_finishFlag = true;
	}
}
