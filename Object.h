#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

class Character;
class CharacterController;
class AttackInfo;
class GraphHandle;
class GraphHandles;
class Animation;


/*
* �I�u�W�F�N�g�̊��N���X
*/
class Object {
protected:
	// ����̍��W
	int m_x1, m_y1;

	// �E���̍��W+1
	int m_x2, m_y2;

	// �폜�t���O true�Ȃ�World�ɏ����Ă��炤
	bool m_deleteFlag;

	// �U���ō폜�\�ȃI�u�W�F�N�g�Ȃ�true
	bool m_ableDelete;

	// �G�t�F�N�g
	GraphHandles* m_effectHandles;

public:
	Object();
	Object(int x1, int y1, int x2, int y2);

	void debugObject(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline bool getDeleteFlag() { return m_deleteFlag; }
	inline bool getAbleDelete() { return m_ableDelete; }
	inline int getX1() const { return m_x1; }
	inline int getX2() const { return m_x2; }
	inline int getY1() const { return m_y1; }
	inline int getY2() const { return m_y2; }

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	virtual GraphHandle* getHandle() const = 0;

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	virtual void drawObject(int x1, int y1, int x2, int y2) const = 0;

	// �Z�b�^
	inline void setDeleteFlag(bool deleteFlag) { m_deleteFlag = deleteFlag; }

	// �L�����Ƃ̓����蔻��
	virtual bool atari(CharacterController* character) = 0;

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	virtual void penetration(CharacterController* characterController) = 0;

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	virtual void atariObject(Object* object) = 0;

	// �����I�u�W�F�N�g�p ���t���[���s��
	virtual void action() = 0;

	// �A�j���[�V�����쐬
	virtual Animation* createAnimation(const Character* character) = 0;
};


/*
* �l�p�`�̃I�u�W�F�N�g�@�����
*/
class BoxObject :
	public Object 
{
private:
	// �i���Ƃ݂Ȃ��Ď����ŏ��z�����鍂��
	const int STAIR_HEIGHT = 50;

	// �I�u�W�F�N�g�̐F
	int m_color;

public:
	BoxObject(int x1, int y1, int x2, int y2, int color);

	void debug(int x, int y, int color) const;

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	GraphHandle* getHandle() const { return nullptr; }

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* character);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController);

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	void atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(const Character* character) { return nullptr; }
};

/*
* ���p�O�p�`�̃I�u�W�F�N�g�܂��
*/
class TriangleObject :
	public Object
{
private:
	// �I�u�W�F�N�g�̐F
	int m_color;

	// �������ɉ������Ă����
	bool m_leftDown;

	// ���WX�ɂ�����Y���W�i�X������Z�o����j
	int getY(int x) const;
public:
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown);

	void debug(int x, int y, int color) const;

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	GraphHandle* getHandle() const { return nullptr; }

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* character);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController);

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	void atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(const Character* character) { return nullptr; }
};

/*
* ���i����e�̃I�u�W�F�N�g
* gx, gy�Ɍ������Ĕ��ł����A��苗���ړ����������
*/
class BulletObject :
	public Object
{
private:
	// ���̍U�����o�����L�����̂h�c ���Ŗh�~�p
	int m_characterId;

	// ���̍U����������Ȃ��O���[�v��ID �`�[���L���h�~�p
	int m_groupId;

	// �I�u�W�F�N�g�̐F
	int m_color;

	// ���a
	int m_rx;
	int m_ry;

	// �X�s�[�h
	int m_v;
	int m_vx;
	int m_vy;

	// �ڕW�n�_
	int m_gx;
	int m_gy;

	// �c��̔򋗗�
	int m_d;

	// �_���[�W
	int m_damage;

public:
	// x, y, gx, gy�͒e�̒��S���W
	BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo, GraphHandles* effectHandles);

	void debug(int x, int y, int color) const;

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	GraphHandle* getHandle() const { return nullptr; }

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �Z�b�^
	// �L�����N�^�[ID���擾
	inline void setCharacterId(int id) { m_characterId = id; }

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* character);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController) {};

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	void atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(const Character* character);
};


/*
* �ߋ����U���̃I�u�W�F�N�g
* ��莞�Ԍo�߂��������
*/
class SlashObject :
	public Object
{
private:
	// ���̍U�����o�����L�����̂h�c ���ł�`�[���L���h�~�p
	int m_characterId;

	// �I�u�W�F�N�g�̉摜
	GraphHandle* m_handle;

	// �_���[�W
	int m_damage;

	// ���t���[���ڂ�
	int m_cnt;

	// �S�̃t���[��
	int m_slashCountSum;

	// �a���̐������(X�����̏���)
	int m_slashImpactX;

	// �a���̐������(Y�����̏���)
	int m_slashImpactY;

public:
	// ���W�A�摜�A�������ԁAAttackInfo
	SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo, GraphHandles* effectHandles);

	// �傫�����w�肵�Ȃ��ꍇ�B�摜����T�C�Y�擾�B�������ԁAAttackInfo
	SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo, GraphHandles* effectHandles);

	void debug(int x, int y, int color) const;

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	GraphHandle* getHandle() const { return m_handle; }

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �Z�b�^
	inline void setCharacterId(int id) { m_characterId = id; }

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* character);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController) {};

	// ���I�u�W�F�N�g�Ƃ̓����蔻��
	void atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(const Character* character);
};

#endif