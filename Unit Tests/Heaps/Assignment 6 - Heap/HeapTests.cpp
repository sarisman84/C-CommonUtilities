#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <algorithm>
#include <limits>
#include "Common\UtilityFunctions.hpp"
#include "Heap.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Heapi = CommonUtilities::Heap<int>;

namespace Assignment6Heap
{		
	TEST_CLASS(HeapTests)
	{
	public:
		
		TEST_METHOD(Constructor)
		{
			Heapi heap;

			Assert::AreEqual(0, heap.GetSize(), L"A just created heap should have a size of 0.");
		}

		TEST_METHOD(Enqueue)
		{
#undef min
#undef max
			Heapi heap;
			int max = std::numeric_limits<int>::min();

			for (int i = 0; i < 300; ++i)
			{
				Assert::AreEqual(i, heap.GetSize(), L"The heap has an incorrect size.");
				heap.Enqueue(TestUtility::GetRandomInt(-100, 100));
			}

			Assert::AreEqual(300, heap.GetSize(), L"The heap has an incorrect size.");
		}

		TEST_METHOD(GetTop)
		{
			Heapi heap;
			int max = std::numeric_limits<int>::min();

			for (int i = 0; i < 300; ++i)
			{
				int number = TestUtility::GetRandomInt(-100, 100);
				heap.Enqueue(number);
				
				if (number > max)
				{
					max = number;
				}
				
				Assert::AreEqual(max, heap.GetTop(), L"The top element on the heap is not the largest.");
			}
		}

		TEST_METHOD(Dequeue)
		{
			Heapi heap;
			std::vector<int> vec;

			for (int i = 0; i < 300; ++i)
			{
				int number = TestUtility::GetRandomInt(-100, 100);
				heap.Enqueue(number);
				vec.push_back(number);
			}

			std::sort(vec.begin(), vec.end());

			for (int i = 0; i < 300; ++i)
			{
				Assert::AreEqual(static_cast<int>(vec.size()), heap.GetSize(), L"Heap size does not match size of control sequence.");
				Assert::AreEqual(vec.back(), heap.Dequeue(), L"Heap top element does not match the largest element in the control sequence.");
				vec.pop_back();
			}
		}

		TEST_METHOD(GetSize)
		{
			Heapi heap;

			for (int i = 0; i < 300; ++i)
			{
				Assert::AreEqual(i, heap.GetSize(), L"The heap has an incorrect size.");
				heap.Enqueue(TestUtility::GetRandomInt(-100, 100));
			}
			
			for (int i = 300; i > 0; --i)
			{
				Assert::AreEqual(i, heap.GetSize(), L"The heap has an incorrect size.");
				heap.Dequeue();
			}

			Assert::AreEqual(0, heap.GetSize(), L"The heap has an incorrect size.");
		}
	};
}