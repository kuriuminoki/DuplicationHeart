#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <string>

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

	// HP -1�Ȃ疳�G
	int m_hp;

	// �_���[�W��ԁi�`��p�j
	int m_damageCnt;
	const int DAMAGE_CNT_SUM = 5;

	// �폜�t���O true�Ȃ�World�ɏ����Ă��炤
	bool m_deleteFlag;

	// �G�t�F�N�g
	GraphHandles* m_effectHandles_p;

	// �T�E���h
	int m_soundHandle_p;

public:
	Object();
	Object(int x1, int y1, int x2, int y2, int hp = -1);

	void debugObject(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline bool getDeleteFlag() const { return m_deleteFlag; }
	inline bool getAbleDelete() const { return m_hp != -1 ? true : false; }
	inline int getX1() const { return m_x1; }
	inline int getX2() const { return m_x2; }
	inline int getY1() const { return m_y1; }
	inline int getY2() const { return m_y2; }
	inline int getCenterX() const { return (m_x1 + m_x2) / 2; }
	inline int getCenterY() const { return (m_y1 + m_y2) / 2; }
	inline int getSoundHandle() const { return m_soundHandle_p; }
	inline int getHp() const { return m_hp; }
	inline int getDamageCnt() const { return m_damageCnt; }

	// �Z�b�^
	inline void setDeleteFlag(bool deleteFlag) { m_deleteFlag = deleteFlag; }

	// HP�����炷
	void decreaseHp(int damageValue);

	// �O���[�vID�̃Q�b�^
	virtual inline int getGroupId() const { return -1; }

	// �U���� �U���I�u�W�F�N�g�p
	virtual inline int getDamage() const { return 0; }

	// ���p
	virtual inline int getAreaNum() const { return -1; }

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	virtual GraphHandle* getHandle() const { return nullptr; }

	// �e�L�X�g��Ԃ� �Ȃ��Ȃ�NULL
	virtual inline std::string getText() const { return ""; }

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	virtual void drawObject(int x1, int y1, int x2, int y2) const {};

	// �L�����Ƃ̓����蔻��
	virtual bool atari(CharacterController* characterController) = 0;

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	virtual void penetration(CharacterController* characterController) {};

	// �U���I�u�W�F�N�g�Ƃ̓����蔻��
	virtual bool atariObject(Object* object) { return false; }

	// �����I�u�W�F�N�g�p ���t���[���s��
	virtual void action() {};

	// �A�j���[�V�����쐬
	virtual Animation* createAnimation(int x, int y, int flameCnt) { return nullptr; };
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
	BoxObject(int x1, int y1, int x2, int y2, int color, int hp = -1);

	void debug(int x, int y, int color) const;

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* characterController);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController);

	// �U���I�u�W�F�N�g�Ƃ̓����蔻��
	bool atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();
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
	TriangleObject(int x1, int y1, int x2, int y2, int color, bool leftDown, int hp = -1);

	void debug(int x, int y, int color) const;

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* characterController);

	// �L�������I�u�W�F�N�g�ɓ��荞��ł���Ƃ��̏���
	void penetration(CharacterController* characterController);

	// �U���I�u�W�F�N�g�Ƃ̓����蔻��
	bool atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();
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
	BulletObject(int x, int y, int color, int gx, int gy, AttackInfo* attackInfo);

	void debug(int x, int y, int color) const;

	// �I�u�W�F�N�g�`��i�摜���Ȃ��Ƃ��Ɏg���j
	void drawObject(int x1, int y1, int x2, int y2) const;

	// �Q�b�^
	inline int getGroupId() const { return m_groupId; }

	// �Z�b�^
	// �L�����N�^�[ID���Z�b�g
	inline void setCharacterId(int id) { m_characterId = id; }
	inline void setGroupId(int id) { m_groupId = id; }

	// �U���� �U���I�u�W�F�N�g�p
	inline int getDamage() const { return m_damage; }

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* characterController);

	// �U���I�u�W�F�N�g�Ƃ̓����蔻��
	bool atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(int x, int y, int flameCnt);
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

	// ���̍U����������Ȃ��O���[�v��ID �`�[���L���h�~�p
	int m_groupId;

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
	SlashObject(int x1, int y1, int x2, int y2, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	// �傫�����w�肵�Ȃ��ꍇ�B�摜����T�C�Y�擾�B�������ԁAAttackInfo
	SlashObject(int x, int y, GraphHandle* handle, int slashCountSum, AttackInfo* attackInfo);

	void debug(int x, int y, int color) const;

	// �摜��Ԃ��@�Ȃ��Ȃ�NULL
	GraphHandle* getHandle() const { return m_handle; }

	// �Q�b�^
	inline int getGroupId() const { return m_groupId; }

	// �Z�b�^
	inline void setCharacterId(int id) { m_characterId = id; }
	inline void setGroupId(int id) { m_groupId = id; }

	// �U���� �U���I�u�W�F�N�g�p
	inline int getDamage() const { return m_damage; }

	// �L�����Ƃ̓����蔻��
	// �������Ă���Ȃ�L�����𑀍삷��B
	bool atari(CharacterController* characterController);

	// �U���I�u�W�F�N�g�Ƃ̓����蔻��
	bool atariObject(Object* object);

	// �����I�u�W�F�N�g�p ���t���[���s��
	void action();

	// �A�j���[�V�����쐬
	Animation* createAnimation(int x, int y, int flameCnt);
};


// ���I�u�W�F�N�g
class DoorObject :
	public Object 
{
private:
	// �摜
	GraphHandle* m_graph;

	// �s��̃G���A�ԍ�
	int m_areaNum;

	// �`���[�g���A���̃e�L�X�g
	std::string m_text;

public:
	DoorObject(int x1, int y1, int x2, int y2, const char* fileName, int areaNum);
	~DoorObject();

	void debug(int x, int y, int color) const;

	// �Q�b�^
	GraphHandle* getHandle() const { return m_graph; }
	inline int getAreaNum() const { return m_areaNum; }
	inline std::string getText() const { return m_text; }

	// �L�����Ƃ̓����蔻��
	virtual bool atari(CharacterController* characterController);
};

#endif