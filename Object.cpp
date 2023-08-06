#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "GraphHandle.h"
#include "Animation.h"
#include "DxLib.h"
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;


Object::Object() :
	Object(0, 0, 0, 0, -1)
{

}

Object::Object(int x1, int y1, int x2, int y2, int hp) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	// 大小関係は 1 <= 2
	if (m_x1 > m_x2) { std::swap(m_x1, m_x2); }
	if (m_y1 > m_y2) { std::swap(m_y1, m_y2); }

	m_hp = hp;
	m_damageCnt = 0;

	m_deleteFlag = false;

	m_effectHandles_p = NULL;
	m_soundHandle_p = -1;
}

// HPを減らす
void Object::decreaseHp(int damageValue) {
	m_hp = max(0, m_hp - damageValue);
	if (m_hp == 0) { setDeleteFlag(true); }
	m_damageCnt = DAMAGE_CNT_SUM;
}

// アニメーション作成
Animation* BulletObject::createAnimation(int x, int y, int flameCnt) {
	if (m_effectHandles_p == NULL) {
		return NULL;
	}
	return new Animation((m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, 3, m_effectHandles_p);
}

// アニメーション作成
Animation* SlashObject::createAnimation(int x, int y, int flameCnt) {
	if (m_effectHandles_p == NULL) {
		return NULL;
	}
	// 座標の平均をとる
	x = (x + (m_x1 + m_x2) / 2) / 2;
	y = (y + (m_y1 + m_y2) / 2) / 2;
	return new Animation(x, y, flameCnt, m_effectHandles_p);
}

/*
* 四角形のオブジェクト
*/
BoxObject::BoxObject(int x1, int y1, int x2, int y2, int color, int hp) :
	Object(x1, y1, x2, y2, hp)
{
	m_color = color;
}

// キャラクターとの当たり判定
bool BoxObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// キャラが上下移動で当たっているか判定
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// 下に移動中のキャラが上から当たっているか判定
		if (characterY2 <= m_y1 && characterY2 + characterVy >= m_y1) {
			// 着地
			characterController->setCharacterGrand(true);
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(m_y1 - characterHeight);
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
			if (characterY2 - STAIR_HEIGHT <= m_y1) {
				// 適切な座標へ
				characterController->setCharacterX(m_x1 - characterWide / 2 - characterVx);
				characterController->setCharacterY(m_y1 - characterHeight);
				// 着地
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
				// キャラは下へ移動できない
				characterController->setActionDownLock(true);
			}
			else {
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
		// 左に移動中のキャラが右から当たっているか判定
		else if (characterX1 >= m_x2 && characterX1 + characterVx <= m_x2) {
			if (characterY2 - STAIR_HEIGHT <= m_y1) {
				// 適切な座標へ
				characterController->setCharacterX(m_x2 - characterWide / 2 + characterVx);
				characterController->setCharacterY(m_y1 - characterHeight);
				// 着地
				characterController->setCharacterGrand(true);
				characterController->setActionBoost();
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
	return false;
}

// キャラがオブジェクトに入り込んでいるときの処理
void BoxObject::penetration(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	// 万が一オブジェクトの中に入り込んでしまったら
	bool slope = characterController->getAction()->getGrandLeftSlope() || characterController->getAction()->getGrandRightSlope();
	if (!slope && characterY2 > m_y1 && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		// キャラが横にはみ出しているなら
		if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// 密着状態まで移動させる
				characterController->setCharacterX(m_x1 - characterWide);
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
			}
			else {
				// 密着状態まで移動させる
				characterController->setCharacterX(m_x2);
				// キャラは左へ移動できない
				characterController->setActionLeftLock(true);
			}
		}
		else if (characterY1 < m_y1 || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (m_y1 + m_y2)) {
				// 真上へ
				characterController->setCharacterY(m_y1 - characterHeight);
				// 着地
				characterController->setCharacterGrand(true);
				// キャラは下へ移動できない
				characterController->setActionDownLock(true);
			}
			else {
				// 真下へ
				characterController->setCharacterY(m_y2);
				// キャラは上へ移動できない
				characterController->setActionUpLock(true);
			}
		}
	}
}

// 攻撃オブジェクトとの当たり判定
bool BoxObject::atariObject(Object* object) {
	// 破壊不能オブジェクト
	if (!object->getAbleDelete()) { return false; }
	// 当たっているなら
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->setDeleteFlag(true);
		// 自分の体力を減らす
		if (getAbleDelete()) {
			decreaseHp(object->getDamage());
			return true;
		}
	}
	return false;
}

void BoxObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
}

TriangleObject::TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown, int hp):
	Object(x1, y1, x2, y2, hp)
{
	m_color = color;
	m_leftDown = leftDown;
}

// 座標XにおけるY座標（傾きから算出する）
int TriangleObject::getY(int x) const {
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
bool TriangleObject::atari(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterVx = characterController->getAction()->getVx();
	int characterVy = characterController->getAction()->getVy();

	// キャラが上下移動で当たっているか判定
	if (characterX2 > m_x1 && characterX1 < m_x2) {
		// 下りているときはこの条件式がtrueになる
		if (characterY2 == getY(characterX1_5 - characterVx)) {
			// 前のフレームでは着地していたので、引き続き着地
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
		}
		// 下に移動中のキャラが上から当たっているか判定
		else if (characterY2 <= getY(characterX1_5) && characterY2 + characterVy >= getY(characterX1_5)) {
			// 着地
			characterController->setCharacterGrand(true);
			if (m_leftDown) {
				characterController->setCharacterGrandRightSlope(true);
			}
			else {
				characterController->setCharacterGrandLeftSlope(true);
			}
			// キャラは下へ移動できない
			characterController->setActionDownLock(true);
			// 密着状態までは移動させる
			characterController->setCharacterY(getY(characterX1_5) - characterHeight);
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
		if (m_leftDown) {
			characterController->setCharacterGrandRightSlope(true);
		}
		else {
			characterController->setCharacterGrandLeftSlope(true);
		}
		// キャラは下へ移動できない
		characterController->setActionDownLock(true);
		// 適切な高さへ移動
		characterController->setCharacterY(getY(characterX1_5) - characterHeight);
	}

	// 坂の鋭角（先端）の当たり判定
	if (m_y2 > characterY1 && m_y2 < characterY2) {
		if (m_leftDown) {// 左肩下がり
			// 右に移動中のキャラが右から当たっているか判定
			if (characterX2 <= m_x1 && characterX2 + characterVx >= m_x1) {
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
				// 密着状態までは移動させる
				characterController->setCharacterX(m_x1 - characterWide);
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
				characterController->setCharacterX(m_x1 - characterWide);
			}
		}
	}
	return false;
}

// キャラがオブジェクトに入り込んでいるときの処理
void TriangleObject::penetration(CharacterController* characterController) {
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterWide = characterController->getAction()->getCharacter()->getWide();
	int characterHeight = characterController->getAction()->getCharacter()->getHeight();
	int characterX1_5 = characterController->getAction()->getCharacter()->getCenterX();
	int characterY1_5 = characterController->getAction()->getCharacter()->getCenterY();
	int characterX2 = characterX1 + characterWide;
	int characterY2 = characterY1 + characterHeight;
	int slopeY = getY(characterX1_5);
	// 万が一オブジェクトの中に入り込んでしまったら
	if (characterY2 > slopeY && characterY1 < m_y2 && characterX2 > m_x1 && characterX1 < m_x2) {
		if (characterY1 < slopeY || characterY2 > m_y2) {
			if ((characterY1 + characterY2) < (slopeY + m_y2)) {
				// 真上へ
				characterController->setCharacterY(slopeY - characterHeight);
				// 着地
				characterController->setCharacterGrand(true);
				// キャラは下へ移動できない
				characterController->setActionDownLock(true);
			}
			else {
				// 真下へ
				characterController->setCharacterY(m_y2);
				// キャラは上へ移動できない
				characterController->setActionUpLock(true);
			}
		}
		// キャラが横にはみ出しているなら
		else if (characterX1 < m_x1 || characterX2 > m_x2) {
			if ((characterX1 + characterX2) < (m_x1 + m_x2)) {
				// 密着状態まで移動させる
				characterController->setCharacterX(m_x1 - characterWide);
				// キャラは右へ移動できない
				characterController->setActionRightLock(true);
			}
			else {
				// 密着状態まで移動させる
				characterController->setCharacterX(m_x2);
				// キャラは左へ移動できない
				characterController->setActionLeftLock(true);
			}
		}
	}
}

// 他オブジェクトとの当たり判定
bool TriangleObject::atariObject(Object* object) {
	// 破壊不能オブジェクト
	if (!object->getAbleDelete()) { return false; }
	// 斜辺を考慮して当たり判定を計算
	int y = object->getY1();
	if (m_leftDown) {
		y = getY(object->getX2());
	}
	else {
		y = getY(object->getX1());
	}
	// 当たっているなら
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && y < object->getY2()) {
		object->setDeleteFlag(true);
		// 自分の体力を減らす
		if (getAbleDelete()) {
			decreaseHp(object->getDamage());
			return true;
		}
	}
	return false;
}

void TriangleObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
}


