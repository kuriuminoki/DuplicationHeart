#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED


#include <vector>
#include <string>


class Character;
class Object;
class SoundPlayer;


// �L�����N�^�[�̏��
enum class CHARACTER_STATE {
	STAND,	// �������Ă��Ȃ�
	DAMAGE,	// �_���[�W�󂯒� ���n�ŉ���
	PREJUMP,	// �W�����v�O
	SQUAT,	// ���Ⴊ�ݏ��
	INIT,	// �{�X�̏����A�j���[�V����
};


/*
* �L�����N�^�[�𓮂����N���X
* Controller���g�p����B����ȊO�̃C���X�^���X����g���邱�Ƃ͂Ȃ��B
*/
class CharacterAction {
protected:

	// ���Ԍv��
	int m_cnt;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// �L�����̏��
	CHARACTER_STATE m_state;

	// �������L�����N�^�[
	Character* m_character_p;

	// �O�̃t���[����leftDirection
	bool m_prevLeftDirection;

	// �L�����̃o�[�W���� �C�x���g��runSpeed�̕ύX�����ꍇ�ɑΏ����邽��
	int m_characterVersion;
	int m_characterMoveSpeed;

	// �L�������n�ʂɂ���
	bool m_grand;

	// �E��������̍�ɂ���
	bool m_grandRightSlope;
	
	// ����������̍�ɂ���
	bool m_grandLeftSlope;

	// �L�����������Ă��Ȃ��Ȃ�-1 �����łȂ��Ȃ瑖�����t���[����
	int m_runCnt;

	// �W�����v�O�̓���
	int m_preJumpCnt;

	// �W�����v�̂��ߎ��Ԃ̍ő�
	const int PRE_JUMP_MAX = 10;

	// ���n���[�V�����̎c�莞��
	int m_landCnt;

	// ���n���[�V�����̑�����
	const int LAND_TIME = 10;

	// �u�[�X�g�A�j���̎c�莞�� �܂��͎󂯐g���
	int m_boostCnt;
	int m_boostDone;// 0:none 1:right 2:left
	const int BOOST_TIME = 30;
	const int BOOST_SPEED = 6;

	// �X�e�b�v
	int m_stepCnt;
	int m_stepDone;// 0:none 1:right 2:left
	const int STEP_STOP_TIME = 10;
	const int STEP_TIME = 20;
	const int STEP_SPEED = 30;

	// �X���C�f�B���O
	int m_slidingCnt;
	int m_slidingDone;// 0:none 1:right 2:left
	const int SLIDING_SPEED = 30;

	// �����Ԃ̎���
	const int DAMAGE_TIME = 10;

	// �m�b�N�o�b�N�Ȃ��̃L�����Ȃ�true
	bool m_heavy = false;

	// �ړ���
	bool m_moveRight;
	bool m_moveLeft;
	bool m_moveUp;
	bool m_moveDown;

	// ���x
	int m_vx; // �L�����̉��ړ��̑��x�i�g�[�^���j
	int m_vy; // �L�����̏c�ړ��̑��x�i�g�[�^���j
	int m_runVx; // Controller�ɂ�鉡�ړ��̑��x�i����ȂǁB�_���[�W�ɂ�鐁����т͏����j
	int m_runVy;

	// ���̃L�����Əd�Ȃ��Ă��邽�ߎ��̃t���[���ňʒu�����炷
	int m_dx;

	// �ړ��̃��b�N�i�I�u�W�F�N�g���œ����Ȃ�������true�j
	bool m_rightLock;
	bool m_leftLock;
	bool m_upLock;
	bool m_downLock;

	// �ˌ��p�J�E���g
	int m_bulletCnt;

	// �a���p�J�E���g
	int m_slashCnt;

	// �U���������
	bool m_attackLeftDirection;

	int m_damageCnt;

protected:

	// action����Ăяo��
	virtual void bulletAction();
	virtual void slashAction();
	virtual void damageAction();
	virtual void otherAction();
	virtual void moveAction();

	// �L�����̉摜��ύX
	virtual void switchHandle() = 0;

	// �摜�̃T�C�Y�ύX�ɂ��ʒu����
	void afterChangeGraph(int beforeX1, int afterX1, int beforeY1, int afterY1, int beforeX2, int afterX2, int beforeY2, int afterY2);

public:

	static const char* ACTION_NAME;
	virtual const char* getActionName() const { return this->ACTION_NAME; }

	CharacterAction();
	CharacterAction(Character* character, SoundPlayer* soundPlayer_p);
	virtual ~CharacterAction() {}

