#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include <Ultralight/include/Ultralight/Ultralight.h>



GameWorld::GameWorld()
{}

GameWorld::~GameWorld() 
{}

void GameWorld::Init()  
{
	myTga2dLogoTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds");
}
void GameWorld::Update(float /*aTimeDelta*/)
{ 	
}

void GameWorld::Render()
{
	Tga2D::SpriteDrawer& spriteDrawer(Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer());
	Tga2D::SpriteSharedData sharedData = {};
	sharedData.myTexture = myTga2dLogoTexture;

	// Create instance data. 
	Tga2D::Sprite2DInstanceData spriteInstance = {};
	spriteInstance.myPivot = { 0.5f, 0.5f };
	spriteInstance.myPosition = { 0.5f, 0.5f };

	spriteDrawer.Draw(sharedData, spriteInstance);
}