#include "World.h"
#include "Object.h"
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

	// �G���A�ɑ��݂���I�u�W�F�N�g�����[�h
	Object* object1 = new BoxObject(0, 900, 1920, 1080, WHITE);
	m_objects.push(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_objects.push(object2);
	m_objects.push(object3);

	// ��l�������[�h
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push(heart);

	// ��l���̓������쐬
	StickAction* heartAction = new StickAction(heart);

	//��l���̃R���g���[���쐬
	CharacterKeyboardController* heartController = new CharacterKeyboardController(heartAction);
	m_characterControllers.push(heartController);
}

World::~World() {
	// �S�I�u�W�F�N�g���폜����B
	size_t objectSum = m_objects.size();
	while (objectSum > 0) {
		Object* object = m_objects.front();
		m_objects.pop();
		delete object;
		objectSum--;
	}

	// �S�R���g���[�����폜����B
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();
		delete controller;
		characterSum--;
	}
}

// ��킹��
void World::battle() {

	// �e�R���g���[���𓮍삳����
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();

		// �s���O�̏���
		controller->init();

		// �����蔻��
		size_t objectSum = m_objects.size();
		while (objectSum > 0) {
			Object* object = m_objects.front();
			m_objects.pop();
			// �����蔻��������ōs��
			object->atari(controller);
			m_objects.push(object);
			objectSum--;
		}

		// ����
		controller->control();

		// ���f
		controller->action();

		m_characterControllers.push(controller);
		characterSum--;
	}
}

// ��b������
void World::talk() {

}