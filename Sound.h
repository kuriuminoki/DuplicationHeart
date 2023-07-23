#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <queue>
#include <string>

class SoundPlayer {
private:
	// 音量
	int m_volume;

	// BGMの名前
	std::string m_bgmName;

	// BGM
	int m_bgmHandle;

	// 再生予定の効果音
	std::queue<int> m_soundQueue;

public:
	SoundPlayer();
	~SoundPlayer();

	void setVolume(int volume);
	inline int getVolume() const { return m_volume; }

	// BGMをセット（変更）
	void setBGM(std::string bgmName);

	// 今流している音楽をチェックする
	inline bool sameBGM(std::string bgmName) const { return m_bgmName == bgmName; }

	// BGMを再生
	void playBGM();

	// BGMをストップ
	void stopBGM();

	// 効果音の再生待機列へプッシュ
	void pushSoundQueue(int soundHandle);

	// 効果音を鳴らす
	void play();
};

#endif