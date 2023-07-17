#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
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
	Object* object1 = new BoxObject(0, 900, 10000, 1080, WHITE);
	m_stageObjects.push(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	m_stageObjects.push(object2);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_stageObjects.push(object3);
	Object* object4 = new TriangleObject(2000, 600, 2600, 900, WHITE, true);
	m_stageObjects.push(object4);
	Object* object5 = new BoxObject(2600, 600, 4000, 900, WHITE);
	m_stageObjects.push(object5);

	// ��l�������[�h �L�����̍폜��World�����\��
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push(heart);

	// �J��������l�����ځA�{��1.0�ō쐬
	m_playerId = heart->getId();
	m_focusId = m_playerId;
	m_camera = new Camera(0, 0, 1.0, CAMERA_SPEED_DEFAULT);
	m_camera->setPoint(heart->getCenterX(), heart->getCenterY());
	updateCamera();

	//��l���̃R���g���[���쐬 Brain��Action�̍폜��Controller�����B
	NormalController* heartController = new NormalController(new KeyboardBrain(m_camera), new StickAction(heart));
	m_characterControllers.push(heartController);

	// CPU�����[�h
	Heart* cp = new Heart(100, 100, 500, 0);
	m_characters.push(cp);
	//CPU�̃R���g���[���쐬 Brain��Action�̍폜��Controller�����B
	NormalController* cpController = new NormalController(new NormalAI(), new StickAction(cp));
	m_characterControllers.push(cpController);
}

World::~World() {
	// �J�������폜����
	delete m_camera;

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

// CharacterAction�̃L���[��Ԃ�
queue<const CharacterAction*> World::getActions() const {
	// �R���g���[���̃R�s�[���쐬�iconst���\�b�h�ɂ��邽�ߑ΍�j
	queue<CharacterController*> controllers = m_characterControllers;

	queue<const CharacterAction*> actions;
	while (!controllers.empty()) {
		// �R���g���[�����擾
		CharacterController* controller = controllers.front();
		controllers.pop();
		actions.push(controller->getAction());
	}
	return actions;
}

// Object�̃L���[��Ԃ�
queue<const Object*> World::getObjects() const {
	// Object�̃R�s�[���쐬�iconst���\�b�h�ɂ��邽�ߑ΍�j
	queue<Object*> stageObjects = m_stageObjects;
	queue<Object*> attackObjects = m_attackObjects;

	queue<const Object*> objects;
	while (!stageObjects.empty()) {
		// Object��push
		objects.push(stageObjects.front());
		stageObjects.pop();
	}
	while (!attackObjects.empty()) {
		// Object��push
		objects.push(attackObjects.front());
		attackObjects.pop();
	}
	return objects;
}

// ��킹��
void World::battle() {

	// HP0�̃L�����폜
	cleanCharacter();

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	updateCharacter();

	// �L�����N�^�[�̓���
	controlCharacter();

	// �I�u�W�F�N�g�̓���
	controlObject();

	// �J�����̍X�V
	updateCamera();

}

// �g�o�O�̃L�����폜
void World::cleanCharacter() {
	size_t characterSum = m_characters.size();
	while (characterSum > 0) {
		Character* character = m_characters.front();
		m_characters.pop();
		if (character->getHp() <= 0) {
			delete character;
		}
		else {
			m_characters.push(character);
		}
		characterSum--;
	}
}

// �L�����̍X�V�i�U���Ώۂ̕ύX�j
void World::updateCharacter() {
	size_t characterSum = m_characters.size();
	while (characterSum > 0) {
		Character* character = m_characters.front();
		m_characters.pop();
		if (GetRand(99) == 0) { // 1%�̊m���ōU���ΏەύX
			
		}
		m_characters.push(character);
		characterSum--;
	}
}

// �J�����̍X�V
void World::updateCamera() {
	size_t characterSum = m_characters.size();
	int x = 0, y = 0;
	while (characterSum > 0) {
		Character* character = m_characters.front();
		m_characters.pop();
		// ���t�H�[�J�X���Ă���L�����̍��W�ɍ��킹��
		if(m_focusId == character->getId()){
			x = character->getCenterX();
			y = character->getCenterY();
			m_camera->setGPoint(x, y);
		}
		m_characters.push(character);
		characterSum--;
	}
	// �J�����͂�����蓮��
	m_camera->move();
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
			stageObject->atariObject(attackObject);

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