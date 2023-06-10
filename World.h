#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<queue>

class CharacterController;
class Character;

class World {
private:
	// 世界に存在するキャラクター
	std::queue<Character*> m_characters;

	// 戦闘のためにキャラを動かすコントローラ
	std::queue<CharacterController*> m_characterControllers;

	// いま世界のどのエリアにいるか（メモリ節約のためプレイヤーの付近のみを読み込む）
	int m_areaNum;

public:
	World(int areaNum);
	~World();

	//デバッグ
	void debug(int x, int y, int color);

	// キャラに戦わせる
	void battle();

	// キャラに会話させる
	void talk();
};

#endif