#ifndef STORY_H_INCLUDED
#define STORY_H_INCLUDED

#include <vector>

class Event;
class World;
class SoundPlayer;
class CharacterLoader;
class ObjectLoader;

// ストーリ－
class Story {
private:

	// 対象の世界
	World* m_world_p;

	// ストーリー番号
	int m_storyNum;

	// 進行中のイベント
	Event* m_nowEvent;
	
	// クリア必須イベント
	std::vector<Event*> m_mustEvent;
	
	// クリア任意イベント
	std::vector<Event*> m_subEvent;

	// キャラクターのデータ
	CharacterLoader* m_characterLoader;

	// オブジェクトのデータ
	ObjectLoader* m_objectLoader;

public:
	Story(int storyNum, World* world, SoundPlayer* soundPlayer);
	~Story();

	void debug(int x, int y, int color);

	bool play();

	// ハートのスキル発動が可能かどうか
	bool skillAble();

	// ゲッタ
	inline int getStoryNum() const { return m_storyNum; }
	inline CharacterLoader* getCharacterLoader() const { return m_characterLoader; }
	inline ObjectLoader* getObjectLoader() const { return m_objectLoader; }
	inline const World* getWorld() const { return m_world_p; }

	// セッタ
	void setWorld(World* world);
};


#endif