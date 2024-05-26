#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE


#include <vector>
#include <string>


class Animation;
class Brain;
class Camera;
class Character;
class CharacterAction;
class CharacterController;
class CharacterData;
class CharacterLoader;
class Conversation;
class DoorData;
class DoorObject;
class GraphHandles;
class Item;
class Movie;
class Object;
class ObjectLoader;
class SoundPlayer;


class World {
private:

	// �𑜓x�̔{��
	double m_exX, m_exY;

	// �����Ȃ�true �w�i���f���[�g���Ȃ�����
	bool m_duplicationFlag;

	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer_p;

	// ��b�C�x���g EventElement�N���X������炤
	Conversation* m_conversation_p;

	Conversation* m_objectConversation;

	// ���[�r�[ EventElement�N���X������炤
	Movie* m_movie_p;

	// �X�L���������̓G���A�Ԃ̈ړ��ł��Ȃ�
	bool m_skillFlag;

	// ��ʂ̖��邳
	int m_brightValue;

	// �J�����Ō��Ă���L������ID
	int m_focusId;

	// �v���C���[�i�n�[�g�j��ID
	int m_playerId;

	// ���ܐ��E�̂ǂ̃G���A�ɂ��邩
	int m_areaNum;
	int m_nextAreaNum;

	// �G���A�ړ�����BGM����������~�߂�Ȃ�true
	bool m_resetBgmFlag;

	// �`�悷�邩�ǂ���(����ƃe�L�X�g�͏���)
	bool m_blindFlag;

	// ���ԑ� 0�͒��A1�͗[���A2�͖�
	int m_date;

	// �G���A�ړ����֎~����Ă���Ȃ�true
	bool m_areaLock;

	// �`��p�̃J���� World���f���[�g����
	Camera* m_camera;

	// �J�����̍ő�E�ŏ��{��
	double m_cameraMaxEx = 1.5;
	double m_cameraMinEx = 0.5;

	// ���E�ɑ��݂���L�����N�^�[ World���f���[�g����
	std::vector<Character*> m_characters;

	// �v���C���[ ����for����ID�������Ȃ��p
	Character* m_player_p;

	// �퓬�̂��߂ɃL�����𓮂����R���g���[�� World���f���[�g����
	std::vector<CharacterController*> m_characterControllers;

	// �ǂ⏰�̃I�u�W�F�N�g World���f���[�g����
	std::vector<Object*> m_stageObjects;

	// �G���A�Ԃ��Ȃ���� World���f���[�g����
	std::vector<Object*> m_doorObjects;

	// �U���̂����蔻��̃I�u�W�F�N�g World���f���[�g����
	std::vector<Object*> m_attackObjects;

	// �G�t�F�N�g���̃A�j���[�V���� World���f���[�g����
	std::vector<Animation*> m_animations;

	// �G���A�ɗ����Ă���A�C�e��
	std::vector<Item*> m_itemVector;

	// �L���������ꂽ���̃G�t�F�N�g�摜
	GraphHandles* m_characterDeadGraph;

	// �L�������_���[�W�󂯂����̃G�t�F�N�g�摜
	GraphHandles* m_characterDamageGraph;

	// �����̉摜
	GraphHandles* m_bombGraph;

	// �����̌��ʉ�
	int m_bombSound;

	// �L���������ꂽ���̌��ʉ�
	int m_characterDeadSound;

	// �h�A�ɓ��������̌��ʉ�
	int m_doorSound;

	// �J�����̃Y�[���C���E�A�E�g�̌��ʉ�
	int m_cameraInSound;
	int m_cameraOutSound;

	// �w�i
	int m_backGroundGraph;
	int m_backGroundColor;

	// �{�X�����ꂽ���̃G�t�F�N�g��
	int m_bossDeadEffectCnt;

public:
	World();
	World(int fromAreaNum, int toAreaNum, SoundPlayer* soundPlayer);
	World(const World* original);
	~World();

	//�f�o�b�O
	void debug(int x, int y, int color) const;

