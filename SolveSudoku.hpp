#pragma once
#include <array>
#include <iostream>

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


	bool DupeCheckInSameBlock(int& /*aTargetNum*/, const int /*anXPos*/, const int /*anYPos*/)
	{
		/*for (int i = 0; i < 9; i++)
		{

		}*/

		return false;
	}


	const int AssignNumber(Board /*aBoard*/, const int /*anXPos*/, const int /*anYPos*/)
	{


		/*


		if (aBoard[anYPos][anXPos] == 0 &&
		DupeCheckInColumn(anYPos, anXPos) &&
		DupeCheckInRow(anYPos, anXPos) &&
		DupeCheckInOtherBlocks(anYPos, anXPos) &&
		DupeCheckInSameBlock(anYPos, anXPos)
	{

	}

	*/
	/*int targetNum;

	int boxRow = anXPos / 3;
	int boxColumn = anYPos / 3;*/

		return 0;
	}


	bool IsBoardValid(Board& aBoard) {


		for (size_t y = 0; y < aBoard.size(); y++)
		{
			for (size_t x = 0; x < aBoard[x].size(); x++)
			{
				if (aBoard[y][x][0] == 0) return false;
			}
		}


		return true;
	}

	bool SolveSudoku(Board& aBoard, int anXPos, int anYPos) {

		if (anXPos >= 9 && anYPos >= 9) return IsBoardValid(aBoard);



		/*	int number = AssignNumber(aBoard, anXPos, anYPos);
			if (number != 0) {
				aBoard[anXPos][anYPos] = number;
			}*/
			//Pick a number
			//Check if pos empty


			//Check if number exists in other blocks

			//Check if number exist in block
			//Check row
			//Check column

		return SolveSudoku(aBoard, (anXPos + 1) % 9, anXPos >= 9 ? anYPos + 1 : anYPos);
	}




	Board CreateBoard(std::array<int, 81> aBoard)
	{
		Board board = Board();

		for (int block = 0; block < board.size(); block++)
		{
			for (int ySlot = 0; ySlot < board[block].size(); ySlot++)
			{
				for (int xSlot = 0; xSlot < board[block][ySlot].size(); xSlot++)
				{
					int i = (xSlot + (9 * ySlot) + (3 * block)) % aBoard.size();

					board[block][ySlot][xSlot] = aBoard[i];
				}
			}
		}


		return board;
	}

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


	bool SolveSudoku(std::array<int, 81>& aBoard)
	{
		Board board = CreateBoard(aBoard);
		PrintBoard(board);
		return false;
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