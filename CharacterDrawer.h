#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;
class Character;

// �̗̓o�[��\��
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int prevHp, int maxHp, int damageColor, int prevHpColor, int hpColor);

// CharacterAction�����ăL������`�悷��
class CharacterDrawer {
private:

	// �L�����̓����̏�� const�֐������Ă΂Ȃ�
	const CharacterAction* m_characterAction;

	const int HP_BAR_WIDE = 200;
	const int HP_BAR_HEIGHT = 20;
	static int HP_COLOR;
	static int PREV_HP_COLOR;
	static int DAMAGE_COLOR;

	// �_�ŗp
	int m_cnt;

	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;

public:

	CharacterDrawer(const CharacterAction* const characterAction);

	// �Z�b�^
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera, int bright = 255);

	void drawPlayerHpBar(const Character* player);

};

#endif