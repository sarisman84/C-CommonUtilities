#pragma once
#include <tga2d/primitives/custom_shape.h>

class CSnakeShape
{
public:
	CSnakeShape();
	~CSnakeShape();
	void Init(float aRandom);
	void Update(float aTime);
	Tga2D::CCustomShape myShape;
private:
	std::vector<VECTOR2F> myPoints;
	VECTOR2F myPosition;
	float myTime;
	float myTime2;
	float myRandom;
};

