#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterActionを見てキャラを描画する
class CharacterDrawer {
private:
	// キャラの動きの情報 const関数しか呼ばない
	const CharacterAction* m_characterAction;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// セッタ
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera);
};

#endif