#pragma once
#include <array>
#include <iostream>
#include <cmath>

namespace CommonUtilities
{
	using CBoard = std::array<std::array<int, 9>, 9>;

	void PrintBoard(CBoard aBoard)
	{


		for (int y = 0; y < aBoard.size(); y++)
		{
			for (int x = 0; x < aBoard[y].size(); x++)
			{
				std::cout << aBoard[y][x] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

	}

	bool IsSafe(CBoard& aBoards, int anXPos, int anYPos, int& aNumber)
	{

		//Column Check
		for (int i = 0; i < aBoards.size(); i++)
		{
			if (aBoards[i][anXPos] == aNumber) return false;
		}

		//Row  Check
		for (int i = 0; i < aBoards[anYPos].size(); i++)
		{
			if (aBoards[anYPos][i] == aNumber) return false;
		}


		int startRow = anYPos - anYPos % 3;
		int	startCol = anXPos - anXPos % 3;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (aBoards[i + startRow][j + startCol] == aNumber)
				{
					return false;
				}
			}
		}


		return true;

	}
	bool SolveSudoku(CBoard& aBoard, int anXPos, int anYPos)
	{

		if (anYPos == aBoard.size() - 1 && anXPos == aBoard[anYPos].size())
		{
			return true;
		}

		if (anXPos == aBoard[anYPos].size())
		{
			anYPos++;
			anXPos = 0;
		}

		if (aBoard[anYPos][anXPos] > 0)
		{
			return SolveSudoku(aBoard, anXPos + 1, anYPos);
		}


		for (int i = 1; i <= 9; i++)
		{


			if (IsSafe(aBoard, anXPos, anYPos, i))
			{



				//Set number
				aBoard[anYPos][anXPos] = i;


				if (SolveSudoku(aBoard, anXPos + 1, anYPos))
				{

					return true;
				}
			}


			//reset number if we are wrong

			aBoard[anYPos][anXPos] = 0;
		}

		return false;
	}

	CBoard CreateBoard(std::array<int, 81> aBoard)
	{
		CBoard board = CBoard();

		for (int i = 0; i < aBoard.size(); i++)
		{

			int x = i % 9;
			int y = i / 9;

			board[y][x] = aBoard[i];
		}


		return board;
	}

	void CopyToSingleArray(std::array<int, 81>& aBoard, CBoard aCBoard)
	{
		for (int y = 0; y < aCBoard.size(); y++)
		{
			for (int x = 0; x < aCBoard[y].size(); x++)
			{
				int i = x + aCBoard.size() * y;
				aBoard[i] = aCBoard[y][x];
			}
		}

	}

	bool SolveSudoku(std::array<int, 81>& aBoard)
	{
		CBoard board = CreateBoard(aBoard);
		PrintBoard(board);
		std::cout << std::endl;
		if (SolveSudoku(board, 0, 0))
		{
			CopyToSingleArray(aBoard, board);
			return true;
		}

		return false;

	}





}