#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE


#include <vector>
#include <string>


class Animation;
class Brain;
class Camera;
class Character;
class CharacterAction;
class CharacterController;
class CharacterData;
class CharacterLoader;
class Conversation;
class DoorData;
class DoorObject;
class GraphHandles;
class Item;
class Movie;
class Object;
class ObjectLoader;
class SoundPlayer;


class World {
private:

	// 解像度の倍率
	double m_exX, m_exY;

	// 複製ならtrue 背景をデリートしないため
	bool m_duplicationFlag;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer_p;

	// 会話イベント EventElementクラスからもらう
	Conversation* m_conversation_p;

	Conversation* m_objectConversation;

	// ムービー EventElementクラスからもらう
	Movie* m_movie_p;

	// スキル発動中はエリア間の移動できない
	bool m_skillFlag;

	// 画面の明るさ
	int m_brightValue;

	// カメラで見ているキャラのID
	int m_focusId;

	// プレイヤー（ハート）のID
	int m_playerId;

	// いま世界のどのエリアにいるか
	int m_areaNum;
	int m_nextAreaNum;

	// エリア移動時にBGMをいったん止めるならtrue
	bool m_resetBgmFlag;

	// 描画するかどうか(動画とテキストは除く)
	bool m_blindFlag;

	// 時間帯 0は昼、1は夕方、2は夜
	int m_date;

	// エリア移動が禁止されているならtrue
	bool m_areaLock;

	// 描画用のカメラ Worldがデリートする
	Camera* m_camera;

	// カメラの最大・最小倍率
	double m_cameraMaxEx = 1.5;
	double m_cameraMinEx = 0.5;

	// 世界に存在するキャラクター Worldがデリートする
	std::vector<Character*> m_characters;

	// プレイヤー 毎回for文でID検索しない用
	Character* m_player_p;

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

	// エリアに落ちているアイテム
	std::vector<Item*> m_itemVector;

	// キャラがやられた時のエフェクト画像
	GraphHandles* m_characterDeadGraph;

	// キャラがダメージ受けた時のエフェクト画像
	GraphHandles* m_characterDamageGraph;

	// 爆発の画像
	GraphHandles* m_bombGraph;

	// 爆発の効果音
	int m_bombSound;

	// キャラがやられた時の効果音
	int m_characterDeadSound;

	// ドアに入った時の効果音
	int m_doorSound;

	// カメラのズームイン・アウトの効果音
	int m_cameraInSound;
	int m_cameraOutSound;

	// 背景
	int m_backGroundGraph;
	int m_backGroundColor;

