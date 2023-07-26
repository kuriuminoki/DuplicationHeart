#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<vector>

class CharacterController;
class CharacterAction;
class Character;
class Object;
class Camera;
class Animation;
class SoundPlayer;

class World {
private:
	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

	// 描画用のカメラ
	Camera* m_camera;

	// カメラの移動速度の初期値
	const int CAMERA_SPEED_DEFAULT = 5;

	// カメラで見ているキャラのID
	int m_focusId;

	// プレイヤー（ハート）のID
	int m_playerId;

	// 世界に存在するキャラクター
	std::vector<Character*> m_characters;

	// 戦闘のためにキャラを動かすコントローラ
	std::vector<CharacterController*> m_characterControllers;

	// 壁や床のオブジェクト
	std::vector<Object*> m_stageObjects;

	// 攻撃のあたり判定のオブジェクト
	std::vector<Object*> m_attackObjects;

	// エフェクト等のアニメーション
	std::vector<Animation*> m_animations;

	// いま世界のどのエリアにいるか（メモリ節約のためプレイヤーの付近のみを読み込む）
	int m_areaNum;

public:
	World(int areaNum, SoundPlayer* soundPlayer);
	~World();

	// ゲッタ
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<const CharacterAction*> getActions() const;
	std::vector<const Object*> getObjects() const;
	std::vector<const Animation*> getAnimations() const;

	//デバッグ
	void debug(int x, int y, int color) const;

	// キャラに戦わせる
	void battle();

	// キャラに会話させる
	void talk();

private:
	// キャラクターとオブジェクトの当たり判定
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects);

	// アニメーションの更新
	void updateAnimation();

	// ＨＰ０のキャラ削除
	void cleanCharacterController();

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