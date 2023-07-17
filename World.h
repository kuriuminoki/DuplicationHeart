#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<queue>

class CharacterController;
class CharacterAction;
class Character;
class Object;
class Camera;

class World {
private:
	// 描画用のカメラ
	Camera* m_camera;

	// カメラの移動速度の初期値
	const int CAMERA_SPEED_DEFAULT = 5;

	// カメラで見ているキャラのID
	int m_focusId;

	// プレイヤー（ハート）のID
	int m_playerId;

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

	// ゲッタとセッタ
	inline const Camera* getCamera() const { return m_camera; }
	std::queue<const CharacterAction*> getActions() const;
	std::queue<const Object*> getObjects() const;

	//デバッグ
	void debug(int x, int y, int color);

	// キャラに戦わせる
	void battle();

	// キャラに会話させる
	void talk();

private:

	// ＨＰ０のキャラ削除
	void cleanCharacter();

	// キャラの更新（攻撃対象の変更）
	void updateCharacter();

	// カメラの更新
	void updateCamera();

	// キャラクターの動き
	void controlCharacter();

	// オブジェクトの動き
	void controlObject();

	// 壁や床<->攻撃の当たり判定
	void atariStageAndAttack();

};

#endif