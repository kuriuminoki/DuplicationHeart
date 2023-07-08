#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterAction�����ăL������`�悷��
class CharacterDrawer {
private:
	// �L�����̓����̏�� const�֐������Ă΂Ȃ�
	const CharacterAction* m_characterAction;

	// �J�E���g �A�j���[�V�����p
	int m_cnt;

	// �`�悷��摜
	int m_handle;

	// �`�悷����W(�摜�̒��S)
	int m_x, m_y;

	// �`�悷��{��
	int m_ex;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// �Z�b�^
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera);
};

#endif