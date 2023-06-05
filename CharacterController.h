#ifndef CHACACTER_CONTROLLER_H_INCLUDED
#define CHACACTER_CONTROLLER_H_INCLUDED

class Character;

/*
  キャラクターを動かすクラス
*/
class CharacterController {
private:
	//動かす対象のキャラクター
	Character* m_character;

public:
	CharacterController(Character* character);
};

#endif
