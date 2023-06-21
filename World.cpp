#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// �L���[�ɓ������S�I�u�W�F�N�g���폜����
void deleteObject(queue<Object*>& objects) {
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// �폜
		delete object;
		objectSum--;
	}
}

// �L���[�ɓ������S�I�u�W�F�N�g�𓮂���
void actionObject(queue<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// �I�u�W�F�N�g�̓���
		object->action();
		// deleteFlag��true�Ȃ�폜����
		if (object->getDeleteFlag()) {
			delete object;
		}
		else {
			// �폜���Ȃ�
			objects.push(object);
		}
		objectSum--;
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
void atariCharacterAndObject(CharacterController* controller, queue<Object*> objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	size_t objectSum = objects.size();
	while (objectSum > 0) {
		Object* object = objects.front();
		objects.pop();
		// �����蔻��������ōs��
		object->atari(controller);
		// deleteFlag��true�Ȃ�폜����
		if (object->getDeleteFlag()) {
			delete object;
		}
		else {
			// �폜���Ȃ�
			objects.push(object);
		}
		objectSum--;
	}
}


/*
* �I�u�W�F�N�g�̃��[�h�Ȃ�
*/
World::World(int areaNum) {
	// ��l���̃X�^�[�g�n�_
	m_areaNum = areaNum;

	// �G���A�ɑ��݂���I�u�W�F�N�g�����[�h
	Object* object1 = new BoxObject(0, 900, 1920, 1080, WHITE);
	m_stageObjects.push(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_stageObjects.push(object3);
	m_stageObjects.push(object2);

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
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

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

	// �L�����N�^�[�̓���
	controlCharacter();

	// �I�u�W�F�N�g�̓���
	controlObject();

}

// �L�����N�^�[�̓���
void World::controlCharacter() {
	// �e�R���g���[���𓮍삳����
	size_t characterSum = m_characterControllers.size();
	while (characterSum > 0) {
		// �R���g���[�����擾
		CharacterController* controller = m_characterControllers.front();
		m_characterControllers.pop();

		// �s���O�̏���
		controller->init();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);

		// ����
		controller->control();

		// �ˌ��U��
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != NULL) { m_attackObjects.push(bulletAttack); }

		// �a���U��
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != NULL) { m_attackObjects.push(slashAttack); }

		// ���f
		controller->action();

		// ���̃��[�v��
		m_characterControllers.push(controller);
		characterSum--;
	}
}

// �I�u�W�F�N�g�̓���
void World::controlObject() {
	// �ǂ⏰�̓���
	actionObject(m_stageObjects);

	// �U�������蔻��̓���
	actionObject(m_attackObjects);

	// �ǂ⏰<->�U���̓����蔻��
	atariStageAndAttack();
}

// �ǂ⏰<->�U���̓����蔻��
void World::atariStageAndAttack() {
	size_t attackObjectSum = m_attackObjects.size();
	while (attackObjectSum > 0) {
		// �U���̃I�u�W�F�N�g
		Object* attackObject = m_attackObjects.front();
		m_attackObjects.pop();

		// ����ǂƂ̓����蔻��
		size_t stageObjectSum = m_stageObjects.size();
		while (stageObjectSum > 0) {
			// �U���̃I�u�W�F�N�g
			Object* stageObject = m_stageObjects.front();
			m_stageObjects.pop();

			// �������Ă��邩����


			// deleteFlag��true�Ȃ�폜����
			if (stageObject->getDeleteFlag()) {
				delete stageObject;
			}
			else {
				// �폜���Ȃ�
				m_stageObjects.push(stageObject);
			}
			stageObjectSum--;
		}

		// deleteFlag��true�Ȃ�폜����
		if (attackObject->getDeleteFlag()) {
			delete attackObject;
		}
		else {
			// �폜���Ȃ�
			m_attackObjects.push(attackObject);
		}

		attackObjectSum--;
	}
}

// ��b������
void World::talk() {

}