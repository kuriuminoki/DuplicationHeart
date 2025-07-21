#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include<string>
#include<map>
#include<vector>

class Object;
class GraphHandle;
class GraphHandles;
class CharacterGraphHandle;
class FaceGraphHandle;
class SoundPlayer;


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

	// �ˌ����Ɍ�������������
	bool m_sameBulletDirection;

	// �W�����v���̉�
	int m_jumpSound;

	// �󂯐g���̉�
	int m_passiveSound;

	// ���n���̉�
	int m_landSound;

public:
	// �f�t�H���g�l�ŏ�����
	CharacterInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	CharacterInfo(const char* characterName);

	~CharacterInfo();

	// �Q�b�^�݂̂����A�Z�b�^�͎����Ȃ�
	inline std::string name() const { return m_name; }
	inline int maxHp() const { return m_maxHp; }
	inline double handleEx() const { return m_handleEx; }
	inline int moveSpeed() const { return m_moveSpeed; }
	inline int jumpHeight() const { return m_jumpHeight; }
	inline bool sameBulletDirection() const { return m_sameBulletDirection; }
	inline int jumpSound() const { return m_jumpSound; }
	inline int passiveSound() const { return m_passiveSound; }
	inline int landSound() const { return m_landSound; }

	// �o�[�W�����ύX
	void changeVersion(int version);

private:

	void setParam(std::map<std::string, std::string>& data);

};


class AttackInfo {
private:
	// �e�ۂ�HP
	int m_bulletHp;

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

	// ���e���ɔ������邩
	bool m_bulletBomb;

	// �a����HP
	int m_slashHp;

	// �a���̃_���[�W
	int m_slashDamage;

	// �a���̑傫��(�����`�̕ӂ̒���)
	int m_slashLenX, m_slashLenY;

	// �a���̑S�̃t���[��
	int m_slashCountSum;

	// �a���̌㌄
	int m_slashInterval;

	// �a���̐������(X�����̏���)
	int m_slashImpactX;

	// �a���̐������(Y�����̏���)
	int m_slashImpactY;

	// �ˌ��U���̃G�t�F�N�g
	GraphHandles* m_bulletEffectHandles;

	// �a���U���̃G�t�F�N�g
	GraphHandles* m_slashEffectHandles;

	// �ˌ��U�������������Ƃ��̃T�E���h
	int m_bulletSoundHandle;

	// �e���ˉ�
	int m_bulletStartSoundHandle;

	// �a���U�������������Ƃ��̃T�E���h
	int m_slashSoundHandle;

	// �a���J�n�T�E���h
	int m_slashStartSoundHandle;

public:
	// �f�t�H���g�l�ŏ�����
	AttackInfo();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	AttackInfo(const char* characterName, double drawEx);

	~AttackInfo();
	
	// �Q�b�^�݂̂����A�Z�b�^�͎����Ȃ�
	int bulletHp() const { return m_bulletHp; }
	int bulletDamage() const { return m_bulletDamage; }
	int bulletRx() const { return m_bulletRx; }
	int bulletRy() const { return m_bulletRy; }
	int bulletSpeed() const { return m_bulletSpeed; }
	int bulletRapid() { return m_bulletRapid; }
	int bulletDistance() const { return m_bulletDistance; }
	int bulletImpactX() const { return m_bulletImpactX; }
	int bulletImpactY() const { return m_bulletImpactY; }
	bool bulletBomb() const { return m_bulletBomb; }
	int slashHp() const { return m_slashHp; }
	int slashDamage() const { return m_slashDamage; }
	int slashLenX() const { return m_slashLenX; }
	int slashLenY() const { return m_slashLenY; }
	int slashCountSum() const { return m_slashCountSum; }
	int slashInterval() const { return m_slashInterval; }
	int slashImpactX() const { return m_slashImpactX; }
	int slashImpactY() const { return m_slashImpactY; }
	GraphHandles* bulletEffectHandle() const { return m_bulletEffectHandles; }
	GraphHandles* slashEffectHandle() const { return m_slashEffectHandles; }
	int bulletSoundeHandle() const { return m_bulletSoundHandle; }
	int slashSoundHandle() const { return m_slashSoundHandle; }
	int bulletStartSoundeHandle() const { return m_bulletStartSoundHandle; }
	int slashStartSoundHandle() const { return m_slashStartSoundHandle; }

