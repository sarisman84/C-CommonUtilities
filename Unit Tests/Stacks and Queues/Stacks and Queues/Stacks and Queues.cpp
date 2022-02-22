#include "pch.h"
#include "CppUnitTest.h"
#include "Stack.hpp"
#include "Queue.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Stackf = CommonUtilities::Stack<float>;
using Queuef = CommonUtilities::Queue<float>;

namespace StacksandQueues
{
	TEST_CLASS(StackTest)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Stackf stack = Stackf();
			Assert::AreEqual(stack.GetSize(), 0, L"[Error] Stack should be empty!");



		}

		TEST_METHOD(GetTop)
		{

		}

		TEST_METHOD(Push)
		{
			Stackf stack = Stackf();
			
			int elementsToAdd = 10;
			for (size_t i = 0; i < elementsToAdd; i++)
			{
				stack.Push(3.0f);
			}
			Assert::AreEqual(stack.GetSize(), 10, L"[Error] Stack should have 10 elements pushed!");
		}




		TEST_METHOD(Pop)
		{
			Stackf stack = Stackf();

			float value = stack.Pop();
			Assert::AreEqual(value, float(), L"[Error] Empty stack didnt assert properly when the pop method was used.");


			for (size_t i = 0; i < 100; i++)
			{
				stack.Push(1.0f);
			}

			int sizePriorToPop = stack.GetSize();


			for (size_t i = 0; i < 20; i++)
			{
				stack.Pop();
			}


			Assert::AreNotEqual(stack.GetSize(), sizePriorToPop, L"[Error] Stack was not resized properly when the pop method was used!");
		}
	};
}
