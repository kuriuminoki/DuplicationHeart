#ifndef GRAPH_HANDLE_H_INCLUDED
#define GRAPH_HANDLE_H_INCLUDED

#include <map>
#include <string>

class Camera;
class CsvReader;

/*
* �摜�f�[�^(�n���h���A�摜�ŗL�̊g�嗦�A����)���܂Ƃ߂Ĉ������߂̃N���X
*/
class GraphHandle {
private:
	// �摜�n���h��
	int m_handle;

	// (���̉摜�ŗL��)�g�嗦
	double m_ex;

	// ����
	double m_angle;

	// �����x��L���ɂ��邩
	bool m_trans;

	// ���]���邩
	bool m_reverseX;
	bool m_reverseY;

public:
	GraphHandle(const char* filePath, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandle();

	// �Q�b�^
	inline int getHandle() const { return m_handle; }
	inline double getEx() const { return m_ex; }
	inline double getAngle() const { return m_angle; }

	// �Z�b�^
	inline void setEx(double ex) { m_ex = ex; }
	inline void setAngle(double angle) { m_angle = angle; }
	inline void setTrans(bool trans) { m_trans = trans; }
	inline void setReverseX(bool reverse) { m_reverseX = reverse; }
	inline void setReverseY(bool reverse) { m_reverseY = reverse; }

	// �`�悷��
	void draw(int x, int y, double ex = 1.0) const;

	// �͈͂��w�肵�ĕό`�`�悷��
	void extendDraw(int x1, int y1, int x2, int y2) const;

	// �͈͂��w�肵�ĕ~���l�߂�悤�ɕ`�悷��
	void lineUpDraw(int x1, int y1, int x2, int y2, const Camera* camera) const;
};


/*
* GraphHandle��z��Ƃ��Ă܂Ƃ߂Ĉ����N���X
*/
class GraphHandles {
private:
	GraphHandle** m_handles;
	int m_handleSum;

public:
	//  filePath�̖����ɂ�".png"�����Ȃ����ƁB
	GraphHandles(const char* filePath, int handleSum, double ex = 1.0, double angle = 0.0, bool trans = false, bool reverseX = false, bool reverseY = false);
	~GraphHandles();

	// �Q�b�^
	inline GraphHandle* getGraphHandle(int i = 0) const { return m_handles[i]; }
	inline int getHandle(int i) { return m_handles[i]->getHandle(); }
	inline int getSize() const { return m_handleSum; }

	// �Z�b�^
	void setEx(double ex);
	void setAngle(double angle);
	void setTrans(bool trans);
	void setReverseX(bool reverse);
	void setReverseY(bool reverse);

	// �`�悷��
	void draw(int x, int y, int index);
};


/*
* �����蔻��̏��
*/
class AtariArea {
private:
	// �����蔻��
	bool m_defaultWide, m_defaultHeight;
	int m_wide, m_height;
	int m_x1, m_y1, m_x2, m_y2;
	bool m_x1none, m_y1none, m_x2none, m_y2none;

public:
	AtariArea(CsvReader* csvReader, const char* graphName, const char* prefix);

	bool getDefaultWide() const { return m_defaultWide; }
	bool getDefaultHeight() const { return m_defaultHeight; }
	bool getWide() const { return m_wide; }
	bool getHeight() const { return m_height; }
	bool getX1() const { return m_x1; }
	bool getY1() const { return m_y1; }
	bool getX2() const { return m_x2; }
	bool getY2() const { return m_y2; }

	void getArea(int* x1, int* y1, int* x2, int* y2, int wide, int height) const;

};


/*
* �����蔻��̏��t����GraphHandles
*/
class GraphHandlesWithAtari {
private:

	GraphHandles* m_graphHandles;

	// �����蔻��
	AtariArea* m_atariArea;
	AtariArea* m_damageArea;

public:

	GraphHandlesWithAtari(GraphHandles* graphHandles, const char* graphName, CsvReader* csvReader);
	~GraphHandlesWithAtari();

	GraphHandles* getGraphHandles() const { return m_graphHandles; }

	void getAtari(int* x1, int* y1, int* x2, int* y2, int index) const;
	void getDamage(int* x1, int* y1, int* x2, int* y2, int index) const;

};


/*
* �L�����̊�̏u������������N���X
*/
class CharacterEyeClose {
private:

	// �����鎞��
	static const int EYE_CLOSE_MIN_TIME = 5;
	static const int EYE_CLOSE_MAX_TIME = 10;
	