BulletObject::BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo) :
	Object(x - attackInfo->bulletRx(), y - attackInfo->bulletRx(), x + attackInfo->bulletRx(), y + attackInfo->bulletRy())
{
	// 必要なら後からセッタで設定
	m_characterId = -1;
	m_groupId = -1;

	// 各パラメータの設定
	m_color = color;
	m_gx = gx;
	m_gy = gy;
	m_rx = attackInfo->bulletRx();
	m_ry = attackInfo->bulletRy();
	m_damage = attackInfo->bulletDamage();
	m_d = attackInfo->bulletDistance();
	m_hp = attackInfo->bulletHp();

	// 角度を計算し、VXとVYを決定
	int dx = gx - x;
	int dy = gy - y;
	double r = std::atan2((double)dy, (double)dx);
	m_v = attackInfo->bulletSpeed();
	m_vx = (int)(m_v * std::cos(r));
	m_vy = (int)(m_v * std::sin(r));

	// エフェクトの画像
	m_effectHandles_p = attackInfo->bulletEffectHandle();

	// サウンド
	m_soundHandle_p = attackInfo->bulletSoundeHandle();
}

// キャラとの当たり判定
// 当たっているならキャラを操作する。
bool BulletObject::atari(CharacterController* characterController) {
	// 自滅防止
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}
	// チームキル防止
	if (m_groupId == characterController->getAction()->getCharacter()->getGroupId()) {
		return false;
	}

	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// 当たり判定
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// 貫通弾じゃないなら消滅
		m_deleteFlag = true;
		characterController->damage(m_vx / 2, m_vy / 2, m_damage);
		return true;
	}
	return false;
}

// 他攻撃オブジェクトとの当たり判定
bool BulletObject::atariObject(Object* object) {
	// どちらかが破壊不能オブジェクト
	if (!object->getAbleDelete() || !getAbleDelete() || m_groupId == object->getGroupId()) { 
		return false;
	}
	// 当たっているなら
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->decreaseHp(m_damage);
		decreaseHp(object->getDamage());
		return true;
	}
	return false;
}

void BulletObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
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


ParabolaBullet::ParabolaBullet(int x, int y, GraphHandle* handle, int gx, int gy, AttackInfo* attackInfo):
	BulletObject(x, y, -1, gx, gy, attackInfo)
{
	m_handle = handle;
	int attackSize = max(attackInfo->bulletRx(), attackInfo->bulletRy());
	int graphX = 0, graphY = 0;
	GetGraphSize(handle->getHandle(), &graphX, &graphY);
	int graphSize = min(graphX, graphY);
	m_handle->setEx((double)attackSize / (double)graphSize);
	//m_handle->setEx((double)graphSize / (double)attackSize);
}

void ParabolaBullet::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
	m_x1 += m_vx;
	m_x2 += m_vx;
	m_vy += G;
	m_y1 += m_vy;
	m_y2 += m_vy;
	double r = std::atan2((double)m_vy, (double)m_vx);
	m_handle->setAngle(r);
}


SlashObject::SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo) :
	Object(x1, y1, x2, y2, attackInfo->slashHp())
{
	// 必要なら後からセッタで設定
	m_characterId = -1;
	m_groupId = -1;

	// 画像
	m_handle = handle;
	
	// ダメージ
	m_damage = attackInfo->slashDamage();

	// 全体フレーム
	m_slashCountSum = slashCountSum;

	// カウント
	m_cnt = 0;

	// 吹っ飛び(X方向の初速)
	m_slashImpactX = attackInfo->slashImpactX();

	// 吹っ飛び(Y方向の初速)
	m_slashImpactY = attackInfo->slashImpactY();


	// エフェクトの画像
	m_effectHandles_p = attackInfo->slashEffectHandle();

	// サウンド
	m_soundHandle_p = attackInfo->slashSoundHandle();
}

// 大きさを指定しない場合。画像からサイズ取得。生存時間、AttackInfo
SlashObject::SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo) {
	int x2 = 0;
	int y2 = 0;
	GetGraphSize(handle->getHandle(), &x2, &y2);
	x2 += x;
	y2 = y;
	m_hp = attackInfo->slashHp();
	SlashObject(x, y, x2, y2, handle, slashCountSum, attackInfo);
}

