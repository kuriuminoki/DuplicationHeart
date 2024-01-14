#include "Define.h"

int GAME_WIDE = 1920;
int GAME_HEIGHT = 1080;


// ‰ð‘œ“x‚Ì”{—¦
void getGameEx(double& exX, double& exY) {
	exX = (double)GAME_WIDE / GAME_WIDE_DEFAULT;
	exY = (double)GAME_HEIGHT / GAME_HEIGHT_DEFAULT;
}