	// �R�s�[�쐬
	virtual CharacterAction* createCopy(std::vector<Character*> characters) = 0;

	// �R�s�[�쐬�p
	void setParam(CharacterAction* action);

	// �f�o�b�O
	void debugAction(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline const Character* getCharacter() const { return m_character_p; }
	inline CHARACTER_STATE getState() const { return m_state; }
	inline bool getGrand() const { return m_grand; }
	inline bool getGrandRightSlope() const { return m_grandRightSlope; }
	inline bool getGrandLeftSlope() const { return m_grandLeftSlope; }
	inline bool getHeavy() const { return m_heavy; }
	inline int getVx() const { return m_vx + m_runVx; }
	inline int getVy() const { return m_vy + m_runVy; }
	inline int getDx() const { return m_dx; }
	inline int getSlashCnt() const { return m_slashCnt; }
	inline int getBulletCnt() const { return m_bulletCnt; }
	inline int getSlidingDone() const { return m_slidingDone; }
	bool getRightLock() const { return m_rightLock; }
	bool getLeftLock() const { return m_leftLock; }
	bool getUpLock() const { return m_upLock; }
	bool getDownLock() const { return m_downLock; }
	const SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }
	inline int getPreJumpCnt() const { return m_preJumpCnt; }
	virtual int getPreJumpMax() const { return PRE_JUMP_MAX; }

	// �Z�b�^
	inline void setCnt(int cnt) { m_cnt = cnt; }
	void setState(CHARACTER_STATE state);
	inline void setCharacterVersion(int version) { m_characterVersion = version; }
	inline void setCharacterMoveSpeed(int moveSpeed) { m_characterMoveSpeed = moveSpeed; }
	inline void setSimpleGrand(bool grand) { m_grand = grand; }
	virtual void setGrand(bool grand);
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	virtual void setBoost(bool leftDirection);
	void finishBoost();
	void setStep(bool leftDirection);
	void finishStep();
	void setSliding(bool leftDirection);
	void finishSliding();
	inline void setGrandRightSlope(bool grand) { m_grandRightSlope = grand; }
	inline void setGrandLeftSlope(bool grand) { m_grandLeftSlope = grand; }
	inline void setRunCnt(int runCnt) { m_runCnt = runCnt; }
	inline void setJumpCnt(int preJumpCnt) { m_preJumpCnt = preJumpCnt; }
	inline void setMoveRight(bool moveRight) { m_moveRight = moveRight; }
	inline void setMoveLeft(bool moveLeft) { m_moveLeft = moveLeft; }
	inline void setMoveUp(bool moveUp) { m_moveUp = moveUp; }
	inline void setMoveDown(bool moveDown) { m_moveDown = moveDown; }
	inline void setVx(int vx) { m_vx = vx; }
	inline void setVy(int vy) { m_vy = vy; }
	inline void setRunVx(int run_vx) { m_runVx = run_vx; }
	inline void setRunVy(int run_vy) { m_runVy = run_vy; }
	inline void setDx(int dx) { m_dx = dx; }
	inline void setBulletCnt(int bulletCnt) { m_bulletCnt = bulletCnt; }
	inline void setSlashCnt(int slashCnt) { m_slashCnt = slashCnt; }
	inline void setAttackLeftDirection(bool attackLeftDirection) { m_attackLeftDirection = attackLeftDirection; }
	inline void setLandCnt(int landCnt) { m_landCnt = landCnt; }
	inline void setBoostCnt(int boostCnt) { m_boostCnt = boostCnt; }
	inline void setBoostDone(int boostDone) { m_boostDone = boostDone; }
	inline void setStepCnt(int stepCnt) { m_stepCnt = stepCnt; }
	inline void setStepDone(int stepDone) { m_stepDone = stepDone; }
	inline void setSlidingCnt(int slidingCnt) { m_slidingCnt = slidingCnt; }
	inline void setSlidingDone(int slidingDone) { m_slidingDone = slidingDone; }
	inline void setDamageCnt(int damageCnt) { m_damageCnt = damageCnt; }
	inline void setHeavy(bool heavy) { m_heavy = heavy; }
	inline void setSoundPlayer(SoundPlayer* soundPlayer) { m_soundPlayer_p = soundPlayer; }

	// ���_���[�W���󂯂Ă��ē����Ȃ�
	inline bool damageFlag() const { return m_state == CHARACTER_STATE::DAMAGE; }

