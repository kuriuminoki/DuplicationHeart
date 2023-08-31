#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE


#include <vector>
#include <string>


class CharacterController;
class CharacterAction;
class Character;
class Object;
class DoorObject;
class Camera;
class Animation;
class SoundPlayer;
class Conversation;
class Brain;
class ObjectLoader;
class CharacterData;


class World {
private:
	// 複製ならtrue 背景をデリートしないため
	bool m_duplicationFlag;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// 会話イベント EventElementクラスからもらう
	Conversation* m_conversation_p;

	// スキル発動中はエリア間の移動できない
	bool m_skillFlag;

	// 画面の明るさ
	int m_brightValue;

	// カメラで見ているキャラのID
	int m_focusId;

	// プレイヤー（ハート）のID
	int m_playerId;

	// いま世界のどのエリアにいるか（メモリ節約のためプレイヤーの付近のみを読み込む）
	int m_areaNum;

	// 描画用のカメラ Worldがデリートする
	Camera* m_camera;

	// 世界に存在するキャラクター Worldがデリートする
	std::vector<Character*> m_characters;

	// 戦闘のためにキャラを動かすコントローラ Worldがデリートする
	std::vector<CharacterController*> m_characterControllers;

	// 壁や床のオブジェクト Worldがデリートする
	std::vector<Object*> m_stageObjects;

	// エリア間をつなげる扉 Worldがデリートする
	std::vector<Object*> m_doorObjects;

	// 攻撃のあたり判定のオブジェクト Worldがデリートする
	std::vector<Object*> m_attackObjects;

	// エフェクト等のアニメーション Worldがデリートする
	std::vector<Animation*> m_animations;

	// 背景
	int m_backGroundGraph;
	int m_backGroundColor;

public:
	World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer);
	~World();

	World(const World* original);

	// ゲッタ
	inline int getFocusId() const { return m_focusId; }
	inline int getPlayerId() const { return m_playerId; }
	inline int getBrightValue() const { return m_brightValue; }
	inline int getAreaNum() const { return m_areaNum; }
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<CharacterController*> getCharacterControllers() const;
	std::vector<const CharacterAction*> getActions() const;
	std::vector<Character*> getCharacters() const;
	std::vector<Object*> getStageObjects() const;
	std::vector<Object*> getDoorObjects() const;
	std::vector<Object*> getAttackObjects() const;
	std::vector<Animation*> getAnimations() const;
	std::vector<const Object*> getFrontObjects() const;
	std::vector<const Object*> getBackObjects() const;
	std::vector<const Animation*> getConstAnimations() const;
	inline const int getBackGroundGraph() const { return m_backGroundGraph; }
	inline const int getBackGroundColor() const { return m_backGroundColor; }
	inline const Conversation* getConversation() const { return m_conversation_p; }
	inline SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }

	// セッタ
	inline void setSkillFlag(bool skillFlag) { m_skillFlag = skillFlag; }
	inline void setFocusId(int id) { m_focusId = id; }

	// ストーリーやイベントによる追加オブジェクト
	void addObject(ObjectLoader* objectLoader);

	//デバッグ
	void debug(int x, int y, int color) const;

	// キャラに戦わせる
	void battle();

	// キャラに会話させる
	void talk();

	// キャラの状態を変更する いないなら作成する
	void asignedCharacterData(const char* name, CharacterData& data);

	// キャラの状態を教える
	void asignCharacterData(const char* name, CharacterData& data, int fromAreaNum);

	/*
	* イベント用
	*/
	Character* getCharacterWithName(std::string characterName) const;
	CharacterController* getControllerWithName(std::string characterName) const;
	Character* getCharacterWithId(int id) const;
	void setBrainWithId(int id, Brain* brain);
	inline void setConversation(Conversation* conversation){ m_conversation_p = conversation; }
	void pushCharacter(Character* character, CharacterController* controller);
	void popCharacter(int id);
	void createRecorder();
	void initRecorder();
	void eraseRecorder();

private:
	// キャラクターとオブジェクトの当たり判定
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects);

	// キャラクターと扉の当たり判定
	void atariCharacterAndDoor(CharacterController* controller, std::vector<Object*>& objects);

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

	// 攻撃<->攻撃の当たり判定
	void atariAttackAndAttack();

	// キャラのセーブデータを自身に反映させる
	void asignedCharacter(Character* character, CharacterData& data);
	CharacterController* createControllerWithData(const Character* character, CharacterData& data);
};

#endif