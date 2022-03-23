#pragma once
#include "Dijkstra.hpp"
//#include <tga2d/math/vector2.h>
#include <array>
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
	std::vector<int> myPath;
	Tga2D::Vector2i myFirstElement, mySecondElement;
};