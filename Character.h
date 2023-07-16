#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include<string>

class Object;
class GraphHandle;
class GraphHandles;


class CharacterInfo {
private:
	// �L�����̖��O
	std::string m_name;

	// �L�����̏����̗�
	int m_maxHp;

	// �摜�̊g�嗦
	double m_handleEx;

	// ����X�s�[�h
	int m_moveSpeed;

	// �W�����v����Y�����̏���
	int m_jumpHeight;

public:
	// �f�t�H���g�l�ŏ�����
	CharacterInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	CharacterInfo(const char* characterName);

	// �Q�b�^�݂̂�����
	inline std::string name() { return m_name; }
	inline int maxHp() { return m_maxHp; }
	inline double handleEx() { return m_handleEx; }
	inline int moveSpeed() { return m_moveSpeed; }
	inline int jumpHeight() { return m_jumpHeight; }
};


class AttackInfo {
private:
	// �e�ۂ̃_���[�W
	int m_bulletDamage;

	// �e�ۂ̑傫��(���a)
	int m_bulletRx, m_bulletRy;

	// �e�ۂ̃X�s�[�h
	int m_bulletSpeed;

	// �e�ۂ̘A�˗�
	int m_bulletRapid;

	// �e�ۂ̔򋗗�
	int m_bulletDistance;

	// �e�ۂ̐������(X�����̏���)
	int m_bulletImpactX;

	// �e�ۂ̐������(Y�����̏���)
	int m_bulletImpactY;

	// �a���̃_���[�W
	int m_slashDamage;

	// �a���̑傫��(�����`�̕ӂ̒���)
	int m_slashLenX, m_slashLenY;

	// �a���̑S�̃t���[��
	int m_slashCountSum;

	// �a���̐������(X�����̏���)
	int m_slashImpactX;

	// �a���̐������(Y�����̏���)
	int m_slashImpactY;

public:
	// �f�t�H���g�l�ŏ�����
	AttackInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	AttackInfo(const char* characterName);
	
	// �Q�b�^�݂̂�����
	int bulletDamage() { return m_bulletDamage; }
	int bulletRx() { return m_bulletRx; }
	int bulletRy() { return m_bulletRy; }
	int bulletSpeed() { return m_bulletSpeed; }
	int bulletRapid() { return m_bulletRapid; }
	int bulletDistance() { return m_bulletDistance; }
	int bulletImpactX() { return m_bulletImpactX; }
	int bulletImpactY() { return m_bulletImpactY; }
	int slashDamage() { return m_slashDamage; }
	int slashLenX() { return m_slashLenX; }
	int slashLenY() { return m_slashLenY; }
	int slashCountSum() { return m_slashCountSum; }
	int slashImpactX() { return m_slashImpactX; }
	int slashImpactY() { return m_slashImpactY; }
};


/*
* �v���C���[��G�l�~�[�̊��N���X
*/
class Character {
protected:
	static int characterId;

	// ID
	int m_id;

	// �O���[�vID �������ʗp
	int m_groupId;

	// �ő�̗�
	int m_maxHp;

	// �c��̗�
	int m_hp;

	// X���W�AY���W
	int m_x, m_y;

	// �����Əc��
	int m_wide, m_height;

	// �\�������摜
	GraphHandle* m_graphHandle;

	// �L�����̏��
	CharacterInfo* m_characterInfo;

	// �U���̏��
	AttackInfo* m_attackInfo;

	// ���������Ă���
	bool m_leftDirection;

public:
	// �R���X�g���N�^
	Character();
	Character(int maxHp, int hp, int x, int y);
	~Character();

	// �f�o�b�O
	void debugCharacter(int x, int y, int color);
	virtual void debug(int x, int y, int color) = 0;

	// �Q�b�^�ƃZ�b�^
	inline int getId() { return m_id; }

	inline int getMaxHp() { return m_maxHp; }

	inline void setMaxHp(int maxHp) { m_maxHp = maxHp; }

	inline int getHp() { return m_hp; }

	inline void setHp(int hp) {
		m_hp = (hp > m_maxHp) ? m_maxHp : hp;
	}

	inline int getX() const { return m_x; }

	inline void setX(int x) { m_x = x; }

	inline int getCenterX() const{ return m_x + (m_wide / 2); }
	inline int getCenterY() const { return m_y + (m_height / 2); }

	inline int getY() const { return m_y; }

	inline void setY(int y) { m_y = y; }

	inline int getWide() const { return m_wide; }

	inline int getHeight() const { return m_height; }

	inline int getMoveSpeed() { return m_characterInfo->moveSpeed(); }

	inline int getJumpHeight() { return m_characterInfo->jumpHeight(); }

	inline std::string getName() { return m_characterInfo->name(); }

	inline bool getLeftDirection() { return m_leftDirection; }

	void setLeftDirection(bool leftDirection);

	inline const GraphHandle* getGraphHandle() const { return m_graphHandle; }

