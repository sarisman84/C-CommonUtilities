#include "stdafx.h"
#include "GameWorld.h"
#include <tga2d/engine.h>
#include <tga2d/error/ErrorManager.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/texture/TextureManager.h>
#include <customutilities/Vector2.h>
#include <customutilities/Input.h>


GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::Init()
{
	myFirstElement = -1;
	mySecondElement = -1;
	myMap = {
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TB ,_TB ,_TB ,_TB ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TB ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,
		_TP, _TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP ,_TP
	};

	myTileMat = {};
	myTileMat.myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/59518.png");


	for (int x = MapWidth - 1; x >= 0; x--)
	{
		for (int y = MapHeight - 1; y >= 0; y--)
		{

			SpriteTile tile;
			tile.mySize = { 1.0f / (float)MapWidth, 1.0f / (float)MapHeight };
			tile.myPos = Tga2D::Vector2f{ 0.25f, 0.05f } + Tga2D::Vector2f{ (0.02f + tile.mySize.x / 8) * (float)x ,(0.02f + tile.mySize.y / 2) * (float)y };



			myPhysMap.push_back(tile);


		}
	}

}


Tga2D::Vector2f ToLinear(CommonUtilities::Vector2<int> aValue)
{
	auto currentRez = Tga2D::Engine::GetInstance()->GetWindowSize();


	return { (float)aValue.x / (float)currentRez.x, (float)aValue.y / (float)currentRez.y };
}



void GameWorld::Update(float /*aTimeDelta*/)
{
	for (int i = 0; i < myPhysMap.size(); i++)
	{


		SpriteTile tile = myPhysMap[i];

		Tga2D::Vector2f pos = ToLinear(Mouse::GetMousePosition());

		//AABB
		auto halfExtendsX = tile.mySize.x / 2;
		auto halfExtendsY = tile.mySize.y / 2;

		auto tilePos = tile.myPos;


		if (pos.x < tilePos.x + halfExtendsX && pos.x > tilePos.x - halfExtendsX &&
			pos.y < tilePos.y + halfExtendsY && pos.y > tilePos.y - halfExtendsY &&
			Mouse::GetButtonDown(Mouse::Key::LeftMouseButton))
		{
			if (myFirstElement == -1)
				myFirstElement = i;
			else if (mySecondElement == -1)
			{
				mySecondElement = i;
				myPath = Dijkstra(myMap, myFirstElement, mySecondElement);
			}
			else
			{
				myFirstElement = -1;
				mySecondElement = -1;
				myPath.clear();
			}

		}
	}



}



void GameWorld::Render()
{


	Tga2D::SpriteDrawer& spriteDrawer = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();




	auto batch = spriteDrawer.BeginBatch(myTileMat);

	for (int i = 0; i < myMap.size(); i++)
	{


		Tga2D::Sprite2DInstanceData ins;
		ins.mySize = myPhysMap[i].mySize;
		ins.myPosition = myPhysMap[i].myPos;


		bool isElementSelected = i == myFirstElement || i == mySecondElement;
		bool isPath = std::find(myPath.begin(), myPath.end(), i) != myPath.end();

		ins.myColor =  WHITE;


		if (myMap[i] == _TP)
		{
			ins.myColor = i == 0 ? Tga2D::Color(0, 1, 1, 1) : WHITE;
		}
		else
		{
			ins.myColor = RED;
		}

		if (isElementSelected)
			ins.myColor = GREEN;

		if (isPath && !isElementSelected)
			ins.myColor = BLUE;


		batch.Draw(ins);


	}
}