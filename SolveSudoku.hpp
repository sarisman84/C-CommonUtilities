#pragma once
#include <array>
#include <iostream>

namespace CommonUtilities
{

	using Board = std::array<int, 81>;
	using Block = std::array<int, 9>;
	//Rules
	//Next tile needs to have: 
	// No dup in row
	// No dup in column
	// No dup in other blocks in same pos


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





	bool SolveSudoku(Board& aBoard, int aCurrentBlockVal) {
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


		return SolveSudoku(aBoard, aCurrentBlockVal + 1);
	}





	bool SolveSudoku(Board& aBoard)
	{
		return SolveSudoku(aBoard, 0);
	}





}