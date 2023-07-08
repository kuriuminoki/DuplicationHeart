#ifndef WORLD_DRAWER_H_INCLUDED
#define WORLD_DRAWER_H_INCLUDED

class World;
class CharacterDrawer;

// ���E��`�悷��
class WorldDrawer {
private:

	// �`�悷�鐢�E
	const World* m_world;

	// �L�����`��p
	CharacterDrawer* m_characterDrawer;

public:
	WorldDrawer(const World* world);

	~WorldDrawer();

	// �Z�b�^
	void setWorld(World* world) { m_world = world; }

	// �`�悷��
	void draw();
};

#endif