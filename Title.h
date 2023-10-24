#ifndef TITLE_H_INCLUDED
#define TITLE_H_INCLUDED


class SoundPlayer;
class Button;
class TitleOption;
class OpMovie;
class AnimationDrawer;
class GameData;


/*
* セーブデータ選択画面
*/
class SelectSaveData {
private:
	
	// セーブデータの数
	static const int GAME_DATA_SUM = 3;
	
	// セーブデータ
	GameData* m_gameData[GAME_DATA_SUM];

	// フォント
	int m_font;

	// セーブデータの選択ボタン
	Button* m_dataButton[GAME_DATA_SUM];
	
	// セーブデータ削除ボタン
	Button* m_dataInitButton[GAME_DATA_SUM];
	int m_initCnt; // 長押しの時間

	// 戻るボタン
	Button* m_cancelButton;

	// 使用するセーブデータが決まっていないとき
	static const int NOT_DECIDE_DATA = -1;

	// 使用するセーブデータのインデックス
	int m_useSaveDataIndex;

public:

	SelectSaveData();

	~SelectSaveData();

	// セーブデータが1つでも存在するか
	bool saveDataExist();

	// セーブデータ選択画面の処理
	bool play(int handX, int handY);

	// 描画
	void draw(int handX, int handY);

	// 使用するセーブデータのディレクトリ名
	const char* useDirName();

};


/*
* タイトル画面
*/
class Title {
private:

	// マウスカーソルの位置
	int m_handX, m_handY;

	// サウンドプレイヤー
	SoundPlayer* m_soundPlayer;

	// タイトルの画像
	int m_titleGraph;

	// オプション画面
	TitleOption* m_option;

	// OPムービー
	OpMovie* m_movie;

	// OP描画用
	AnimationDrawer* m_animationDrawer;

	// セーブデータ選択画面
	SelectSaveData* m_selectSaveData;

	// 今どの画面
	enum TITLE_STATE {
		OP,
		TITLE,
		SELECT,
		OPTION
	};
	TITLE_STATE m_state;

	// ボタン
	int m_font;
	Button* m_selectButton;
	Button* m_optionButton;

public:

	Title();

	~Title();

	// タイトル画面の処理 終了ならtrue
	bool play();

	// 描画
	void draw();

	// 使用するセーブデータのフォルダ名
	inline const char* useSaveFile() { 
		return m_selectSaveData->useDirName();
	}

};


#endif