#ifndef GRAPH_HANDLE_H_INCLUDED
#define GRAPH_HANDLE_H_INCLUDED

#include <map>
#include <string>

class Camera;

/*
* 画像データ(ハンドル、画像固有の拡大率、向き)をまとめて扱うためのクラス
*/
class GraphHandle {
private:
	// 画像ハンドル
	int m_handle;

	// (この画像固有の)拡大率
	double m_ex;

	// 向き
	double m_angle;

	// 透明度を有効にするか
	bool m_trans;

	// 反転するか
	bool m_reverseX;
	bool m_reverseY;

public:
	GraphHandle(const char* filePath, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandle();

	// ゲッタ
	inline int getHandle() const { return m_handle; }
	inline double getEx() const { return m_ex; }
	inline double getAngle() const { return m_angle; }

	// セッタ
	inline void setEx(double ex) { m_ex = ex; }
	inline void setAngle(double angle) { m_angle = angle; }
	inline void setTrans(bool trans) { m_trans = trans; }
	inline void setReverseX(bool reverse) { m_reverseX = reverse; }
	inline void setReverseY(bool reverse) { m_reverseY = reverse; }

	// 描画する
	void draw(int x, int y, double ex = 1.0) const;

	// 範囲を指定して変形描画する
	void extendDraw(int x1, int y1, int x2, int y2) const;

	// 範囲を指定して敷き詰めるように描画する
	void lineUpDraw(int x1, int y1, int x2, int y2, const Camera* camera) const;
};


/*
* GraphHandleを配列としてまとめて扱うクラス
*/
class GraphHandles {
private:
	GraphHandle** m_handles;
	int m_handleSum;

public:
	//  filePathの末尾には".png"をつけないこと。
	GraphHandles(const char* filePath, int handleSum, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandles();

	// ゲッタ
	inline GraphHandle* getGraphHandle(int i = 0) const { return m_handles[i]; }
	inline int getHandle(int i) { return m_handles[i]->getHandle(); }
	inline int getSize() const { return m_handleSum; }

	// セッタ
	void setEx(double ex);
	void setAngle(double angle);
	void setTrans(bool trans);
	void setReverseX(bool reverse);
	void setReverseY(bool reverse);

	// 描画する
	void draw(int x, int y, int index);
};


/*
* キャラの眼の瞬きを処理するクラス
*/
class CharacterEyeClose {
private:

	// 眼を閉じる時間
	static const int EYE_CLOSE_MIN_TIME = 5;
	static const int EYE_CLOSE_MAX_TIME = 10;
	
	// 眼を閉じる残り時間
	int m_cnt;

public:

	CharacterEyeClose();

	// 瞬きスタートでtrue
	bool closeFlag();

private:

	// 眼を閉じる時間をカウント
	void count();

	// 瞬き中ならtrue
	inline bool closeNow() { return m_cnt > 0; }

};


/*
* キャラの画像を管理するクラス
*/
class CharacterGraphHandle {
private:
	// 表示される画像
	GraphHandle* m_graphHandle_p;

	double m_ex;

	int m_wide, m_height;

	// 瞬き
	CharacterEyeClose m_characterEyeClose;

	// キャラのパーツの画像
	// 斬撃攻撃画像
	GraphHandles* m_slashHandles;

	// 射撃攻撃画像
	GraphHandles* m_bulletHandles;

	// キャラ本体の画像
	// 立ち画像
	GraphHandles* m_standHandles;

	// しゃがみ画像
	GraphHandles* m_squatHandles;

	// しゃがみ射撃画像
	GraphHandles* m_squatBulletHandles;

	// 立ち射撃画像
	GraphHandles* m_standBulletHandles;

	// 立ち斬撃画像
	GraphHandles* m_standSlashHandles;

	// 走り画像
	GraphHandles* m_runHandles;

	// 走り射撃画像
	GraphHandles* m_runBulletHandles;

	// 着地画像
	GraphHandles* m_landHandles;

	// 上昇画像
	GraphHandles* m_jumpHandles;

	// 下降画像
	GraphHandles* m_downHandles;

	// ジャンプ前画像
	GraphHandles* m_preJumpHandles;

