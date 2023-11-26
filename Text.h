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
* イベント中に挿入される画像
*/
class EventAnime {

private:

	// アニメのスピード
	int m_speed = 5;

	// 挿絵
	GraphHandles* m_handles;
	Animation* m_animation;

	// 画像の明るさ
	int m_bright;

	// 暗くなっていく方向 明るくなるならfalse
	bool m_toDark;

	bool m_finishFlag;

public:

	EventAnime(const char* filePath, int sum, int speed = -1);

	~EventAnime();

	// ゲッタ
	inline const bool getToDark() const { return m_toDark; }
	inline const int getBright() const { return m_bright; }
	inline const Animation* getAnime() const { return m_animation; }

	// セッタ
	inline void setToDark(bool toDark) { m_toDark = toDark; }
	inline void setBright(int bright) { m_bright = bright; }
	inline void setFinishFlag(bool flag) { m_finishFlag = flag; }

	// アニメの再生が終わったか
	bool getFinishAnime() const;

	// falseの間は操作不可
	void play();

};


/*
* 会話イベント
*/
class Conversation {
private:

	// 終了時、少しだけ待機時間
	const int FINISH_COUNT = 30;
	int m_finishCnt;

	// イベント終了したか
	bool m_finishFlag;

	// 文字表示の速さ 1が最速
	const unsigned int TEXT_SPEED = 5;
	unsigned int m_textSpeed;

	// テキストを飛ばせるようになるまでの時間
	const unsigned int MOVE_FINAL_ABLE = 10;

	// ファイルポインタ
	int m_fp;

	// 世界
	World* m_world_p;

	// 世界のサウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// アニメイベント
	EventAnime* m_eventAnime;

	// 発言者の名前
	std::string m_speakerName;

	// 発言者の顔画像がない
	bool m_noFace;

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

	// BGMを変更しても戻せるよう
	std::string m_originalBgmPath;

public:
	Conversation(int textNum, World* world, SoundPlayer* soundPlayer);
	~Conversation();

	// ゲッタ
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

	// 今アニメ再生中か
	bool animePlayNow() const { return m_eventAnime == nullptr ? false : !m_eventAnime->getFinishAnime(); }

	// 会話を行う
	bool play();

private:
	void loadNextBlock();
	void setNextText(const int size, char* buff);
	void setSpeakerGraph(const char* faceName);
};


#endif