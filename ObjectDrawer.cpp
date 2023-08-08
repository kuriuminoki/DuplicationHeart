#include "ObjectDrawer.h"
#include "Camera.h"
#include "Object.h"
#include "GraphHandle.h"
#include "Define.h"
#include "DxLib.h"


ObjectDrawer::ObjectDrawer(const Object* object) {
	m_object = object;
}

void ObjectDrawer::drawObject(const Camera* const camera) {
	// À•W‚ÆŠg‘å—¦Žæ“¾
	int x1, y1, x2, y2;
	double ex = 1.0;
	x1 = m_object->getX1();
	y1 = m_object->getY1();
	x2 = m_object->getX2();
	y2 = m_object->getY2();

	// ‰æ‘œ‚ª‚È‚¢‚È‚ç}Œ`•`‰æA‚ ‚é‚È‚ç•’Ê‚É‰æ‘œ•`‰æ
	GraphHandle* graphHandle = m_object->getHandle();
	if (graphHandle == nullptr) {
		// ƒJƒƒ‰‚Å’²®
		camera->setCamera(&x1, &y1, &ex);
		camera->setCamera(&x2, &y2, &ex);
		// •`‰æ
		m_object->drawObject(x1, y1, x2, y2);
	}
	else {
		camera->setCamera(&x1, &y1, &ex);
		camera->setCamera(&x2, &y2, &ex);
		// •`‰æ
		if (m_object->extendGraph()) {
			graphHandle->extendDraw(x1, y1, x2, y2);
		}
		else {
			graphHandle->draw((x1 + x2) / 2, (y1 + y2) / 2, ex * graphHandle->getEx());
		}
	}
	if (m_object->getText() != "") {
		DrawBox(x1, y1 - 50, x2, y1 - 10, WHITE, TRUE);
		DrawFormatString(x1, y1 - 40, BLACK, "%s", m_object->getText().c_str());
	}
}