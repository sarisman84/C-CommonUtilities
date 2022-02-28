#pragma once
#include <Vector2.h>
#include <tga2d/sprite/sprite.h>

namespace Tga2D
{
	class CSpriteDrawer;
}


struct Sprite
{
	CommonUtilities::Vector2<float> myPosition;
	Tga2D::SSpriteInstanceData myObjectInfo;
	Tga2D::SSpriteSharedData myMaterialInfo;

	void Render(Tga2D::CSpriteDrawer* aSpriteDrawer);

};



class CGameWorld
{
public:
	CGameWorld(); 
	~CGameWorld();

	void Init();
	void Update(float aTimeDelta); 
	void Render();
private:
	Tga2D::CSpriteDrawer* mySpriteDrawer;


	Sprite myLeftStick;
	Sprite myRightStick;

	Sprite myAButton;
	Sprite myBButton;
	Sprite myXButton;
	Sprite myYButton;


	Sprite myLShoulder;
	Sprite myRShoulder;

	Sprite myLTrigger;
	Sprite myRTrigger;

};