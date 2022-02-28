#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>
#include <tga2d/d3d/direct_3d.h>

#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
#include <Input.h>


#define DEFAULT_TEXT Tga2D::CEngine::GetInstance()->GetTextureManager().GetTexture("Sprites/Tga_logo.dds")


CGameWorld::CGameWorld()
{}

CGameWorld::~CGameWorld()
{}

void CGameWorld::Init()
{
	myLeftStick.myPosition = { 0.3f, 0.7f };
	myLeftStick.myMaterialInfo.myTexture = DEFAULT_TEXT;
	myLeftStick.myObjectInfo.mySize = { 0.2f, 0.2f };


	myRightStick.myPosition = { 0.7f, 0.7f };
	myRightStick.myMaterialInfo.myTexture = DEFAULT_TEXT;
	myRightStick.myObjectInfo.mySize = { 0.2f, 0.2f };


	mySpriteDrawer = &Tga2D::CEngine::GetInstance()->GetDirect3D().GetSpriteDrawer();

	CommonUtilities::Gamepad::AddGamepad();
}

void CGameWorld::Update(float /*aTimeDelta*/)
{

	

	auto lStick = CommonUtilities::Gamepad::GetGamepad(1).GetLeftStick();
	myLeftStick.myObjectInfo.myPosition = { (lStick.x * 0.1f + myLeftStick.myPosition.x) , (-lStick.y * 0.1f + myLeftStick.myPosition.y) };

	auto rStick = CommonUtilities::Gamepad::GetGamepad(1).GetRightStick();
	myRightStick.myObjectInfo.myPosition = { (rStick.x * 0.1f + myRightStick.myPosition.x) , (-rStick.y * 0.1f + myRightStick.myPosition.y) };

	CommonUtilities::Gamepad::Update();
}

void CGameWorld::Render()
{
	myLeftStick.Render(mySpriteDrawer);
	myRightStick.Render(mySpriteDrawer);

	//myAButton.Render(mySpriteDrawer);
	//myBButton.Render(mySpriteDrawer);
	//myYButton.Render(mySpriteDrawer);
	//myXButton.Render(mySpriteDrawer);


	//myLShoulder.Render(mySpriteDrawer);
	//myRShoulder.Render(mySpriteDrawer);

	//myLTrigger.Render(mySpriteDrawer);
	//myRTrigger.Render(mySpriteDrawer);

	//Tga2D::CSpriteDrawer& spriteDrawer(Tga2D::CEngine::GetInstance()->GetDirect3D().GetSpriteDrawer());
	//Tga2D::SSpriteSharedData sharedData = {};
	//sharedData.myTexture = myTga2dLogoTexture;

	//// Create instance data. 
	//Tga2D::SSpriteInstanceData spriteInstance = {};
	//spriteInstance.myPivot = { 0.5f, 0.5f };
	//spriteInstance.myPosition = { 0.5f, 0.5f };

	//spriteDrawer.Draw(sharedData, spriteInstance);
}

void Sprite::Render(Tga2D::CSpriteDrawer* aSpriteDrawer)
{
	aSpriteDrawer->Draw(myMaterialInfo, myObjectInfo);
}
