#pragma once
#include <vector>
#include "Heap.hpp"

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
		Node(int anIndex, int aDist) { myIndex = anIndex; myDist = aDist; }
		int myIndex;
		int myDist;
	};



	bool Contains(const std::vector<Node>& someNodes, const Node& aNode)
	{
		return std::find(someNodes.begin(), someNodes.end(), aNode) != someNodes.end();
	}


	std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex)
	{

		if (aStartIndex < 0 || aStartIndex >= TileCount || anEndIndex < 0 || anEndIndex >= TileCount) return;
		if (aMap[anEndIndex] == Tile::Impassable) return;
		if (aMap[aStartIndex] == Tile::Impassable) return;

		std::vector<Node> myNodes;
		std::vector<int> result;
		int range = std::abs(anEndIndex - aStartIndex);
		CommonUtilities::Heap<Node*, HeapType::Min, true> nodesToCheck;


		for (size_t i = 0; i < range; i++)
		{
			myNodes.push_back(Node(aStartIndex + i, INT_MAX));
			nodesToCheck.Enqueue(&myNodes.back());
		}


		


		int currentNode = aStartIndex;
		myNodes[currentNode].myDist = 0;


	
		while (nodesToCheck.GetSize() > 0)
		{
			
			int width = currentNode % 9;
			int height = currentNode / 9;

			for (size_t y = 0; y < height; y++)
			{
				for (size_t x = 0; x < width; x++)
				{
					if (x == 2 && y == 2 || x + currentNode * y >= myNodes.size() || aMap[x + currentNode * y] == Tile::Impassable) continue;
					if (myNodes[x + currentNode * y].myDist > myNodes[currentNode].myDist + std::abs(myNodes[x + currentNode * y].myIndex - myNodes[currentNode].myIndex))
					{
						myNodes[x + currentNode * y].myDist = myNodes[currentNode].myDist + std::abs(myNodes[x + currentNode * y].myIndex - myNodes[currentNode].myIndex);
						result.push_back(currentNode);
					}

				}
			}

			currentNode = nodesToCheck.Dequeue()->myIndex;

		}

		return result;
	}

}
