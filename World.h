#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<vector>

class CharacterController;
class CharacterAction;
class Character;
class Object;
class Camera;
class Animation;
class SoundPlayer;

class World {
private:
	// �T�E���h�v���C���[
	SoundPlayer* m_soundPlayer;

	// �`��p�̃J����
	Camera* m_camera;

	// �J�����̈ړ����x�̏����l
	const int CAMERA_SPEED_DEFAULT = 5;

	// �J�����Ō��Ă���L������ID
	int m_focusId;

	// �v���C���[�i�n�[�g�j��ID
	int m_playerId;

	// ���E�ɑ��݂���L�����N�^�[
	std::vector<Character*> m_characters;

	// �퓬�̂��߂ɃL�����𓮂����R���g���[��
	std::vector<CharacterController*> m_characterControllers;

	// �ǂ⏰�̃I�u�W�F�N�g
	std::vector<Object*> m_stageObjects;

	// �U���̂����蔻��̃I�u�W�F�N�g
	std::vector<Object*> m_attackObjects;

	// �G�t�F�N�g���̃A�j���[�V����
	std::vector<Animation*> m_animations;

	// ���ܐ��E�̂ǂ̃G���A�ɂ��邩�i�������ߖ�̂��߃v���C���[�̕t�߂݂̂�ǂݍ��ށj
	int m_areaNum;

public:
	World(int areaNum, SoundPlayer* soundPlayer);
	~World();

	// �Q�b�^
	inline const Camera* getCamera() const { return m_camera; }
	std::vector<const CharacterAction*> getActions() const;
	std::vector<const Object*> getObjects() const;
	std::vector<const Animation*> getAnimations() const;

	//�f�o�b�O
	void debug(int x, int y, int color) const;

	// �L�����ɐ�킹��
	void battle();

	// �L�����ɉ�b������
	void talk();

private:
	// �L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
	void atariCharacterAndObject(CharacterController* controller, std::vector<Object*>& objects);

	// �A�j���[�V�����̍X�V
	void updateAnimation();

	// �g�o�O�̃L�����폜
	void cleanCharacterController();

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	void updateCharacter();

	// �J�����̍X�V
	void updateCamera();

	// �L�����N�^�[�̓���
	void controlCharacter();

	// �I�u�W�F�N�g�̓���
	void controlObject();

	// �ǂ⏰<->�U���̓����蔻��
	void atariStageAndAttack();

};

#endif