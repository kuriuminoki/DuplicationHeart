#ifndef BRAIN_H_INCLUDED
#define BRAIN_H_INCLUDED

#include <vector>
#include <string>


class Character;
class CharacterAction;
class Camera;


// Controller�ɖ��߂���N���X�i�L�[�{�[�h���}�E�X��`�h�j
class Brain {
protected:
	// ���̃L�����̏�Ԃ��l�����Ďˌ��̖ڕW���W�⎟�̍s�������߂邽�ߕK�v
	const CharacterAction* m_characterAction_p;

public:
	static const char* BRAIN_NAME;
	virtual const char* getBrainName() const { return this->BRAIN_NAME; }

	Brain();
	virtual ~Brain(){}

	virtual Brain* createCopy(std::vector<Character*> characters, const Camera* camera) = 0;

	virtual void debug(int x, int y, int color) const = 0;

	// �b�������������������
	virtual bool actionOrder() { return false; }

	// �Z�b�^
	virtual void setGx(int gx) { }
	virtual void setGy(int gy) { }
	virtual void setCharacterAction(const CharacterAction* characterAction) = 0;

	// �������U���̖ڕW���W
	virtual void bulletTargetPoint(int& x, int& y) = 0;

	// �ߋ����U���̖ڕW���W
	virtual void slashTargetPoint(int& x, int& y) = 0;

	// �ړ��i�㉺���E�̓��́j
	virtual void moveOrder(int& right, int& left, int& up, int& down) = 0;

	// �W�����v�̐���
	virtual int jumpOrder() = 0;

	// ���Ⴊ�݂̐���
	virtual int squatOrder() = 0;

	// �ߋ����U��
	virtual int slashOrder() = 0;

	// �������U��
	virtual int bulletOrder() = 0;

	// �U���Ώۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	virtual void searchTarget(const Character* character) { }

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchTarget() const { return false; }

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	virtual void searchFollow(const Character* character) { }

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchFollow() const { return false; }

	// �U���Ώۂ�ǐՑΏۂ̏����擾
	virtual int getTargetId() const { return -1; }
	virtual const char* getTargetName() const { return ""; }
	virtual int getFollowId() const { return -1; }
	virtual const char* getFollowName() const { return ""; }
	virtual const Character* getFollow() const { return nullptr; }

	virtual void setTarget(Character* character) {  }

	// �ǐՑΏۂ̋߂��ɂ��邩����
	virtual bool checkAlreadyFollow() { return true; }

	// �ڕW�n�_�ֈړ����邾�� �B���ς݂Ȃ�true�ŉ������Ȃ�
	virtual bool moveGoalOrder(int& right, int& left, int& up, int& down, int& jump) { return true; }
};


// �N���X������Brain���쐬����֐�
Brain* createBrain(const std::string brainName, const Camera* camera_p);


/*
* �L�[�{�[�h�ŃL�����̑���𖽗߂���N���X
*/
class KeyboardBrain :
	public Brain
{
private:
	// �J����
	const Camera* m_camera_p;

public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	KeyboardBrain(const Camera* camera);

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera) { return new KeyboardBrain(camera); }

	void debug(int x, int y, int color) const;

	// �Z�b�^
	inline void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }

	void bulletTargetPoint(int& x, int& y);
	void slashTargetPoint(int& x, int& y);
	bool actionOrder();
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};


/*
* �S�������Ȃ�AI
*/
class Freeze :
	public Brain
{
public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	Freeze() { }

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera) { return new Freeze(); }

	void debug(int x, int y, int color) const { }

	// �Z�b�^
	void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }

	bool actionOrder() { return false; }
	void bulletTargetPoint(int& x, int& y) {  }
	void slashTargetPoint(int& x, int& y) {  }
	void moveOrder(int& right, int& left, int& up, int& down) { right = 0; left = 0; up = 0; down = 0; }
	int jumpOrder() { return 0; }
	int squatOrder() { return 0; }
	int slashOrder() { return 0; }
	int bulletOrder() { return 0; }
};


/*
*  ���ʂɓG�Ɛ키�悤���߂���`�h�̃N���X
*/
class NormalAI :
	public Brain
{
protected:
	// �ړ��p
	int m_rightKey, m_leftKey, m_upKey, m_downKey;

	// �W�����v�̒���
	int m_jumpCnt;

	// ���Ⴊ�ޒ���
	int m_squatCnt;

	// �U���Ώۂ�F�m���鋗��
	const int TARGET_DISTANCE = 2000;

	// �U���Ώ�
	const Character* m_target_p;

	// �ˌ��̐��x
	const int BULLET_ERROR = 400;

	// ���ɈӖ��̂Ȃ��ړ�������m��
	const int MOVE_RAND = 59;

	// �ړ��ڕW
	int m_gx, m_gy;

	// �ړ��ڕW�B���Ƃ݂Ȃ��덷 �}GX_ERROR
	const int GX_ERROR = 100;
	const int GY_ERROR = 50;

	// �ړ�����
	int m_moveCnt;

	// �ړ�����߂�܂ł̎���
	const int GIVE_UP_MOVE_CNT = 300;

public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	NormalAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);
	void setParam(NormalAI* brain);

	void debug(int x, int y, int color) const;

	// �Z�b�^
	void setRightKey(int rightKey) { m_rightKey = rightKey; }
	void setLeftKey(int leftKey) { m_leftKey = leftKey; }
	void setUpKey(int upKey) { m_upKey = upKey; }
	void setDownKey(int downKey) { m_downKey = downKey; }
	void setJumpCnt(int cnt) { m_jumpCnt = cnt; }
	void setSquatCnt(int cnt) { m_squatCnt = cnt; }
	void setGx(int gx) { m_gx = gx; }
	void setGy(int gy) { m_gy = gy; }
	void setMoveCnt(int cnt) { m_moveCnt = cnt; }
	void setTarget(Character* character) { m_target_p = character; }
	void setCharacterAction(const CharacterAction* characterAction);

	void bulletTargetPoint(int& x, int& y);
	void slashTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	void moveUpDownOrder(int x, int y, bool& tryFlag);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();

	// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
	void searchTarget(const Character* character);

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
	bool needSearchTarget() const;

	// �U���Ώۂ̏����擾�i�I�[�o�[���C�h�j
	int getTargetId() const;
	const char* getTargetName() const;

	// �ڕW�n�_�ֈړ����邾�� �B���ς݂Ȃ�true�ŉ������Ȃ�
	bool moveGoalOrder(int& right, int& left, int& up, int& down, int& jump);