	// �o�[�W�����ύX
	void changeVersion(const char* characterName, int version);

private:

	void setParam(std::map<std::string, std::string>& data);

};


/*
* �v���C���[��G�l�~�[�̊��N���X
*/
class Character {
public:

	// �L������h�炵�ĕ`�悷��Ȃ�true
	const bool SHAKING_FLAG = false;

	const int SKILL_MAX = 100;

	const int DEFAULT_ANIME_SPEED = 3;

protected:
	static int characterId;

	AttackInfo* m_slidingInfo;

	bool m_duplicationFlag;

	// ���ꂽ��True�BHP==0�ɂȂ��Ă���ʂɔ�\���ɂ���͎̂��̃t���[������ɂ��邽�ߎg��
	bool m_deadFlag;

	// ID
	int m_id;

	// �O���[�vID �������ʗp
	int m_groupId;

	// Info�̃o�[�W����
	int m_version;

	// �c��̗�
	int m_hp;

	// �_���[�W���󂯂�O�̗̑�
	int m_prevHp;

	// HP�o�[��\������c�莞��
	int m_dispHpCnt;

	// �X�L���Q�[�W �ő�100
	int m_skillGage;

	// ���G�Ȃ�true
	bool m_invincible;

	// X���W�AY���W
	int m_x, m_y;

	// ���������Ă���
	bool m_leftDirection;

	// �ꎞ�I�ɓ����Ȃ���ԁi�n�[�g�̃X�L�������Ȃǁj
	bool m_freeze;

	// �{�X�Ȃ�true
	bool m_bossFlag;

	// �`��p
	int m_drawCnt;

	// �L�����̏��
	CharacterInfo* m_characterInfo;

	// �U���̏��
	AttackInfo* m_attackInfo;

	// �L�����摜
	CharacterGraphHandle* m_graphHandle;

	// ��摜
	FaceGraphHandle* m_faceHandle;

	// �l���������� World�ɓn������0�ɂ���
	int m_money;

	// �t���[�Y���Ȃ炻�̎c�莞�Ԃ�ێ�
	int m_stopCnt;

public:
	// �R���X�g���N�^
	Character();
	Character(int hp, int x, int y, int groupId);
	virtual ~Character();

	virtual Character* createCopy() = 0;
	void setParam(Character* character);

	// �f�o�b�O
	void debugCharacter(int x, int y, int color) const;
	virtual void debug(int x, int y, int color) const = 0;

