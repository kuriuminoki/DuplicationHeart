#include "Sound.h"
#include "DxLib.h"
#include <algorithm>

using namespace std;


// ���ʒ���
void changeSoundVolume(int soundHandle, int volume) {
	ChangeVolumeSoundMem(255 * (volume) / 100, soundHandle);
}


SoundPlayer::SoundPlayer() {
	m_volume = 50;
	m_bgmName = "";
	m_bgmHandle = -1;

}

SoundPlayer::~SoundPlayer() {
	DeleteSoundMem(m_bgmHandle);
}

void SoundPlayer::setVolume(int volume) {
	if (volume < 0) { m_volume = 0; }
	else {
		m_volume = min(m_volume, 100);
	}
	changeSoundVolume(m_bgmHandle, m_volume);
}

// BGM���Z�b�g�i�ύX�j
void SoundPlayer::setBGM(std::string bgmName) {
	m_bgmName = bgmName;
	m_bgmHandle = LoadSoundMem(bgmName.c_str());
	// ���ʒ���
	changeSoundVolume(m_bgmHandle, m_volume);
}

// BGM���Đ�
void SoundPlayer::playBGM() {
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

// BGM���X�g�b�v
void SoundPlayer::stopBGM() {
	StopSoundMem(m_bgmHandle);
}

// ���ʉ��̍Đ��ҋ@��փv�b�V��
void SoundPlayer::pushSoundQueue(int soundHandle) {
	m_soundQueue.push(soundHandle);
}

// ���ʉ���炷
void SoundPlayer::play() {
	while (!m_soundQueue.empty()) {
		int handle = m_soundQueue.front();
		m_soundQueue.pop();
		changeSoundVolume(handle, m_volume);
		PlaySoundMem(handle, DX_PLAYTYPE_BACK);
	}
}