	// ������c�莞��
	int m_cnt;

public:

	CharacterEyeClose();

	// �u���X�^�[�g��true
	bool closeFlag();

private:

	// �����鎞�Ԃ��J�E���g
	void count();

	// �u�����Ȃ�true
	inline bool closeNow() { return m_cnt > 0; }

};


/*
* �L�����̉摜���Ǘ�����N���X
*/
class CharacterGraphHandle {
private:
	// �\�������摜
	GraphHandlesWithAtari* m_dispGraphHandle_p;
	int m_dispGraphIndex;

	double m_ex;

	int m_wide, m_height;

	// �����蔻��
	int m_atariX1, m_atariY1, m_atariX2, m_atariY2;

	// �u��
	CharacterEyeClose m_characterEyeClose;

	// �L�����̃p�[�c�̉摜
	// �a���U���摜
	GraphHandlesWithAtari* m_slashHandles;

	// �󒆎a���U���摜
	GraphHandlesWithAtari* m_airSlashEffectHandles;

	// �ˌ��U���摜
	GraphHandlesWithAtari* m_bulletHandles;

	// �L�����{�̂̉摜
	// �����摜
	GraphHandlesWithAtari* m_standHandles;

	// ���Ⴊ�݉摜
	GraphHandlesWithAtari* m_squatHandles;

	// ���Ⴊ�ݎˌ��摜
	GraphHandlesWithAtari* m_squatBulletHandles;

	// �����ˌ��摜
	GraphHandlesWithAtari* m_standBulletHandles;

	// �����a���摜
	GraphHandlesWithAtari* m_standSlashHandles;

	// ����摜
	GraphHandlesWithAtari* m_runHandles;

	// ����ˌ��摜
	GraphHandlesWithAtari* m_runBulletHandles;

	// ���n�摜
	GraphHandlesWithAtari* m_landHandles;

	// �㏸�摜
	GraphHandlesWithAtari* m_jumpHandles;

	// ���~�摜
	GraphHandlesWithAtari* m_downHandles;

	// �W�����v�O�摜
	GraphHandlesWithAtari* m_preJumpHandles;

	// �_���[�W�摜
	GraphHandlesWithAtari* m_damageHandles;

	// �u�[�X�g�摜
	GraphHandlesWithAtari* m_boostHandles;

	// �󒆎ˌ��摜
	GraphHandlesWithAtari* m_airBulletHandles;

	// �󒆎a���摜
	GraphHandlesWithAtari* m_airSlashHandles;

	// �X�e�b�v
	GraphHandlesWithAtari* m_stepHandles;

	// �X���C�f�B���O
	GraphHandlesWithAtari* m_slidingHandles;

	// �u���摜
	GraphHandlesWithAtari* m_closeHandles;

	// ����摜
	GraphHandlesWithAtari* m_deadHandles;

	// �{�X�̏����A�j���[�V����
	GraphHandlesWithAtari* m_initHandles;

	// �ǉ��摜
	GraphHandlesWithAtari* m_special1Handles;

public:
	// �f�t�H���g�l�ŏ�����
	CharacterGraphHandle();
	// csv�t�@�C����ǂݍ��݁A�L�������Ō������p�����[�^�擾
	CharacterGraphHandle(const char* characterName, double drawEx);
	// �摜���폜
	~CharacterGraphHandle();

	// �\������摜��Ԃ�
	inline GraphHandlesWithAtari* getDispGraphHandle() { return m_dispGraphHandle_p; }
	inline GraphHandle* getHandle() { return m_dispGraphHandle_p->getGraphHandles()->getGraphHandle(m_dispGraphIndex); }
	inline int getDispGraphIndex() const { return m_dispGraphIndex; }
	inline int getWide() const { return m_wide; }
	inline int getHeight() const { return m_height; }
	void getAtari(int* x1, int* y1, int* x2, int* y2) const;
	void getDamage(int* x1, int* y1, int* x2, int* y2) const;

