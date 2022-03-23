#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>


GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	myMap = {
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP
	};
}
void GameWorld::Update(float /*aTimeDelta*/)
{

	
}

#define INDEX x * MapWidth + y

void GameWorld::Render()
{
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{

			Tga2D::Sprite2DInstanceData ins;
			ins.mySize = { 1.0f / (float)MapWidth, 1.0f / (float)MapHeight };
			ins.myPosition = { (float)x,(float)y };


			bool isElementSelected = Tga2D::Vector2i{ x,y } == myFirstElement || Tga2D::Vector2i{ x, y } == mySecondElement;
			bool isPath = std::find(myPath.begin(), myPath.end(), 1) != myPath.end();
			ins.myColor = myMap[INDEX] == _P ? WHITE : myMap[INDEX] == _B ? RED : isElementSelected ? BLUE : isPath ? GREEN : ins.myColor;

		}
	}
}