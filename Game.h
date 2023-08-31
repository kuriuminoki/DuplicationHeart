#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <string>

class SoundPlayer;
class World;
class Story;
class Character;


// �L�����̃Z�[�u�f�[�^
class CharacterData {
private:

	// ���O
	const char* m_name;

	// HP
	int m_hp;

	// ID
	int m_id;

	// GroupID
	int m_groupId;

	// �ǂ��̃G���A�ɂ��邩
	int m_areaNum;

	// ���W
	int m_x, m_y;

	// Brain�̃N���X��
	std::string m_brainName;

	// �U���Ώۂ̖��O ���Ȃ��Ȃ�󕶎�
	std::string m_targetName;

	// �ǐՑΏۂ̖��O ���Ȃ��Ȃ�󕶎�
	std::string m_followName;

	// CharacterAction�̃N���X��
	std::string m_actionName;

	// ���ʉ����肩
	bool m_soundFlag;

	// CharacterController�̃N���X��
	std::string m_controllerName;

public:
	CharacterData(const char* name);

	// �Q�b�^
	inline const char* name() const { return m_name; }
	inline int hp() const { return m_hp; }
	inline int id() const { return m_id; }
	inline int groupId() const { return m_groupId; }
	inline int areaNum() const { return m_areaNum; }
	inline int x() const { return m_x; }
	inline int y() const { return m_y; }
	inline std::string brainName() const { return m_brainName; }
	inline std::string targetName() const { return m_targetName; }
	inline std::string followName() const { return m_followName; }
	inline std::string actionName() const { return m_actionName; }
	inline bool soundFlag() const { return m_soundFlag; }
	inline std::string controllerName() const { return m_controllerName; }

	// �Z�b�^
	inline void setHp(int hp) { m_hp = hp; }
	inline void setId(int id) { m_id = id; }
	inline void setGroupId(int groupId) { m_groupId = groupId; }
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setBrainName(const char* brainName) { m_brainName = brainName; }
	inline void setTargetName(const char* targetName) { m_targetName = targetName; }
	inline void setFollowName(const char* followName) { m_followName = followName; }
	inline void setActionName(const char* actionName) { m_actionName = actionName; }
	inline void setSoundFlag(bool soundFlag) { m_soundFlag = soundFlag; }
	inline void setControllerName(const char* controllerName) { m_controllerName = controllerName; }
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

	// ����
	int m_soundVolume;

public:
	GameData();
	GameData(const char* saveFilePath);

	// �Q�b�^
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }
	inline int getSoundVolume() const { return m_soundVolume; }

	// �Z�b�^
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }
	inline void setSoundVolume(int soundVolume) { m_soundVolume = soundVolume; }

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

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// ���ʉ�
	int m_sound;

public:
	HeartSkill(int loopNum, World* world, SoundPlayer* soundPlayer);
	~HeartSkill();

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