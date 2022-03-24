#pragma once
#include <vector>
#include "Heap.hpp"
#include <array>
#include <tga2d/math/vector2.h>

namespace CommonUtilities
{
	static const int MapWidth = 20;
	static const int MapHeight = 20;
	static const int TileCount = MapWidth * MapHeight;

	enum class Tile
	{
		Impassable,
		Passable
	};

	struct Node
	{
		int myIndex;
		int* myDist;

		inline bool operator >(Node anOtherNode)
		{
			return myDist > anOtherNode.myDist;
		}


		inline bool operator <(Node anOtherNode)
		{
			return myDist < anOtherNode.myDist;
		}
	};



	inline std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex)
	{

		if (aStartIndex < 0 || aStartIndex >= TileCount || anEndIndex < 0 || anEndIndex >= TileCount) return std::vector<int>();
		if (aMap[anEndIndex] == Tile::Impassable) return std::vector<int>();
		if (aMap[aStartIndex] == Tile::Impassable) return std::vector<int>();

		std::array<int, TileCount> myNodeDist;
		std::vector<int> result;
		CommonUtilities::Heap<Node, HeapType::Min> nodesToCheck;


		for (int i = 0; i < TileCount; i++)
		{
			myNodeDist[i] = INT_MAX;
			nodesToCheck.Enqueue(Node{ i, &myNodeDist[i] });
		}



	
		myNodeDist[aStartIndex] = 0;


		bool isPathFound = false;
		int currentNode;
		while (nodesToCheck.GetSize() > 0)
		{
			currentNode = nodesToCheck.Dequeue().myIndex;

			if (currentNode == anEndIndex)
			{
				isPathFound = true;
				break;
			}


			Tga2D::Vector2i twoDCoord = {};
			twoDCoord.x = currentNode % MapWidth;
			twoDCoord.y = currentNode / MapHeight;

			//int startPos = currentNode - currentNode % 3;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (x == 0 && y == 0) continue;

					Tga2D::Vector2i neighbour = twoDCoord + Tga2D::Vector2i(x, y);

					//int neighbourNode = currentNode + x + (MapWidth * y); //i = x + width * y
					if (neighbour.x < 0 || neighbour.x >= MapWidth || neighbour.y < 0 || neighbour.y >= MapHeight) continue;

					int neighbourIndex = neighbour.x + MapWidth * neighbour.y;
					if (aMap[neighbourIndex] == Tile::Impassable) continue;

					if (myNodeDist[neighbourIndex] > myNodeDist[currentNode])
					{
						myNodeDist[neighbourIndex] = myNodeDist[currentNode];
						result.push_back(neighbourIndex);
					}




					result.push_back(neighbourIndex);
				}
			}





			
		}






		if (!isPathFound) result.clear();

		return result;

	}



}

