#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <queue>
#include <string>
#include <vector>

class GraphHandle;
class GraphHandles;
class SoundPlayer;
class AnimationDrawer;

class Animation {
private:
	// �摜�n���h��
	GraphHandles* m_handles_p;

	// ���W
	int m_x, m_y;
	
	// �J�E���g
	int m_cnt;

	// �I���܂ł̃J�E���g
	int m_finishCnt;

	// �摜�P���̕\������
	int m_flameCnt;

	// �A�j���[�V�������I��
	bool m_finishFlag;

	// ���[�v�Đ�
	bool m_loopFlag;

public:
	Animation(int x, int y, int flameCnt, GraphHandles* graphHandles);

	Animation* createCopy();

	// �Q�b�^
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getCnt() const { return m_cnt; }

	// �Z�b�^
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setCnt(int cnt) { m_cnt = cnt; }
	inline void setFinishCnt(int finishCnt) { m_finishCnt = finishCnt; }
	inline void setFinishFlag(bool finishFlag) { m_finishFlag = finishFlag; }
	inline void setLoopFlag(bool loopFlag) { m_loopFlag = loopFlag; }

	// ������
	void init();

	// �A�j���[�V�����̐؂�ւ�
	void changeGraph(GraphHandles* nextGraph, int flameCnt = -1);

	// �J�E���g
	void count();

	// �`��p
	GraphHandle* getHandle() const;
};


// ����̊��N���X
class Movie {
protected:

	// �𑜓x�̕ύX�ɑΉ�
	double m_ex;

	int m_flameWide, m_flameHeight;

	// �I��������true
	bool m_finishFlag;

	// �J�n����̌o�ߎ���
	int m_cnt;

	// �摜�����ē��������C���摜
	int m_centerX, m_centerY;
	Animation* m_animation;
	AnimationDrawer* m_animationDrawer;

	// �T�u�摜 cnt��0�ɂȂ������̂�pop���Ă���
	std::queue<Animation*> m_subAnimation;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// BGM�̃p�X
	std::string m_bgmPath;

	// ���Ƃ��Ɨ����Ă���BGM
	std::string m_originalBgmPath;

public:
	Movie(SoundPlayer* soundPlayer_p);
	virtual ~Movie();

	// �Q�b�^
	bool getFinishFlag() const { return m_finishFlag; }
	Animation* getAnimation() const { return m_animation; }
	std::queue<Animation*> getSubAnimation() const { return m_subAnimation; }
	inline int getCnt() const { return m_cnt; }

	// �Đ�
	virtual void play();

	// �`��
	virtual void draw();

protected:
	void drawFlame();
};


// �I�[�v�j���O�p
class PartOneCharacter {
private:

	// �摜
	GraphHandle* m_character;

	// ���W
	int m_initX, m_initY;
	int m_x, m_y;

	// ���x
	int m_vx, m_vy;

	double m_ex;

public:
	PartOneCharacter(GraphHandle* character, int initX, int initY, int vx, double ex);

	void play();
	void draw();
};


// �I�[�v�j���O
class OpMovie:
	public Movie
{
private:

	// �摜
	// �^�C�g��
	GraphHandles* m_titleH;
	GraphHandles* m_title;
	GraphHandles* m_titleChara;
	GraphHandles* m_titleBlue;
	GraphHandles* m_titleOrange;
	GraphHandles* m_titleHeart;
	GraphHandles* m_heartHide;
	GraphHandles* m_heartHide2;

	// part1
	GraphHandles* m_darkHeart;
	GraphHandles* m_heartEye;
	std::vector<PartOneCharacter*> m_partOneCharacters;
	GraphHandle* m_archive1;
	GraphHandle* m_aigis1;
	GraphHandle* m_assault1;
	GraphHandle* m_vermelia1;
	GraphHandle* m_exlucina1;
	GraphHandle* m_msadi1;
	GraphHandle* m_elnino1;
	GraphHandle* m_onyx1;
	GraphHandle* m_courir1;
	GraphHandle* m_cornein1;
	GraphHandle* m_koharu1;
	GraphHandle* m_siesta1;
	GraphHandle* m_hierarchy1;
	GraphHandle* m_troy1;
	GraphHandle* m_ancient1;
	GraphHandle* m_valkiria1;
	GraphHandle* m_chocola1;
	GraphHandle* m_titius1;
	GraphHandle* m_fred1;
	GraphHandle* m_french1;
	GraphHandle* m_mascara1;
	GraphHandle* m_yuri1;
	GraphHandle* m_rabbi1;

	// part2
	GraphHandles* m_heartAndMem;
	GraphHandles* m_heartCry1;
	GraphHandles* m_heartCry2;
	GraphHandles* m_memSad1;
	GraphHandles* m_memSad2;
	GraphHandles* m_eyeFocus;

	// �L����
	GraphHandles* m_archive;
	GraphHandles* m_aigis;
	GraphHandles* m_assault;
	GraphHandles* m_vermelia;
	GraphHandles* m_exlucina;
	GraphHandles* m_msadi;
	GraphHandles* m_elnino;
	GraphHandles* m_onyx;
	GraphHandles* m_courir;
	GraphHandles* m_cornein;
	GraphHandles* m_koharu;
	GraphHandles* m_siesta;
	GraphHandles* m_chocola;
	GraphHandles* m_titius;
	GraphHandles* m_heart;
	GraphHandles* m_fred;
	GraphHandles* m_french;
	GraphHandles* m_mascara;
	GraphHandles* m_memoryA;
	GraphHandles* m_memoryB;
	GraphHandles* m_yuri;
	GraphHandles* m_rabbi;
	GraphHandles* m_tank;

	// �T�r
	GraphHandles* m_orange;
	GraphHandles* m_duplications;
	Animation* m_orangeAnime;
	Animation* m_duplicationsAnime;
	GraphHandles* m_heartFlame;
	GraphHandles* m_rmem;
	GraphHandles* m_heartSabi;
	GraphHandles* m_tvSiesta;
	GraphHandles* m_tvHierarchy;
	GraphHandles* m_tvValkiria;
	GraphHandles* m_tvTroy;
	GraphHandles* m_tvHeart;
	GraphHandles* m_tvShine;
	GraphHandles* m_tvRshine;
	GraphHandles* m_titleFinal;

	// �L���������ɕ\������p (graph, cntSum)
	std::queue<std::pair<GraphHandles*, int> > characterQueue;

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// �Đ�
	void play();

	// �`��
	void draw();

private:
	void pushPartOneCharacter(int index, bool front, GraphHandle* character);
};


#endif