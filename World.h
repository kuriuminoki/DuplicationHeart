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

	// ���ܐ��E�̂ǂ̃G���A�ɂ��邩�i�������ߖ�̂��߃v���C���[�̕t�߂݂̂�ǂݍ��ށj
	int m_areaNum;
	int m_nextAreaNum;

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
	Character* m_player;

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

	// �L���������ꂽ���̌��ʉ�
	int m_characterDeadSound;

	// �w�i
	int m_backGroundGraph;
	int m_backGroundColor;

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
	inline const Movie* getMovie() const { return m_movie_p; }
	inline SoundPlayer* getSoundPlayer() const { return m_soundPlayer_p; }
	inline double getCameraMaxEx() const { return m_cameraMaxEx; }
	inline double getCameraMinEx() const { return m_cameraMinEx; }
	inline GraphHandles* getCharacterDeadGraph() const { return m_characterDeadGraph; }
	inline int getCharacterDeadSound() const { return m_characterDeadSound; }

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

	// ID�w���Brain�ύX
	void setBrainWithId(int id, Brain* brain);

	// �X�g�[���[�ɂ��L�����ǉ�
	void addCharacter(CharacterLoader* characterLoader);

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

	// �f�[�^�Ǘ��F�v���C���[�Ƃ��̒��Ԃ��h�A�̑O�Ɉړ�
	void setPlayerOnDoor(int from);

	// �L�����ɐ�킹��
	void battle();

	// �L�����ɉ�b������
	void talk();

	// ���[�r�[�𗬂�
	void moviePlay();

private:

	// �f�[�^�Ǘ��F�J�����̈ʒu�����Z�b�g
	void cameraPointInit();

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

	// Battle�F�L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects);

	// Battle�F�L�����N�^�[�Ɣ��̓����蔻��
	void atariCharacterAndDoor(CharacterController* controller, std::vector<Object*>& objects);

	// Battle�F�ǂ⏰<->�U���̓����蔻��
	void atariStageAndAttack();

	// Battle�F�U��<->�U���̓����蔻��
	void atariAttackAndAttack();

};

#endif