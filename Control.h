#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED
//////�L�[�{�[�h����̓��͂��i�[���A�Ԃ��֐��Q///////////

/////////////���͏��/////////////////////

//�X�e�B�b�N�̓��͏��
enum {
	RIGHT_KEY,
	LEFT_KEY,
	UP_KEY,
	DOWN_KEY
};
////////////////////////////////////////////

int updateKey();

void mouseLimit(int& hand_x, const int hand_y);
//���N���b�N�̏��
void mouseClick();
int leftClick();
int rightClick();

//�X�e�B�b�N�̓��͏��
int controlStick();

//FPS�\���̃I���I�t
int controlDebug();

//�X�y�[�X�L�[
int controlSpace();

//ESC�L�[�F�Q�[���I��
int controlEsc();

#endif