#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <string>

class SoundPlayer;
class World;
class Story;
class Character;
class BattleOption;


// �L�����̃Z�[�u�f�[�^
class CharacterData {
private:

	// ���O
	std::string m_name;

	// Info�̃o�[�W����
	int m_version;

	// HP
	int m_hp;

	// ���G�Ȃ�true
	bool m_invincible;

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

	// �Z�[�u�ƃ��[�h
	void save(FILE* intFp, FILE* strFp);
	void load(FILE* intFp, FILE* strFp);

	// �Q�b�^
	inline const char* name() const { return m_name.c_str(); }
	inline const int version() const { return m_version; }
	inline int hp() const { return m_hp; }
	inline bool invincible() const { return m_invincible; }
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
	inline void setVersion(int version) { m_version = version; }
	inline void setHp(int hp) { m_hp = hp; }
	inline void setInvincible(bool invincible) { m_invincible = invincible; }
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


class DoorData {
private:

	// ���W
	int m_x1, m_y1, m_x2, m_y2;

	// �ǂ�����ǂ��ւ̃h�A��
	int m_from, m_to;

	// �摜�̃t�@�C����
	std::string m_fileName;

public:
	DoorData(FILE* intFp, FILE* strFp);
	DoorData(int x1, int y1, int x2, int y2, int from, int to, const char* fileName);

	// �Z�[�u�ƃ��[�h
	void save(FILE* intFp, FILE* strFp);
	void load(FILE* intFp, FILE* strFp);

	// �Q�b�^
	inline int x1() const { return m_x1; }
	inline int y1() const { return m_y1; }
	inline int x2() const { return m_x2; }
	inline int y2() const { return m_y2; }
	inline int from() const { return m_from; }
	inline int to() const { return m_to; }
	inline const char* fileName() const { return m_fileName.c_str(); }

	// �Z�b�^
	inline void setX1(int x1) { m_x1 = x1; }
	inline void setY1(int y1) { m_y1 = y1; }
	inline void setX2(int x2) { m_x2 = x2; }
	inline void setY2(int y2) { m_y2 = y2; }
	inline void setFrom(int from) { m_from = from; }
	inline void setTo(int to) { m_to = to; }
	inline void setFileName(const char* fileName) { m_fileName = fileName; }
};


/*
* �N���A�����C�x���g�̃��X�g
*/
class EventData {
private:

	// �N���A�����C�x���g�ԍ�
	std::vector<int> m_clearEvent;

public:

	EventData();
	EventData(FILE* eventFp);

	void save(FILE* eventFp);
	void load(FILE* eventFp);

	// ����̃C�x���g���N���A���Ă邩
	bool checkClearEvent(int eventNum);

	//����̃C�x���g���N���A����
	void setClearEvent(int eventNum);

};


// �Z�[�u�f�[�^
class GameData {
private:
	// �Z�[�u����ꏊ
	std::string m_saveFilePath;

	// �Z�[�u�f�[�^�����݂��邩
	bool m_exist;

	// �L�����̃f�[�^
	std::vector<CharacterData*> m_characterData;

	// �h�A�̃f�[�^
	std::vector<DoorData*> m_doorData;

	// �C�x���g�̃f�[�^
	EventData* m_eventData;

	// ������G���A
	int m_areaNum;

	// ������Ă���X�g�[���[
	int m_storyNum;

	// �ŐV�̖��N���A�X�g�[���[
	int m_latestStoryNum;

	// ����
	int m_soundVolume;

	// �Z�[�u�����̒ʒm��\������c�莞��
	int m_noticeSaveDone;

public:
	GameData();
	GameData(const char* saveFilePath);
	GameData(const char* saveFilePath, int storyNum);
	~GameData();

	// �Z�[�u�����̒ʒm��\�����鎞��
	const int NOTICE_SAVE_DONE_TIME = 300;

	// �Z�[�u�ƃ��[�h
	bool save();
	bool load();
	bool saveChapter();
	// �S�Z�[�u�f�[�^����
	bool saveCommon(int soundVolume, int gameWide, int gameHeight);
	bool loadCommon(int* soundVolume, int* gameWide, int* gameHeight);

	// �Q�b�^
	inline bool getExist() const { return m_exist; }
	inline int getAreaNum() const { return m_areaNum; }
	inline int getStoryNum() const { return m_storyNum; }
	inline int getSoundVolume() const { return m_soundVolume; }
	inline const char* getSaveFilePath() const { return m_saveFilePath.c_str(); }
	inline int getDoorSum() const { return (int)m_doorData.size(); }
	inline int getFrom(int i) const { return m_doorData[i]->from(); }
	inline int getTo(int i) const { return m_doorData[i]->to(); }
	inline int getLatestStoryNum() const { return m_latestStoryNum; }
	inline EventData* getEventData() { return m_eventData; }
	inline int getNoticeSaveDone() const { return m_noticeSaveDone; }
	CharacterData* getCharacterData(std::string characterName);

	// �Z�b�^
	inline void setAreaNum(int areaNum) { m_areaNum = areaNum; }
	inline void setStoryNum(int storyNum) { m_storyNum = storyNum; }
	inline void setSoundVolume(int soundVolume) { m_soundVolume = soundVolume; }
	inline void setNoticeSaveDone(int noticeSaveDone) { m_noticeSaveDone = noticeSaveDone; }

	// �Z�[�u�f�[�^�폜
	void removeSaveData();

	// ���g�̃f�[�^��World�Ƀf�[�^���f������
	void asignWorld(World* world, bool playerHpReset = false);

	// World�̃f�[�^�����g�ɔ��f������
	void asignedWorld(const World* world, bool notCharacterPoint);

	// �X�g�[���[���i�񂾎��ɃZ�[�u�f�[�^���X�V����
	void updateStory(Story* story);

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

	// �X�L���i�s�� �X�L���I������true
	bool play();

	// ��킹��i����L�^������Ƃ������������������j
	void battle();

	// ����L�^���I��������ǂ����̔���
	bool finishRecordFlag();

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

	// �Q�[���I�[�o�[�̕\��
	int m_gameoverCnt;

	// �X�g�[���[
	Story* m_story;

	// �X�L��
	HeartSkill* m_skill;

	// �ꎞ��~���
	BattleOption* m_battleOption;

	// �ꎞ��~��ʉ�
	int m_pauseSound;

	// �Q�[���̍ċN���i�^�C�g���֖߂�j��v��
	bool m_rebootFlag;

public:
	Game(const char* saveFilePath = "savedata/test/", int storyNum = -1);
	~Game();

	// �Q�b�^
	World* getWorld() const { return m_world; }
	HeartSkill* getSkill() const { return m_skill; }
	BattleOption* getGamePause() const { return m_battleOption; }
	bool getRebootFlag() const { return m_rebootFlag; }
	inline int getGameoverCnt() const { return m_gameoverCnt; }
	inline const GameData* getGameData() const { return m_gameData; }

	// �f�o�b�O
	void debug(int x, int y, int color) const;

	// �Q�[�����v���C����
	bool play();

	// �Z�[�u�f�[�^�����[�h�i�O�̃Z�[�u�|�C���g�֖߂�j
	void backPrevSave();
};

#endif