#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterActionを見てキャラを描画する
class CharacterDrawer {
private:
	// キャラの動きの情報 const関数しか呼ばない
	const CharacterAction* m_characterAction;

	const int HP_BAR_WIDE = 200;
	const int HP_BAR_HEIGHT = 20;
	static int HP_COLOR;
	static int DAMAGE_COLOR;

	// 点滅用
	int m_cnt;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// セッタ
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera);
};

#endif