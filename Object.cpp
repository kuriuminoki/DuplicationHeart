#include "Object.h"
#include "CharacterController.h"
#include "Define.h"
#include "DxLib.h"
#include <algorithm>
#include <cmath>


Object::Object() :
	Object(0, 0, 0, 0)
{

}

Object::Object(int x1, int y1, int x2, int y2) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	// 大小関係は 1 <= 2
	if (m_x1 > m_x2) { std::swap(m_x1, m_x2); }
	if (m_y1 > m_y2) { std::swap(m_y1, m_y2); }

	m_deleteFlag = false;
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
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// キャラは上へ移動できない
			characterController->setActionUpLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y2);
		}
	}

	// キャラが左右移動で当たっているか判定
	if (characterY2 + characterVy > m_y1 && characterY1 + characterVy < m_y2) {
		// 右に移動中のキャラが左から当たっているか判定
		if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
			// 段差とみなして乗り越える
			if (characterY2 - STAIR_HEIGHT <= m_y1 && characterX2 == m_x1) {
				// 適切な座標へ
				DrawFormatString(1000, 100, WHITE, "ok");
				characterController->setCharacterX(characterX1 + characterController->getCharacterWide() / 2);
				characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
				// 着地
				characterController->setCharacterGrand(true);
				// キャラは下へ移動できない
				characterController->setActionDownLock(true);
			}
			else {
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
			}
		}
		// 左に移動中のキャラが右から当たっているか判定
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			// 段差とみなして乗り越える
			if (characterY2 - STAIR_HEIGHT <= m_y1 && characterX1 == m_x2) {
				DrawFormatString(1000, 100, WHITE, "ok");
				// 適切な座標へ
				characterController->setCharacterX(characterX1 - characterController->getCharacterWide() / 2);
				characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
				// 着地
				characterController->setCharacterGrand(true);
				// キャラは下へ移動できない
				characterController->setActionDownLock(true);
			}
			else {
				// キャラは左へ移動できない
				characterController->setActionLeftLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x2);
			}
		}
	}

	// 万が一オブジェクトの中に入り込んでしまったら
	if (characterVy != 0 && characterY2 > m_y1 && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		// 真上へ
		characterController->setCharacterY(m_y1 - characterController->getCharacterHeight());
		// 着地
		characterController->setCharacterGrand(true);
		// キャラは下へ移動できない
		characterController->setActionDownLock(true);
	}
}

void BoxObject::action() {

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
		// 下りているときはこの条件式がtrueになる
		if (characterY2 == getY(characterX1_5 - characterVx)) {
			// 前のフレームでは着地していたので、引き続き着地
			characterController->setCharacterGrand(true);
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
		}
		// 下に移動中のキャラが上から当たっているか判定
		else if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// 着地
			characterController->setCharacterGrand(true);
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
		}
		// 上に移動中のキャラが下から当たっているか判定
		else if (characterY1 >= m_y2 && characterY1 + characterVy <= m_y2) {
			// キャラは上へ移動できない
			characterController->setActionUpLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y2);
		}
	}

	// 坂を移動
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 <= m_y2 && characterY2 >= getY(characterX1_5)) {
		// 着地
		characterController->setCharacterGrand(true);
		// キャラは下へ移動できない
		characterController->setActionDownLock(true);
		// 適切な高さへ移動
		characterController->setCharacterY(getY(characterX1_5) - characterController->getCharacterHeight());
	}

	// 坂の鋭角（先端）の当たり判定
	if (m_y2 > characterY1 && m_y2 < characterY2) {
		if (m_leftDown) {// 左肩下がり
			// 右に移動中のキャラが右から当たっているか判定
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x1 - characterController->getCharacterWide());
			}
		}
		else {
			// 左に移動中のキャラが左から当たっているか判定
			if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
				// キャラは左へ移動できない
				characterController->setActionLeftLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x2);
			}
		}
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

void TriangleObject::action() {

}


BulletObject::BulletObject(int x1, int y1, int x2, int y2, int color, int gx, int gy, int damage, int speed, int distance) :
	Object(x1, y1, x2, y2)
{
	// 必要なら後からセッタで設定
	m_characterId = -1;

	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = (m_x2 - m_x1) / 2;
	m_ry = (m_y2 - m_y1) / 2;
	m_damage = damage;

	// 角度を計算し、VXとVYを決定
	int dx = gx - x1;
	int dy = gy - y1;
	double r = std::atan2((double)dy, (double)dx);
	m_v = speed;
	m_vx = (int)speed * std::cos(r);
	m_vy = (int)speed * std::sin(r);

	m_d = distance;
}

// キャラとの当たり判定
// 当たっているならキャラを操作する。
void BulletObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();

	// 当たり判定
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		// 貫通弾じゃないなら消滅
		m_deleteFlag = true;
	}
}

void BulletObject::action() {
	m_x1 += m_vx;
	m_x2 += m_vx;
	m_y1 += m_vy;
	m_y2 += m_vy;
	m_d -= m_v;
	// 飛距離がきたら消滅
	if (m_d <= 0) {
		m_deleteFlag = true;
	}
}


SlashObject::SlashObject(int x1, int y1, int x2, int y2, int* handle, int handleSum, int damage, int slashCountSum) :
	Object(x1, y1, x2, y2)
{
	// 必要なら後からセッタで設定
	m_characterId = -1;

	// 画像
	m_handle = handle;
	m_handleSum = handleSum;
	
	// ダメージ
	m_damage = damage;

	// 全体フレーム
	m_slashCountSum = slashCountSum;

	// カウント
	m_cnt = 0;
}

// キャラとの当たり判定
// 当たっているならキャラを操作する。
void SlashObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getCharacterX();
	int characterY1 = characterController->getCharacterY();
	int characterX2 = characterX1 + characterController->getCharacterWide();
	int characterY2 = characterY1 + characterController->getCharacterHeight();

	// 当たり判定
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		// 貫通弾じゃないなら消滅
		m_deleteFlag = true;
	}
}

void SlashObject::action() {
	m_cnt++;

	if (m_cnt == m_slashCountSum) {
		m_deleteFlag = true;
	}
}