	// ボスがやられた時のエフェクト中
	int m_bossDeadEffectCnt;

public:
	World();
	World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer);
	World(const World* original);
	~World();

	//デバッグ
	void debug(int x, int y, int color) const;

	// ゲッタ
	inline int getFocusId() const { return m_focusId; }
	inline int getPlayerId() const { return m_playerId; }
	inline int getBrightValue() const { return m_brightValue; }
	inline int getAreaNum() const { return m_areaNum; }
	inline int getNextAreaNum() const { return m_nextAreaNum; }
	inline int getDate() const { return m_date; }
	inline bool getResetBgmFlag() const { return m_resetBgmFlag; }
	inline bool getBlindFlag() const { return m_blindFlag; }
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<CharacterController*> getCharacterControllers() const { return m_characterControllers; }
	std::vector<Character*> getCharacters() const { return m_characters; }
	std::vector<Object*> getStageObjects() const { return m_stageObjects; }
	std::vector<Object*> getDoorObjects() const { return m_doorObjects; }
	std::vector<Object*> getAttackObjects() const { return m_attackObjects; }
	std::vector<Animation*> getAnimations() const { return m_animations; }
	std::vector<Item*> getItemVector() const { return m_itemVector; }
	inline const int getBackGroundGraph() const { return m_backGroundGraph; }
	inline const int getBackGroundColor() const { return m_backGroundColor; }
	inline const Conversation* getConversation() const { return m_conversation_p; }
	inline const Conversation* getObjectConversation() const { return m_objectConversation; }
	inline Movie* getMovie() const { return m_movie_p; }
	inline SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }
	inline double getCameraMaxEx() const { return m_cameraMaxEx; }
	inline double getCameraMinEx() const { return m_cameraMinEx; }
	inline GraphHandles* getCharacterDeadGraph() const { return m_characterDeadGraph; }
	inline GraphHandles* getCharacterDamageGraph() const { return m_characterDamageGraph; }
	inline GraphHandles* getBombGraph() const { return m_bombGraph; }
	inline int getCharacterDeadSound() const { return m_characterDeadSound; }
	inline int getBombSound() const { return m_bombSound; }
	inline int getDoorSound() const { return m_doorSound; }
	inline bool getSkillFlag() const { return m_skillFlag; }
	inline int getBossDeadEffextCnt() const { return m_bossDeadEffectCnt; }

	// Drawer用のゲッタ
	std::vector<const CharacterAction*> getActions() const;
	std::vector<const Object*> getFrontObjects() const;
	std::vector<const Object*> getBackObjects() const;
	std::vector<const Animation*> getConstAnimations() const;

	// 名前でキャラ検索
	Character* getCharacterWithName(std::string characterName) const;

	// 名前でコントローラ検索
	CharacterController* getControllerWithName(std::string characterName) const;

	// IDでキャラ検索
	Character* getCharacterWithId(int id) const;

	// セッタ
	inline void setFocusId(int id) { m_focusId = id; }
	inline void setConversation(Conversation* conversation) { m_conversation_p = conversation; }
	inline void setMovie(Movie* movie) { m_movie_p = movie; }
	inline void setAreaLock(bool lock) { m_areaLock = lock; }
	inline void setDate(int date) { m_date = date; }
	inline void setBlindFlag(bool blindFlag) { m_blindFlag = blindFlag; }

	// 強制的にエリア移動
	inline void moveArea(int nextArea) { m_brightValue--; m_nextAreaNum = nextArea; m_resetBgmFlag = true; }

	// ID指定でBrain変更
	void setBrainWithId(int id, Brain* brain);

	// ストーリーによるキャラ追加
	void addCharacter(CharacterLoader* characterLoader);

	// ストーリーによるキャラの性能変化
	void changeCharacterVersion(int version);

	// ストーリーによるオブジェクト追加
	void addObject(ObjectLoader* objectLoader);

	// プレイヤーのHPが0ならtrue
	bool playerDead();

	// プレイヤーのHPをMAXにする
	void playerHpReset();

	// スキル発動：ハートをFreezeにする
	void setSkillFlag(bool skillFlag);

	// スキル発動：複製のハート追加用
	void pushCharacter(Character* character, CharacterController* controller);

	// スキル発動：複製のハート削除用
	void popCharacterController(int id);

	// スキル発動：レコーダを作成し使用を開始
	void createRecorder();

	// スキル発動：レコーダの時間を最初に戻す
	void initRecorder();

	// スキル発動：レコーダの使用をやめて削除する
	void eraseRecorder();

	// データ管理：キャラの状態を変更する いないなら作成する
	void asignedCharacterData(const char* name, CharacterData* data);

	// データ管理：キャラの状態を教える
	void asignCharacterData(const char* name, CharacterData* data, int fromAreaNum, bool notCharacterPoint) const;

	// データ管理：Doorの状態を変更する いないなら作成する
	void asignedDoorData(DoorData* data);

	// データ管理：Doorの状態を教える
	void asignDoorData(std::vector<DoorData*>& data, int fromAreaNum) const;

	// データ管理：プレイヤーをドアの前まで移動
	void setPlayerOnDoor(int from);

	// データ管理：カメラの位置をリセット
	void cameraPointInit();

	// プレイヤーを特定の座標へ移動
	void setPlayerPoint(CharacterData* characterData);

	// 仲間をプレイヤーの位置へ移動
	void setPlayerFollowerPoint();

	// キャラに戦わせる
	void battle();

	// 各キャラが目標地点へ移動するだけ
	bool moveGoalCharacter();

	// 画面が暗転するだけ キャラが動けない期間ならtrue
	bool dealBrightValue();

	// キャラに会話させる
	void talk();

	// ムービーを流す
	void moviePlay();

private:

	// データ管理：キャラのセーブデータを自身に反映させる
	void asignedCharacter(Character* character, CharacterData* data, bool changePosition);

	// データ管理：コントローラ1個の情報を世界に反映
	CharacterController* createControllerWithData(const Character* character, CharacterData* data);

	// Battle：カメラの更新
	void updateCamera();

	// Battle：アニメーションの更新
	void updateAnimation();

	// Battle：キャラの更新（攻撃対象の変更）
	void updateCharacter();

	// Battle：キャラクターの動き
	void controlCharacter();

	// Battle：オブジェクトの動き
	void controlObject();

	// Battle：アイテムの動き
	void controlItem();

	// Battle：キャラクターとオブジェクトの当たり判定 slope=trueならslopeが対象falseならそれ以外
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects, bool slope);

	// Battle：キャラクターと扉の当たり判定
	void atariCharacterAndDoor(CharacterController* controller, std::vector<Object*>& objects);

	// Battle：壁や床<->攻撃の当たり判定
	void atariStageAndAttack();

	// Battle：攻撃<->攻撃の当たり判定
	void atariAttackAndAttack();

	// Battle: 爆発を起こす
	void createBomb(int x, int y, Object* attackObject);

	// Battle: ダメージエフェクト作成
	void createDamageEffect(int x, int y, int sum);

	// Battle: ボスがやられたときの爆発エフェクト
	void createBossDeadEffect();

};

#endif