#include "stdafx.h"
#include "CppUnitTest.h"

#include <array>

//#include "Student Code\SolveSudoku.hpp"
#include <SolveSudoku.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonUtilities;
using Board = std::array<int, 81>;

namespace Assignment3Recursion
{		
	TEST_CLASS(RecursionTests)
	{
	public:
		TEST_METHOD(Completed)
		{
			Board original = {	9, 7, 4,	2, 3, 6,	1, 5, 8,
								6, 3, 8,	5, 9, 1,	7, 4, 2,
								1, 2, 5,	4, 8, 7,	9, 3, 6,

								3, 1, 6,	7, 5, 4,	2, 8, 9,
								7, 4, 2,	9, 1, 8,	5, 6, 3,
								5, 8, 9,	3, 6, 2,	4, 1, 7,

								8, 6, 7,	1, 2, 5,	3, 9, 4,
								2, 5, 3,	6, 4, 9,	8, 7, 1,
								4, 9, 1,	8, 7, 3,	6, 2, 5
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution: the board is already solved.");
			AssertBoardGivensUnchanged(original, board);
		}

		TEST_METHOD(OneEmpty)
		{
			Board original = {	9, 7, 4,	2, 3, 0,	1, 5, 8,
								6, 3, 8,	5, 9, 1,	7, 4, 2,
								1, 2, 5,	4, 8, 7,	9, 3, 6,

								3, 1, 6,	7, 5, 4,	2, 8, 9,
								7, 4, 2,	9, 1, 8,	5, 6, 3,
								5, 8, 9,	3, 6, 2,	4, 1, 7,

								8, 6, 7,	1, 2, 5,	3, 9, 4,
								2, 5, 3,	6, 4, 9,	8, 7, 1,
								4, 9, 1,	8, 7, 3,	6, 2, 5
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution: the board only has one empty cell.");
			Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution (board has only one empty cell).");
			AssertBoardGivensUnchanged(original, board);
		}

		TEST_METHOD(SoleCandidates)
		{
			Board original = {	3, 0, 5,	4, 2, 0,	8, 1, 0,
								4, 8, 7,	9, 0, 1,	5, 0, 6,
								0, 2, 9,	0, 5, 6,	3, 7, 4,

								8, 5, 0,	7, 9, 3,	0, 4, 1,
								6, 1, 3,	2, 0, 8,	9, 5, 7,
								0, 7, 4,	0, 6, 5,	2, 8, 0,
								
								2, 4, 1,	3, 0, 9,	0, 6, 5,
								5, 0, 8,	6, 7, 0,	1, 9, 2,
								0, 9, 6,	5, 1, 2,	4, 0, 8
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution: the empty cells each have only one valid solution.");
			Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution.");
			AssertBoardGivensUnchanged(original, board);
		}

		TEST_METHOD(HiddenSingles)
		{
			Board original = {	0, 0, 2,	0, 3, 0,	0, 0, 8,
								0, 0, 0,	0, 0, 8,	0, 0, 0,
								0, 3, 1,	0, 2, 0,	0, 0, 0,

								0, 6, 0,	0, 5, 0,	2, 7, 0,
								0, 1, 0,	0, 0, 0,	0, 5, 0,
								2, 0, 4,	0, 6, 0,	0, 3, 1,

								0, 0, 0,	0, 8, 0,	6, 0, 5,
								0, 0, 0,	0, 0, 0,	0, 1, 3,
								0, 0, 5,	3, 1, 0,	4, 0, 0
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution.");
			Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution.");
			AssertBoardGivensUnchanged(original, board);
		}

		TEST_METHOD(Empty)
		{
			Board board = {	0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0,

							0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0,

							0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0,
							0, 0, 0,	0, 0, 0,	0, 0, 0
			};

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution: the board is empty so any valid sudoku is a solution.");
			Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution (board with no givens).");
		}

		TEST_METHOD(SingleGiven)
		{
			Board original = {	0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0,

								0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 1, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0,

								0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution: the board only has one given so there are many valid solutions.");
			Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution (board with only one given).");
			AssertBoardGivensUnchanged(original, board);
		}

		TEST_METHOD(DuplicateGivenBox) // Unsolvable
		{
			Board original = {	0, 0, 9,	0, 7, 0,	0, 0, 5,
								0, 0, 2,	1, 0, 0,	9, 0, 0,
								1, 0, 0,	0, 2, 8,	0, 0, 0,

								0, 7, 0,	0, 0, 5,	0, 0, 1,
								0, 0, 8,	5, 1, 0,	0, 0, 0,
								0, 5, 0,	0, 0, 0,	3, 0, 0,

								0, 0, 0,	0, 0, 3,	0, 0, 6,
								8, 0, 0,	0, 0, 0,	0, 0, 0,
								2, 1, 0,	0, 0, 0,	0, 8, 7
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: the board is unsolvable as one box has two 5s.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(DuplicateGivenRow) // Unsolvable
		{
			Board original = {	0, 4, 0,	1, 0, 0,	3, 5, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 0, 0,	2, 0, 5,	0, 0, 0,

								0, 0, 0,	4, 0, 8,	9, 0, 0,
								2, 6, 0,	0, 0, 0,	0, 1, 2,
								0, 5, 0,	3, 0, 0,	0, 0, 7,

								0, 0, 4,	0, 0, 0,	1, 6, 0,
								6, 0, 0,	0, 0, 7,	0, 0, 0,
								0, 1, 0,	0, 8, 0,	0, 2, 0
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: the board is unsolvable as one row has two 2s.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(DuplicateGivenColumn) // Unsolvable
		{
			Board original = {	6, 0, 1,	5, 9, 0,	0, 0, 0,
								0, 9, 0,	0, 1, 0,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 4,

								0, 7, 0,	3, 1, 4,	0, 0, 6,
								0, 2, 4,	0, 0, 0,	0, 0, 5,
								0, 0, 3,	0, 0, 0,	0, 1, 0,

								0, 0, 6,	0, 0, 0,	0, 0, 3,
								0, 0, 0,	9, 0, 2,	0, 4, 0,
								0, 0, 0,	0, 0, 1,	6, 0, 0
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: the board is unsolvable as one column has two 1s.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(NoCandiates) // Unsolvable (Unsolvable Square)
		{
			Board original = {	0, 0, 9,	0, 2, 8,	7, 0, 0,
								8, 0, 6,	0, 0, 4,	0, 0, 5,
								0, 0, 3,	0, 0, 0,	0, 0, 4,

								6, 0, 0,	0, 0, 0,	0, 0, 0,
								0, 2, 0,	7, 1, 3,	4, 5, 0,
								0, 0, 0,	0, 0, 0,	0, 0, 2,

								3, 0, 0,	0, 0, 0,	5, 0, 0,
								9, 0, 0,	4, 0, 0,	8, 0, 7,
								0, 0, 1,	2, 5, 0,	3, 0, 0
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: the board is unsolvable as one cell has no possible candidates.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(NoCandiatesBox) // Unsolvable (Unsolvable Box)
		{
			Board original = {	0, 9, 0,	3, 0, 0,	0, 0, 1,
								0, 0, 0,	0, 8, 0,	0, 4, 6,
								0, 0, 0,	0, 0, 0,	8, 0, 0,

								4, 0, 5,	0, 6, 0,	0, 3, 0,
								0, 0, 3,	2, 7, 5,	6, 0, 0,
								0, 6, 0,	0, 1, 0,	9, 0, 4,

								0, 0, 1,	0, 0, 0,	0, 0, 0,
								5, 8, 0,	0, 2, 0,	0, 0, 0,
								2, 0, 0,	0, 0, 7,	0, 6, 0
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: there is no way to place the number 4 in the middle box.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(NoCandiatesRow) // Unsolvable (Unsolvable Row)
		{
			Board original = {	9, 0, 0,	1, 0, 0,	0, 0, 4,
								0, 1, 4,	0, 3, 0,	8, 0, 0,
								0, 0, 3,	0, 0, 0,	0, 9, 0,

								0, 0, 0,	7, 0, 8,	0, 0, 1,
								8, 0, 0,	0, 0, 3,	0, 0, 0,
								0, 0, 0,	0, 0, 0,	0, 3, 0,

								0, 2, 1,	0, 0, 0,	0, 7, 0,
								0, 0, 9,	0, 4, 0,	5, 0, 0,
								5, 0, 0,	0, 1, 6,	0, 0, 3
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: there is no way to place the number 1 in the middle row.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

		TEST_METHOD(NoCandiatesColumn) // Unsolvable (Unsolvable Column)
		{
			Board original = {	0, 0, 0,	0, 4, 1,	0, 0, 0,
								0, 6, 0,	0, 0, 0,	0, 2, 0,
								0, 0, 2,	0, 0, 0,	0, 0, 0,

								3, 2, 0,	6, 0, 0,	0, 0, 0,
								0, 0, 0,	0, 5, 0,	0, 4, 1,
								7, 0, 0,	0, 0, 0,	0, 0, 2,

								0, 0, 0,	0, 0, 0,	2, 3, 0,
								0, 4, 8,	0, 0, 0,	0, 0, 0,
								5, 0, 1,	0, 0, 2,	0, 0, 0
			};
			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: there is no way to place the number 2 in the middle column.");
			Assert::IsTrue(AreEqual(original, board), L"Failing to find a solution should not change the input.");
		}

	private:
		// Function to validate a solution
		bool IsValidSolution(const Board& anArray)
		{
			const int Dimension = 9;

			// Check rows
			for (int row = 0; row < Dimension; ++row)
			{
				std::array<bool, 10> numberCount = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				for (int element = 0; element < Dimension; ++element)
				{
					numberCount[anArray[(row * Dimension) + element]]++;
				}

				if (numberCount[0] != 0)
				{
					return false;
				}

				for (int i = 1; i < numberCount.size(); ++i)
				{
					if (numberCount[i] != 1)
					{
						return false;
					}
				}
			}

			// Check columns
			for(int column = 0; column < Dimension; ++column)
			{
				std::array<bool, 10> numberCount = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				for (int element = 0; element < Dimension; ++element)
				{
					numberCount[anArray[(element * Dimension) + column]]++;
				}

				if (numberCount[0] != 0)
				{
					return false;
				}

				for (int i = 1; i < numberCount.size(); ++i)
				{
					if (numberCount[i] != 1)
					{
						return false;
					}
				}
			}

			// Check boxes
			for (int box = 0; box < Dimension; ++box)
			{
				int boxFirstIndex = (box / 3) * 27 + (box % 3) * 3;

				std::array<bool, 10> numberCount = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				for (int row = 0; row < 3; ++row)
				{
					for (int column = 0; column < 3; ++column)
					{
						numberCount[anArray[boxFirstIndex + (row * 9) + column]]++;
					}
				}

				if (numberCount[0] != 0)
				{
					return false;
				}

				for (int i = 1; i < numberCount.size(); ++i)
				{
					if (numberCount[i] != 1)
					{
						return false;
					}
				}
			}

			return true;
		}
		
		bool AreEqual(const Board& anOriginal, const Board aResult)
		{
			if (anOriginal.size() != aResult.size())
			{
				return false;
			}

			for (int i = 0; i < anOriginal.size(); ++i)
			{
				if (aResult[i] != anOriginal[i])
				{
					return false;
				}
			}

			return true;
		}

		void AssertBoardGivensUnchanged(const Board& anOriginal, const Board aResult)
		{
			Assert::AreEqual(anOriginal.size(), aResult.size(), L"Board size of the result is incorrect.");

			for (int i = 0; i < anOriginal.size(); ++i)
			{
				if (anOriginal[i] > 0)
				{
					Assert::AreEqual(anOriginal[i], aResult[i], L"The function has changed some of the values (not zero) in the original board. Result is invalid.");
				}
			}
		}
	};
}