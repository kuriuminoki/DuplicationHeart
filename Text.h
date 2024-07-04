#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <string>
#include <vector>


class Animation;
class SoundPlayer;
class World;
class GraphHandle;
class GraphHandles;
class Button;


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

	bool m_heavyFlag;
	bool m_lightFlag;
	int m_alpha;

public:

	EventAnime(const char* filePath, int sum, int speed = -1);

	~EventAnime();

	// �Q�b�^
	inline const bool getToDark() const { return m_toDark; }
	inline const int getBright() const { return m_bright; }
	inline const Animation* getAnime() const { return m_animation; }
	inline const int getAlpha() const { return m_alpha; }

	// �Z�b�^
	inline void setToDark(bool toDark) { m_toDark = toDark; }
	inline void setBright(int bright) { m_bright = bright; }
	inline void setFinishFlag(bool flag) { m_finishFlag = flag; }
	inline void setHeavyTrue() { m_heavyFlag = true; m_alpha = 0; }
	inline void setLightTrue() { m_lightFlag = true; m_alpha = 255; }

	// �A�j���C�x���g���I�������
	bool getFinishAnimeEvent() const;

	// false�̊Ԃ͑���s��
	void play();

};


/*
* �t�L�_�V�̃A�N�V����
*/
class TextAction {
private:

	// �W�����v��
	bool m_jumpFlag;

	// ���W�̂���
	int m_dx, m_dy;

	// ���x
	int m_vy;

	// �U������c�莞��
	int m_quakeCnt;

	// �U���ɂ�邸��
	int m_quakeDx, m_quakeDy;

public:

	TextAction();

	// �Q�b�^
	int getDx() const { return m_dx + m_quakeDx; }
	int getDy() const { return m_dy + m_quakeDy; }

	// �Z�b�^
	void setVy(int vy) { m_vy = vy; m_jumpFlag = true; }
	void setQuakeCnt(int cnt) { m_quakeCnt = cnt; }

	void init();

	void play();

};


/*
* ��b�C�x���g
*/
class Conversation {
private:

	// �I�����A���������ҋ@����
	const int FINISH_COUNT = 30;
	// 0 -> FINISH_COUNT�Ŕ����I��
	int m_finishCnt;
	// FINISH_CINT -> 0�Ŕ����J�n
	int m_startCnt;

	// Z�L�[�̒���������
	int m_skipCnt;

	// �C�x���g�I��������
	bool m_finishFlag;

	// �����\���̑��� 1���ő�
	const unsigned int TEXT_SPEED = 5;
	unsigned int m_textSpeed;

	// ���̃e�L�X�g�֍s����悤�ɂȂ�܂ł̎���
	const unsigned int NEXT_TEXT_ABLE = 30;

	// �e�L�X�g���΂���悤�ɂȂ�܂ł̎���
	const unsigned int MOVE_FINAL_ABLE = 10;

	// �t�@�C���|�C���^
	int m_fp;

	// ��������p
	bool m_if; // true�Ȃ�if�u���b�N�������s��
	std::vector<std::string> m_marks; // �}�[�N
	int m_font;
	Button* m_yesButton;
	Button* m_noButton;
	bool m_selectFlag; // ���I��҂�

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
	GraphHandles* m_speakerGraph_p;

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

	// �N���b�N�G�t�F�N�g���̃A�j���[�V���� ���̃N���X���f���[�g����
	GraphHandles* m_clickGraph;
	std::vector<Animation*> m_animations;

	// �����I�����̈�摜
	GraphHandle* m_textFinishGraph;

	// �t�L�_�V�̃A�N�V����
	TextAction m_textAction;

	// ���ʉ�
	int m_sound;

	// ��摜�̕\�����@
	enum FaceDrawMode {
		NORMAL,	// ���ʂɃA�j�����J��Ԃ�
		FREEZE,	// �A�j���𓮂����Ȃ�
		ONCE	// ��x�����Đ�
	};
	FaceDrawMode m_faceDrawMode;

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
	inline int getSkipCnt() const { return m_skipCnt; }
	inline int getStartCnt() const { return m_startCnt; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getTextNow() const { return m_textNow; }
	inline int getCnt() const { return m_cnt; }
	inline const Animation* getAnime() const { 
		if (m_eventAnime == nullptr) { return nullptr; }
		return m_eventAnime->getAnime();
	}
	inline int getAnimeBright() const { return m_eventAnime->getBright(); }
	inline int getAnimeAlpha() const { return m_eventAnime->getAlpha(); }
	inline const std::vector<Animation*> getAnimations() const { return m_animations; }
	inline const GraphHandle* getTextFinishGraph() const { return m_textFinishGraph; }
	inline const EventAnime* getEventAnime() const { return m_eventAnime; }
	inline const TextAction getTextAction() const { return m_textAction; }
	inline const Button* getYesButton() const { return m_selectFlag ? m_yesButton : nullptr; }
	inline const Button* getNoButton() const { return m_selectFlag ? m_noButton : nullptr; }

	// �Z�b�^
	void setWorld(World* world);

	// ���A�j���Đ�����
	bool animePlayNow() const { return m_eventAnime == nullptr ? false : !m_eventAnime->getFinishAnimeEvent(); }

	// ��b���s��
	bool play();

	// �Ō�܂ŃZ���t�\��������
	bool finishText() const;

	// ���̃Z���t�ֈڍs�ł��邩
	bool nextTextAble() const { return m_cnt > NEXT_TEXT_ABLE; }

private:
	void loadNextBlock();
	void setNextText(const int size, char* buff);
	void setSpeakerGraph(const char* faceName);
};


#endif