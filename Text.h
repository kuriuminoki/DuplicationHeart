#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include <string>
#include <vector>


class SoundPlayer;
class World;
class GraphHandle;
class GraphHandles;


class Conversation {
private:

	// �C�x���g�I��������
	bool m_finishFlag;

	// �����\���̑��� 1���ő�
	const unsigned int TEXT_SPEED = 5;

	// �e�L�X�g���΂���悤�ɂȂ�܂ł̎���
	const unsigned int MOVE_FINAL_ABLE = 30;

	// �t�@�C���|�C���^
	int m_fp;

	World* m_world_p;

	SoundPlayer* m_soundPlayer_p;

	// �����҂̖��O
	std::string m_speakerName;

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

public:
	Conversation(int textNum, World* world, SoundPlayer* soundPlayer);
	~Conversation();

	// �Q�b�^
	std::string getText() const;
	inline std::string getFullText() const { return m_text; }
	int getTextSize() const;
	GraphHandle* getGraph();
	inline 	std::string getSpeakerName() const { return m_speakerName; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getTextNow() const { return m_textNow; }
	inline int getCnt() const { return m_cnt; }

	// ��b���s��
	bool play();

private:
	void setNextText();
	void setSpeakerGraph(const char* faceName);
};


#endif