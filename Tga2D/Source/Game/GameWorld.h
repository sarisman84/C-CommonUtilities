#pragma once
#include <Vector2.h>
#include <tga2d/sprite/sprite.h>
#include <array>
#include <tga2d/text/text.h>

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

struct Number
{
	Number() = default;
	Number(const int aNumber) {
		myNumber = aNumber;
		myLabel = new Tga2D::CText();
	}
	int myNumber;
	Tga2D::CText* myLabel;

	void Draw();

	void operator =(const int aNumber) {
		myNumber = aNumber;
	}
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
	std::array<Number, 81> mySudokuBoard;

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