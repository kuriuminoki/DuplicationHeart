#ifndef OBJECT_DRAWER_H_INCLUDED
#define OBJECT_DRAWER_H_INCLUDED

class Object;
class Camera;

class ObjectDrawer {
private:

	// 描画対象
	const Object* m_object;

	// 1920を基準としたGAME_WIDEの倍率
	double m_exX;
	// 1080を基準としたGAME_HEIGHTの倍率
	double m_exY;

	// フォント
	int m_font;
	int m_fontSize;

public:
	ObjectDrawer(const Object* object);

	~ObjectDrawer();

	inline void setObject(const Object* object) { m_object = object; }

	void drawObject(const Camera* const camera);
};

#endif