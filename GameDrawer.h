#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	// �e�L�X�g��t�H���g�̃T�C�Y�̔{��
	double m_exX;
	double m_exY;

	WorldDrawer* m_worldDrawer;

	// �X�L���̏��̃t�H���g
	const int SKILL_SIZE = 100;
	int m_skillHandle;

	// �Q�[���I�[�o�[�̉摜
	int m_gameoverHandle;

	// �Z�[�u�����ʒm�̉摜
	int m_noticeSaveDataHandle;
	int m_noticeX, m_noticeY;
	double m_noticeEx;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw();
};


#endif