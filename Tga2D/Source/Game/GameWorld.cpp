#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/error_manager.h>
#include <tga2d/d3d/direct_3d.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/sprite_drawer.h>
#include <tga2d/texture/texture_manager.h>
#include <Input.h>


CGameWorld::CGameWorld()
{}

CGameWorld::~CGameWorld()
{}

void CGameWorld::Init()
{
	//myTga2dLogoTexture = Tga2D::CEngine::GetInstance()->GetTextureManager().GetTexture("sprites/tga_logo.dds");
}

void CGameWorld::Update(float /*aTimeDelta*/)
{
	if (CommonUtilities::Mouse::GetButtonDown(CommonUtilities::MouseKey::RightMouseButton))
		std::cout << "Left Clicked" << std::endl;
}

void CGameWorld::Render()
{
	//Tga2D::CSpriteDrawer& spriteDrawer(Tga2D::CEngine::GetInstance()->GetDirect3D().GetSpriteDrawer());
	//Tga2D::SSpriteSharedData sharedData = {};
	//sharedData.myTexture = myTga2dLogoTexture;

	//// Create instance data. 
	//Tga2D::SSpriteInstanceData spriteInstance = {};
	//spriteInstance.myPivot = { 0.5f, 0.5f };
	//spriteInstance.myPosition = { 0.5f, 0.5f };

	//spriteDrawer.Draw(sharedData, spriteInstance);
}