	// squat==true�Ȃ炵�Ⴊ�ށAfalse�Ȃ痧��
	virtual void setSquat(bool squat);

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);
	void setCharacterLeftDirection(bool leftDirection);
	void setCharacterFreeze(bool freeze);

	// �s���O�̏��� ���t���[���s��
	virtual void init();

	// �������Z ���t���[���s��
	virtual void action();

	// �ړ� �����͂S������ �L�����ɂ���Ă͎΂߂Ɉړ��ł��邽�߁B
	virtual void move(bool right, bool left, bool up, bool down) = 0;

	// �W�����v rate%�̗͂Ŕ�яオ��B
	virtual void jump(int rate) = 0;

	// �ˌ��U��
	virtual std::vector<Object*>* bulletAttack(int gx, int gy) = 0;

	// �a���U��
	virtual std::vector<Object*>* slashAttack(int gx, int gy) = 0;

	// �X���C�f�B���O�U��
	virtual std::vector<Object*>* slidingAttack() { return nullptr; }

	// �_���[�W �K�v�ɉ����ăI�[�o�[���C�h
	virtual void damage(int vx, int vy, int damageValue);

	// �ˌ��J�n�̏��� �K�v�ɉ����ăI�[�o�[���C�h
	virtual void startBullet();

	// �ˌ��I���̏��� �K�v�ɉ����ăI�[�o�[���C�h
	virtual void finishBullet();

	// �a���J�n�̏��� �K�v�ɉ����ăI�[�o�[���C�h
	virtual void startSlash();

	// �a���I���̏��� �K�v�ɉ����ăI�[�o�[���C�h
	virtual void finishSlash();

	// �����G���Ԃ���Ȃ�
	bool ableDamage() const;

	// ���U���\���
	virtual bool ableAttack() const;

	// ����������
	virtual bool ableWalk() const;

	// �����]���\ FreezeAI�p
	virtual bool ableChangeDirection() const;

	// �����n�߂�
	void startMoveLeft();
	void startMoveRight();
	void startMoveUp();
	void startMoveDown();

	// �����̂���߂�
	void stopMoveLeft();
	void stopMoveRight();
	void stopMoveUp();
	void stopMoveDown();

	// �L�����̃X�g�b�v���Ԃ�����
	void consumeStopCnt();

	// �L�������X�g�b�v
	void stopCharacter(int cnt);

};


// �N���X������CharacterAction���쐬����֐�
CharacterAction* createAction(const std::string actionName, Character* character, SoundPlayer* soundPlayer_p);


/*
* ����΂Ȃ����ʂ̖_�l��
*/
class StickAction :
	public CharacterAction 
{
private:
	// �d�͉����x
	const int G = 1;

protected:
	// ���֕��� �����͉E�ƍ��̂Q��
	void walk(bool right, bool left);

	// �L�����̉摜�����(state)�ɉ����ĕύX
	void switchHandle();

public:
	static const char* ACTION_NAME;
	const char* getActionName() const { return this->ACTION_NAME; }

	StickAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

	// �������Z ���t���[���s��
	void action();

	// �ړ� �����͂S������
	void move(bool right, bool left, bool up, bool down);

	// �W�����v cnt�t���[����
	void jump(int cnt);

	// �ˌ��U��
	std::vector<Object*>* bulletAttack(int gx, int gy);

	// �a���U��
	std::vector<Object*>* slashAttack(int gx, int gy);

	// �X���C�f�B���O�U��
	std::vector<Object*>* slidingAttack();
};


/*
* ���@���L���A�pAction
*/
class ValkiriaAction :
	public StickAction
{
private:
	// �W�����v�̂��ߎ��Ԃ̍ő�
	const int PRE_JUMP_MAX = 30;

	// �a���U���ɂ��ړ����x
	const int SLASH_MOVE_SPEED = 25;

	bool m_slashNow;

public:
	static const char* ACTION_NAME;
	const char* getActionName() const { return this->ACTION_NAME; }

	ValkiriaAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

	int getPreJumpMax() const { return PRE_JUMP_MAX; }

	inline void setSlashNow(bool slashNow) { m_slashNow = slashNow; }
	void setGrand(bool grand);

	// �a���J�n�̏���
	void startSlash();

	// �a���I���̏���
	void finishSlash();

	// �_���[�W
	void damage(int vx, int vy, int damageValue);
};


