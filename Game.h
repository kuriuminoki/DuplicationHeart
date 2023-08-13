#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>

class SoundPlayer;
class World;
class Story;
class Character;


// �L�����̃Z�[�u�f�[�^
class CharacterData {
private:
	const char* m_name;
	int m_hp;
public:
	CharacterData(const char* name);

	// �Q�b�^
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }

	// �Z�b�^
	inline void setHp(int hp) { m_hp = hp; }
};


// �Z�[�u�f�[�^
class GameData {
private:
	// �Z�[�u����ꏊ
	const char* m_saveFilePath;

	// �L�����̃f�[�^
	std::vector<CharacterData> m_characterData;

	// ������G���A
	int m_areaNum;

	// ������Ă���X�g�[���[
	int m_storyNum;

public:
	GameData();
	GameData(const char* saveFilePath);

	// �Q�b�^
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }

	// �Z�b�^
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }

	// ���g�̃f�[�^��World�Ƀf�[�^���f������
	void asignWorld(World* world);

	// World�̃f�[�^�����g�ɔ��f������
	void asignedWorld(World* world);
};


// �n�[�g�̃X�L��
class HeartSkill {
private:
	// �����̐�
	int m_loopNum;

	// �������[�v�ڂ�
	int m_loopNow;

	// ���̐��E
	World* m_world_p;

	// ����
	World* m_duplicationWorld;

	// ���b�Ԃ�
	const int DUPLICATION_TIME = 600;

	// DUPLICATION_TIME�܂ŃJ�E���g����
	int m_cnt;

	// �����̃L����ID �X�L���I�����ɏ�������
	std::vector<int> m_duplicationId;

public:
	HeartSkill(int loopNum, World* world);

	// �Q�b�^
	inline int getLoopNum() const { return m_loopNum; }
	inline int getLoopNow() const { return m_loopNow; }
	inline World* getWorld() const { return m_loopNow < m_loopNum ? m_duplicationWorld : m_world_p; }
	inline double getCnt() const { return ((double)DUPLICATION_TIME / 60.0) - ((double)m_cnt / 60.0); }

	// �X�L���i�s��
	bool play();

private:
	// ���E�̃R�s�[����� �R�s�[�̕ύX�̓I���W�i���ɉe�����Ȃ�
	World* createDuplicationWorld(const World* world);

	// ����L�^���R�s�[����
	void copyRecord(const World* from, World* to);

	void createDuplicationHeart();
};


class Game {
private:
	GameData* m_gameData;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer;

	// ���E
	World* m_world;

	// �X�g�[���[
	Story* m_story;

	// �X�L��
	HeartSkill* m_skill;

public:
	Game();
	~Game();

	// �Q�b�^
	World* getWorld() const { return m_world; }
	HeartSkill* getSkill() const { return m_skill; }

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �Q�[�����v���C����
	bool play();
};

#endif