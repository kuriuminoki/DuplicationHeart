#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


class World;


class Game {
private:
	World* m_world;

public:
	Game();
	~Game();

	// �f�o�b�O
	void debug(int x, int y, int color);

	// �Q�[�����v���C����
	void play();
};

#endif