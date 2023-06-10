#include "World.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


/*
* �I�u�W�F�N�g�̃��[�h�Ȃ�
*/
World::World(int areaNum) {
	// ��l���̃X�^�[�g�n�_
	m_areaNum = areaNum;

	// ��l�������[�h
	Heart* heart = new Heart(100, 100, 0, 0, 5);
	m_characters.push(heart);

	// ��l���̓������쐬
	StickAction* heartAction = new StickAction(heart);

	//��l���̃R���g���[���쐬
	CharacterKeyboardController* heartController = new CharacterKeyboardController(heartAction);
	m_characterControllers.push(heartController);
}

World::~World() {
	// �S�R���g���[�����폜����B
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		delete controller;
		characterSum--;
	}
}

void World::battle() {

	// �e�R���g���[���𓮍삳����
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		controller->control();
		m_characterControllers.push(controller);
		characterSum--;
	}
}

void World::talk() {

}