protected:
	// �X�e�B�b�N����
	void stickOrder(int& right, int& left, int& up, int& down);
};


/*
* ���Ԃɂ��Ă����키NormalAI
*/
class FollowNormalAI :
	public NormalAI
{
protected:
	// ���Ă����L����
	const Character* m_follow_p;

	// �ǐՑΏۂ̋߂��ɂ���Ƃ݂Ȃ��덷 �}GX_ERROR
	const int FOLLOW_X_ERROR = 600;

public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	FollowNormalAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void debug(int x, int y, int color) const;

	// �ǐՑΏۂ̏����擾�i�I�[�o�[���C�h�j
	int getFollowId() const;
	const char* getFollowName() const;
	const Character* getFollow() const;

	// �ǐՑΏۂ��Z�b�g
	void setFollow(Character* character) { m_follow_p = character; }

	// �ړ��̖ڕW�n�_�ݒ�
	void moveOrder(int& right, int& left, int& up, int& down);

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	void searchFollow(const Character* character);

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	bool needSearchFollow() const;

	// �ǐՑΏۂ̋߂��ɂ��邩����
	bool checkAlreadyFollow();
};


/*
* ParabolaBullet���g��AI
*/
class ParabolaAI :
	public NormalAI
{
public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	ParabolaAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void bulletTargetPoint(int& x, int& y);
};


/*
* ParabolaBullet���g��AI
*/
class FollowParabolaAI :
	public FollowNormalAI
{
public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	FollowParabolaAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void bulletTargetPoint(int& x, int& y);
};


/*
* ���@���L���A�pAI �a���̊ԍ�����n�[�g�Ƃ̋����ɋC��t����
*/
class ValkiriaAI :
	public FollowNormalAI
{
private:

	const int SLASH_REACH = 1200;

public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	ValkiriaAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	int slashOrder();
	int bulletOrder() { return 0; }
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
	bool needSearchTarget() const;

};


/*
* ����ԃL�����p��AI
*/
class FlightAI :
	public NormalAI
{
private:
	// �ǂɂԂ������Ƃ��Atrue�ɂ��ďォ���ֈړ�����Btrue�̂Ƃ��V��⏰�ɂԂ����Ă�����t�ֈړ�
	bool m_try;
public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	FlightAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void moveOrder(int& right, int& left, int& up, int& down);

	// �ڕW�n�_�ֈړ����邾�� �B���ς݂Ȃ�true�ŉ������Ȃ�
	bool moveGoalOrder(int& right, int& left, int& up, int& down, int& jump);
};


/*
* ����ԒǐՃL����AI
*/
class FollowFlightAI :
	public FollowNormalAI
{
private:
	// �ǂɂԂ������Ƃ��Atrue�ɂ��ďォ���ֈړ�����Btrue�̂Ƃ��V��⏰�ɂԂ����Ă�����t�ֈړ�
	bool m_try;
public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	FollowFlightAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	void moveOrder(int& right, int& left, int& up, int& down);

	// �ڕW�n�_�ֈړ����邾�� �B���ς݂Ȃ�true�ŉ������Ȃ�
	bool moveGoalOrder(int& right, int& left, int& up, int& down, int& jump);
};


/*
* �_�����Ŏˌ�����q�G�����L�[
*/
class HierarchyAI :
	public NormalAI
{
public:

	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	HierarchyAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	int bulletOrder() { return 1; }
	void bulletTargetPoint(int& x, int& y);

	void moveOrder(int& right, int& left, int& up, int& down) { return; }
	int jumpOrder() { return 0; }
	int squatOrder() { m_squatCnt = 0; return 0; }

};


/*
* �t�����`�pAI
*/
class FrenchAI :
	public NormalAI
{
private:

	const int SLASH_REACH = 1500;

public:
	static const char* BRAIN_NAME;
	const char* getBrainName() const { return this->BRAIN_NAME; }

	FrenchAI();

	Brain* createCopy(std::vector<Character*> characters, const Camera* camera);

	int slashOrder();
	int bulletOrder() { return 0; }
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder() { return 0; }
	int squatOrder() { m_squatCnt = 0; return 0; }
};


#endif