	// �摜�̃Q�b�^
	inline GraphHandlesWithAtari* getSlashHandle() { return m_slashHandles; }
	inline GraphHandlesWithAtari* getAirSlashEffectHandle() { return m_airSlashEffectHandles; }
	inline GraphHandlesWithAtari* getBulletHandle() { return m_bulletHandles; }
	inline GraphHandlesWithAtari* getStandHandle() { return m_standHandles; }
	inline GraphHandlesWithAtari* getStandBulletHandle() { return m_standBulletHandles; }
	inline GraphHandlesWithAtari* getStandSlashHandle() { return m_standSlashHandles; }
	inline GraphHandlesWithAtari* getSquatHandle() { return m_squatHandles; }
	inline GraphHandlesWithAtari* getSquatBulletHandle() { return m_squatBulletHandles; }
	inline GraphHandlesWithAtari* getRunHandle() { return m_runHandles; }
	inline GraphHandlesWithAtari* getRunBulletHandle() { return m_runBulletHandles; }
	inline GraphHandlesWithAtari* getLandHandle() { return m_landHandles; }
	inline GraphHandlesWithAtari* getJumpHandle() { return m_jumpHandles; }
	inline GraphHandlesWithAtari* getDownHandle() { return m_downHandles; }
	inline GraphHandlesWithAtari* getPreJumpHandle() { return m_preJumpHandles; }
	inline GraphHandlesWithAtari* getDamageHandle() { return m_damageHandles; }
	inline GraphHandlesWithAtari* getBoostHandle() { return m_boostHandles; }
	inline GraphHandlesWithAtari* getAirBulletHandle() { return m_airBulletHandles; }
	inline GraphHandlesWithAtari* getAirSlashHandle() { return m_airSlashHandles; }
	inline GraphHandlesWithAtari* getStepHandle() { return m_stepHandles; }
	inline GraphHandlesWithAtari* getSlidingHandle() { return m_slidingHandles; }
	inline GraphHandlesWithAtari* getCloseHandle() { return m_closeHandles; }
	inline GraphHandlesWithAtari* getDeadHandle() { return m_deadHandles; }
	inline GraphHandlesWithAtari* getInitHandle() { return m_initHandles; }
	inline GraphHandlesWithAtari* getSpecial1Handle() { return m_special1Handles; }

	// �摜�T�C�Y���Z�b�g
	void setGraphSize();

	// �����蔻����Z�b�g
	void setAtari();

	// �摜���Z�b�g�A���݂��Ȃ��摜�Ȃ炻�̂܂܁@�T�C�Y������
	void setGraph(GraphHandlesWithAtari* graphHandles, int index);

	// �����摜���Z�b�g
	void switchStand(int index = 0);
	// �����ˌ��摜���Z�b�g
	void switchBullet(int index = 0);
	// �����a���摜���Z�b�g
	void switchSlash(int index = 0);
	// ���Ⴊ�݉摜���Z�b�g
	void switchSquat(int index = 0);
	// ���Ⴊ�ݎˌ��摜���Z�b�g
	void switchSquatBullet(int index = 0);
	// ����摜���Z�b�g
	void switchRun(int index = 0);
	// ����ˌ��摜���Z�b�g
	void switchRunBullet(int index = 0);
	// ���n�摜���Z�b�g
	void switchLand(int index = 0);
	// �㏸�摜���Z�b�g
	void switchJump(int index = 0);
	// �~���摜���Z�b�g
	void switchDown(int index = 0);
	// �W�����v�O�摜���Z�b�g
	void switchPreJump(int index = 0);
	// �_���[�W�摜���Z�b�g
	void switchDamage(int index = 0);
	// �u�[�X�g�摜���Z�b�g
	void switchBoost(int index = 0);
	// �󒆎ˌ��摜���Z�b�g
	void switchAirBullet(int index = 0);
	// �󒆎a���摜���Z�b�g
	void switchAirSlash(int index = 0);
	// �X�e�b�v�摜���Z�b�g
	void switchStep(int index = 0);
	// �X���C�f�B���O�摜���Z�b�g
	void switchSliding(int index = 0);
	// �u���摜���Z�b�g
	void switchClose(int index = 0);
	// ����摜���Z�b�g
	void switchDead(int index = 0);
	// �{�X�̏����A�j���[�V�������Z�b�g
	void switchInit(int index = 0);
	// �ǉ��摜���Z�b�g
	void switchSpecial1(int index = 0);
};


/*
* �L�����̊�摜
*/
class FaceGraphHandle {
private:
	double m_ex;

	// ��摜 <�摜��, �摜�n���h��>
	std::map<std::string, GraphHandles*> m_faceHandles;

public:
	FaceGraphHandle();
	FaceGraphHandle(const char* characterName, double drawEx);
	~FaceGraphHandle();

	GraphHandles* getGraphHandle(const char* faceName);
};


#endif