#ifndef WORLD_DRAWER_H_INCLUDED
#define WORLD_DRAWER_H_INCLUDED

class World;
class CharacterDrawer;

// ¢ŠE‚ğ•`‰æ‚·‚é
class WorldDrawer {
private:

	// •`‰æ‚·‚é¢ŠE
	const World* m_world;

	// ƒLƒƒƒ‰•`‰æ—p
	CharacterDrawer* m_characterDrawer;

public:
	WorldDrawer(const World* world);

	~WorldDrawer();

	// ƒZƒbƒ^
	void setWorld(World* world) { m_world = world; }

	// •`‰æ‚·‚é
	void draw();
};

#endif