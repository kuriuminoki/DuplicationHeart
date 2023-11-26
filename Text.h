#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <string>
#include <vector>


class Animation;
class SoundPlayer;
class World;
class GraphHandle;
class GraphHandles;


/*
* �C�x���g���ɑ}�������摜
*/
class EventAnime {

private:

	// �A�j���̃X�s�[�h
	int m_speed = 5;

	// �}�G
	GraphHandles* m_handles;
	Animation* m_animation;

	// �摜�̖��邳
	int m_bright;

	// �Â��Ȃ��Ă������� ���邭�Ȃ�Ȃ�false
	bool m_toDark;

	bool m_finishFlag;

public:

	EventAnime(const char* filePath, int sum, int speed = -1);

	~EventAnime();

	// �Q�b�^
	inline const bool getToDark() const { return m_toDark; }
	inline const int getBright() const { return m_bright; }
	inline const Animation* getAnime() const { return m_animation; }

	// �Z�b�^
	inline void setToDark(bool toDark) { m_toDark = toDark; }
	inline void setBright(int bright) { m_bright = bright; }
	inline void setFinishFlag(bool flag) { m_finishFlag = flag; }

	// �A�j���̍Đ����I�������
	bool getFinishAnime() const;

	// false�̊Ԃ͑���s��
	void play();

};


/*
* ��b�C�x���g
*/
class Conversation {
private:

	// �I�����A���������ҋ@����
	const int FINISH_COUNT = 30;
	int m_finishCnt;

	// �C�x���g�I��������
	bool m_finishFlag;

	// �����\���̑��� 1���ő�
	const unsigned int TEXT_SPEED = 5;
	unsigned int m_textSpeed;

	// �e�L�X�g���΂���悤�ɂȂ�܂ł̎���
	const unsigned int MOVE_FINAL_ABLE = 10;

	// �t�@�C���|�C���^
	int m_fp;

	// ���E
	World* m_world_p;

	// ���E�̃T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// �A�j���C�x���g
	EventAnime* m_eventAnime;

	// �����҂̖��O
	std::string m_speakerName;

	// �����҂̊�摜���Ȃ�
	bool m_noFace;

	// �����҂̊�摜
	GraphHandles* m_speakerGraph;

	// ����
	std::string m_text;

	// ���������ڂ܂Ŕ���������
	unsigned int m_textNow;

	// �J�E���g
	unsigned int m_cnt;

	// �����\�����ʉ�
	int m_displaySound;

	// ������ʉ�
	int m_nextSound;

	// BGM��ύX���Ă��߂���悤
	std::string m_originalBgmPath;

public:
	Conversation(int textNum, World* world, SoundPlayer* soundPlayer);
	~Conversation();

	// �Q�b�^
	std::string getText() const;
	inline std::string getFullText() const { return m_text; }
	int getTextSize() const;
	GraphHandle* getGraph() const;
	inline bool getNoFace() const { return m_noFace; }
	inline 	std::string getSpeakerName() const { return m_speakerName; }
	inline int getFinishCnt() const { return m_finishCnt; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getTextNow() const { return m_textNow; }
	inline int getCnt() const { return m_cnt; }
	inline const Animation* getAnime() const { 
		if (m_eventAnime == nullptr) { return nullptr; }
		return m_eventAnime->getAnime();
	}
	inline int getAnimeBright() const { return m_eventAnime->getBright(); }

	// ���A�j���Đ�����
	bool animePlayNow() const { return m_eventAnime == nullptr ? false : !m_eventAnime->getFinishAnime(); }

	// ��b���s��
	bool play();

private:
	void loadNextBlock();
	void setNextText(const int size, char* buff);
	void setSpeakerGraph(const char* faceName);
};


#endif