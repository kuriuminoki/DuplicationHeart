#ifndef OBJECT_DRAWER_H_INCLUDED
#define OBJECT_DRAWER_H_INCLUDED

class Object;
class Camera;

class ObjectDrawer {
private:
	const Object* m_object;

public:
	ObjectDrawer(const Object* object);

	inline void setObject(const Object* object) { m_object = object; }

	void drawObject(const Camera* const camera);
};

#endif