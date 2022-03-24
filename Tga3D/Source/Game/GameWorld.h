#pragma once
#include "Dijkstra.hpp"
#include <tga2d/math/vector2.h>
#include <array>
#include <tga2d/sprite/sprite.h>
namespace Tga2D
{
	class Texture;
}


using namespace CommonUtilities;

#define _TP CommonUtilities::Tile::Passable
#define _TB CommonUtilities::Tile::Impassable

#define GREEN Tga2D::Color(0,1,0,1)
#define BLUE Tga2D::Color(0,0,1,1)
#define RED Tga2D::Color(1,0,0,1)
#define WHITE Tga2D::Color(1, 1, 1, 1)


struct SpriteTile
{
	Tga2D::Vector2f myPos;
	Tga2D::Vector2f mySize;
};


class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();
private:
	std::vector<Tile> myMap;
	std::vector<SpriteTile> myPhysMap;
	std::vector<int> myPath;
	int myFirstElement, mySecondElement;
	Tga2D::SpriteSharedData myTileMat;
};