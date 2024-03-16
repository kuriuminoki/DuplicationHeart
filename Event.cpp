#include "Event.h"
#include "World.h"
#include "Sound.h"
#include "CsvReader.h"
#include "CharacterController.h"
#include "CharacterAction.h"
#include "Character.h"
#include "Text.h"
#include "Brain.h"
#include "Animation.h"
#include <sstream>

using namespace std;


// csv����擾����param1�`paramN��map��vector�ɕϊ�
vector<string> mapParam2vector(map<string, string> paramMap) {
	vector<string> res;
	int paramNum = 0;
	while (true) {
		ostringstream oss;
		oss << "param" << paramNum;
		if (paramMap.find(oss.str()) != paramMap.end()) {
			res.push_back(paramMap[oss.str()]);
			paramNum++;
		}
		else {
			break;
		}
	}
	return res;
}


/*
* �C�x���g
*/
Event::Event(int eventNum, World* world, SoundPlayer* soundPlayer) {

	m_eventNum = eventNum;
	m_nowElement = 0;
	m_eventElement = nullptr;

	m_world_p = world;
	m_soundPlayer_p = soundPlayer;

	ostringstream oss;
	oss << "data/event/event" << m_eventNum << ".csv";
	CsvReader2 csvReader2(oss.str().c_str());

	// ���Ώ���
	vector<map<string, string> > fireData = csvReader2.getDomainData("FIRE:");
	for (unsigned int i = 0; i < fireData.size(); i++) {
		createFire(mapParam2vector(fireData[i]), world, soundPlayer);
	}

	m_elementsData = csvReader2.getDomainData("ELEMENT:");

}

Event::~Event() {
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		delete m_eventFire[i];
	}
	delete m_eventElement;
}

bool Event::skillAble() {
	if (m_eventElement == nullptr) { return true; }
	return m_eventElement->skillAble();
}

// World��ݒ肵�Ȃ���
void Event::setWorld(World* world) {
	m_world_p = world;
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		m_eventFire[i]->setWorld(m_world_p);
	}
	if (m_eventElement != nullptr) {
		m_eventElement->setWorld(m_world_p);
	}
}

// Fire�̍쐬
void Event::createFire(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventFire* fire = nullptr;

	if (param0 == "CharacterPoint") {
		fire = new CharacterPointFire(world, param);
	}
	else if (param0 == "CharacterNear") {
		fire = new CharacterNearFire(world, param);
	}
	else if (param0 == "Auto") {
		fire = new AutoFire(world);
	}
	else if (param0 == "Non") {
		fire = new NonFire(world);
	}

	if (fire != nullptr) { m_eventFire.push_back(fire); }
}

// Element�̍쐬
void Event::createElement(vector<string> param, World* world, SoundPlayer* soundPlayer) {
	string param0 = param[0];
	EventElement* element = nullptr;

	if (param0 == "LockArea") {
		element = new LockAreaEvent(world, param);
	}
	else if (param0 == "Invincinble") {
		element = new InvincinbleEvent(world, param);
	}
	else if (param0 == "SetGoalPoint") {
		element = new SetGoalPointEvent(world, param);
	}
	else if (param0 == "MoveGoal") {
		element = new MoveGoalEvent(world, param);
	}
	else if (param0 == "ChangeBrain") {
		element = new ChangeBrainEvent(world, param);
	}
	else if (param0 == "ChangeGroup") {
		element = new ChangeGroupEvent(world, param);
	}
	else if (param0 == "ChangeInfoVersion") {
		element = new ChangeInfoVersionEvent(world, param);
	}
	else if (param0 == "ChangeCharacterPoint") {
		element = new ChangeCharacterPointEvent(world, param);
	}
	else if (param0 == "ChangeCharacterDirection") {
		element = new ChangeCharacterDirectionEvent(world, param);
	}
	else if (param0 == "DeadCharacter") {
		element = new DeadCharacterEvent(world, param);
	}
	else if (param0 == "DeadGroup") {
		element = new DeadGroupEvent(world, param);
	}
	else if (param0 == "Talk") {
		element = new TalkEvent(world, soundPlayer, param);
	}
	else if (param0 == "Movie") {
		element = new MovieEvent(world, soundPlayer, param);
	}
	else if (param0 == "PlayerDead") {
		element = new PlayerDeadEvent(world, param);
	}
	else if (param0 == "MoveArea"){
		element = new MoveAreaEvent(world, param);
	}
	else if (param0 == "BlindWorld") {
		element = new BlindWorldEvent(world, param);
	}

	if (element != nullptr) { 
		m_eventElement = element;
		m_eventElement->init();
	}
}

