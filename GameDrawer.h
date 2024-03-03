#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	// テキストやフォントのサイズの倍率
	double m_exX;
	double m_exY;

	WorldDrawer* m_worldDrawer;

	// スキルの情報のフォント
	const int SKILL_SIZE = 100;
	int m_skillHandle;

	// ゲームオーバーの画像
	int m_gameoverHandle;

	// セーブ完了通知の画像
	int m_noticeSaveDataHandle;
	int m_noticeX, m_noticeY;
	double m_noticeEx;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw();
};


#endif