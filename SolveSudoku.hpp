#pragma once
#include <array>
#include <iostream>
#include <cmath>

namespace CommonUtilities
{
	using Block = std::array<std::array<int, 3>, 3>;
	using Board = std::array<Block, 9>;

	//Rules
	//Next tile needs to have: 
	// No dup in row
	// No dup in column
	// No dup in other blocks in same pos
	// No dup in same block

	void PrintBoard(Board aBoard) {

		for (int b = 0; b < aBoard.size(); b++)
		{
			for (int y = 0; y < aBoard[b].size(); y++)
			{
				for (int x = 0; x < aBoard[b][y].size(); x++)
				{
					std::cout << aBoard[b][y][x] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}


	const bool NumExistsAlready(Board& aBoard, int& aBoardPos, int& anXPos, int& anYPos, int& aNumber)
	{


		//Column check for the board
		for (size_t b = aBoardPos / 3; b < aBoard.size(); b += 3)
		{
			for (size_t y = 0; y < aBoard[b].size(); y++)
			{
				if (aBoard[b][y][anXPos] == aNumber) return true;
			}
		}


		//Row check for the board
		for (size_t b = aBoardPos % 3; b < aBoard.size(); b++)
		{
			for (size_t x = 0; x < aBoard[b][anYPos].size(); x++)
			{
				if (aBoard[b][anYPos][x] == aNumber) return true;
			}
		}

		//Same Block - other slot dup check
		for (size_t y = 0; y < aBoard[aBoardPos].size(); y++)
		{
			for (size_t x = 0; x < aBoard[aBoardPos][y].size(); x++)
			{
				if (aBoard[aBoardPos][y][x] == aNumber) {
					return true;
				}
			}
		}





		return false;
	}

	void GoToNextPos(Board& aBoard, int& aBoardPos, int& anXPos, int& anYPos)
	{
		anXPos++;
		bool isAtLimit = anXPos >= aBoard[aBoardPos][anYPos].size();

		anYPos = isAtLimit ? anYPos + 1 : anYPos;


		if (anYPos >= 3 && anXPos >= 3)
		{
			anXPos = 0;
			anYPos = 0;
			aBoardPos++;
		}

		anXPos = isAtLimit ? 0 : anXPos;


	}

	bool IsSafe(Board& aBoard, int aBoardPos, int anXPos, int anYPos, int& aNumber)
	{

		if (aBoard[aBoardPos][anYPos][anXPos] != 0)
		{

			return false;
		};

		if (NumExistsAlready(aBoard, aBoardPos, anXPos, anYPos, aNumber))
		{
			aNumber++;
			if (aNumber >= 10)
			{
				aNumber = 1;
				return false;

			}

			return IsSafe(aBoard, aBoardPos, anXPos, anYPos, aNumber);
		}


		return true;

	}




	bool IsBoardValid(Board& aBoard)
	{

		for (size_t b = 0; b < aBoard.size(); b++)
		{
			for (size_t y = 0; y < aBoard[b].size(); y++)
			{
				for (size_t x = 0; x < aBoard[b][y].size(); x++)
				{
					if (aBoard[b][y][x] == 0) return false;
				}
			}
		}



		return true;
	}

	bool SolveSudoku(Board& aBoard, int& aBoardPos, int& anXPos, int& anYPos)
	{

		if (aBoardPos >= aBoard.size()) return IsBoardValid(aBoard);

		int num = 1;

		if (IsSafe(aBoard, aBoardPos, anXPos, anYPos, num))
		{
			aBoard[aBoardPos][anYPos][anXPos] = num;
		}

		GoToNextPos(aBoard, aBoardPos, anXPos, anYPos);
		std::cout << std::endl;
		std::cout << "Attempt ----------------------- [" << aBoardPos << "]" << std::endl;
		PrintBoard(aBoard);

		return SolveSudoku(aBoard, aBoardPos, anXPos, anYPos);
	}


	int Pow(int aVal, int anExp) {

		int result = aVal;

		for (size_t i = 0; i < anExp; i++)
		{
			result *= aVal;
		}


		return result;
	}


	Board CreateBoard(std::array<int, 81> aBoard)
	{
		Board board = Board();
		int z = 0;
		for (int i = 0; i < aBoard.size(); i++)
		{
			/*int x = (i % 3);
			int y = (i / 3) % 3;
			int z = i / (3 * 3);*/

			if (i % 9 == 0 && i != 0) z++;

			int y = (i / 3) % 3;


			int x = i % 3;


			board[z][y][x] = aBoard[i];
		}


		return board;
	}




	bool SolveSudoku(std::array<int, 81>& aBoard)
	{
		Board board = CreateBoard(aBoard);
		int boardPos = 0;
		int slotXPos = 0;
		int slotYPos = 0;
		return SolveSudoku(board, boardPos, slotXPos, slotYPos);
	}



	/*

	const bool IsNumberInSameColumn(const Block aBlock, const int aNumber, const int aLocalPosition, int aCurrentRow)
	{
		std::cout << " " << aLocalPosition + aCurrentRow;
		if (aLocalPosition + aCurrentRow >= 9) return false;
		if (aLocalPosition + aCurrentRow >= 0 && aBlock[aLocalPosition + aCurrentRow] == aNumber) {

			std::cout << " <Dub detected!> ";
			return true;
		}
		return IsNumberInSameColumn(aBlock, aNumber, aLocalPosition, aCurrentRow + 3);
	}

	const bool IsNumberInSameRow(const Block aBlock, const int aNumber, const int aLocalPosition, int aCurrentColumn)
	{
		std::cout << " " << aLocalPosition + aCurrentColumn;
		if (aCurrentColumn > 1) return false;
		if (aLocalPosition + aCurrentColumn >= 0 && aLocalPosition + aCurrentColumn < aBlock.size() && aBlock[aLocalPosition + aCurrentColumn] == aNumber)
		{
			std::cout << " <Dub detected!> ";
			return true;
		}
		return IsNumberInSameRow(aBlock, aNumber, aLocalPosition, aCurrentColumn + 1);

	}


	const bool IsNumberAlreadyInSamePosOfOtherBlocks(const Board aBoard, const int aNumber, const int aLocalPositions, int aCurrentBlock) {
		std::cout << " " << aCurrentBlock;
		if (aCurrentBlock >= 9) return false;
		if (aBoard[aLocalPositions + (9 * aCurrentBlock)] == aNumber)
		{
			std::cout << " <Dub detected!> ";
			return true;
		}

		return IsNumberAlreadyInSamePosOfOtherBlocks(aBoard, aNumber, aLocalPositions, aCurrentBlock + 1);
	}


	const int GetFirstAvaliableNumber(const Block aBlock, int aNumber) {

		if (std::find(aBlock.begin(), aBlock.end(), aNumber) == aBlock.end()) {
			return aNumber;
		}
		else if (aNumber <= 9) {
			return GetFirstAvaliableNumber(aBlock, aNumber + 1);
		}
		else {
			return 0;
		}

	}

	if (aCurrentBlockVal >= 9) return std::find(aBoard.begin(), aBoard.end(), 0) == aBoard.end();

		Block aBlock;

		int offset = (9 * aCurrentBlockVal);

		std::cout << std::endl;
		std::cout << "<--------------------------->" << std::endl;

		int localOffset = 0;
		for (int i = 0; i < aBlock.size(); i++)
		{
			if (i != 8)
				if (i % 3 == 0) localOffset += 6;
			std::cout << aBoard[i + offset + localOffset] << std::endl;
			aBlock[i] = aBoard[i + offset + localOffset];
		}

		std::cout << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << offset << std::endl;

		for (int i = 0; i < aBlock.size(); i++)
		{
			if (aBlock[i] == 0) //Is slot empty
			{
				int number = GetFirstAvaliableNumber(aBlock, 1);

				if (number == 0) continue;
				std::cout << std::endl;
				std::cout << "Check Dup in other blocks for " << number << ": ";
				if (IsNumberAlreadyInSamePosOfOtherBlocks(aBoard, number, i, 0)) continue;
				std::cout << std::endl;

				std::cout << "Check Dup in same block " << number << " - Column Check: ";
				if (IsNumberInSameColumn(aBlock, number, i, -1)) continue;
				std::cout << std::endl;

				std::cout << "Check Dup in same block " << number << " - Row Check: ";
				if (IsNumberInSameRow(aBlock, number, i, -3)) continue;
				aBoard[i + offset] = number;


			}
		}




	*/

}