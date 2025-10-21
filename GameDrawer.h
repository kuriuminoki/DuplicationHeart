#ifndef GAME_DRAWER_H_INCLUDED
#define GAME_DRAWER_H_INCLUDED

class Game;
class WorldDrawer;

class GameDrawer {
private:
	const Game* m_game;

	int m_screenEffectHandle;

	// �e�L�X�g��t�H���g�̃T�C�Y�̔{��
	double m_exX;
	double m_exY;

	WorldDrawer* m_worldDrawer;

	// �X�L���̏��̃t�H���g
	const int SKILL_SIZE = 100;
	int m_skillHandle;

	// �X�L���̏��̘g
	int m_skillInfoHandle;
	int m_skillInfoBackHandle;

	// ����
	int m_timeBarNoonHandle;
	int m_timeBarEveningHandle;
	int m_timeBarNightHandle;
	int m_needleHandle;

	// �Q�[���I�[�o�[�̉摜
	int m_gameoverHandle;

	// �X�s�[�h�A�b�v���[�h
	int m_quickModeHandle;
	const int QUICK_SIZE = 50;
	int m_quickModeFontHandle;
	int m_quickModeCnt = 0;

	// �Z�[�u�����ʒm�̉摜
	int m_noticeSaveDataHandle;
	int m_noticeX, m_noticeY;
	double m_noticeEx;

	// ��ʂ̉��H�p
	const int THIN = 4;
	int m_tmpScreenR;
	int m_tmpScreenG;
	int m_tmpScreenB;

public:
	GameDrawer(const Game* game);

	~GameDrawer();

	void draw(int screen);
};


#endif