#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>
#include <tga2d/d3d/direct_3d.h>

#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
#include <tga2d/text/text.h>

#include <Input.h>
#include <SolveSudoku.hpp>


#define DEFAULT_TEXT Tga2D::CEngine::GetInstance()->GetTextureManager().GetTexture("Sprites/Tga_logo.dds")

using N = Number;

#define EMPTY N(0)

CGameWorld::CGameWorld()
{}

CGameWorld::~CGameWorld()
{}

void CGameWorld::Init()
{
	mySudokuBoard = {
		N(5), N(3), EMPTY,    EMPTY, N(7),EMPTY,    EMPTY, EMPTY, EMPTY,
		N(6), EMPTY, EMPTY,   N(1), N(9), N(5),	    EMPTY, EMPTY, EMPTY,
		EMPTY, N(9), N(8),    EMPTY, EMPTY, EMPTY,  EMPTY, N(6),  EMPTY,

		N(8), EMPTY, EMPTY,   EMPTY, N(6), EMPTY,   EMPTY, EMPTY, N(3),
		N(4), EMPTY, EMPTY,   N(8), EMPTY, N(3),    EMPTY, EMPTY, N(1),
		N(7), EMPTY, EMPTY,   EMPTY, N(2), EMPTY,   EMPTY, EMPTY, N(6),

		EMPTY, N(6), EMPTY,   EMPTY, EMPTY, EMPTY , N(2), N(8), EMPTY,
		EMPTY, EMPTY, EMPTY,  N(4), N(1), N(9),     EMPTY, EMPTY, N(5),
		EMPTY, EMPTY, EMPTY,  EMPTY, N(8), EMPTY,   EMPTY, N(7), N(9),
	};


	CommonUtilities::Board board =
	{
		5, 3, 0,    0, 7,0,   0, 0, 0,
		6, 0, 0,    1, 9, 5,  0, 0, 0,
		0, 9, 8,    0, 0, 0,  0, 6, 0,

		8, 0, 0,   0, 6, 0,   0, 0, 3,
		4, 0, 0,   8, 0, 3,   0, 0, 1,
		7, 0, 0,   0, 2, 0,   0, 0, 6,

		0, 6, 0,   0, 0, 0 ,  2, 8, 0,
		0, 0, 0,   4, 1, 9,   0, 0, 5,
		0, 0, 0,   0, 8, 0,   0, 7, 9,

	};



	
		std::cout << (CommonUtilities::SolveSudoku(board) ? "Sudoku board solved!" : "Sudoku board not solved! Sadge") << std::endl;
	



	/*
	myLeftStick.myPosition = { 0.3f, 0.7f };
	myLeftStick.myMaterialInfo.myTexture = DEFAULT_TEXT;
	myLeftStick.myObjectInfo.mySize = { 0.2f, 0.2f };


	myRightStick.myPosition = { 0.7f, 0.7f };
	myRightStick.myMaterialInfo.myTexture = DEFAULT_TEXT;
	myRightStick.myObjectInfo.mySize = { 0.2f, 0.2f };


	mySpriteDrawer = &Tga2D::CEngine::GetInstance()->GetDirect3D().GetSpriteDrawer();

	CommonUtilities::Gamepad::AddGamepad();*/
}

void CGameWorld::Update(float /*aTimeDelta*/)
{
	/*CommonUtilities::Mouse::GetButtonUp(CommonUtilities::Mouse::Key::LeftMouseButton);


	auto lStick = CommonUtilities::Gamepad::GetGamepad(1).GetLeftStick();
	myLeftStick.myObjectInfo.myPosition = { (lStick.x * 0.1f + myLeftStick.myPosition.x) , (-lStick.y * 0.1f + myLeftStick.myPosition.y) };

	auto rStick = CommonUtilities::Gamepad::GetGamepad(1).GetRightStick();
	myRightStick.myObjectInfo.myPosition = { (rStick.x * 0.1f + myRightStick.myPosition.x) , (-rStick.y * 0.1f + myRightStick.myPosition.y) };

	CommonUtilities::Gamepad::Update();*/
}

void CGameWorld::Render()
{

	for (size_t i = 0; i < mySudokuBoard.size(); i++)
	{
		mySudokuBoard[i].Draw();
	}
	/*myLeftStick.Render(mySpriteDrawer);
	myRightStick.Render(mySpriteDrawer);*/

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

void Number::Draw()
{
	if (!myLabel) return;
	myLabel->SetText(std::to_string(myNumber));
	myLabel->Render();
}
