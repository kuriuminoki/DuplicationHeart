#include "World.h"
#include "Object.h"
#include "Character.h"
#include "CharacterAction.h"
#include "CharacterController.h"
#include "Camera.h"
#include "Animation.h"
#include "Define.h"
#include "DxLib.h"


using namespace std;


// vector�ɓ������S�I�u�W�F�N�g���폜����
void deleteAllObject(vector<Object*>& objects) {
	for (int i = (int)objects.size() - 1; i >= 0; i--) {
		delete objects[i];
		objects.pop_back();
	}
}

// vector�ɓ�����deleteFlag��true�̃I�u�W�F�N�g���폜����
void deleteObject(vector<Object*>& objects) {
	for (unsigned int i = 0; i < objects.size(); i++) {
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// vector�ɓ������S�I�u�W�F�N�g�𓮂���
void actionObject(vector<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �I�u�W�F�N�g�̓���
		objects[i]->action();
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̊ђʎ�����
void penetrationCharacterAndObject(CharacterController* controller, vector<Object*> objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		objects[i]->penetration(controller);
	}
}


/*
* �I�u�W�F�N�g�̃��[�h�Ȃ�
*/
World::World(int areaNum) {
	// ��l���̃X�^�[�g�n�_
	m_areaNum = areaNum;

	// �G���A�ɑ��݂���I�u�W�F�N�g�����[�h
	Object* object1 = new BoxObject(-500, 900, 10000, 1080, WHITE);
	m_stageObjects.push_back(object1);
	Object* object2 = new TriangleObject(700, 600, 1300, 900, WHITE, false);
	m_stageObjects.push_back(object2);
	Object* object3 = new BoxObject(300, 600, 700, 900, WHITE);
	m_stageObjects.push_back(object3);
	Object* object4 = new TriangleObject(2000, 600, 2600, 900, WHITE, true);
	m_stageObjects.push_back(object4);
	Object* object5 = new BoxObject(2600, 600, 4000, 900, WHITE);
	m_stageObjects.push_back(object5);
	Object* object6 = new BoxObject(-500, -1000, -400, 1080, WHITE);
	m_stageObjects.push_back(object6);
	Object* object7 = new BoxObject(5000, -1000, 5100, 1080, WHITE);
	m_stageObjects.push_back(object7);
	Object* object8 = new BoxObject(3000, 400, 3300, 600, WHITE);
	m_stageObjects.push_back(object8);

	// ��l�������[�h �L�����̍폜��World�����\��
	Heart* heart = new Heart(100, 100, 0, 0);
	m_characters.push_back(heart);

	// �J��������l�����ځA�{��1.0�ō쐬
	m_playerId = heart->getId();
	m_focusId = m_playerId;
	m_camera = new Camera(0, 0, 1.0, CAMERA_SPEED_DEFAULT);
	m_camera->setPoint(heart->getCenterX(), heart->getCenterY());
	updateCamera();

	//��l���̃R���g���[���쐬 Brain��Action�̍폜��Controller�����B
	NormalController* heartController = new NormalController(new KeyboardBrain(m_camera), new StickAction(heart));
	m_characterControllers.push_back(heartController);

	// CPU�����[�h
	Heart* cp = new Heart(100, 100, 500, 0);
	m_characters.push_back(cp);
	//CPU�̃R���g���[���쐬 Brain��Action�̍폜��Controller�����B
	NormalController* cpController = new NormalController(new NormalAI(), new StickAction(cp));
	m_characterControllers.push_back(cpController);
}

World::~World() {
	// �J�������폜����
	delete m_camera;

	// �S�I�u�W�F�N�g���폜����B
	deleteAllObject(m_stageObjects);
	deleteAllObject(m_attackObjects);

	// �S�R���g���[�����폜����B
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		delete m_characterControllers[i];
	}
}

// CharacterAction��vector��Ԃ�
vector<const CharacterAction*> World::getActions() const {
	vector<const CharacterAction*> actions;
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		actions.push_back(m_characterControllers[i]->getAction());
	}
	return actions;
}

// Object�̃L���[��Ԃ�
vector<const Object*> World::getObjects() const {

	vector<const Object*> allObjects;
	allObjects.insert(allObjects.end(), m_stageObjects.begin(), m_stageObjects.end());
	allObjects.insert(allObjects.end(), m_attackObjects.begin(), m_attackObjects.end());

	return allObjects;
}

