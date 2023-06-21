#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDE

#include<queue>

class CharacterController;
class Character;
class Object;

class World {
private:
	// ���E�ɑ��݂���L�����N�^�[
	std::queue<Character*> m_characters;

	// �퓬�̂��߂ɃL�����𓮂����R���g���[��
	std::queue<CharacterController*> m_characterControllers;

	// �ǂ⏰�̃I�u�W�F�N�g
	std::queue<Object*> m_stageObjects;

	// �U���̂����蔻��̃I�u�W�F�N�g
	std::queue<Object*> m_attackObjects;

	// ���ܐ��E�̂ǂ̃G���A�ɂ��邩�i�������ߖ�̂��߃v���C���[�̕t�߂݂̂�ǂݍ��ށj
	int m_areaNum;

public:
	World(int areaNum);
	~World();

	//�f�o�b�O
	void debug(int x, int y, int color);

	// �L�����ɐ�킹��
	void battle();

	// �L�����N�^�[�̓���
	void controlCharacter();

	// �I�u�W�F�N�g�̓���
	void controlObject();

	// �ǂ⏰<->�U���̓����蔻��
	void atariStageAndAttack();

	// �L�����ɉ�b������
	void talk();

};

#endif