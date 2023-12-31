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


// csvから取得したparam1〜paramNのmapをvectorに変換
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
* イベント
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

	// 発火条件
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

// Worldを設定しなおす
void Event::setWorld(World* world) {
	m_world_p = world;
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		m_eventFire[i]->setWorld(m_world_p);
	}
	if (m_eventElement != nullptr) {
		m_eventElement->setWorld(m_world_p);
	}
}

// Fireの作成
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

// Elementの作成
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
	else if (param0 == "ChangeInfoVersionEvent") {
		element = new ChangeInfoVersionEvent(world, param);
	}
	else if (param0 == "ChangeCharacterPointEvent") {
		element = new ChangeCharacterPointEvent(world, param);
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
	else if (param0 == "PlayerDeadEvent") {
		element = new PlayerDeadEvent(world, param);
	}

	if (element != nullptr) { 
		m_eventElement = element;
		m_eventElement->init();
	}
}

bool Event::fire() {

	// 条件を"すべて"満たしたら発火
	for (unsigned int i = 0; i < m_eventFire.size(); i++) {
		if (!m_eventFire[i]->fire()) {
			return false;
		}
	}
	
	// 発火して初めてイベントを作る
	createElement(mapParam2vector(m_elementsData[m_nowElement++]), m_world_p, m_soundPlayer_p);
	return true;
}

EVENT_RESULT Event::play() {

	EVENT_RESULT elementResult = m_eventElement->play();

	// Elementが一つ成功した
	if (elementResult == EVENT_RESULT::SUCCESS) {

		// Storyに前のセーブポイントへ戻るよう要求
		if (m_eventElement->needBackPrevSave()) {
			m_backPrevSaveFlag = true;
		}

		if (m_nowElement == m_elementsData.size()) { 
			// EventElementが残っていないのでイベントおわり
			return EVENT_RESULT::SUCCESS;
		}
		else { 
			// EventElementは残っているのでまだイベント続く
			delete m_eventElement;
			createElement(mapParam2vector(m_elementsData[m_nowElement++]), m_world_p, m_soundPlayer_p);
			return EVENT_RESULT::NOW;
		}
	}

	// FAILUREまたはNOW
	return elementResult;
}



/*
* イベントの発火条件
*/
EventFire::EventFire(World* world) {
	m_world_p = world;
}

// 特定のキャラが指定した座標にいる
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
	// 特定のキャラが指定した場所にいるか判定
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


// 特定のキャラが特定のキャラの近くにいる
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
	// 特定のキャラが指定した場所にいるか判定
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
* イベントの構成要素
*/
EventElement::EventElement(World* world) {
	m_world_p = world;
}

EventElement::~EventElement() {

}

// エリア移動を禁止する
LockAreaEvent::LockAreaEvent(World* world, std::vector<std::string> param):
	EventElement(world)
{
	m_lock = param[1] == "1" ? true : false;
}
EVENT_RESULT LockAreaEvent::play() {
	m_world_p->setAreaLock(m_lock);
	return EVENT_RESULT::SUCCESS;
}

// キャラを無敵にする
InvincinbleEvent::InvincinbleEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_invincible = param[1] == "1" ? true : false;
	m_character_p = m_world_p->getCharacterWithName(param[2]);
	m_param = param;
}
EVENT_RESULT InvincinbleEvent::play() {

	// 対象のキャラを無敵にする
	m_character_p->setInvincible(m_invincible);

	return EVENT_RESULT::SUCCESS;
}
void InvincinbleEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// キャラのBrainを変更する
ChangeBrainEvent::ChangeBrainEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_brainName = param[1];
	m_controller_p = m_world_p->getControllerWithName(param[2]);
	m_param = param;
}
EVENT_RESULT ChangeBrainEvent::play() {

	// 対象のキャラのBrainを変更する
	Brain* brain = createBrain(m_brainName, m_world_p->getCamera());
	m_controller_p->setBrain(brain);

	// 追跡対象が必要なBrainは追跡対象を設定
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

// キャラの目標地点を設定を変える
SetGoalPointEvent::SetGoalPointEvent(World* world, vector<string> param) :
	EventElement(world)
{
	m_gx = stoi(param[1]);
	m_gy = stoi(param[2]);
	m_controller_p = m_world_p->getControllerWithName(param[3]);
	m_param = param;
}
EVENT_RESULT SetGoalPointEvent::play() {

	// 対象のキャラの目標地点を設定
	m_controller_p->setGoal(m_gx, m_gy);

	return EVENT_RESULT::SUCCESS;
}
void SetGoalPointEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_controller_p = m_world_p->getControllerWithName(m_param[3]);
}

// 全キャラが目標地点へ移動するまで待機
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

// キャラのGroupIDを変更する
ChangeGroupEvent::ChangeGroupEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_groupId = stoi(param[1]);
	m_character_p = m_world_p->getCharacterWithName(param[2]);
}
EVENT_RESULT ChangeGroupEvent::play() {
	// 対象のキャラのGroupIdを変更する
	m_character_p->setGroupId(m_groupId);
	return EVENT_RESULT::SUCCESS;
}
void ChangeGroupEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// キャラのversionを変更する
ChangeInfoVersionEvent::ChangeInfoVersionEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_version = stoi(param[1]);
	m_character_p = m_world_p->getCharacterWithName(param[2]);
}
EVENT_RESULT ChangeInfoVersionEvent::play() {
	// 対象のキャラのGroupIdを変更する
	m_character_p->changeInfoVersion(m_version);
	return EVENT_RESULT::SUCCESS;
}
void ChangeInfoVersionEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// キャラの座標を変える
ChangeCharacterPointEvent::ChangeCharacterPointEvent(World* world, std::vector<string> param) :
	EventElement(world)
{
	m_param = param;
	m_x = stoi(param[1]);
	m_y = stoi(param[2]);
	m_character_p = m_world_p->getCharacterWithName(param[3]);
}
EVENT_RESULT ChangeCharacterPointEvent::play() {
	// 対象のキャラの座標を変更する
	m_character_p->setX(m_x);
	m_character_p->setY(m_y - m_character_p->getHeight());
	return EVENT_RESULT::SUCCESS;
}
void ChangeCharacterPointEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[2]);
}

// 特定のキャラのHPが0になるまで戦う
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
	// 対象のキャラのHPをチェックする
	if (m_character_p->getHp() == 0) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}
void DeadCharacterEvent::setWorld(World* world) {
	EventElement::setWorld(world);
	m_character_p = m_world_p->getCharacterWithName(m_param[1]);
}

// 特定のグループが全滅するまで戦う
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
	//int textNum = stoi(param[1]);
	m_movie = new OpMovie(soundPlayer);
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


// 特定のエリアでプレイヤーがやられるイベント
PlayerDeadEvent::PlayerDeadEvent(World* world, std::vector<std::string> param) :
	EventElement(world)
{
	m_areaNum = stoi(param[1]);
}
EVENT_RESULT PlayerDeadEvent::play() {
	m_world_p->battle();
	// 対象のキャラのHPをチェックする
	if (m_world_p->getAreaNum() == m_areaNum && m_world_p->playerDead() && m_world_p->getBrightValue() == 0) {
		return EVENT_RESULT::SUCCESS;
	}
	return EVENT_RESULT::NOW;
}
