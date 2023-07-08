#include "ObjectDrawer.h"
#include "Camera.h"
#include "Object.h"
#include "GraphHandle.h"


ObjectDrawer::ObjectDrawer(const Object* object) {
	m_object = object;
}

void ObjectDrawer::drawObject(const Camera* const camera) {
	// ���W�Ɗg�嗦�擾
	int x1, y1, x2, y2;
	double ex = 1.0;
	x1 = m_object->getX1();
	y1 = m_object->getY1();
	x2 = m_object->getX2();
	y2 = m_object->getY2();

	// �摜���Ȃ��Ȃ�}�`�`��A����Ȃ畁�ʂɉ摜�`��
	GraphHandle* graphHandle = m_object->getHandle();
	if (graphHandle == nullptr) {
		// �J�����Œ���
		camera->setCamera(&x1, &y1, &ex);
		camera->setCamera(&x2, &y2, &ex);
		// �`��
		m_object->drawObject(x1, y1, x2, y2);
	}
	else {
		// �摜�̒��S�����W�Ƃ���
		x1 = (x1 + x2) / 2;
		y1 = (y1 + y2) / 2;
		// �摜�ŗL�̊g�嗦�擾
		ex = graphHandle->getEx();
		// �J�����Œ���
		camera->setCamera(&x1, &y1, &ex);
		// �`��
		graphHandle->draw(x1, y1, ex);
	}	
}