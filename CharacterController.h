#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class Character;

/*
  �L�����N�^�[�𓮂����N���X
*/
class CharacterController {
private:
	//�������Ώۂ̃L�����N�^�[
	Character* m_character;

public:
	CharacterController(Character* character);
};

#endif
