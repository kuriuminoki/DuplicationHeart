#include "Animation.h"
#include "AnimationDrawer.h"
#include "Control.h"
#include "GraphHandle.h"
#include "Sound.h"
#include "DrawTool.h"
#include "Define.h"
#include "DxLib.h"

#include<string>

using namespace std;


/*
* �A�j���[�V�����̃N���X
*/
Animation::Animation(int x, int y, int flameCnt, GraphHandles* graphHandles) {
	m_x = x;
	m_y = y;
	m_vx = 0;
	m_vy = 0;
	m_movable = false;
	m_handles_p = graphHandles;
	m_flameCnt = flameCnt;
	m_loopFlag = false;
	init();
}

Animation* Animation::createCopy() {
	Animation* res = new Animation(m_x, m_y, m_flameCnt, m_handles_p);
	res->setVx(m_vx);
	res->setVy(m_vy);
	res->setMovable(m_movable);
	res->setCnt(m_cnt);
	res->setFinishCnt(m_finishCnt);
	res->setFinishFlag(m_finishFlag);
	return res;
}

// ������
void Animation::init() {
	m_cnt = 0;
	m_finishCnt = m_flameCnt * m_handles_p->getSize();
	m_finishFlag = false;
}

// �A�j���[�V�����̐؂�ւ�
void Animation::changeGraph(GraphHandles* nextGraph, int flameCnt) {
	m_handles_p = nextGraph;
	if (flameCnt > 0) { m_flameCnt = flameCnt; }
	init();
}


// �J�E���g
void Animation::count() { 
	if (m_movable) {
		m_x += m_vx;
		m_y += m_vy;
	}
	if (m_cnt == m_finishCnt) {
		if (m_loopFlag) {
			init();
		}
		else {
			m_finishFlag = true;
		}
	}
	else {
		m_cnt++;
	}
};

// �`��p
GraphHandle* Animation::getHandle() const {
	return m_handles_p->getGraphHandle((m_cnt - 1) / m_flameCnt);
}



/*
* ����̊��N���X
*/
Movie::Movie(SoundPlayer* soundPlayer_p) {
	getGameEx(m_exX, m_exY);
	m_ex = min(m_exX, m_exY);
	m_finishFlag = false;
	m_cnt = 0;
	m_animation = nullptr;
	m_animationDrawer = new AnimationDrawer(m_animation);
	m_soundPlayer_p = soundPlayer_p;
	m_bgmPath = "";
	m_originalBgmPath = m_soundPlayer_p->getBgmName();

	m_flameWide = (GAME_WIDE - (int)(GAME_WIDE_DEFAULT * m_ex)) / 2;
	m_flameHeight = (GAME_HEIGHT - (int)(GAME_HEIGHT_DEFAULT * m_ex)) / 2;

	// �t�H���g�f�[�^
	m_textHandle = CreateFontToHandle(nullptr, (int)(TEXT_SIZE * m_exX), 3);
}

Movie::~Movie() {
	if (m_animation != nullptr) {
		delete m_animation;
	}
	m_soundPlayer_p->setBGM(m_originalBgmPath);
	// �t�H���g�f�[�^�폜
	DeleteFontToHandle(m_textHandle);
}

void Movie::play() {

	if (m_cnt == 0) {
		// ���y�J�n
		if (m_bgmPath != "") {
			m_soundPlayer_p->setBGM(m_bgmPath.c_str());
			m_soundPlayer_p->playBGM();
		}
		m_soundPlayer_p->clearSoundQueue();
	}

	m_cnt++;

	// ���C���摜
	if (m_animation != nullptr) {
		m_animation->count();
	}

	// Z�L�[�������ŏI��
	if (controlZ() > 0) {
		if (m_skipCnt++ == FPS_N) {
			m_finishFlag = true;
		}
	}
	else { m_skipCnt = 0; }
}

