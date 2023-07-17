#ifndef CHACACTER_ACTION_H_INCLUDED
#define CHACACTER_ACTION_H_INCLUDED

class Character;
class Object;


// �L�����N�^�[�̏��
enum class CHARACTER_STATE {
	STAND,	// �������Ă��Ȃ�
	DAMAGE,	// �_���[�W�󂯒� ���n�ŉ���
	BULLET,	// �ˌ���
	SLASH,	// �a����
	PREJUMP,	// �W�����v�O
};


/*
* �L�����N�^�[�𓮂����N���X
* Controller���g�p����B����ȊO�̃C���X�^���X����g���邱�Ƃ͂Ȃ��B
*/
class CharacterAction {
protected:
	// �������L�����N�^�[
	Character* m_character;

	// �L�����̏��
	CHARACTER_STATE m_state;

	// �L�������n�ʂɂ���
	bool m_grand;

	// �L�����������Ă��Ȃ��Ȃ�-1 �����łȂ��Ȃ瑖�����t���[����
	int m_runCnt;

	// ���Ⴊ�ݒ�
	bool m_squat;

	// �W�����v�O�̓���
	int m_preJumpCnt;

	// �W�����v�̂��ߎ��Ԃ̍ő�
	const int PRE_JUMP_MAX = 10;

	// ���n���[�V�����̎c�莞��
	int m_landCnt;

	// ���n���[�V�����̑�����
	const int LAND_TIME = 10;

	int m_boostCnt;
	const int BOOST_TIME = 10;

	// �ړ���
	bool m_moveRight;
	bool m_moveLeft;
	bool m_moveUp;
	bool m_moveDown;

	// �����x
	int m_vx;
	int m_vy;

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

public:
	CharacterAction();
	CharacterAction(Character* character);

	// �f�o�b�O
	void debugAction(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �Q�b�^�ƃZ�b�^
	inline CHARACTER_STATE getState() const { return m_state; }
	inline bool getGrand() const { return m_grand; }
	inline void setGrand(bool grand) { 
		if (m_vy > 0) { // ���n���[�V�����ɂȂ�
			m_landCnt = LAND_TIME;
		}
		m_grand = grand;
	}
	inline int getVx() const { return m_vx; }
	inline int getVy() const { return m_vy; }
	inline int getSlashCnt() { return m_slashCnt; }
	bool getRightLock() const { return m_rightLock; }
	bool getLeftLock() const { return m_leftLock; }
	bool getUpLock() const { return m_upLock; }
	bool getDownLock() const { return m_downLock; }
	void setRightLock(bool lock);
	void setLeftLock(bool lock);
	void setUpLock(bool lock);
	void setDownLock(bool lock);
	inline void setBoost() { m_boostCnt = BOOST_TIME; }
	inline const Character* getCharacter() const { return m_character; }

	// squat==true�Ȃ炵�Ⴊ�ށAfalse�Ȃ痧��
	void setSquat(bool squat);

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);
	void setCharacterLeftDirection(bool leftDirection);

	// �s���O�̏��� ���t���[���s��
	virtual void init() = 0;

	// �������Z ���t���[���s��
	virtual void action() = 0;

	// �L�����̉摜��ύX
	virtual void switchHandle() = 0;

	// �ړ� �����͂S������ �L�����ɂ���Ă͎΂߂Ɉړ��ł��邽�߁B
	virtual void move(bool right, bool left, bool up, bool down) = 0;

	// �W�����v rate%�̗͂Ŕ�яオ��B
	virtual void jump(int rate) = 0;

	// �ˌ��U��
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// �a���U��
	virtual Object* slashAttack() = 0;

protected:
	// �摜�̃T�C�Y�ύX�ɂ��ʒu����
	void afterChangeGraph(int beforeWide, int beforeHeight, int afterWide, int afterHeight);
};


/*
* ����΂Ȃ����ʂ̖_�l��
*/
class StickAction :
	public CharacterAction 
{
private:
	// �d�͉����x
	const int G = 1;

private:
	// ���֕��� �����͉E�ƍ��̂Q��
	void walk(bool right, bool left);

	// �L�����̉摜�����(state)�ɉ����ĕύX
	void switchHandle();

public:
	StickAction(Character* character);

	void debug(int x, int y, int color);

	//�s���O�̏��� ���t���[���s��
	void init();

	// �������Z ���t���[���s��
	void action();

	// �ړ� �����͂S������
	void move(bool right, bool left, bool up, bool down);

	// �W�����v cnt�t���[����
	void jump(int cnt);

	// �ˌ��U��
	Object* bulletAttack(int gx, int gy);

	// �a���U��
	Object* slashAttack();
};

#endif