#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <queue>
#include <string>

class SoundPlayer {
private:
	// ����
	int m_volume;

	// BGM�̖��O
	std::string m_bgmName;

	// BGM
	int m_bgmHandle;

	// �Đ��\��̌��ʉ�
	std::queue<int> m_soundQueue;

public:
	SoundPlayer();
	~SoundPlayer();

	void setVolume(int volume);
	inline int getVolume() const { return m_volume; }

	// BGM���Z�b�g�i�ύX�j
	void setBGM(std::string bgmName);

	// �������Ă��鉹�y���`�F�b�N����
	inline bool sameBGM(std::string bgmName) const { return m_bgmName == bgmName; }

	// BGM���Đ�
	void playBGM();

	// BGM���X�g�b�v
	void stopBGM();

	// ���ʉ��̍Đ��ҋ@��փv�b�V��
	void pushSoundQueue(int soundHandle);

	// ���ʉ���炷
	void play();
};

#endif