void Movie::draw() {
	if (m_animation != nullptr) {
		m_animationDrawer->setAnimation(m_animation);
		m_animationDrawer->drawAnimation();
	}
}

void Movie::drawFlame() {
	if (m_flameWide > 0) {
		DrawBox(0, 0, m_flameWide + 1, GAME_HEIGHT, BLACK, TRUE);
		DrawBox(GAME_WIDE - m_flameWide - 1, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
	}
	if (m_flameHeight > 0) {
		DrawBox(0, 0, GAME_WIDE, m_flameHeight + 1, BLACK, TRUE);
		DrawBox(0, GAME_HEIGHT - m_flameHeight - 1, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
	}

	// �f�o�b�O�p
	//DrawFormatString(0, GAME_HEIGHT - 100, BLACK, "COUNT = %d", m_cnt);
	//DrawFormatString(0, GAME_HEIGHT - 50, WHITE, "COUNT = %d", m_cnt);
}


// �I�[�v�j���O�p
PartOneCharacter::PartOneCharacter(GraphHandle* character, int initX, int initY, int vx, double ex) {
	m_character = character;
	m_initX = initX;
	m_initY = initY;
	m_x = m_initX;
	m_y = m_initY;
	m_vx = vx;
	m_vy = 0;
	m_ex = ex;
}

void PartOneCharacter::play() {
	m_x += m_vx;
	m_y += m_vy;
	if (m_y < m_initY) {
		m_vy++;
	}
	if (m_y >= m_initY) {
		m_y = m_initY;
		m_vy = 0;
	}
	if (m_y == m_initY && GetRand(100) < 1) {
		m_vy = (int)(-20 * m_ex);
	}
}
void PartOneCharacter::draw() {
	m_character->draw(m_x, m_y, m_ex);
}


void OpMovie::pushPartOneCharacter(int index, bool front, GraphHandle* character) {
	int y = GAME_HEIGHT * 5 / 6 - m_flameHeight / 2;
	int vx = -8;
	double ex = m_ex;
	int wide = (int)(800 * m_ex);
	int x = 0;
	if (!front) { // ���̃L����
		double backEx = 0.3;
		y = GAME_HEIGHT/ 5 + m_flameHeight / 2;
		vx = (int)(5 * m_ex);
		ex *= backEx;
		wide = (int)(400 * m_ex);
		x = index * (-wide) + (GAME_WIDE / 2);
	}
	else {
		double frontEx = 0.7;
		vx = (int)(-10 * m_ex);
		ex *= frontEx;
		wide = (int)(700 * m_ex);
		x = GAME_WIDE / 2 + (index * wide);
	}
	m_partOneCharacters.push_back(new PartOneCharacter(character, x, y, vx, ex));
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
	m_heartHide = new GraphHandles((path + "title/" + "�n�[�g�B��").c_str(), 3, m_ex);
	m_heartHide2 = new GraphHandles((path + "title/" + "�n�[�g�B��").c_str(), 1, m_ex);

	// part1
	m_darkHeart = new GraphHandles((path + "part1/" + "darkHeart").c_str(), 3, m_ex);
	m_heartEye = new GraphHandles((path + "part1/" + "heartEye").c_str(), 4, m_ex);
	m_archive1 = new GraphHandle((path + "part1/character/" + "�A�[�J�C�u.png").c_str(), m_ex, 0.0, true);
	m_aigis1 = new GraphHandle((path + "part1/character/" + "�A�C�M�X.png").c_str(), m_ex, 0.0, true);
	m_assault1 = new GraphHandle((path + "part1/character/" + "�A�T���g03.png").c_str(), m_ex, 0.0, true);
	m_vermelia1 = new GraphHandle((path + "part1/character/" + "���F�������A.png").c_str(), m_ex, 0.0, true);
	m_exlucina1 = new GraphHandle((path + "part1/character/" + "�G�N�X���L�i.png").c_str(), m_ex, 0.0, true);
	m_msadi1 = new GraphHandle((path + "part1/character/" + "�G���E�T�f�B.png").c_str(), m_ex, 0.0, true);
	m_elnino1 = new GraphHandle((path + "part1/character/" + "�G���j�[�j��.png").c_str(), m_ex, 0.0, true);
	m_onyx1 = new GraphHandle((path + "part1/character/" + "�I�j���N�X.png").c_str(), m_ex, 0.0, true);
	m_courir1 = new GraphHandle((path + "part1/character/" + "�N�[���[��.png").c_str(), m_ex, 0.0, true);
	m_cornein1 = new GraphHandle((path + "part1/character/" + "�R�[�l�C��.png").c_str(), m_ex, 0.0, true);
	m_koharu1 = new GraphHandle((path + "part1/character/" + "�R�n��.png").c_str(), m_ex, 0.0, true);
	m_siesta1 = new GraphHandle((path + "part1/character/" + "�V�G�X�^.png").c_str(), m_ex, 0.0, true);
	m_hierarchy1 = new GraphHandle((path + "part1/character/" + "�q�G�����L�[.png").c_str(), m_ex, 0.0, true);;
	m_troy1 = new GraphHandle((path + "part1/character/" + "�g���C.png").c_str(), m_ex, 0.0, true);;
	m_ancient1 = new GraphHandle((path + "part1/character/" + "�G���V�����g.png").c_str(), m_ex, 0.0, true);;
	m_valkiria1 = new GraphHandle((path + "part1/character/" + "���@���L���A.png").c_str(), m_ex, 0.0, true);;
	m_chocola1 = new GraphHandle((path + "part1/character/" + "�V���R��.png").c_str(), m_ex, 0.0, true);
	m_titius1 = new GraphHandle((path + "part1/character/" + "�e�B�e�B�E�X.png").c_str(), m_ex, 0.0, true);
	m_fred1 = new GraphHandle((path + "part1/character/" + "�t���b�h.png").c_str(), m_ex, 0.0, true);
	m_french1 = new GraphHandle((path + "part1/character/" + "�t�����`.png").c_str(), m_ex, 0.0, true);
	m_mascara1 = new GraphHandle((path + "part1/character/" + "�}�X�J�[��.png").c_str(), m_ex, 0.0, true);
	m_yuri1 = new GraphHandle((path + "part1/character/" + "���[��.png").c_str(), m_ex, 0.0, true);
	m_rabbi1 = new GraphHandle((path + "part1/character/" + "���r�\.png").c_str(), m_ex, 0.0, true);
	pushPartOneCharacter(0, false, m_fred1);
	pushPartOneCharacter(1, false, m_yuri1);
	pushPartOneCharacter(2, false, m_aigis1);
	pushPartOneCharacter(3, false, m_koharu1);
	pushPartOneCharacter(4, false, m_exlucina1);
	pushPartOneCharacter(5, false, m_elnino1);
	pushPartOneCharacter(6, false, m_troy1);
	pushPartOneCharacter(7, false, m_chocola1);
	pushPartOneCharacter(8, false, m_onyx1);

	pushPartOneCharacter(0, true, m_vermelia1);
	pushPartOneCharacter(1, true, m_mascara1);
	pushPartOneCharacter(2, true, m_archive1);
	pushPartOneCharacter(3, true, m_rabbi1);
	pushPartOneCharacter(4, true, m_ancient1);
	pushPartOneCharacter(5, true, m_cornein1);
	pushPartOneCharacter(6, true, m_hierarchy1);
	pushPartOneCharacter(7, true, m_assault1);
	pushPartOneCharacter(8, true, m_valkiria1);


	// part2
	m_heartAndMem = new GraphHandles((path + "part2/" + "heart&Mem").c_str(), 1, m_ex);
	m_heartCry1 = new GraphHandles((path + "part2/" + "heartA").c_str(), 3, m_ex);
	m_heartCry2 = new GraphHandles((path + "part2/" + "heartB").c_str(), 4, m_ex);
	m_memSad1 = new GraphHandles((path + "part2/" + "memA").c_str(), 3, m_ex);
	m_memSad2 = new GraphHandles((path + "part2/" + "memB").c_str(), 3, m_ex);
	m_eyeFocus = new GraphHandles((path + "part2/" + "eyeFocus").c_str(), 5, m_ex);

	// �L����
	double charaEx = m_ex * 1.1;
	m_archive = new GraphHandles((path + "�A�[�J�C�u").c_str(), 5, charaEx);
	m_aigis = new GraphHandles((path + "�A�C�M�X").c_str(), 4, charaEx, 0, false, true);
	m_assault = new GraphHandles((path + "�A�T���g03").c_str(), 4, charaEx);
	m_vermelia = new GraphHandles((path + "���F�������A").c_str(), 1, charaEx);
	m_exlucina = new GraphHandles((path + "�G�N�X���L�i").c_str(), 4, charaEx);
	m_msadi = new GraphHandles((path + "�G���T�f�B").c_str(), 4, charaEx);
	m_elnino = new GraphHandles((path + "�G���j�[�j��").c_str(), 4, charaEx);
	m_onyx = new GraphHandles((path + "�I�j���N�X").c_str(), 4, charaEx);
	m_courir = new GraphHandles((path + "�N�[���[��").c_str(), 4, charaEx, 0, false, true);
	m_cornein = new GraphHandles((path + "�R�[�l�C��").c_str(), 5, charaEx);
	m_koharu = new GraphHandles((path + "�R�n��").c_str(), 4, charaEx);
	m_siesta = new GraphHandles((path + "�V�G�X�^").c_str(), 5, charaEx);
	m_chocola = new GraphHandles((path + "�V���R��").c_str(), 4, charaEx);
	m_titius = new GraphHandles((path + "�e�B�e�B�E�X").c_str(), 5, charaEx);
	m_heart = new GraphHandles((path + "�n�[�g").c_str(), 1, charaEx);
	m_fred = new GraphHandles((path + "�t���b�h").c_str(), 1, charaEx);
	m_french = new GraphHandles((path + "�t�����`").c_str(), 1, charaEx);
	m_mascara = new GraphHandles((path + "�}�X�J�[��").c_str(), 4, charaEx);
	m_memoryA = new GraphHandles((path + "memA").c_str(), 6, m_ex);
	m_memoryB = new GraphHandles((path + "memB").c_str(), 10, m_ex);
	m_yuri = new GraphHandles((path + "���[��").c_str(), 4, charaEx);
	m_rabbi = new GraphHandles((path + "���r�\").c_str(), 4, charaEx);
	// �\�����鏇��push
	const int CHARA_TIME = 35;
	characterQueue.push(make_pair(m_koharu, CHARA_TIME));
	characterQueue.push(make_pair(m_titius, CHARA_TIME));
	characterQueue.push(make_pair(m_assault, CHARA_TIME));
	characterQueue.push(make_pair(m_msadi, CHARA_TIME));
	characterQueue.push(make_pair(m_french, CHARA_TIME));
	characterQueue.push(make_pair(m_vermelia, CHARA_TIME));
	characterQueue.push(make_pair(m_chocola, CHARA_TIME));
	characterQueue.push(make_pair(m_exlucina, CHARA_TIME));
	//characterQueue.push(make_pair(m_tank, CHARA_TIME));
	characterQueue.push(make_pair(m_yuri, CHARA_TIME));
	characterQueue.push(make_pair(m_cornein, CHARA_TIME));
	characterQueue.push(make_pair(m_aigis, CHARA_TIME));
	characterQueue.push(make_pair(m_onyx, CHARA_TIME));
	characterQueue.push(make_pair(m_courir, CHARA_TIME));
	characterQueue.push(make_pair(m_fred, CHARA_TIME));
	characterQueue.push(make_pair(m_elnino, CHARA_TIME));
	characterQueue.push(make_pair(m_mascara, CHARA_TIME));
	characterQueue.push(make_pair(m_rabbi, CHARA_TIME));
	characterQueue.push(make_pair(m_archive, CHARA_TIME));
	characterQueue.push(make_pair(m_siesta, CHARA_TIME));

	// �T�r
	m_orange = new GraphHandles((path + "sabi/" + "orange").c_str(), 3, m_ex, 0, true);
	m_duplications = new GraphHandles((path + "sabi/" + "duplication").c_str(), 16, m_ex, 0, true);
	m_heartFlame = new GraphHandles((path + "sabi/" + "heartFlame").c_str(), 1, m_ex, 0, true);
	m_rmem = new GraphHandles((path + "sabi/" + "rmem").c_str(), 8, m_ex);
	m_heartSabi = new GraphHandles((path + "sabi/" + "heart").c_str(), 2, m_ex);
	m_tvSiesta = new GraphHandles((path + "sabi/" + "�V�G�X�^").c_str(), 1, m_ex);
	m_tvHierarchy = new GraphHandles((path + "sabi/" + "�q�G�����L�[").c_str(), 1, m_ex);
	m_tvValkiria = new GraphHandles((path + "sabi/" + "���@���L���A").c_str(), 1, m_ex);
	m_tvTroy = new GraphHandles((path + "sabi/" + "�g���C").c_str(), 1, m_ex);
	m_tvHeart = new GraphHandles((path + "sabi/" + "�n�[�g").c_str(), 1, m_ex);
	m_tvShine = new GraphHandles((path + "sabi/" + "shine").c_str(), 4, m_ex);
	m_tvRshine = new GraphHandles((path + "sabi/" + "rshine").c_str(), 4, m_ex);
	m_titleFinal = new GraphHandles((path + "sabi/" + "titleFinal").c_str(), 1, m_ex, 0.0, true);

	// �ŏ��̉摜
	m_centerX = GAME_WIDE / 2;
	m_centerY = GAME_HEIGHT / 2;
	m_animation = new Animation(m_centerX, m_centerY, 120, m_titleH);

	// �T�r�p
	m_orangeAnime = new Animation(m_centerX, m_centerY, 10, m_orange);
	m_orangeAnime->setLoopFlag(true);
	m_duplicationsAnime = new Animation(m_centerX, m_centerY, 42, m_duplications);

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
	delete m_heartHide2;
	// part1
	delete m_darkHeart;
	delete m_heartEye;
	delete m_archive1;
	delete m_aigis1;
	delete m_assault1;
	delete m_vermelia1;
	delete m_exlucina1;
	delete m_msadi1;
	delete m_elnino1;
	delete m_onyx1;
	delete m_courir1;
	delete m_cornein1;
	delete m_koharu1;
	delete m_siesta1;
	delete m_hierarchy1;
	delete m_troy1;
	delete m_ancient1;
	delete m_valkiria1;
	delete m_chocola1;
	delete m_titius1;
	delete m_fred1;
	delete m_french1;
	delete m_mascara1;
	delete m_yuri1;
	delete m_rabbi1;
	for (unsigned int i = 0; i < m_partOneCharacters.size(); i++) {
		delete m_partOneCharacters[i];
	}
	// part2
	delete m_heartAndMem;
	delete m_heartCry1;
	delete m_heartCry2;
	delete m_memSad1;
	delete m_memSad2;
	delete m_eyeFocus;
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
	delete m_memoryA;
	delete m_memoryB;
	delete m_yuri;
	delete m_rabbi;

	// �T�r
	delete m_orange;
	delete m_duplications;
	delete m_orangeAnime;
	delete m_duplicationsAnime;
	delete m_heartFlame;
	delete m_rmem;
	delete m_heartSabi;
	delete m_tvSiesta;
	delete m_tvHierarchy;
	delete m_tvValkiria;
	delete m_tvTroy;
	delete m_tvHeart;
	delete m_tvShine;
	delete m_tvRshine;
	delete m_titleFinal;

	// ���y���~�߂�
	m_soundPlayer_p->stopBGM();
}

void OpMovie::play() {

	static int internalCnt = 0;
	if (m_cnt == 0) { internalCnt = 0; }

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
	else if (m_cnt >= 440 && m_cnt < 560) {
		if (m_cnt == 440) {
			m_animation->changeGraph(m_heartHide, 60);
		}
		else if (m_cnt == 460) {
			m_animation->changeGraph(m_heartHide, 6);
		}
		else if (m_cnt == 500) {
			m_animation->changeGraph(m_heartHide2, 60);
		}
	}
	else if (m_cnt < 600 && m_cnt >= 560) {
		m_animation->changeGraph(m_titleHeart, 60);
		m_animation->setX(m_animation->getX() + (int)(15 * m_ex));
	}
	else if (m_cnt < 690 && m_cnt >= 600) {
		m_animation->setX(GAME_WIDE / 2);
		if (m_cnt / 5 % 2 == 1) {
			m_animation->changeGraph(m_titleBlue, 60);
		}
		else {
			m_animation->changeGraph(m_titleOrange, 60);
		}
		if (m_cnt == 690) {
			m_animation->changeGraph(m_titleBlue, 60);
		}
	}
	else if (m_cnt < 1470 && m_cnt >= 840) { // part1
		if (m_cnt == 840) {
			m_animation->changeGraph(m_darkHeart, 1000);
		}
		if (m_cnt == 970) {
			m_animation->changeGraph(m_darkHeart, 6);
		}
		if (m_cnt == 1050) {
			m_animation->changeGraph(m_heartEye, 100);
		}
		// �h��
		if (m_cnt == 1469) {
			m_animation->setX(m_centerX);
			m_animation->setY(m_centerY);
		}
		else {
			m_animation->setX(m_animation->getX() + GetRand(2) - 1);
			m_animation->setY(m_animation->getY() + GetRand(2) - 1);
		}
	}
	else if (m_cnt < 2130 && m_cnt >= 1470) { // part2
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, WHITE, TRUE);
		if (m_cnt < 2080 && m_cnt % 4 == 1) {
			m_animation->setY(m_animation->getY() + 1);
		}
		// �n�[�g���������[
		if (m_cnt == 1470) {
			m_animation->changeGraph(m_heartAndMem, 1000);
		}
		// �n�[�g
		if (m_cnt == 1780) {
			m_animation->setY(m_centerY);
			m_animation->changeGraph(m_heartCry1, 1000);
		}
		else if (m_cnt == 1840) {
			m_animation->changeGraph(m_heartCry1, 10);
		}
		else if (m_cnt == 1900) {
			m_animation->changeGraph(m_heartCry2, 6);
		}
		// �������[
		if (m_cnt == 1950) {
			m_animation->setY(m_centerY);
			m_animation->changeGraph(m_memSad1, 1000);
		}
		else if (m_cnt == 2010) {
			m_animation->changeGraph(m_memSad1, 10);
		}
		else if (m_cnt == 2050) {
			m_animation->changeGraph(m_memSad2, 6);
		}
		// ��
		if (m_cnt == 2080) {
			m_animation->setY(m_centerY);
			m_animation->changeGraph(m_eyeFocus, 9);
		}
		// ���ɖ߂�
		if (m_cnt == 2129) {
			m_animation->setX(m_centerX);
			m_animation->setY(m_centerY);
		}
	}
	else if (m_cnt < 2780 && m_cnt >= 2130) { // �T�r�O
		if (m_cnt > 3100) {
			m_animation->setX(m_centerX);
			m_animation->setY(m_centerY);
		}
		else {
			m_animation->setX(m_animation->getX() + GetRand(2) - 1);
			m_animation->setY(m_animation->getY() + GetRand(2) - 1);
		}
		if (m_animation->getFinishFlag() && !characterQueue.empty()) {
			if (internalCnt == 0) {
				GraphHandles* next = characterQueue.front().first;
				if (next->getSize() == 1) {
					m_animation->changeGraph(next, 32);
				}
				else {
					m_animation->changeGraph(next, 8);
				}
				//m_animation->changeGraph(next, characterQueue.front().second / next->getSize());
				characterQueue.pop();
				internalCnt = 0;
			}
			else {
				internalCnt++;
			}
		}
	}
	else if (m_cnt < 3050 && m_cnt >= 2780) {
		if (m_cnt == 2780) {
			m_heart->setEx(m_ex * 10.0);
			m_animation->changeGraph(m_heart);
		}
		if (m_heart->getGraphHandle()->getEx() > m_ex) {
			m_heart->setEx(m_heart->getGraphHandle()->getEx() * 4 / 5);
		}
		if (m_heart->getGraphHandle()->getEx() < m_ex) {
			m_heart->setEx(m_ex);
		}
		if (m_cnt < 2870) {
			m_animation->setX(m_animation->getX() + GetRand(2) - 1);
			m_animation->setY(m_animation->getY() + GetRand(2) - 1);
		}
		if (m_cnt == 2870) {
			m_animation->setX(m_centerX);
			m_animation->setY(m_centerY);
			m_animation->changeGraph(m_memoryA, 6);
		}
		if (m_cnt == 2950) {
			m_animation->changeGraph(m_memoryB, 6);
		}
	}
	else if (m_cnt < 3750 && m_cnt >= 3050) { // �T�r1
		m_orangeAnime->count();
		if (m_cnt > 3100) {
			m_duplicationsAnime->count();
		}
		if (m_cnt == 3050) {
			m_animation->setX(m_centerX);
			m_animation->setY(m_centerY);
			m_heartFlame->setEx(m_ex * 11);
			m_animation->changeGraph(m_heartFlame);
		}
		if (m_heartFlame->getGraphHandle()->getEx() > m_ex) {
			m_heartFlame->setEx(m_heartFlame->getGraphHandle()->getEx() * 239 / 240);
		}
		if (m_cnt > 3610 && m_cnt <= 3640) {
			m_animation->changeGraph(m_rmem, 3);
		}
		if (m_cnt == 3665) {
			m_heartSabi->setEx(m_ex * 1.1);
			m_animation->changeGraph(m_heartSabi, 24);
		}
		if (m_cnt > 3710 && m_heartSabi->getGraphHandle()->getEx() > m_ex) {
			m_heartSabi->setEx(m_heartSabi->getGraphHandle()->getEx() * 99 / 100);
		}
		if (m_heartSabi->getGraphHandle()->getEx() < m_ex) {
			m_heartSabi->setEx(m_ex);
		}
	}
	else if (m_cnt >= 3750) { // �T�r2
		if (m_cnt == 3770) {
			m_animation->changeGraph(m_tvRshine, 3);
		}

		if (m_cnt == 3900 - 100) {
			m_animation->changeGraph(m_tvShine, 3);
		}
		if (m_cnt == 3900 - 100 + 13) {
			m_animation->changeGraph(m_tvSiesta, 30);
		}
		if (m_cnt == 3900 - 30) {
			m_animation->changeGraph(m_tvRshine, 3);
		}

		if (m_cnt == 4000 - 100) {
			m_animation->changeGraph(m_tvShine, 3);
		}
		if (m_cnt == 4000 - 100 + 13) {
			m_animation->changeGraph(m_tvTroy, 30);
		}
		if (m_cnt == 4000 - 30) {
			m_animation->changeGraph(m_tvRshine, 3);
		}

		if (m_cnt == 4100 - 100) {
			m_animation->changeGraph(m_tvShine, 3);
		}
		if (m_cnt == 4100 - 100 + 13) {
			m_animation->changeGraph(m_tvHierarchy, 30);
		}
		if (m_cnt == 4100 - 30) {
			m_animation->changeGraph(m_tvRshine, 3);
		}

		if (m_cnt == 4200 - 100) {
			m_animation->changeGraph(m_tvShine, 3);
		}
		if (m_cnt == 4200 - 100 + 13) {
			m_animation->changeGraph(m_tvValkiria, 30);
		}
		if (m_cnt == 4200 - 30) {
			m_animation->changeGraph(m_tvRshine, 3);
		}

		if (m_cnt == 4300 - 100) {
			m_animation->changeGraph(m_tvShine, 3);
		}
		if (m_cnt == 4300 - 100 + 13) {
			m_animation->changeGraph(m_tvHeart, 30);
		}
		if (m_cnt == 4350 - 20) {
			m_animation->changeGraph(m_tvRshine, 3);
		}
	}
	if (m_cnt >= 4350) {
		if (m_cnt == 4350) {
			m_animation->changeGraph(m_titleFinal, 1000);
		}
	}


	// �I��
	if (m_cnt == 4800) {
		m_finishFlag = true;
	}
}

void OpMovie::draw() {
	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, WHITE, TRUE);
	// �Ŋ��̃^�C�g���摜�����X�ɓ����x��Ⴍ����
	if (m_cnt >= 4350) {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
		int alpha = min(255, m_cnt - 4350);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	if (m_cnt < 3750 && m_cnt >= 3050) {
		m_animationDrawer->setAnimation(m_orangeAnime);
		m_animationDrawer->drawAnimation();
		if (m_cnt > 3100 && (m_cnt / 2) % 2 == 0) {
			m_animationDrawer->setAnimation(m_duplicationsAnime);
			m_animationDrawer->drawAnimation();
		}
	}
	Movie::draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (m_cnt > 980 && m_cnt < 1470) {
		for (unsigned int i = 0; i < m_partOneCharacters.size(); i++) {
			m_partOneCharacters[i]->play();
			m_partOneCharacters[i]->draw();
		}
	}
	if (m_cnt > 2950 && m_cnt < 3050 && m_animation->getFinishFlag()) {
		DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, BLACK, TRUE);
	}
	drawFlame();

	// Z�L�[�������ŃX�L�b�v�̕\��
	drawSkip(m_skipCnt, m_exX, m_exY, m_textHandle);
}


OpMovieMp4::OpMovieMp4(SoundPlayer* soundPlayer_p) :
	Movie(soundPlayer_p)
{
	m_mp4 = LoadGraph("picture/movie/DuplicationHeartOp.mp4");
	double rate = 10 * m_soundPlayer_p->getVolume() / 100.0;
	rate = log10(rate);
	int volume = (int)(rate * 3000);
	SetMovieVolumeToGraph(7000 + volume, m_mp4);
	PlayMovieToGraph(m_mp4);
}
OpMovieMp4::~OpMovieMp4() {
	DeleteGraph(m_mp4);
}

// �Đ�
void OpMovieMp4::play() {
	Movie::play();
	// �I��
	if (GetMovieStateToGraph(m_mp4) != 1) {
		m_finishFlag = true;
	}
}

// �`��
void OpMovieMp4::draw() {

	DrawRotaGraph(GAME_WIDE / 2, GAME_HEIGHT / 2, m_ex, 0, m_mp4, TRUE);

	drawFlame();

	// Z�L�[�������ŃX�L�b�v�̕\��
	drawSkip(m_skipCnt, m_exX, m_exY, m_textHandle);

	// ��ʂ̂�����h�~
	WaitTimer(15);
}