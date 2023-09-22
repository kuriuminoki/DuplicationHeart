#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class GraphHandle;
class GraphHandles;
class SoundPlayer;

class Animation {
private:
	// �摜�n���h��
	GraphHandles* m_handles;

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

	// �Z�b�^
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
	// �I��������true
	bool m_finishFlag;

	// �J�n����̌o�ߎ���
	int m_cnt;

	// �摜�����ē�����
	Animation* m_animation;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

public:
	Movie(SoundPlayer* soundPlayer_p);
	~Movie();

	// �Q�b�^
	bool getFinishFlag() const { return m_finishFlag; }
	Animation* getAnimation() const { return m_animation; }
	inline int getCnt() const { return m_cnt; }

	// �Đ�
	virtual void play() = 0;
};


// �I�[�v�j���O
class OpMovie:
	public Movie
{
private:

	// �摜
	GraphHandles* m_title;
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

public:
	OpMovie(SoundPlayer* soundPlayer_p);
	~OpMovie();

	// �Đ�
	void play();
};


#endif