#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class CharacterAction;
class Object;
class Camera;


// Controller�ɖ��߂���N���X�i�L�[�{�[�h���}�E�X��`�h�j
class Brain {
protected:
	// ���̃L�����̏�Ԃ��l�����Ďˌ��̖ڕW���W�⎟�̍s�������߂邽�ߕK�v
	const CharacterAction* m_characterAction;

public:
	Brain();

	// �Z�b�^
	void setCharacterAction(const CharacterAction* characterAction) { m_characterAction = characterAction; }
	
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
};

/*
* �L�[�{�[�h�ŃL�����̑���𖽗߂���N���X
*/
class KeyboardBrain :
	public Brain
{
private:
	// �J����
	const Camera* m_camera;

public:
	KeyboardBrain(const Camera* camera);
	void bulletTargetPoint(int& x, int& y);
	void moveOrder(int& right, int& left, int& up, int& down);
	int jumpOrder();
	int squatOrder();
	int slashOrder();
	int bulletOrder();
};

/*
*  ���ʂɓG�Ɛ키�悤���߂���`�h�̃N���X
*/
//class NormalAI :
//	public Brain
//{
//private:
//
//public:
//	NormalAI(CharacterAction* characterAction);
//	void moveOrder(int& right, int& left, int& up, int& down);
//	int jumpOrder();
//	int squatOrder();
//	int slashOrder();
//	int bulletOrder();
//};



/*
* �R���g���[���̊��N���X�i�L�����N�^�[�𑀍삷��N���X�j
*/
class CharacterController {
protected:
	// ����������𖽗߂��Ă���
	Brain* m_brain;

	// ����Ώ�
	CharacterAction* m_characterAction;

public:
	CharacterController();
	CharacterController(Brain* brain, CharacterAction* characterAction);
	~CharacterController();

	// �f�o�b�O
	void debugController(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �A�N�V�����̏��擾
	inline const CharacterAction* getAction() const { return m_characterAction; }

	// �A�N�V�����̃Z�b�^
	void setCharacterGrand(bool grand);
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

	// �L�����̑���
	virtual void control() = 0;

	// ����ⓖ���蔻��̌��ʂ𔽉f�i���ۂɃL�����𓮂����j���t���[���s��
	void action();

	// �ˌ��U��
	virtual Object* bulletAttack() = 0;

	// �a���U��
	virtual Object* slashAttack() = 0;
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

	void debug(int x, int y, int color);

	// �L�����̈ړ��₵�Ⴊ��(;�U���ȊO��)���� ���t���[���s��
	void control();

	// �ˌ��U��
	Object* bulletAttack();

	// �a���U��
	Object* slashAttack();
};

#endif
