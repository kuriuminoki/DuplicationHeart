#include "Character.h"
#include "Object.h"
#include "CsvReader.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"
#include <cstdlib>


using namespace std;


CharacterInfo::CharacterInfo() :
	CharacterInfo("test")
{

}
// CSV�t�@�C������ǂݍ���
CharacterInfo::CharacterInfo(const char* characterName) {
	CsvReader reader("data/characterInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);

	// �p�����[�^��ݒ�
	m_name = data["name"];
	m_maxHp = stoi(data["maxHp"]);
	m_handleEx = stod(data["handleEx"]);
	m_moveSpeed = stoi(data["moveSpeed"]);
	m_jumpHeight = stoi(data["jumpHeight"]);
}


AttackInfo::AttackInfo():
	AttackInfo("test")
{

}
// CSV�t�@�C������ǂݍ���
AttackInfo::AttackInfo(const char* characterName) {
	CsvReader reader("data/attackInfo.csv");

	// �L�������Ńf�[�^������
	map<string, string> data = reader.findOne("name", characterName);

	// �p�����[�^��ݒ�
	m_bulletDamage = stoi(data["bulletDamage"]);
	m_bulletRx = stoi(data["bulletRx"]);
	m_bulletRy = stoi(data["bulletRy"]);
	m_bulletSpeed = stoi(data["bulletSpeed"]);
	m_bulletRapid = stoi(data["bulletRapid"]);
	m_bulletDistance = stoi(data["bulletDistance"]);
	m_bulletImpactX = stoi(data["bulletImpactX"]);
	m_bulletImpactY = stoi(data["bulletImpactY"]);
	m_slashDamage = stoi(data["slashDamage"]);
	m_slashLenX = stoi(data["slashLenX"]);
	m_slashLenY = stoi(data["slashLenY"]);
	m_slashCountSum = stoi(data["slashCountSum"]);
	m_slashImpactX = stoi(data["slashImpactX"]);
	m_slashImpactY = stoi(data["slashImpactY"]);
}


int Character::characterId;

Character::Character() :
	Character(100, 100, 0, 0)
{

}

Character::Character(int maxHp, int hp, int x, int y) {
	// ID��U��
	characterId++;
	m_id = characterId;

	m_maxHp = maxHp;
	m_hp = hp;
	m_x = x;
	m_y = y;

	m_wide = 0;
	m_height = 0;
	m_graphHandle = NULL;

	m_characterInfo = NULL;
	m_attackInfo = NULL;

	m_leftDirection = true;
}

Character::~Character() {
	// CharacterInfo�̍폜
	if (m_characterInfo != NULL) {
		delete m_characterInfo;
	}
	// AttackInfo�̍폜
	if (m_attackInfo != NULL) {
		delete m_attackInfo;
	}
}

void Character::setHandle(GraphHandle* handle) {

	m_graphHandle = handle;

	// �摜�̏c���Ɖ������擾����B
	GetGraphSize(m_graphHandle->getHandle(), &m_wide, &m_height);

	// �摜�̊g�嗦���l�����ăT�C�Y������
	m_wide = (int)(m_wide * m_characterInfo->handleEx());
	m_height = (int)(m_height * m_characterInfo->handleEx());
}

// �ړ�����i���W�𓮂����j
void Character::moveRight(int d) {
	m_x += d;
}
void Character::moveLeft(int d) {
	m_x -= d;
}
void Character::moveUp(int d) {
	m_y -= d;
}
void Character::moveDown(int d) {
	m_y += d;
}


/*
* �n�[�g
*/
Heart::Heart(int maxHp, int hp, int x, int y) :
	Character(maxHp, hp, x, y)
{
	// �L�����ŗL�̏��ݒ�
	m_characterInfo = new CharacterInfo(NAME);
	m_attackInfo = new AttackInfo(NAME);

	// �e�摜�̃��[�h
	double ex = m_characterInfo->handleEx();
	m_standHandle = new GraphHandle("picture/stick/stand.png", ex);

	m_slashHandles = new GraphHandles("picture/stick/zangeki", 3, ex);
}

// �f�X�g���N�^
Heart::~Heart() {
	// �摜���폜
	delete m_standHandle;
	delete m_slashHandles;
}

// �ˌ��U��������(�L�������ƂɈႤ)
Object* Heart::bulletAttack(int gx, int gy) {
	BulletObject* attackObject = new BulletObject(m_x, m_y, WHITE, gx, gy, m_attackInfo);
	attackObject->setCharacterId(m_id);
	return attackObject;
}

// �a���U��������(�L�������ƂɈႤ)
Object* Heart::slashAttack(bool leftDirection, int cnt) {
	// �U���͈͂�����
	int x2 = m_x;
	int y2 = m_y + m_attackInfo->slashLenY();
	if (leftDirection) { // �������ɍU��
		x2 -= m_attackInfo->slashLenX();
	}
	else { // �E�����ɍU��
		x2 += m_attackInfo->slashLenX();
	}

	// �U���̉摜�Ǝ�������(cnt���l�����Č���)
	int index = 0;
	int slashCountSum = 5;
	SlashObject* attackObject = NULL;
	// cnt���U���̃^�C�~���O�Ȃ�I�u�W�F�N�g����
	if (cnt == m_attackInfo->slashCountSum()) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() * 2 / 3) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	else if (cnt == m_attackInfo->slashCountSum() / 3) {
		attackObject = new SlashObject(m_x, m_y, x2, y2,
			m_slashHandles->getGraphHandle(index), slashCountSum, m_attackInfo);
	}
	return attackObject;
}
