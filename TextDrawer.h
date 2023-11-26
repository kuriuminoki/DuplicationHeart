#ifndef TEXT_DRAWER_H_INCLUDED
#define TEXT_DRAWER_H_INCLUDED


#include <string>

class Conversation;
class AnimationDrawer;


class ConversationDrawer {
private:
	
	// ��b
	const Conversation* m_conversation;

	// �A�j���C�x���g�p
	AnimationDrawer* m_animationDrawer;

	// �e�L�X�g��t�H���g�̃T�C�Y�̔{��
	double m_exX;
	double m_exY;
	
	// �t�H���g�i�e�L�X�g�j
	int m_textHandle;
	const int TEXT_SIZE = 50;

	// ��s�ɕ\�����镶����
	const int MAX_TEXT_LEN = 48;

	// �t�H���g�i���O�j
	int m_nameHandle;
	const int NAME_SIZE = 70;

	// �t�L�_�V�摜
	int m_frameHandle;

public:
	ConversationDrawer(Conversation* conversation);
	~ConversationDrawer();

	void setConversation(const Conversation* conversation) { m_conversation = conversation; }

	void draw();

	void drawText(int x, int y, int height, std::string text, int color, int font);
};


#endif