bool Event::fire() {

	// ������"���ׂ�"���������甭��
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		if (!m_eventFire[i]->fire()) {
			return false;
		}
	}
	
	// ���΂��ď��߂ăC�x���g�����
	createElement(mapParam2vector(m_elementsData[m_nowElement++]), m_world_p, m_soundPlayer_p);
	return true;
}

EVENT_RESULT Event::play() {

	EVENT_RESULT elementResult = m_eventElement->play();

	// Element�����������
	if (elementResult == EVENT_RESULT::SUCCESS) {

		// Story�ɑO�̃Z�[�u�|�C���g�֖߂�悤�v��
		if (m_eventElement->needBackPrevSave()) {
			m_backPrevSaveFlag = true;
		}

		if (m_nowElement == m_elementsData.size()) { 
			// EventElement���c���Ă��Ȃ��̂ŃC�x���g�����
			return EVENT_RESULT::SUCCESS;
		}
		else { 
			// EventElement�͎c���Ă���̂ł܂��C�x���g����
			delete m_eventElement;
			createElement(mapParam2vector(m_elementsData[m_nowElement++]), m_world_p, m_soundPlayer_p);
			return EVENT_RESULT::NOW;
		}
	}

	// FAILURE�܂���NOW
	return elementResult;
}



/*
* �C�x���g�̔��Ώ���
*/
EventFire::EventFire(World* world) {
	m_world_p = world;
}

// ����̃L�������w�肵�����W�ɂ���
CharacterPointFire::CharacterPointFire(World* world, std::vector<std::string> param) :
	EventFire(world)
{
	m_param = param;
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
	m_areaNum = stoi(m_param[2]);
	m_x = stoi(m_param[3]);
	m_y = stoi(m_param[4]);
	m_dx = stoi(m_param[5]);
	m_dy = stoi(m_param[6]);
}
bool CharacterPointFire::fire() {
	// ����̃L�������w�肵���ꏊ�ɂ��邩����
	if (m_world_p->getAreaNum() != m_areaNum) { return false; }
	int x = m_character_p->getCenterX();
	int y = m_character_p->getY() + m_character_p->getHeight();
	if (x > m_x - m_dx && x < m_x + m_dx && y > m_y - m_dy && y < m_y + m_dy) {
		return true;
	}
	return false;
}
void CharacterPointFire::setWorld(World* world) {
	EventFire::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}


// ����̃L����������̃L�����̋߂��ɂ���
CharacterNearFire::CharacterNearFire(World* world, std::vector<std::string> param) :
	EventFire(world)
{
	m_param = param;
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
	m_target_p = m_world_p->getCharacterWithName(m_param[2]);
	m_dx = stoi(param[3]);
	m_dy = stoi(param[4]);

	m_areaNum = m_world_p->getAreaNum();
}
bool CharacterNearFire::fire() {
	// ����̃L�������w�肵���ꏊ�ɂ��邩����
	if (m_world_p->getAreaNum() != m_areaNum) { 
		m_areaNum = m_world_p->getAreaNum();
		m_target_p = m_world_p->getCharacterWithName(m_param[2]);
	}
	if (m_target_p == nullptr) { return false; }
	int x = m_character_p->getCenterX();
	int y = m_character_p->getY() + m_character_p->getHeight();
	int targetX = m_target_p->getCenterX();
	int targetY = m_target_p->getY() + m_target_p->getHeight();
	if (x > targetX - m_dx && x < targetX + m_dx && y > targetY - m_dy && y < targetY + m_dy) {
		return true;
	}
	return false;
}
void CharacterNearFire::setWorld(World* world) {
	EventFire::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}



/*
* �C�x���g�̍\���v�f
*/
EventElement::EventElement(World* world) {
	m_world_p = world;
}

EventElement::~EventElement() {

}

// �G���A�ړ����֎~����
LockAreaEvent::LockAreaEvent(World* world, std::vector<std::string> param):
	EventElement(world)
{
	m_lock = param[1] == "1" ? true : false;
}
EVENT_RESULT LockAreaEvent::play() {
	m_world_p->setAreaLock(m_lock);
	return EVENT_RESULT::SUCCESS;
}

