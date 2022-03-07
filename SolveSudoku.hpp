#pragma once
#include <array>
#include <iostream>
#include <cmath>

namespace CommonUtilities
{

	using Board = std::array<std::array<int, 9>, 9>;

	//Rules
	//Next tile needs to have: 
	// No dup in row
	// No dup in column
	// No dup in other blocks in same pos
	// No dup in same block

	void PrintBoard(Board aBoard) {


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









	bool IsSafe(Board& aBoards, int anXPos, int anYPos, int& aNumber)
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






	bool SolveSudoku(Board& aBoard, int anXPos, int anYPos)
	{

		if (anYPos == aBoard.size() - 1 && anXPos == aBoard[anYPos].size())
		{
			std::cout << "End Result" << std::endl;
			PrintBoard(aBoard);
			std::cout << std::endl;
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


			if (IsSafe(aBoard, anXPos, anYPos, i)) {



				//Set number
				aBoard[anYPos][anXPos] = i;


				if (SolveSudoku(aBoard, anXPos + 1, anYPos)) {
					
					return true;
				}
			}


			//reset number if we are wrong

			aBoard[anYPos][anXPos] = 0;
		}

		return false;
	}





	Board CreateBoard(std::array<int, 81> aBoard)
	{
		Board board = Board();

		for (int i = 0; i < aBoard.size(); i++)
		{

			int x = i % 9;
			int y = i / 9;

			board[y][x] = aBoard[i];
		}


		return board;
	}




	bool SolveSudoku(std::array<int, 81>& aBoard)
	{
		Board board = CreateBoard(aBoard);
		std::cout << "Starting Board" << std::endl;
		PrintBoard(board);
		std::cout << std::endl;
		return SolveSudoku(board, 0, 0);
	}





}