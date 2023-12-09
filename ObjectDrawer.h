#ifndef OBJECT_DRAWER_H_INCLUDED
#define OBJECT_DRAWER_H_INCLUDED

class Object;
class Camera;

class ObjectDrawer {
private:

	// �`��Ώ�
	const Object* m_object;

	// 1920����Ƃ���GAME_WIDE�̔{��
	double m_exX;
	// 1080����Ƃ���GAME_HEIGHT�̔{��
	double m_exY;

	// �t�H���g
	int m_font;
	int m_fontSize;

public:
	ObjectDrawer(const Object* object);

	~ObjectDrawer();

	inline void setObject(const Object* object) { m_object = object; }

	void drawObject(const Camera* const camera);
};

#endif