	// �Q�b�^
	inline bool getDeadFlag() const { return m_deadFlag; }
	inline int getId() const { return m_id; }
	inline int getGroupId() const { return m_groupId; }
	inline int getVersion() const { return m_version; }
	inline int getHp() const { return m_hp; }
	inline int getPrevHp() const { return m_prevHp; }
	inline int getSkillGage() const { return m_skillGage; }
	inline int getMaxSkillGage() const { return SKILL_MAX; }
	inline int getDispHpCnt() const { return m_dispHpCnt; }
	inline bool getInvincible() const { return m_invincible; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline bool getLeftDirection() const { return m_leftDirection; }
	inline bool getFreeze() const { return m_freeze; }
	inline bool getBossFlag() const { return m_bossFlag; }
	FaceGraphHandle* getFaceHandle() const { return m_faceHandle; }
	inline CharacterGraphHandle* getCharacterGraphHandle() const { return m_graphHandle; }
	inline AttackInfo* getAttackInfo() const { return m_attackInfo; }
	inline CharacterInfo* getCharacterInfo() const { return m_characterInfo; }
	inline int getMoney() const { return m_money; }
	inline int getStopCnt() const { return m_stopCnt; }

	// �Z�b�^
	inline void setDeadFlag(bool deadFlag) { m_deadFlag = deadFlag; }
	inline void setHp(int hp) { m_hp = (hp > m_characterInfo->maxHp()) ? m_characterInfo->maxHp() : hp; m_prevHp = m_hp; }
	inline void setPrevHp(int prevHp) { 
		m_prevHp = (prevHp < m_hp) ? m_hp : prevHp;
		if (m_prevHp == m_hp && m_dispHpCnt > 0) { m_dispHpCnt--; }
	}
	inline void setSkillGage(int skillGage) { 
		m_skillGage = skillGage > SKILL_MAX ? SKILL_MAX : (skillGage < 0 ? 0 : skillGage);
	}
	inline void setInvincible(bool invincible) { m_invincible = invincible; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setId(int id) { m_id = id; }
	inline void setGroupId(int id) { m_groupId = id; }
	inline void setFreeze(bool freeze) { m_freeze = freeze; }
	inline void setBossFlag(bool bossFlag) { m_bossFlag = bossFlag; }
	inline void setStopCnt(int stopCnt) { m_stopCnt = stopCnt; }
	// �L�����̌����ύX�́A�摜�̔��]���s��
	void setLeftDirection(bool leftDirection);
	inline void setDuplicationFlag(bool flag) { m_duplicationFlag = flag; }
	inline void setAttackInfo(AttackInfo* attackInfo) { m_attackInfo = attackInfo; }
	inline void setCharacterInfo(CharacterInfo* characterInfo) { m_characterInfo = characterInfo; }
	inline void setMoney(int money) { m_money = money; }

	// CharacterInfo����L�����̃X�y�b�N���擾
	inline std::string getName() const { return m_characterInfo->name(); }
	inline int getMaxHp() const { return m_characterInfo->maxHp(); }
	inline int getMoveSpeed() const { return m_characterInfo->moveSpeed(); }
	inline int getJumpHeight() const { return m_characterInfo->jumpHeight(); }
	inline int getJumpSound() const { return m_characterInfo->jumpSound(); }
	inline int getPassiveSound() const { return m_characterInfo->passiveSound(); }
	inline int getLandSound() const { return m_characterInfo->landSound(); }

	// AttackInfo����U���̃X�y�b�N���擾
	inline int getBulletRapid() const { return m_attackInfo->bulletRapid(); }
	inline int getSlashCountSum() const { return m_attackInfo->slashCountSum(); }
	inline int getSlashInterval() const { return m_attackInfo->slashInterval(); }

	// �����蔻��͈̔͂��擾
	void getAtariArea(int* x1, int* y1, int* x2, int* y2) const;
	void getDamageArea(int* x1, int* y1, int* x2, int* y2) const;

	// Info�̃o�[�W������ύX����
	void changeInfoVersion(int version);

	// �摜�̏����擾
	int getCenterX() const;
	int getCenterY() const;
	int getWide() const;
	int getHeight() const;
	int getAtariCenterX() const;
	int getAtariCenterY() const;
	// ���`�悷��摜���擾
	GraphHandle* getGraphHandle() const;
	void getHandleSize(int& wide, int& height) const;

	// �����摜���Z�b�g
	virtual void switchStand(int cnt = 0);
	// �����ˌ��摜���Z�b�g
	virtual void switchBullet(int cnt = 0);
	// �����a���摜���Z�b�g
	virtual void switchSlash(int cnt = 0);
	// ���Ⴊ�݉摜���Z�b�g
	virtual void switchSquat(int cnt = 0);
	// ���Ⴊ�ݎˌ��摜���Z�b�g
	virtual void switchSquatBullet(int cnt = 0);
	// ����摜���Z�b�g
	virtual void switchRun(int cnt = 0);
	// ����ˌ��摜���Z�b�g
	virtual void switchRunBullet(int cnt = 0);
	// ���n�摜���Z�b�g
	virtual void switchLand(int cnt = 0);
	// �㏸�摜���Z�b�g
	virtual void switchJump(int cnt = 0);
	// �~���摜���Z�b�g
	virtual void switchDown(int cnt = 0);
	// �W�����v�O�摜���Z�b�g
	virtual void switchPreJump(int cnt = 0);
	// �_���[�W�摜���Z�b�g
	virtual void switchDamage(int cnt = 0);
	// �u�[�X�g�摜���Z�b�g
	virtual void switchBoost(int cnt = 0);
	// �X�e�b�v�摜���Z�b�g
	virtual void switchStep(int cnt = 0);
	// �X���C�f�B���O�摜���Z�b�g
	virtual void switchSliding(int cnt = 0);
	// �󒆎ˌ��摜���Z�b�g
	virtual void switchAirBullet(int cnt = 0);
	// �󒆎a���摜���Z�b�g
	virtual void switchAirSlash(int cnt = 0);
	// ����摜���Z�b�g
	virtual void switchDead(int cnt = 0);
	// �{�X�̏����A�j���[�V�������Z�b�g
	virtual void switchInit(int cnt = 0);
	// �ǉ��摜���Z�b�g
	virtual void switchSpecial1(int cnt = 0);

	// HP����
	void damageHp(int value);

	// �ړ�����i���W�𓮂����j
	void moveRight(int d);
	void moveLeft(int d);
	void moveUp(int d);
	void moveDown(int d);

	// �ˌ��U��������(�L�������ƂɈႤ)
	virtual std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) { return nullptr; }

	// �a���U��������(�L�������ƂɈႤ) ���������Ă��邩�A�����J�E���g��
	virtual std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) { return nullptr; }

