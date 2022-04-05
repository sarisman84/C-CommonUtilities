#pragma once
#include <tga2d/primitives/CustomShape.h>

class CSnakeShape
{
public:
	CSnakeShape();
	~CSnakeShape();
	void Init(float aRandom);
	void Update(float aTime);
	Tga2D::CustomShape myShape;
private:
	std::vector<Tga2D::Vector2f> myPoints;
	Tga2D::Vector2f myPosition;
	float myTime;
	float myTime2;
	float myRandom;
};

