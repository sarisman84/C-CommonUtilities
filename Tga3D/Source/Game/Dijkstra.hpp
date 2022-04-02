#pragma once
#include <vector>
#include <array>
#include <tga2d/math/vector2.h>
#include <algorithm>

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
		int myDist;
		std::vector<int> myConnections;
	};

	inline std::vector<int> Dijkstra(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex)
	{

		if (aStartIndex < 0 || aStartIndex >= TileCount || anEndIndex < 0 || anEndIndex >= TileCount) return std::vector<int>();
		if (aMap[anEndIndex] == Tile::Impassable) return std::vector<int>();
		if (aMap[aStartIndex] == Tile::Impassable) return std::vector<int>();

		std::array<Node, TileCount> myNodeDist;
		std::vector<int> nodesToCheck;
		std::vector<int> checkedNodes;

		for (int i = 0; i < TileCount; i++)
		{
			myNodeDist[i] = Node{ i,  i == aStartIndex ? 0 : INT_MAX };
			nodesToCheck.push_back(i);
		}

		int currentNode = 0;
		while (nodesToCheck.size() > 0)
		{
			std::sort(nodesToCheck.begin(), nodesToCheck.end(), [&](int aLhs, int aRhs)->bool { return myNodeDist[aLhs].myDist < myNodeDist[aRhs].myDist; });

			currentNode = nodesToCheck.front();
			nodesToCheck[0] = nodesToCheck.back();
			nodesToCheck.pop_back();

			checkedNodes.push_back(currentNode);
			if (currentNode == anEndIndex)
			{
				break;
			}

			Tga2D::Vector2i twoDCoord = {};
			twoDCoord.x = currentNode % MapWidth;
			twoDCoord.y = currentNode / MapHeight;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (x == 0 && y == 0) continue;
					if (y == -1 && x != 0) continue;
					if (y == 1 && x != 0) continue;

					Tga2D::Vector2i neighbour = twoDCoord + Tga2D::Vector2i(x, y);
					if (neighbour.x < 0 || neighbour.x >= MapWidth || neighbour.y < 0 || neighbour.y >= MapHeight) continue;

					int neighbourIndex = neighbour.x + MapWidth * neighbour.y;
					if (aMap[neighbourIndex] == Tile::Impassable || std::find(checkedNodes.begin(), checkedNodes.end(), neighbourIndex) != checkedNodes.end()) continue;

					if (myNodeDist[neighbourIndex].myDist > myNodeDist[currentNode].myDist + std::abs(anEndIndex - currentNode))
					{
						myNodeDist[neighbourIndex].myDist = myNodeDist[currentNode].myDist + std::abs(anEndIndex - currentNode);
						myNodeDist[neighbourIndex].myConnections.push_back(currentNode);
					}
				}
			}
		}
	

		std::vector<int> path;
		currentNode = anEndIndex;
		while (currentNode != aStartIndex)
		{
			int dist = INT_MAX;

			if (myNodeDist[currentNode].myConnections.size() == 0)
			{
				path.clear();
				break;
			}

			for (auto& connection : myNodeDist[currentNode].myConnections)
			{
				if (dist >= myNodeDist[connection].myDist)
				{
					dist = myNodeDist[connection].myDist;
					currentNode = connection;
				}
			}
			path.push_back(currentNode);
		}
		return path;
	}
}

