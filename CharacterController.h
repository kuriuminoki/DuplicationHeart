#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class Character;
class CharacterAction;
class Object;
class Camera;


// Controller�ɖ��߂���N���X�i�L�[�{�[�h���}�E�X��`�h�j
class Brain {
protected:
	// ���̃L�����̏�Ԃ��l�����Ďˌ��̖ڕW���W�⎟�̍s�������߂邽�ߕK�v
	const CharacterAction* m_characterAction_p;

public:
	Brain();

	virtual void debug(int x, int y, int color) const = 0;

	// �b�������������������
	virtual bool actionOrder() { return false; }

	// �Z�b�^
	virtual void setCharacterAction(const CharacterAction* characterAction) = 0;
	
	// �������U���̖ڕW���W
	virtual void bulletTargetPoint(int& x, int& y) = 0;

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
	virtual void searchTarget(const Character* character) { };

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchTarget() const { return false; }

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	virtual void searchFollow(const Character* character) { };

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	virtual bool needSearchFollow() const { return false; }
};

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
	KeyboardBrain(const Camera* camera);
	void debug(int x, int y, int color) const;
	inline void setCharacterAction(const CharacterAction* characterAction) { m_characterAction_p = characterAction; }
	void bulletTargetPoint(int& x, int& y);
	bool actionOrder();
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};

/*
*  ���ʂɓG�Ɛ키�悤���߂���`�h�̃N���X
*/
class NormalAI :
	public Brain
{
private:
	// �U���Ώ�
	const Character* m_target_p;

	// �ړ��p
	int m_rightKey, m_leftKey, m_upKey, m_downKey;

	// �W�����v�̒���
	int m_jumpCnt;

	// ���Ⴊ�ޒ���
	int m_squatCnt;

protected:
	// �ˌ��̐��x
	const int BULLET_ERROR = 400;

	// �ړ��ڕW
	int m_gx, m_gy;

	// �ړ��ڕW�B���Ƃ݂Ȃ��덷 �}GX_ERROR
	const int GX_ERROR = 100;

	// �ړ�����
	int m_moveCnt;

	// �ړ�����߂�܂ł̎���
	const int GIVE_UP_MOVE_CNT = 300;

public:
	NormalAI();
	void debug(int x, int y, int color) const;
	void setCharacterAction(const CharacterAction* characterAction);
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();

	// �U���Ώۂ����߂�(target�̂܂܂��Acharacter�ɕύX���邩)
	void searchTarget(const Character* character);

	// �U���Ώۂ�ύX����K�v������Ȃ�true�ŃA�s�[������B
	bool needSearchTarget() const;

protected:
	// �X�e�B�b�N����
	void stickOrder(int& right, int& left, int& up, int& down);
};


class FollowNormalAI :
	public NormalAI 
{
private:
	// ���Ă����L����
	const Character* m_follow_p;
public:
	FollowNormalAI();

	void debug(int x, int y, int color) const;

	int getFollowId() const;

	// �ړ��̖ڕW�n�_�ݒ�
	void moveOrder(int& right, int& left, int& up, int& down);

	// �ǐՑΏۂ����߂�(AI�N���X�ŃI�[�o���C�h����B)
	void searchFollow(const Character* character);

	// �ǐՑΏۂ�ύX����K�v������Ȃ�true�ŃA�s�[������(AI�N���X�ŃI�[�o���C�h����)�B
	bool needSearchFollow() const;
};



/*
* �R���g���[���̊��N���X�i�L�����N�^�[�𑀍삷��N���X�j
*/
class CharacterController {
protected:
	// ����������𖽗߂��Ă��� Controller���f���[�g����
	Brain* m_brain;

	// ����Ώ� Controller���f���[�g����
	CharacterAction* m_characterAction;

public:
	CharacterController();
	CharacterController(Brain* brain, CharacterAction* characterAction);
	~CharacterController();

	// �f�o�b�O
	void debugController(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline const CharacterAction* getAction() const { return m_characterAction; }
	inline const Brain* getBrain() const { return m_brain; }

	// �b������������ɓ������肷��{�^����true��
	virtual bool getActionKey() const { return m_brain->actionOrder(); }

	// �A�N�V�����̃Z�b�^
	void setCharacterGrand(bool grand);
	void setCharacterGrandRightSlope(bool grand);
	void setCharacterGrandLeftSlope(bool grand);
	void setActionRightLock(bool lock);
	void setActionLeftLock(bool lock);
	void setActionUpLock(bool lock);
	void setActionDownLock(bool lock);
	void setActionBoost();

	// �L�����N�^�[�̃Z�b�^
	void setCharacterX(int x);
	void setCharacterY(int y);

	// �s���O�̏��� ���t���[���s��
	void init();

	// �U���Ώۂ�ύX
	void searchTargetCandidate(Character* character);

	// �ǐՑΏۂ�ύX
	void searchFollowCandidate(Character* character);

	// ����ⓖ���蔻��̌��ʂ𔽉f�i���ۂɃL�����𓮂����j���t���[���s��
	void action();

	// �L�����̑���
	virtual void control() = 0;

	// �ˌ��U��
	virtual Object* bulletAttack() = 0;

	// �a���U��
	virtual Object* slashAttack() = 0;

	// �_���[�W
	virtual void damage(int vx, int vy, int damageValue) = 0;
};

/*
* ���ʂ̃R���g���[��
*/
class NormalController :
	public CharacterController {

private:
	// �W�����v�L�[�𒷉�������ő厞��
	const int JUMP_KEY_LONG = 10;
public:
	NormalController(Brain* brain, CharacterAction* characterAction);

	void debug(int x, int y, int color) const;

	// �L�����̈ړ��₵�Ⴊ��(;�U���ȊO��)���� ���t���[���s��
	void control();

	// �ˌ��U��
	Object* bulletAttack();

	// �a���U��
	Object* slashAttack();

	// �_���\�W
	void damage(int vx, int vy, int damageValue);
};

#endif