	// �X���C�f�B���O�U��
	std::vector<Object*>* slidingAttack(int sound, SoundPlayer* soundPlayer);

	// �ˌ��U���������Ă��邩
	bool haveBulletAttack() const { return m_attackInfo->bulletDamage() != 0; }
	// �a���U���������Ă��邩
	bool haveSlashAttack() const { return m_attackInfo->slashDamage() != 0; }

	// ����̉摜�����邩
	bool haveStepGraph() const;
	bool haveSlidingGraph() const;
	bool haveDeadGraph() const;

	// HP��0�ł���摜���Ȃ��A��ʂ��������ׂ���
	inline bool noDispForDead() const { return m_hp == 0 && !haveDeadGraph() && m_deadFlag; }

protected:
	void countDrawCnt(){ if (SHAKING_FLAG) { m_drawCnt++; } }
};


Character* createCharacter(const char* characterName, int hp = 100, int x = 0, int y = 0, int groupId = 0);


/*
* �n�[�g�i��l���j
*/
class Heart :
	public Character
{
protected:
	//// ����A�j���̃X�s�[�h
	const int RUN_ANIME_SPEED = 6;
	
	//// �W�����v�O�A�j���̃X�s�[�h
	const int RUN_PREJUMP_SPEED = 6;

	// �e�̐F
	int m_bulletColor;
	
public:
	// �R���X�g���N�^
	Heart(const char* name, int hp, int x, int y, int groupId);
	Heart(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	// �f�X�g���N�^
	~Heart();

	Character* createCopy();

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �摜�ύX�֐��̃I�[�o�[���C�h
	// �����摜���Z�b�g
	void switchStand(int cnt = 0);

	// ���Ⴊ�݉摜���Z�b�g
	void switchSquat(int cnt = 0);

	// ����摜���Z�b�g
	void switchRun(int cnt = 0);

	// ����ˌ��摜���Z�b�g
	void switchRunBullet(int cnt = 0);

	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int cnt = 0);

	// �����a���摜���Z�b�g
	void switchSlash(int cnt = 0);

	// �����ˌ��摜���Z�b�g
	void switchBullet(int cnt = 0);

	// �󒆎ˌ��摜���Z�b�g
	void switchAirBullet(int cnt = 0);

	// �󒆎a���摜���Z�b�g
	void switchAirSlash(int cnt = 0);

	// ���Ⴊ�ݎˌ��摜���Z�b�g
	void switchSquatBullet(int cnt = 0);

	// ����摜���Z�b�g
	void switchDead(int cnt = 0);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);

};


/*
* �V�G�X�^
*/
class Siesta :
	public Heart
{
public:
	// �R���X�g���N�^
	Siesta(const char* name, int hp, int x, int y, int groupId);
	Siesta(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �����ˌ��摜���Z�b�g
	void switchBullet(int cnt = 0);

	// ���Ⴊ�ݎˌ��摜���Z�b�g
	void switchSquatBullet(int cnt = 0);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);
};


/*
* �q�G�����L�[
*/
class Hierarchy :
	public Heart
{
public:
	// �R���X�g���N�^
	Hierarchy(const char* name, int hp, int x, int y, int groupId);
	Hierarchy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);
};


