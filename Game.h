#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


class SoundPlayer;
class World;


class Game {
private:
	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer;

	World* m_world;

public:
	Game();
	~Game();

	// �Q�b�^
	World* getWorld() const { return m_world; }

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �Q�[�����v���C����
	void play();
};

#endif