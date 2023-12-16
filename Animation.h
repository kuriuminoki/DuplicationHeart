#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <queue>
#include <string>

class GraphHandle;
class GraphHandles;
class SoundPlayer;

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
	inline void setFinishFlag(int finishFlag) { m_finishFlag = finishFlag; }

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

	// �I��������true
	bool m_finishFlag;

	// �J�n����̌o�ߎ���
	int m_cnt;

	// �摜�����ē��������C���摜
	Animation* m_animation;

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
	GraphHandles* m_yuri;
	GraphHandles* m_rabbi;
	GraphHandles* m_tank;

	// �L���������ɕ\������p (graph, cntSum)
	std::queue<std::pair<GraphHandles*, int> > characterQueue;

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// �Đ�
	void play();
};


#endif