// �L�����𖳓G�ɂ���
InvincinbleEvent::InvincinbleEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_invincible = param[1] == "1" ? true : false;
	m_character_p = m_world_p->getCharacterWithName(param[2]);
	m_param = param;
}
EVENT_RESULT InvincinbleEvent::play() {

	// �Ώۂ̃L�����𖳓G�ɂ���
	m_character_p->setInvincible(m_invincible);

	return EVENT_RESULT::SUCCESS;
}
void InvincinbleEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// �L������Brain��ύX����
ChangeBrainEvent::ChangeBrainEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_brainName = param[1];
	m_controller_p = m_world_p->getControllerWithName(param[2]);
	m_param = param;
}
EVENT_RESULT ChangeBrainEvent::play() {

	// �Ώۂ̃L������Brain��ύX����
	Brain* brain = createBrain(m_brainName, m_world_p->getCamera());
	m_controller_p->setBrain(brain);

	// �ǐՑΏۂ��K�v��Brain�͒ǐՑΏۂ�ݒ�
	if (brain->getBrainName() == FollowNormalAI::BRAIN_NAME ||
		brain->getBrainName() == FollowParabolaAI::BRAIN_NAME ||
		brain->getBrainName() == ValkiriaAI::BRAIN_NAME ||
		brain->getBrainName() == FollowFlightAI::BRAIN_NAME) {
		Character* follow = m_world_p->getCharacterWithName(m_param[3]);
		brain->searchFollow(follow);
	}

	return EVENT_RESULT::SUCCESS;
}
void ChangeBrainEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_controller_p = m_world_p->getControllerWithName(m_param[2]);
}

// �L�����̖ڕW�n�_��ݒ��ς���
SetGoalPointEvent::SetGoalPointEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_gx = stoi(param[1]);
	m_gy = stoi(param[2]);
	m_controller_p = m_world_p->getControllerWithName(param[3]);
	m_param = param;
}
EVENT_RESULT SetGoalPointEvent::play() {

	// �Ώۂ̃L�����̖ڕW�n�_��ݒ�
	m_controller_p->setGoal(m_gx, m_gy);

	return EVENT_RESULT::SUCCESS;
}
void SetGoalPointEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_controller_p = m_world_p->getControllerWithName(m_param[3]);
}

// �S�L�������ڕW�n�_�ֈړ�����܂őҋ@
MoveGoalEvent::MoveGoalEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	int x = 1;
}
EVENT_RESULT MoveGoalEvent::play() {
	if (m_world_p->moveGoalCharacter()) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}

// �L������GroupID��ύX����
ChangeGroupEvent::ChangeGroupEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_groupId = stoi(param[1]);
	m_character_p = m_world_p->getCharacterWithName(param[2]);
}
EVENT_RESULT ChangeGroupEvent::play() {
	// �Ώۂ̃L������GroupId��ύX����
	m_character_p->setGroupId(m_groupId);
	return EVENT_RESULT::SUCCESS;
}
void ChangeGroupEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// �L������version��ύX����
ChangeInfoVersionEvent::ChangeInfoVersionEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_version = stoi(param[1]);
	m_character_p = m_world_p->getCharacterWithName(param[2]);
}
EVENT_RESULT ChangeInfoVersionEvent::play() {
	// �Ώۂ̃L������version��ύX����
	m_character_p->changeInfoVersion(m_version);
	return EVENT_RESULT::SUCCESS;
}
void ChangeInfoVersionEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// �L�����̍��W��ς���
ChangeCharacterPointEvent::ChangeCharacterPointEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_x = stoi(param[1]);
	m_y = stoi(param[2]);
	m_character_p = m_world_p->getCharacterWithName(param[3]);
}
EVENT_RESULT ChangeCharacterPointEvent::play() {
	// �Ώۂ̃L�����̍��W��ύX����
	m_character_p->setX(m_x);
	m_character_p->setY(m_y - m_character_p->getHeight());
	return EVENT_RESULT::SUCCESS;
}
void ChangeCharacterPointEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// �L�����̌�����ς���
ChangeCharacterDirectionEvent::ChangeCharacterDirectionEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_leftDirection = false;
	m_targetCharacter_p = nullptr;
	if (m_param[1] == "left" || m_param[1] == "true") {
		// �����w��
		m_leftDirection = true;
	}
	else if (m_param[1] == "right" || m_param[1] == "false") {
		// �E���w��
		m_leftDirection = false;
	}
	else {
		// ����̃L�����̕������w��
		m_targetCharacter_p = m_world_p->getCharacterWithName(m_param[1]);
	}
	// ������ς���Ώۂ̃L����
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}
EVENT_RESULT ChangeCharacterDirectionEvent::play() {
	// �Ώۂ̃L�����̌�����ύX����
	if (m_targetCharacter_p != nullptr) {
		// ����̃L�����̕����֌���
		m_character_p->setLeftDirection(m_character_p->getCenterX() > m_targetCharacter_p->getCenterX());
	}
	else {
		// �w�肵�������֌���
		m_character_p->setLeftDirection(m_leftDirection);
	}
	return EVENT_RESULT::SUCCESS;
}
void ChangeCharacterDirectionEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_targetCharacter_p = m_world_p->getCharacterWithName(m_param[1]);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// ����̃L������HP��0�ɂȂ�܂Ő키
DeadCharacterEvent::DeadCharacterEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_character_p = m_world_p->getCharacterWithName(param[1]);
}
EVENT_RESULT DeadCharacterEvent::play() {
	m_world_p->battle();
	if (m_character_p == nullptr) {
		return EVENT_RESULT::NOW;
	}
	// �Ώۂ̃L������HP���`�F�b�N����
	if (m_character_p->getHp() == 0) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}
void DeadCharacterEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}

// ����̃O���[�v���S�ł���܂Ő키
DeadGroupEvent::DeadGroupEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_groupId = stoi(param[1]);
	m_areaNum = stoi(param[2]);
}
EVENT_RESULT DeadGroupEvent::play() {
	m_world_p->battle();
	if (m_world_p->getAreaNum() != m_areaNum || m_world_p->getBrightValue() < 255) {
		return EVENT_RESULT::NOW;
	}
	vector<const CharacterAction*> actions = m_world_p->getActions();
	for (unsigned int i = 0; i < actions.size(); i++) {
		if (actions[i]->getCharacter()->getGroupId() == m_groupId && actions[i]->getCharacter()->getHp() > 0) {
			return EVENT_RESULT::NOW;
		}
	}
	return EVENT_RESULT::SUCCESS;
}


TalkEvent::TalkEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param):
	EventElement(world)
{
	int textNum = stoi(param[1]);
	m_conversation = new Conversation(textNum, world, soundPlayer);
}

TalkEvent::~TalkEvent() {
	delete m_conversation;
}

EVENT_RESULT TalkEvent::play() {
	m_world_p->setConversation(m_conversation);
	m_world_p->talk();
	if (m_conversation->getFinishFlag()) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}

void TalkEvent::setWorld(World* world) { 
	EventElement::setWorld(world);
	m_conversation->setWorld(world);
}


MovieEvent::MovieEvent(World* world, SoundPlayer* soundPlayer, std::vector<std::string> param) :
	EventElement(world)
{
	if (param[1] == "op") {
		m_movie = new OpMovie(soundPlayer);
	}
}

MovieEvent::~MovieEvent() {
	delete m_movie;
}

void MovieEvent::init() {
	m_world_p->setMovie(m_movie);
}

EVENT_RESULT MovieEvent::play() {
	m_world_p->moviePlay();
	if (m_movie->getFinishFlag()) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}


// ����̃G���A�Ńv���C���[�������C�x���g
PlayerDeadEvent::PlayerDeadEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_areaNum = stoi(param[1]);
}
EVENT_RESULT PlayerDeadEvent::play() {
	m_world_p->battle();
	// �Ώۂ̃L������HP���`�F�b�N����
	if (m_world_p->getAreaNum() == m_areaNum && m_world_p->playerDead() && m_world_p->getBrightValue() == 0) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}


// ����̃G���A�֋����I�Ɉړ�����
MoveAreaEvent::MoveAreaEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_areaNum = stoi(param[1]);
}
EVENT_RESULT MoveAreaEvent::play() {
	m_world_p->moveArea(m_areaNum);
	return EVENT_RESULT::SUCCESS;
}


// ���E�̕`�������E���Ȃ�
BlindWorldEvent::BlindWorldEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_flag = (bool)stoi(param[1]);
}
EVENT_RESULT BlindWorldEvent::play() {
	m_world_p->setBlindFlag(m_flag);
	return EVENT_RESULT::SUCCESS;
}