	// �Q�b�^
	inline int getFocusId() const { return m_focusId; }
	inline int getPlayerId() const { return m_playerId; }
	inline int getBrightValue() const { return m_brightValue; }
	inline int getAreaNum() const { return m_areaNum; }
	inline int getNextAreaNum() const { return m_nextAreaNum; }
	inline int getDate() const { return m_date; }
	inline bool getResetBgmFlag() const { return m_resetBgmFlag; }
	inline bool getBlindFlag() const { return m_blindFlag; }
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<CharacterController*> getCharacterControllers() const { return m_characterControllers; }
	std::vector<Character*> getCharacters() const { return m_characters; }
	std::vector<Object*> getStageObjects() const { return m_stageObjects; }
	std::vector<Object*> getDoorObjects() const { return m_doorObjects; }
	std::vector<Object*> getAttackObjects() const { return m_attackObjects; }
	std::vector<Animation*> getAnimations() const { return m_animations; }
	std::vector<Item*> getItemVector() const { return m_itemVector; }
	inline const int getBackGroundGraph() const { return m_backGroundGraph; }
	inline const int getBackGroundColor() const { return m_backGroundColor; }
	inline const Conversation* getConversation() const { return m_conversation_p; }
	inline const Conversation* getObjectConversation() const { return m_objectConversation; }
	inline Movie* getMovie() const { return m_movie_p; }
	inline SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }
	inline double getCameraMaxEx() const { return m_cameraMaxEx; }
	inline double getCameraMinEx() const { return m_cameraMinEx; }
	inline GraphHandles* getCharacterDeadGraph() const { return m_characterDeadGraph; }
	inline GraphHandles* getCharacterDamageGraph() const { return m_characterDamageGraph; }
	inline GraphHandles* getBombGraph() const { return m_bombGraph; }
	inline int getCharacterDeadSound() const { return m_characterDeadSound; }
	inline int getBombSound() const { return m_bombSound; }
	inline int getDoorSound() const { return m_doorSound; }
	inline bool getSkillFlag() const { return m_skillFlag; }
	inline int getBossDeadEffextCnt() const { return m_bossDeadEffectCnt; }

	// Drawer�p�̃Q�b�^
	std::vector<const CharacterAction*> getActions() const;
	std::vector<const Object*> getFrontObjects() const;
	std::vector<const Object*> getBackObjects() const;
	std::vector<const Animation*> getConstAnimations() const;

	// ���O�ŃL��������
	Character* getCharacterWithName(std::string characterName) const;

	// ���O�ŃR���g���[������
	CharacterController* getControllerWithName(std::string characterName) const;

	// ID�ŃL��������
	Character* getCharacterWithId(int id) const;

	// �Z�b�^
	inline void setFocusId(int id) { m_focusId = id; }
	inline void setConversation(Conversation* conversation) { m_conversation_p = conversation; }
	inline void setMovie(Movie* movie) { m_movie_p = movie; }
	inline void setAreaLock(bool lock) { m_areaLock = lock; }
	inline void setDate(int date) { m_date = date; }
	inline void setBlindFlag(bool blindFlag) { m_blindFlag = blindFlag; }

	// �����I�ɃG���A�ړ�
	inline void moveArea(int nextArea) { m_brightValue--; m_nextAreaNum = nextArea; m_resetBgmFlag = true; }

	// ID�w���Brain�ύX
	void setBrainWithId(int id, Brain* brain);

	// �X�g�[���[�ɂ��L�����ǉ�
	void addCharacter(CharacterLoader* characterLoader);

	// �X�g�[���[�ɂ��L�����̐��\�ω�
	void changeCharacterVersion(int version);

	// �X�g�[���[�ɂ��I�u�W�F�N�g�ǉ�
	void addObject(ObjectLoader* objectLoader);

	// �v���C���[��HP��0�Ȃ�true
	bool playerDead();

	// �v���C���[��HP��MAX�ɂ���
	void playerHpReset();

	// �X�L�������F�n�[�g��Freeze�ɂ���
	void setSkillFlag(bool skillFlag);

	// �X�L�������F�����̃n�[�g�ǉ��p
	void pushCharacter(Character* character, CharacterController* controller);

	// �X�L�������F�����̃n�[�g�폜�p
	void popCharacterController(int id);

	// �X�L�������F���R�[�_���쐬���g�p���J�n
	void createRecorder();

	// �X�L�������F���R�[�_�̎��Ԃ��ŏ��ɖ߂�
	void initRecorder();

	// �X�L�������F���R�[�_�̎g�p����߂č폜����
	void eraseRecorder();

	// �f�[�^�Ǘ��F�L�����̏�Ԃ�ύX���� ���Ȃ��Ȃ�쐬����
	void asignedCharacterData(const char* name, CharacterData* data);

	// �f�[�^�Ǘ��F�L�����̏�Ԃ�������
	void asignCharacterData(const char* name, CharacterData* data, int fromAreaNum, bool notCharacterPoint) const;

	// �f�[�^�Ǘ��FDoor�̏�Ԃ�ύX���� ���Ȃ��Ȃ�쐬����
	void asignedDoorData(DoorData* data);

	// �f�[�^�Ǘ��FDoor�̏�Ԃ�������
	void asignDoorData(std::vector<DoorData*>& data, int fromAreaNum) const;

	// �f�[�^�Ǘ��F�v���C���[���h�A�̑O�܂ňړ�
	void setPlayerOnDoor(int from);

	// �f�[�^�Ǘ��F�J�����̈ʒu�����Z�b�g
	void cameraPointInit();

	// �v���C���[�����̍��W�ֈړ�
	void setPlayerPoint(CharacterData* characterData);

	// ���Ԃ��v���C���[�̈ʒu�ֈړ�
	void setPlayerFollowerPoint();

	// �L�����ɐ�킹��
	void battle();

	// �e�L�������ڕW�n�_�ֈړ����邾��
	bool moveGoalCharacter();

	// ��ʂ��Ó]���邾�� �L�����������Ȃ����ԂȂ�true
	bool dealBrightValue();

	// �L�����ɉ�b������
	void talk();

	// ���[�r�[�𗬂�
	void moviePlay();