// キャラとの当たり判定
// 当たっているならキャラを操作する。
bool SlashObject::atari(CharacterController* characterController) {
	// 自滅防止
	if (m_characterId == characterController->getAction()->getCharacter()->getId()) {
		return false;
	}
	// チームキル防止
	if (m_groupId == characterController->getAction()->getCharacter()->getGroupId()) {
		return false;
	}

	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// 当たり判定
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2 && characterController->getAction()->ableDamage()) {
		// 貫通弾じゃないなら消滅
		 // m_deleteFlag = true;
		if (characterX1 + characterX2 < m_x1 + m_x2) {
			characterController->damage(-m_slashImpactX, -m_slashImpactY, m_damage);
		}
		else {
			characterController->damage(m_slashImpactX, -m_slashImpactY, m_damage);
		}
		return true;
	}
	return false;
}

// 他攻撃オブジェクトとの当たり判定
bool SlashObject::atariObject(Object* object) {
	// どちらかが破壊不能オブジェクト
	if (!object->getAbleDelete() || !getAbleDelete() || m_groupId == object->getGroupId()) {
		return false;
	}
	// 当たっているなら
	if (m_x2 > object->getX1() && m_x1 < object->getX2() && m_y2 > object->getY1() && m_y1 < object->getY2()) {
		object->decreaseHp(m_damage);
		decreaseHp(object->getDamage());
		return true;
	}
	return false;
}

void SlashObject::action() {
	if (m_damageCnt > 0) { m_damageCnt--; }
	// 時間経過
	m_cnt++;

	// 時間が来たので消滅
	if (m_cnt == m_slashCountSum) {
		m_deleteFlag = true;
	}
}

DoorObject::DoorObject(int x1, int y1, int x2, int y2, const char* fileName, int areaNum) :
	Object(x1, y1, x2, y2)
{
	m_graph = new GraphHandle(fileName, 1.0, 0.0, true);
	m_areaNum = areaNum;
	m_text = "";
}
DoorObject::~DoorObject() {
	delete m_graph;
}
bool DoorObject::atari(CharacterController* characterController) {
	if (!characterController->getAction()->ableDamage() || !characterController->getAction()->getGrand()) {
		m_text = "";
		return false;
	}
	// キャラの情報　座標と移動スピード
	int characterX1 = characterController->getAction()->getCharacter()->getX();
	int characterY1 = characterController->getAction()->getCharacter()->getY();
	int characterX2 = characterX1 + characterController->getAction()->getCharacter()->getWide();
	int characterY2 = characterY1 + characterController->getAction()->getCharacter()->getHeight();

	// 当たり判定
	if (characterX2 > m_x1 && characterX1 < m_x2 && characterY2 > m_y1 && characterY1 < m_y2) {
		m_text = "Ｗキーで入る";
		return true;
	}
	m_text = "";
	return false;
}

// 描画用
// オブジェクト描画（画像がないときに使う）
void BoxObject::drawObject(int x1, int y1, int x2, int y2) const {
	if (m_damageCnt > 0) {
		DrawBox(x1, y1, x2, y2, RED, TRUE);
	}
	else {
		DrawBox(x1, y1, x2, y2, m_color, TRUE);
	}
}
// オブジェクト描画（画像がないときに使う）
void TriangleObject::drawObject(int x1, int y1, int x2, int y2) const {
	if (m_leftDown) {
		if (m_damageCnt > 0) {
			DrawTriangle(x2, y1, x2, y2, x1, y2, RED, TRUE);
		}
		else {
			DrawTriangle(x2, y1, x2, y2, x1, y2, m_color, TRUE);
		}
	}
	else {
		if (m_damageCnt > 0) {
			DrawTriangle(x1, y1, x2, y2, x1, y2, RED, TRUE);
		}
		else {
			DrawTriangle(x1, y1, x2, y2, x1, y2, m_color, TRUE);
		}
	}
}
// オブジェクト描画（画像がないときに使う）
void BulletObject::drawObject(int x1, int y1, int x2, int y2) const {
	int rx = abs(x2 - x1) / 2;
	int ry = abs(y2 - y1) / 2;
	if (m_damageCnt > 0) {
		DrawOval(x1 + rx, y1 + ry, rx, ry, RED, TRUE);
	}
	else {
		DrawOval(x1 + rx, y1 + ry, rx, ry, m_color, TRUE);
	}
}