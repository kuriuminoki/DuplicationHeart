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

	// イベント終了したか
	bool m_finishFlag;

	// 文字表示の速さ 1が最速
	const unsigned int TEXT_SPEED = 5;

	// テキストを飛ばせるようになるまでの時間
	const unsigned int MOVE_FINAL_ABLE = 30;

	// ファイルポインタ
	int m_fp;

	World* m_world_p;

	SoundPlayer* m_soundPlayer_p;

	// 発言者の名前
	std::string m_speakerName;

	// 発言者の顔画像
	GraphHandles* m_speakerGraph;

	// 発言
	std::string m_text;

	// 今何文字目まで発言したか
	unsigned int m_textNow;

	// カウント
	unsigned int m_cnt;

	// 文字表示効果音
	int m_displaySound;

	// 決定効果音
	int m_nextSound;

public:
	Conversation(int textNum, World* world, SoundPlayer* soundPlayer);
	~Conversation();

	// ゲッタ
	std::string getText() const;
	inline std::string getFullText() const { return m_text; }
	int getTextSize() const;
	GraphHandle* getGraph();
	inline 	std::string getSpeakerName() const { return m_speakerName; }
	inline bool getFinishFlag() const { return m_finishFlag; }
	inline int getTextNow() const { return m_textNow; }
	inline int getCnt() const { return m_cnt; }

	// 会話を行う
	bool play();

private:
	void setNextText();
	void setSpeakerGraph(const char* faceName);
};


#endif