private:

	// �f�[�^�Ǘ��F�L�����̃Z�[�u�f�[�^�����g�ɔ��f������
	void asignedCharacter(Character* character, CharacterData* data, bool changePosition);

	// �f�[�^�Ǘ��F�R���g���[��1�̏��𐢊E�ɔ��f
	CharacterController* createControllerWithData(const Character* character, CharacterData* data);

	// Battle�F�J�����̍X�V
	void updateCamera();

	// Battle�F�A�j���[�V�����̍X�V
	void updateAnimation();

	// Battle�F�L�����̍X�V�i�U���Ώۂ̕ύX�j
	void updateCharacter();

	// Battle�F�L�����N�^�[�̓���
	void controlCharacter();

	// Battle�F�I�u�W�F�N�g�̓���
	void controlObject();

	// Battle�F�A�C�e���̓���
	void controlItem();

	// Battle�F�L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻�� slope=true�Ȃ�slope���Ώ�false�Ȃ炻��ȊO
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects, bool slope);

	// Battle�F�L�����N�^�[�Ɣ��̓����蔻��
	void atariCharacterAndDoor(CharacterController* controller, std::vector<Object*>& objects);

	// Battle�F�ǂ⏰<->�U���̓����蔻��
	void atariStageAndAttack();

	// Battle�F�U��<->�U���̓����蔻��
	void atariAttackAndAttack();

	// Battle: �������N����
	void createBomb(int x, int y, Object* attackObject);

	// Battle: �_���[�W�G�t�F�N�g�쐬
	void createDamageEffect(int x, int y, int sum);

	// Battle: �{�X�����ꂽ�Ƃ��̔����G�t�F�N�g
	void createBossDeadEffect();

};

#endif