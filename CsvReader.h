#ifndef CSV_READER_H_INCLUDED
#define CSV_READER_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <sstream>

class CsvReader {
private:
	/*
	* データ
	* m_data[行番号]<カラム名, データ>
	*/
	std::vector<std::map<std::string, std::string> > m_data;

	/*
	* カラム名のリスト
	*/
	std::vector<std::string> m_columnNames;

public:
	// ファイル名を指定してCSVファイルを読み込む
	CsvReader(const char* fileName);

	/*
	* カラム名がvalueのデータを取得
	* 例：findOne("Name", "キャラ名");
	*/ 
	std::map<std::string, std::string> findOne(const char* columnName, const char* value);

	/*
	* 全データを返す
	*/
	std::vector<std::map<std::string, std::string> > getData() const;
};


class Character;
class CharacterController;
class Object;
class DoorObject;
class SoundPlayer;
class Camera;

class AreaReader {
private:
	// アクション作成する用
	SoundPlayer* m_soundPlayer_p;

	// カメラはこのクラスで作成する
	Camera* m_camera_p;

	// 今カメラで見ているキャラ
	int m_focusId;

	// プレイヤーが操作するキャラ
	int m_playerId;

	// BGM
	std::string m_bgmName;
	int m_bgmVolume;

	// キャラクター
	std::vector<Character*> m_characters;

	// コントローラ
	std::vector<CharacterController*> m_characterControllers;

	// オブジェクト
	std::vector<Object*> m_objects;

	// 扉オブジェクト
	std::vector<DoorObject*> m_doorObjects;

	// 背景画像と色
	int m_backGroundGraph, m_backGroundColor;

	enum class LOAD_AREA {
		BGM,
		CHARACTER,
		OBJECT,
		BACKGROUND
	};

public:
	AreaReader(int m_areaNum, SoundPlayer* soundPlayer);

	inline Camera* getCamera() const { return m_camera_p; }

	inline int getFocusId() const { return m_focusId; }

	inline int getPlayerId() const { return m_playerId; }

	inline std::string getBgm() const { return m_bgmName; }

	inline int getBgmVolume() const { return m_bgmVolume; }

	inline std::vector<Character*> getCharacters() const { return m_characters; }

	inline std::vector<CharacterController*> getCharacterControllers() const { return m_characterControllers; }

	inline std::vector<Object*> getObjects() const { return m_objects; }

	inline std::vector<DoorObject*> getDoorObjects() const { return m_doorObjects; }

	inline void getBackGround(int& graphHandle, int& colorHandle) const {
		graphHandle = m_backGroundGraph;
		colorHandle = m_backGroundColor;
	}

private:
	void map2instance(std::map<std::string, std::string> dataMap, LOAD_AREA now);
	void loadBGM(std::map<std::string, std::string> dataMap);
	void loadCharacter(std::map<std::string, std::string> dataMap);
	void loadObject(std::map<std::string, std::string> dataMap);
	void loadBackGround(std::map<std::string, std::string> dataMap);
};

#endif