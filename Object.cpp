#include "Object.h"
#include "CharacterController.h"
#include "Define.h"
#include "DxLib.h"


Object::Object() :
	Object(0, 0, 0, 0)
{

}

Object::Object(int x1, int y1, int x2, int y2) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
}


/*
* 四角形のオブジェクト
*/
BoxObject::BoxObject(int x1, int y1, int x2, int y2, int color) :
	Object(x1, y1, x2, y2)
{
	m_color = color;
}

// キャラクターとの当たり判定
void BoxObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();
	int characterVx = characterController->getCharacterVx();
	int characterVy = characterController->getCharacterVy();

	// キャラが上下移動で当たっているか判定
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// 下に移動中のキャラが上から当たっているか判定
		if (characterY2 <= m_y1 && characterY2 + characterVy >= m_y1) {
			// 着地
			characterController->setCharacterGrand(true);
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
		}
		// 上に移動中のキャラが下から当たっているか判定
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y1) {
			// キャラは上へ移動できない
			characterController->setActionUpLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y2);
		}
	}

	// キャラが左右移動で当たっているか判定
	if (characterY2 > m_y1 && characterY1 < m_y2) {
		// 右に移動中のキャラが左から当たっているか判定
		if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
			// キャラは右へ移動できない
			characterController->setActionRightLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
		}
		// 左に移動中のキャラが右から当たっているか判定
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			// キャラは左へ移動できない
			characterController->setActionLeftLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterX(m_x2);
		}
	}
}

TriangleObject::TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown):
	Object(x1, y1, x2, y2)
{
	m_color = color;
	m_leftDown = leftDown;
}

// 座標XにおけるY座標（傾きから算出する）
int TriangleObject::getY(int x) {
	if (x < m_x1) {
		if (m_leftDown) {
			return m_y2;
		}
		else {
			return m_y1;
		}
	}
	if (x > m_x2) {
		if (m_leftDown) {
			return m_y1;
		}
		else {
			return m_y2;
		}
	}
	// 傾き
	// 分子
	double a1 = (double)(m_y1 - m_y2);
	if (!m_leftDown) {
		a1 *= -1;
	}
	// 分母
	double a2 = (double)(m_x2 - m_x1);

	// 切片
	double b;
	if (m_leftDown) {
		b = m_y1 - (a1 * m_x2 / a2);
	}
	else {
		b = m_y1 - (a1 * m_x1 / a2);
	}

	// 座標
	double y = a1 * x / a2 + b;

	return (int)y;
}

// キャラクターとの当たり判定
void TriangleObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX1_5 = characterX1 + (characterController->getCharacterWide() / 2);
	int characterY1_5 = characterY1 + (characterController->getCharacterHeight() / 2);
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();
	int characterVx = characterController->getCharacterVx();
	int characterVy = characterController->getCharacterVy();

	// キャラが上下移動で当たっているか判定
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// 下に移動中のキャラが上から当たっているか判定
		if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// 着地
			characterController->setCharacterGrand(true);
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
		}
		// 上に移動中のキャラが下から当たっているか判定
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y1) {
			// キャラは上へ移動できない
			characterController->setActionUpLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y2);
		}
	}

	// 坂を移動
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 <= m_y2 && characterY2 >= getY(characterX1_5)) {
		DrawFormatString(900, 100, WHITE, "y=%d", getY(characterX1_5));
		// 着地
		characterController->setCharacterGrand(true);
		// キャラは下へ移動できない
		characterController->setActionDownLock(true);
		// 既に中にいる
		characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
	}

	// キャラが左右移動で当たっているか判定
	if (characterY2 > m_y1 && characterY1 < m_y2) {
		if (m_leftDown) {// 左肩下がり
			// 左に移動中のキャラが右から当たっているか判定
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// キャラは左へ移動できない
				characterController->setActionLeftLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x2);
			}
		}
		else {
			// 右に移動中のキャラが左から当たっているか判定
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
			}
		}
	}
}