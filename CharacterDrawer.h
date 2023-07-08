#ifndef CHARACTER_DRAWER_H_INCLUDED
#define CHARACTER_DRAWER_H_INCLUDED

class Camera;
class CharacterAction;

// CharacterActionを見てキャラを描画する
class CharacterDrawer {
private:
	// キャラの動きの情報 const関数しか呼ばない
	const CharacterAction* m_characterAction;

	// カウント アニメーション用
	int m_cnt;

	// 描画する画像
	int m_handle;

	// 描画する座標(画像の中心)
	int m_x, m_y;

	// 描画する倍率
	int m_ex;

public:
	CharacterDrawer(const CharacterAction* const characterAction);

	// セッタ
	void setCharacterAction(const CharacterAction* action) { m_characterAction = action; }

	void drawCharacter(const Camera* const camera);
};

#endif