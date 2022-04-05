#include "stdafx.h"
#include "CppUnitTest.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include "Common\UtilityFunctions.hpp"
#include <C++/C-CommonUtilities/Sort.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Assignment8Sorting
{		
	TEST_CLASS(SortingTests)
	{
	public:
		
		TEST_METHOD(SelectionSort)
		{
			std::vector<int> numbers;

			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}

			CommonUtilities::SelectionSort(numbers);
			for (int index = 1; index < static_cast<int>(numbers.size()); ++index)
			{
				Assert::IsTrue(numbers[index - 1] <= numbers[index], L"The vector is not sorted after SelectionSort.");
			}

			std::vector<int> sortReversed;
			for (int iterator = static_cast<int>(numbers.size()) - 1; iterator >= 0; --iterator)
			{
				sortReversed.push_back(numbers[iterator]);
			}

			CommonUtilities::SelectionSort(sortReversed);
			for (int index = 1; index < static_cast<int>(sortReversed.size()); ++index)
			{
				Assert::IsTrue(sortReversed[index - 1] <= sortReversed[index], L"The reverse sorted vector is not sorted after SelectionSort.");
			}
		}

		TEST_METHOD(BubbleSort)
		{
			std::vector<int> numbers;

			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}

			CommonUtilities::BubbleSort(numbers);
			for (int index = 1; index < static_cast<int>(numbers.size()); ++index)
			{
				Assert::IsTrue(numbers[index - 1] <= numbers[index], L"The vector is not sorted after BubbleSort.");
			}

			std::vector<int> sortReversed;
			for (int iterator = static_cast<int>(numbers.size()) - 1; iterator >= 0; --iterator)
			{
				sortReversed.push_back(numbers[iterator]);
			}

			CommonUtilities::BubbleSort(sortReversed);
			for (int index = 1; index < static_cast<int>(sortReversed.size()); ++index)
			{
				Assert::IsTrue(sortReversed[index - 1] <= sortReversed[index], L"The reverse sorted vector is not sorted after BubbleSort.");
			}
		}

		TEST_METHOD(QuickSort)
		{
			std::vector<int> numbers;

			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}

			CommonUtilities::QuickSort(numbers);

			for (int index = 1; index < static_cast<int>(numbers.size()); ++index)
			{
				Assert::IsTrue(numbers[index - 1] <= numbers[index], L"The vector is not sorted after QuickSort.");
			}

			std::vector<int> sortReversed;
			for (int iterator = static_cast<int>(numbers.size()) - 1; iterator >= 0; --iterator)
			{
				sortReversed.push_back(numbers[iterator]);
			}

			CommonUtilities::QuickSort(sortReversed);
			for (int index = 1; index < static_cast<int>(sortReversed.size()); ++index)
			{
				Assert::IsTrue(sortReversed[index - 1] <= sortReversed[index], L"The reverse sorted vector is not sorted after QuickSort.");
			}
		}

		TEST_METHOD(MergeSort)
		{
			std::vector<int> numbers;

			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}

			CommonUtilities::MergeSort(numbers);

			for (int index = 1; index < static_cast<int>(numbers.size()); ++index)
			{
				Assert::IsTrue(numbers[index - 1] <= numbers[index], L"The vector is not sorted after MergeSort.");
			}

			std::vector<int> sortReversed;
			for (int iterator = static_cast<int>(numbers.size()) - 1; iterator >= 0; --iterator)
			{
				sortReversed.push_back(numbers[iterator]);
			}

			CommonUtilities::MergeSort(sortReversed);
			for (int index = 1; index < static_cast<int>(sortReversed.size()); ++index)
			{
				Assert::IsTrue(sortReversed[index - 1] <= sortReversed[index], L"The reverse sorted vector is not sorted after MergeSort.");
			}
		}

		// Run test in debug mode for how much memory is leaked
		TEST_METHOD(MemoryLeaks)
		{
			struct CrtCheckMemory
			{
				_CrtMemState state1;
				_CrtMemState state2;
				_CrtMemState state3;
				CrtCheckMemory()
				{
					_CrtMemCheckpoint(&state1);
				}
				~CrtCheckMemory()
				{
					_CrtMemCheckpoint(&state2);

					if (_CrtMemDifference(&state3, &state1, &state2)) {
						_CrtMemDumpStatistics(&state3);
						Assert::Fail(L"Memory leaks detected.");
					}
					_CrtMemDumpStatistics(&state3);
				}
			};
			CrtCheckMemory crt;

			std::vector<int> numbers;
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::MergeSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::QuickSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::BubbleSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::SelectionSort(numbers);
		}

#define TEST_LESS_THAN
#ifdef TEST_LESS_THAN
		TEST_METHOD(OnlyUsesLessThan)
		{
			struct TestStruct
			{
				int myInt = 0;

				TestStruct() = default;
				TestStruct(const int aInt) :myInt(aInt) {}
				TestStruct(const TestStruct& aOther) :myInt(aOther.myInt) {}
				TestStruct& operator=(const TestStruct& arhs)
				{
					myInt = arhs.myInt;
					return *this;
				}
				bool operator<(const TestStruct& arhs) const
				{
					return myInt < arhs.myInt;
				}
			};
			std::vector<TestStruct> numbers;
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::MergeSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::QuickSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::BubbleSort(numbers);
			numbers.clear();
			for (int iterator = 0; iterator < TestSample; ++iterator)
			{
				numbers.push_back(TestUtility::GetRandomInt(0, RandomMax));
			}
			CommonUtilities::SelectionSort(numbers);
		}
#else
		TEST_METHOD(SkippedLessThan)
		{
			Assert::Fail(L"Skipped testing less than use.")
		}
#endif // TEST_LESS_THAN

	private:
		const int TestSample = 9000;
		const int RandomMax = 18000;
	};
}