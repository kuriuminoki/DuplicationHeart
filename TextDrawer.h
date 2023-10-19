#ifndef TEXT_DRAWER_H_INCLUDED
#define TEXT_DRAWER_H_INCLUDED


class Conversation;


class ConversationDrawer {
private:
	
	// 会話
	const Conversation* m_conversation;

	// テキストやフォントのサイズの倍率
	double m_ex;
	
	// フォント（テキスト）
	int m_textHandle;
	const int TEXT_SIZE = 50;

	// 一行に表示する文字数
	const int MAX_TEXT_LEN = 48;

	// フォント（名前）
	int m_nameHandle;
	const int NAME_SIZE = 70;

	// フキダシ画像
	int m_frameHandle;

public:
	ConversationDrawer(Conversation* conversation);
	~ConversationDrawer();

	void setConversation(const Conversation* conversation) { m_conversation = conversation; }

	void draw();
};


#endif