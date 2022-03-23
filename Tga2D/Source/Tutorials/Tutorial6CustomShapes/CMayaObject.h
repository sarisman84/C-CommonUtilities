#pragma once
#include <tga2d/primitives/custom_shape.h>
class CMayaObject
{
public:
	CMayaObject();
	~CMayaObject();
	void Init(const char* aObjPath);
	void Render(float aDelta);
private:
	Tga2D::CCustomShape* myShape;
	float myTotalTime = 0;
};

