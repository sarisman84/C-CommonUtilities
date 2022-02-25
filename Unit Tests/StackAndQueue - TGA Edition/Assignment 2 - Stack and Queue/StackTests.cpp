#include "stdafx.h"
#include "CppUnitTest.h"

//#include "Student Code\Stack.hpp"
#include <Stack.hpp>
//#include "Student Code\Stack.h"
#include "Common\UtilityFunctions.hpp"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Stackf = CommonUtilities::Stack<float>;

namespace Assignment2StackandQueue
{
	TEST_CLASS(StackTests)
	{
	public:

		TEST_METHOD(S_Constructor)
		{
			Stackf stack;

			Assert::AreEqual(stack.GetSize(), 0, L"The size of an empty stack should be zero.");
		}

		TEST_METHOD(S_Push_GetSize)
		{
			Stackf stack;

			for (int i = 0; i < 60; ++i)
			{
				Assert::AreEqual(stack.GetSize(), i, L"GetSize() does not return the correct stack size.");
				stack.Push(static_cast<float>(i));
			}

			for (int i = 0; i < 600; ++i)
			{
				stack.Push(static_cast<float>(i));
			}

			Assert::AreEqual(stack.GetSize(), 660, L"GetSize() does not return the correct stack size.");
		}

		TEST_METHOD(S_GetTop)
		{
			Stackf stack;

			for (int i = 0; i < 60; ++i)
			{
				float rng = TestUtility::GetRandomFloat();
				stack.Push(rng);
				Assert::AreEqual(stack.GetTop(), rng, L"GetTop() does not return the correct value.");
			}
		}

		TEST_METHOD(S_GetTop_Const)
		{
			Stackf stack;
			const Stackf& constStack = stack;

			for (int i = 0; i < 60; ++i)
			{
				float rng = TestUtility::GetRandomFloat();
				stack.Push(rng);
				Assert::AreEqual(stack.GetTop(), rng, L"GetTop() does not return the correct value.");
			}
		}

		TEST_METHOD(S_Pop)
		{
			Stackf stack;
			std::vector<float> control;

			for (int i = 0; i < 60; ++i)
			{
				stack.Push(static_cast<float>(i));
			}

			for (int i = 0; i < 60; ++i)
			{
				float rng = TestUtility::GetRandomFloat();
				stack.Push(rng);
				control.push_back(rng);
			}

			for (int i = 59; i >= 0; --i)
			{
				Assert::AreEqual(control[i], stack.Pop(), L"Pop() does not return the expected value.");
			}

			for (int i = 59; i >= 0; --i)
			{
				Assert::AreEqual(static_cast<float>(i), stack.Pop(), L"Pop() does not return the expected value.");
			}
		}

		TEST_METHOD(S_SHOULDCRASH_Pop_Empty)
		{
			Stackf stack;

			stack.Pop();

			//Assert::AreEqual(0.0f, stack.Pop(), L"Pop() should return 0 from an empty stack.");
		}

		TEST_METHOD(S_SHOULDCRASH_Pop)
		{
			Stackf stack;

			for (int i = 0; i < 60; ++i)
			{
				stack.Push(static_cast<float>(i));
			}

			for (int i = 0; i < 61; ++i)
			{
				stack.Pop();
			}
		}

		TEST_METHOD(S_SHOULDCRASH_GetTop)
		{
			const Stackf stack;

			stack.GetTop();
		}

		TEST_METHOD(S_SHOULDCRASH_GetTop_Const)
		{
			const Stackf stack;

			stack.GetTop();
		}
	};
}