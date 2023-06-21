#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<queue>

class CharacterController;
class Character;
class Object;

class World {
private:
	// 世界に存在するキャラクター
	std::queue<Character*> m_characters;

	// 戦闘のためにキャラを動かすコントローラ
	std::queue<CharacterController*> m_characterControllers;

	// 壁や床のオブジェクト
	std::queue<Object*> m_stageObjects;

	// 攻撃のあたり判定のオブジェクト
	std::queue<Object*> m_attackObjects;

	// いま世界のどのエリアにいるか（メモリ節約のためプレイヤーの付近のみを読み込む）
	int m_areaNum;

public:
	World(int areaNum);
	~World();

	//デバッグ
	void debug(int x, int y, int color);

	// キャラに戦わせる
	void battle();

	// キャラクターの動き
	void controlCharacter();

	// オブジェクトの動き
	void controlObject();

	// 壁や床<->攻撃の当たり判定
	void atariStageAndAttack();

	// キャラに会話させる
	void talk();

};

#endif