// ��킹��
void World::battle() {

	// HP0�̃L�����R���g���[���폜
	cleanCharacterController();

	// deleteFlag��true�̃I�u�W�F�N�g���폜����B
	deleteObject(m_stageObjects);
	deleteObject(m_attackObjects);

	// �L�����̍X�V�i�U���Ώۂ̕ύX�j
	// ��ŃL�������폜��������X�V��������K�v
	updateCharacter();

	// �L�����N�^�[�̓���
	controlCharacter();

	// �I�u�W�F�N�g�̓���
	controlObject();

	// �J�����̍X�V
	updateCamera();

	// �A�j���[�V�����̍X�V
	updateAnimation();

}

// �g�o�O�̃L�����R���g���[���폜
void World::cleanCharacterController() {
	for (unsigned int i = 0; i < m_characterControllers.size(); i++) {
		if (m_characterControllers[i]->getAction()->getCharacter()->getHp() == 0) {
			delete m_characterControllers[i];
			m_characterControllers[i] = m_characterControllers.back();
			m_characterControllers.pop_back();
			i--;
		}
	}
}

// �L�����̍X�V�i�U���Ώۂ̕ύX�j
void World::updateCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		// Brain�̗v���ōU���ΏەύX
		if (m_characterControllers[i]->getBrain()->needSearchTarget()) { 
			Character* target = m_characters[GetRand((int)m_characters.size() - 1)];
			m_characterControllers[i]->searchTargetCandidate(target);
		}
	}
}

// �J�����̍X�V
void World::updateCamera() {
	size_t size = m_characters.size();
	int x = 0, y = 0;
	for (unsigned int i = 0; i < size; i++) {
		// ���t�H�[�J�X���Ă���L�����̍��W�ɍ��킹��
		if (m_focusId == m_characters[i]->getId()) {
			x = m_characters[i]->getCenterX();
			y = m_characters[i]->getCenterY();
			m_camera->setGPoint(x, y);
		}
	}
	// �J�����͂�����蓮��
	m_camera->move();
}

// �A�j���[�V�����̍X�V
void World::updateAnimation() {
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		m_animations[i]->count();
		if (m_animations[i]->getFinishFlag()) {
			delete m_animations[i];
			m_animations[i] = m_animations.back();
			m_animations.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�ƃI�u�W�F�N�g�̓����蔻��
void World::atariCharacterAndObject(CharacterController* controller, vector<Object*>& objects) {
	// �ǂ⏰�I�u�W�F�N�g�̏��� (�����蔻��Ɠ���)
	for (unsigned int i = 0; i < objects.size(); i++) {
		// �����蔻��������ōs��
		if (objects[i]->atari(controller)) {
			// ���������ꍇ �G�t�F�N�g�쐬
			m_animations.push_back(objects[i]->createAnimation());
		}
		// deleteFlag��true�Ȃ�폜����
		if (objects[i]->getDeleteFlag()) {
			delete objects[i];
			// �������폜�����������
			objects[i] = objects.back();
			objects.pop_back();
			i--;
		}
	}
}

// �L�����N�^�[�̓���
void World::controlCharacter() {
	size_t size = m_characterControllers.size();
	for (unsigned int i = 0; i < size; i++) {
		CharacterController* controller = m_characterControllers[i];
		// �s���O�̏���
		controller->init();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		atariCharacterAndObject(controller, m_stageObjects);
		atariCharacterAndObject(controller, m_attackObjects);

		// ����
		controller->control();

		// �ˌ��U��
		Object* bulletAttack = controller->bulletAttack();
		if (bulletAttack != NULL) { m_attackObjects.push_back(bulletAttack); }

		// �a���U��
		Object* slashAttack = controller->slashAttack();
		if (slashAttack != NULL) { m_attackObjects.push_back(slashAttack); }

		// ���f
		controller->action();

		// �I�u�W�F�N�g�Ƃ̊ђʔ���
		penetrationCharacterAndObject(controller, m_stageObjects);
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
	for (unsigned int i = 0; i < m_attackObjects.size(); i++) {
		for (unsigned int j = 0; j < m_stageObjects.size(); j++) {
			// �U�����Ǐ��ɓ������Ă��邩����
			m_stageObjects[j]->atariObject(m_attackObjects[i]);
			// �Ǐ���deleteFlag��true�Ȃ�폜����
			if (m_stageObjects[j]->getDeleteFlag()) {
				delete m_stageObjects[j];
				m_stageObjects[j] = m_stageObjects.back();
				m_stageObjects.pop_back();
				j--;
			}
		}
		// �U����deleteFlag��true�Ȃ�폜����
		if (m_attackObjects[i]->getDeleteFlag()) {
			delete m_attackObjects[i];
			m_attackObjects[i] = m_attackObjects.back();
			m_attackObjects.pop_back();
			i--;
		}
	}
}

// ��b������
void World::talk() {

}