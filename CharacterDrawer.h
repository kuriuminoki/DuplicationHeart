#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterAction�����ăL������`�悷��
class CharacterDrawer {
private:
	// �L�����̓����̏�� const�֐������Ă΂Ȃ�
	const CharacterAction* m_characterAction;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// �Z�b�^
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera);
};

#endif