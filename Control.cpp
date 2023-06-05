#include "DxLib.h"
#include "Control.h"


//////////�L�[�{�[�h�̓��͏�Ԃ��i�[����////////////////

int Key[256]; // �L�[��������Ă���t���[�������i�[����

			  // �L�[�̓��͏�Ԃ��X�V����
int updateKey() {
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}

void mouseLimit(int& hand_x, const int hand_y) {
	if (hand_x < 0 && hand_y > 0 && hand_y < 480) {
		hand_x = 0;
		SetMousePoint(0, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
	else if (hand_x > 640 && hand_y > 0 && hand_y < 480) {
		hand_x = 640;
		SetMousePoint(640, hand_y);//�}�E�X�J�[�\���̈ʒu
	}
}

///////////�}�E�X�̓��͏�Ԃ�Ԃ�//////////////
static int left_cnt = 0;
static int right_cnt = 0;
void mouseClick() {
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) { left_cnt++; }
	else { left_cnt = 0; }
	if (GetMouseInput() == MOUSE_INPUT_RIGHT) { right_cnt++; }
	else { right_cnt = 0; }
}
int leftClick() {
	return left_cnt;
}
int rightClick() {
	return right_cnt;
}


//���E�̈ړ�
int controlStick()
{
	//�������͂ɂ͒Z���ԉ����Ă�������̗p
	if (Key[KEY_INPUT_D] >= 1 && Key[KEY_INPUT_A] >= 1) {
		if (Key[KEY_INPUT_D] < Key[KEY_INPUT_A]) {
			return RIGHT_KEY;
		}
		else {
			return LEFT_KEY;
		}
	}
	else if (Key[KEY_INPUT_D] >= 1) { // �E�L�[��������Ă�����
		return RIGHT_KEY;
	}
	else if (Key[KEY_INPUT_A] >= 1) { // ���L�[��������Ă�����
		return LEFT_KEY;
	}
	return FALSE;
}

//�f�o�b�O���[�h�N���p
int controlDebug() {
	if (Key[KEY_INPUT_F] == 1) { // F�L�[��������Ă�����
		return TRUE;
	}
	return FALSE;
}

//space�L�[
int controlSpace()
{
	// space�L�[�������ꂽ�u��
	if (Key[KEY_INPUT_SPACE] == 1) {
		return TRUE;
	}
	return FALSE;
}

//�Q�[���I���p
int controlEsc() {
	if (Key[KEY_INPUT_ESCAPE] == 1) { //ESC�L�[��1�J�E���g������Ă�����
		return TRUE;
	}
	return FALSE;
}