	// ダメージ画像
	GraphHandles* m_damageHandles;

	// ブースト画像
	GraphHandles* m_boostHandles;

	// 空中射撃画像
	GraphHandles* m_airBulletHandles;

	// 空中斬撃画像
	GraphHandles* m_airSlashHandles;

	// 瞬き画像
	GraphHandles* m_closeHandles;

public:
	// デフォルト値で初期化
	CharacterGraphHandle();
	// csvファイルを読み込み、キャラ名で検索しパラメータ取得
	CharacterGraphHandle(const char* characterName, double drawEx);
	// 画像を削除
	~CharacterGraphHandle();

	// 表示する画像を返す
	inline GraphHandle* getHandle() { return m_graphHandle_p; }
	inline int getWide() const { return m_wide; }
	inline int getHeight() const { return m_height; }

	// 画像のゲッタ
	inline GraphHandles* getSlashHandle() { return m_slashHandles; }
	inline GraphHandles* getBulletHandle() { return m_bulletHandles; }
	inline GraphHandles* getStandHandle() { return m_standHandles; }
	inline GraphHandles* getStandBulletHandle() { return m_standBulletHandles; }
	inline GraphHandles* getStandSlashHandle() { return m_standSlashHandles; }
	inline GraphHandles* getSquatHandle() { return m_squatHandles; }
	inline GraphHandles* getSquatBulletHandle() { return m_squatBulletHandles; }
	inline GraphHandles* getRunHandle() { return m_runHandles; }
	inline GraphHandles* getRunBulletHandle() { return m_runBulletHandles; }
	inline GraphHandles* getLandHandle() { return m_landHandles; }
	inline GraphHandles* getJumpHandle() { return m_jumpHandles; }
	inline GraphHandles* getDownHandle() { return m_downHandles; }
	inline GraphHandles* getPreJumpHandle() { return m_preJumpHandles; }
	inline GraphHandles* getDamageHandle() { return m_damageHandles; }
	inline GraphHandles* getBoostHandle() { return m_boostHandles; }
	inline GraphHandles* getAirBulletHandle() { return m_airBulletHandles; }
	inline GraphHandles* getAirSlashHandle() { return m_airSlashHandles; }
	inline GraphHandles* getCloseHandle() { return m_closeHandles; }

	// 画像サイズをセット
	void setGraphSize();

	// 画像をセット、存在しない画像ならそのまま　サイズも決定
	void setGraph(const GraphHandles* graphHandles, int index);
	void setGraph(GraphHandle* graphHandle);

	// 立ち画像をセット
	void switchStand(int index = 0);
	// 立ち射撃画像をセット
	void switchBullet(int index = 0);
	// 立ち斬撃画像をセット
	void switchSlash(int index = 0);
	// しゃがみ画像をセット
	void switchSquat(int index = 0);
	// しゃがみ射撃画像をセット
	void switchSquatBullet(int index = 0);
	// 走り画像をセット
	void switchRun(int index = 0);
	// 走り射撃画像をセット
	void switchRunBullet(int index = 0);
	// 着地画像をセット
	void switchLand(int index = 0);
	// 上昇画像をセット
	void switchJump(int index = 0);
	// 降下画像をセット
	void switchDown(int index = 0);
	// ジャンプ前画像をセット
	void switchPreJump(int index = 0);
	// ダメージ画像をセット
	void switchDamage(int index = 0);
	// ブースト画像をセット
	void switchBoost(int index = 0);
	// 空中射撃画像をセット
	void switchAirBullet(int index = 0);
	// 空中斬撃画像をセット
	void switchAirSlash(int index = 0);
	// 瞬き画像をセット
	void switchClose(int index = 0);
};


/*
* キャラの顔画像
*/
class FaceGraphHandle {
private:
	double m_ex;

	// 顔画像 <画像名, 画像ハンドル>
	std::map<std::string, GraphHandles*> m_faceHandles;

public:
	FaceGraphHandle();
	FaceGraphHandle(const char* characterName, double drawEx);
	~FaceGraphHandle();

	GraphHandles* getGraphHandle(const char* faceName);
};


#endif