	// AttackInfo�̃Z�b�^�ƃQ�b�^
	inline int getBulletRapid() { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() { return m_attackInfo->slashCountSum(); }

	// �摜�̃Z�b�^�B�摜�̉���(wide)�Əc��(height)���Z�b�g����B
	void setHandle(GraphHandle* handle);
	void getHandleSize(int& wide, int& height);

	// �����摜���Z�b�g
	virtual void switchStand(int cnt = 0) = 0;
	// �����ˌ��摜���Z�b�g
	virtual void switchBullet(int cnt = 0) = 0;
	// �����a���摜���Z�b�g
	virtual void switchSlash(int cnt = 0) = 0;
	// ���Ⴊ�݉摜���Z�b�g
	virtual void switchSquat(int cnt = 0) = 0;
	// ����摜���Z�b�g
	virtual void switchRun(int cnt = 0) = 0;
	// ���n�摜���Z�b�g
	virtual void switchLand(int cnt = 0) = 0;
	// �㏸�摜���Z�b�g
	virtual void switchJump(int cnt = 0) = 0;
	// �~���摜���Z�b�g
	virtual void switchDown(int cnt = 0) = 0;
	// �W�����v�O�摜���Z�b�g
	virtual void switchPreJump(int cnt = 0) = 0;
	// �_���[�W�摜���Z�b�g
	virtual void switchDamage(int cnt = 0) = 0;
	// �u�[�X�g�摜���Z�b�g
	virtual void switchBoost(int cnt = 0) = 0;
	// �󒆎ˌ��摜���Z�b�g
	virtual void switchAirBullet(int cnt = 0) = 0;
	// �󒆎a���摜���Z�b�g
	virtual void switchAirSlash(int cnt = 0) = 0;


	// �ړ�����i���W�𓮂����j
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// �ˌ��U��������(�L�������ƂɈႤ)
	virtual Object* bulletAttack(int gx, int gy) = 0;

	// �a���U��������(�L�������ƂɈႤ) ���������Ă��邩�A�����J�E���g��
	virtual Object* slashAttack(bool leftDirection, int cnt) = 0;
};


/*
* �n�[�g�i��l���j
*/
class Heart :
	public Character
{
private:
	// �L�����̖��O
	const char* const NAME = "�n�[�g";

	// �����摜
	GraphHandle* m_standHandle;

	// �a���U���摜
	GraphHandles* m_slashHandles;

	// ���Ⴊ��
	GraphHandle* m_squatHandle;

	// �����ˌ�
	GraphHandle* m_standBulletHandle;

	// �����a��
	GraphHandle* m_standSlashHandle;

	// ����
	const int RUN_ANIME_SPEED = 6;
	GraphHandles* m_runHandles;

	// ���n
	GraphHandle* m_landHandle;

	// �㏸
	GraphHandle* m_jumpHandle;

	// ���~
	GraphHandle* m_downHandle;

	// �W�����v�O
	const int RUN_PREJUMP_SPEED = 6;
	GraphHandles* m_preJumpHandles;

	// �_���[�W
	GraphHandle* m_damageHandle;

	// �u�[�X�g
	GraphHandle* m_boostHandle;

	// �󒆎ˌ�
	GraphHandle* m_airBulletHandle;

	// �󒆎a��
	GraphHandle* m_airSlashHandle;
	
public:
	// �R���X�g���N�^
	Heart(int maxHp, int hp, int x, int y);

	// �f�X�g���N�^
	~Heart();

	// �f�o�b�O
	void debug(int x, int y, int color);

	// �����摜���Z�b�g
	inline void switchStand(int cnt = 0) { setHandle(m_standHandle); }
	// �����ˌ��摜���Z�b�g
	inline void switchBullet(int cnt = 0) { setHandle(m_standBulletHandle); }
	// �����a���摜���Z�b�g
	inline void switchSlash(int cnt = 0) { setHandle(m_standSlashHandle); }
	// ���Ⴊ�݉摜���Z�b�g
	inline void switchSquat(int cnt = 0) { setHandle(m_squatHandle); }
	// ����摜���Z�b�g
	void switchRun(int cnt = 0);
	// ���n�摜���Z�b�g
	inline void switchLand(int cnt = 0) { setHandle(m_landHandle); }
	// �㏸�摜���Z�b�g
	inline void switchJump(int cnt = 0) { setHandle(m_jumpHandle); }
	// �~���摜���Z�b�g
	inline void switchDown(int cnt = 0) { setHandle(m_downHandle); }
	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int cnt = 0);
	// �_���[�W�摜���Z�b�g
	inline void switchDamage(int cnt = 0) { setHandle(m_damageHandle); }
	// �u�[�X�g�摜���Z�b�g
	inline void switchBoost(int cnt = 0) { setHandle(m_boostHandle); }
	// �󒆎ˌ��摜���Z�b�g
	inline void switchAirBullet(int cnt = 0) { setHandle(m_airBulletHandle); }
	// �󒆎a���摜���Z�b�g
	inline void switchAirSlash(int cnt = 0) { setHandle(m_airSlashHandle); }

	// �ˌ��U��������(�L�������ƂɈႤ)
	Object* bulletAttack(int gx, int gy);

	// �a���U��������(�L�������ƂɈႤ)
	Object* slashAttack(bool leftDirection, int cnt);
};


#endif