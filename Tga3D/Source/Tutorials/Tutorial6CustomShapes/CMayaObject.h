#pragma once
#include <tga2d/primitives/CustomShape.h>
class CMayaObject
{
public:
	CMayaObject();
	~CMayaObject();
	void Init(const char* aObjPath);
	void Render(float aDelta);
private:
	Tga2D::CustomShape* myShape;
	float myTotalTime = 0;
};