/*
* ����ԃL����
*/
class FlightAction :
	public CharacterAction
{
protected:

	void damageAction();
	void otherAction();

	// �L�����̉摜�����(state)�ɉ����ĕύX
	void switchHandle();

	void walk(bool right, bool left, bool up, bool down);

public:
	static const char* ACTION_NAME;
	const char* getActionName() const { return this->ACTION_NAME; }

	FlightAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

	void setSquat(bool squat) {}

	void action();

	// �ړ� �����͂S������
	void move(bool right, bool left, bool up, bool down);

	// �W�����v cnt�t���[����
	void jump(int cnt);

	void setBoost(bool leftDirection);

	// �ˌ��U��
	std::vector<Object*>* bulletAttack(int gx, int gy);

	// �a���U��
	std::vector<Object*>* slashAttack(int gx, int gy);

};


/*
* �R�n���pAction
*/
class KoharuAction :
	public StickAction
{
private:

	// �ˌ��U���ɂ��ړ����x
	const int BULLET_MOVE_SPEED = 2;

public:

	static const char* ACTION_NAME;
	const char* getActionName() const { return this->ACTION_NAME; }

	KoharuAction(Character* character, SoundPlayer* soundPlayer_p);

	CharacterAction* createCopy(std::vector<Character*> characters);

	void debug(int x, int y, int color) const;

	// �ˌ��U��
	std::vector<Object*>* bulletAttack(int gx, int gy);

	void startBullet();

	void finishBullet();

	bool ableAttack() const;

	bool ableWalk() const;

};


class BossFreezeAction :
	public CharacterAction
{
protected:

	// �L�����̉摜��ύX
	void switchHandle();

public:

	static const char* ACTION_NAME;
	virtual const char* getActionName() const { return this->ACTION_NAME; }

	BossFreezeAction(Character* character, SoundPlayer* soundPlayer_p);

	// �R�s�[�쐬
	CharacterAction* createCopy(std::vector<Character*> characters);

	// �f�o�b�O
	void debug(int x, int y, int color) const { }
	// �s���O�̏��� ���t���[���s��
	void init() { }

	// �������Z ���t���[���s��
	void action() { switchHandle(); }

	// �ړ� �����͂S������ �L�����ɂ���Ă͎΂߂Ɉړ��ł��邽�߁B
	void move(bool right, bool left, bool up, bool down) { }

	// �W�����v rate%�̗͂Ŕ�яオ��B
	void jump(int rate) { }

	// �ˌ��U��
	std::vector<Object*>* bulletAttack(int gx, int gy) { return nullptr; }

	// �a���U��
	std::vector<Object*>* slashAttack(int gx, int gy) { return nullptr; }

	// �_���[�W �K�v�ɉ����ăI�[�o�[���C�h
	void damage(int vx, int vy, int damageValue) { }

	// �ˌ��J�n�̏��� �K�v�ɉ����ăI�[�o�[���C�h
	void startBullet() { }

	// �ˌ��I���̏��� �K�v�ɉ����ăI�[�o�[���C�h
	void finishBullet() { }

	// �a���J�n�̏��� �K�v�ɉ����ăI�[�o�[���C�h
	void startSlash() { }

	// �a���I���̏��� �K�v�ɉ����ăI�[�o�[���C�h
	void finishSlash() { }

	// �����G���Ԃ���Ȃ�
	bool ableDamage() const { return false; }

	// ���U���\���
	bool ableAttack() const { return false; }

	// ����������
	bool ableWalk() const { return false; }

	// �����]���\ FreezeAI�p
	bool ableChangeDirection() const { return false; }

};


/*
* Boss1: �T��
*/
class SunAction :
	public FlightAction
{
private:

	// �{�X�̏����A�j���[�V�����J�E���g�p
	int m_initCnt;
	const int NOT_HIDE_CNT = 80;

	// �����̗�
	int m_initHp;

	// ���G���
	bool m_hideFlag;

	int m_startAnimeCnt;

public:
	static const char* ACTION_NAME;
	const char* getActionName() const { return this->ACTION_NAME; }

	SunAction(Character* character, SoundPlayer* soundPlayer_p, bool duplicationFlag);

	CharacterAction* createCopy(std::vector<Character*> characters);

	// �Z�b�^
	inline void setInitCnt(int initCnt) { m_initCnt = initCnt; }
	inline void setInitHp(int initHp) { m_initHp = initHp; }
	inline void setHideFlag(int hideFlag) { m_hideFlag = hideFlag; }
	inline void setStartAnimeCnt(int startAnimeCnt) { m_startAnimeCnt = startAnimeCnt; }

	void action();

protected:

	// ��Ԃɉ����ĉ摜�Z�b�g
	void switchHandle();

};


#endif