/*
* ���@���L���A
*/
class Valkyria :
	public Heart
{
public:
	// �R���X�g���N�^
	Valkyria(const char* name, int hp, int x, int y, int groupId);
	Valkyria(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �����a���摜���Z�b�g
	void switchSlash(int cnt = 0);
	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int cnt = 0);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer){ return nullptr; }

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);
};


/*
* �g���C
*/
class Troy :
	public Heart
{
public:
	// �R���X�g���N�^
	Troy(const char* name, int hp, int x, int y, int groupId);
	Troy(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// ����摜���Z�b�g
	void switchRun(int cnt = 0);
	// ����ˌ��摜���Z�b�g
	void switchRunBullet(int cnt = 0);
	// �㏸�摜���Z�b�g
	void switchJump(int cnt = 0);
	// �~���摜���Z�b�g
	void switchDown(int cnt = 0);
	// �󒆎ˌ��摜���Z�b�g
	void switchAirBullet(int cnt = 0);

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);
};


/*
* �R�n��
*/
class Koharu :
	public Heart
{
public:
	// �R���X�g���N�^
	Koharu(const char* name, int hp, int x, int y, int groupId);
	Koharu(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);

};


/*
* ���ʂ̎ˌ��݂̂�����L����
*/
class BulletOnly :
	public Heart
{
public:
	// �R���X�g���N�^
	BulletOnly(const char* name, int hp, int x, int y, int groupId);
	BulletOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �㏸�摜���Z�b�g
	void switchJump(int cnt = 0);

	// �a���U��������(�L�������ƂɈႤ)
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) { return nullptr; }
};


/*
* ���ʂ̎a���݂̂�����L����
*/
class SlashOnly :
	public Heart
{
public:
	// �R���X�g���N�^
	SlashOnly(const char* name, int hp, int x, int y, int groupId);
	SlashOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �㏸�摜���Z�b�g
	void switchJump(int cnt = 0);
	// �~���摜���Z�b�g
	void switchDown(int cnt = 0);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) { return nullptr; }
};


/*
* ParabolaBullet�݂̂����L����
*/
class ParabolaOnly :
	public Heart
{
public:
	// �R���X�g���N�^
	ParabolaOnly(const char* name, int hp, int x, int y, int groupId);
	ParabolaOnly(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);
};


/*
* Boss1: �T��
*/
class Sun :
	public Heart
{
public:
	// �R���X�g���N�^
	Sun(const char* name, int hp, int x, int y, int groupId);
	Sun(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	// �{�X�̏����A�j���[�V�������Z�b�g
	void switchInit(int cnt);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer);

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer) { return nullptr; }

};


/*
* Boss2: �A�[�J�C�u
*/
class Archive :
	public Heart
{
public:
	// �R���X�g���N�^
	Archive(const char* name, int hp, int x, int y, int groupId);
	Archive(const char* name, int hp, int x, int y, int groupId, AttackInfo* attackInfo);

	Character* createCopy();

	void switchJump(int cnt);
	void switchSlash(int cnt);

	// �ˌ��U��������
	std::vector<Object*>* bulletAttack(int cnt, int gx, int gy, SoundPlayer* soundPlayer) { return nullptr; }

	// �a���U��������
	std::vector<Object*>* slashAttack(bool leftDirection, int cnt, bool grand, SoundPlayer* soundPlayer);
};


#endif