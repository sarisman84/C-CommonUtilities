#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include "Common\UtilityFunctions.hpp"
#include "BinaryTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using BSTSeti = CommonUtilities::BinaryTree<int>;

namespace Assignment4BinarySearchTree
{
	TEST_CLASS(BSTTests)
	{
	public:
		TEST_METHOD(Insert_HasElement)
		{
			BSTSeti tree;
			std::vector<int> vec;

			tree.Insert(6);
			Assert::IsTrue(tree.HasElement(6), L"The tree does not contain an element just inserted.");

			for (int i = 0; i < 50; ++i)
			{
				int number = TestUtility::GetRandomInt(-30, 30);
				tree.Insert(number);
				vec.push_back(number);
				Assert::IsTrue(tree.HasElement(number), L"The tree does not contain an element that was just inserted.");
			}

			for (int j = 0; j < vec.size(); ++j)
			{
				Assert::IsTrue(tree.HasElement(vec[j]), L"The tree does not contain an element that was previously inserted.");
			}
		}

		TEST_METHOD(Remove)
		{
			BSTSeti tree;
			std::vector<int> vec;

			for (int i = 0; i < 50; ++i)
			{
				int number = TestUtility::GetRandomInt(-300, 300);
				for (int j = 0; j < vec.size(); ++j)
				{
					if (vec[j] == number)
					{
						i--;
						continue;
					}
				}
				vec.push_back(number);
				tree.Insert(number);
			}

			for (int j = 0; j < vec.size(); ++j)
			{
				tree.Remove(vec[j]);
				Assert::IsFalse(tree.HasElement(vec[j]), L"The tree contains an element just deleted.");
			}

		}

		TEST_METHOD(InsertDuplicates)
		{
			BSTSeti tree;
			std::vector<int> vec;

			tree.Insert(5);
			Assert::IsTrue(tree.HasElement(5), L"The tree does not contain an element just inserted.");
			tree.Insert(5);
			tree.Remove(5);
			Assert::IsFalse(tree.HasElement(5), L"The tree contains an element just deleted.");

		}

		TEST_METHOD(RemoveRootSingleElement)
		{
			BSTSeti tree;
			std::vector<int> vec;

			tree.Insert(5);
			tree.Remove(5);
			Assert::IsFalse(tree.HasElement(5), L"The tree should be empty.");
			tree.Insert(5); // try to insert after emptying the tree
		}

		TEST_METHOD(RemoveLeaf)
		{
			BSTSeti tree;
			std::vector<int> vec{ 5, 2 , 1, 3, 4, 8, 7, 6, 10, 9 };
			for (int i : vec)
				tree.Insert(i);

			tree.Remove(4); // right leaf element
			vec.erase(vec.begin() + 4);
			Assert::IsFalse(tree.HasElement(4), L"Leaf element still exists after removal");

			for (int i = 0; i < vec.size(); ++i)
				Assert::IsTrue(tree.HasElement(vec[i]), L"Element missing after removing right leaf element.");

			tree.Remove(6); // left leaf element
			vec.erase(vec.begin() + 6);
			for (int i = 0; i < vec.size(); ++i)
				Assert::IsTrue(tree.HasElement(vec[i]), L"Element missing after removing left leaf element.");
		}

		TEST_METHOD(RemoveNodeWithOneChild)
		{
			BSTSeti tree;
			std::vector<int> vec{ 5, 2 , 1, 3, 4, 8, 7, 6, 10, 9, 20, 22, 21, 30 };
			for (int i : vec)
				tree.Insert(i);

			tree.Remove(20); // right node element with one child
			vec.erase(vec.begin() + 10);
			Assert::IsFalse(tree.HasElement(20), L"Element still exists after removal");

			for (int i = 0; i < vec.size(); ++i)
				Assert::IsTrue(tree.HasElement(vec[i]), L"Element missing after removing node with one child element.");
		}

		TEST_METHOD(RemoveNodeWithTwoChildren)
		{
			BSTSeti tree;
			std::vector<int> vec{ 5, 2 , 1, 3, 4, 8, 7, 6, 10, 9, 20, 22, 21, 30 };
			for (int i : vec)
				tree.Insert(i);

			tree.Remove(8); // node element with two children (not root)
			vec.erase(vec.begin() + 5);
			Assert::IsFalse(tree.HasElement(8), L"Element still exists after removal");

			for (int i = 0; i < vec.size(); ++i)
				Assert::IsTrue(tree.HasElement(vec[i]), L"Element missing after removing node with two children.");
		}

		TEST_METHOD(RemoveRootNodeWithTwoChildren)
		{
			BSTSeti tree;
			std::vector<int> vec{ 5, 2 , 1, 3, 4, 8, 7, 6, 10, 9, 20, 22, 21, 30 };
			for (int i : vec)
				tree.Insert(i);

			tree.Remove(5); // root element with two children 
			vec.erase(vec.begin());
			Assert::IsFalse(tree.HasElement(5), L"Element still exists after removal");

			for (int i = 0; i < vec.size(); ++i)
				Assert::IsTrue(tree.HasElement(vec[i]), L"Element missing after removing node with two children.");
		}

		TEST_METHOD(EpicRandomTest)
		{
			for (int testCount = 0; testCount < 300; testCount++)
			{
				BSTSeti tree;
				std::vector<int> vec;
				int treeSize = (rand() % 200) + 1;
				for (int elementCount = 0; elementCount < treeSize; elementCount++)
				{
					int newElement = rand() % 10000000;
					for (int doubleCheckIndex = 0; doubleCheckIndex < vec.size(); ++doubleCheckIndex)
					{
						if (vec[doubleCheckIndex] == newElement)
						{
							newElement = rand() % 10000000;
							doubleCheckIndex = 0;
						}
					}
					vec.push_back(newElement);
					tree.Insert(newElement);
				}

				for (int elementIndex = 0; elementIndex < vec.size(); elementIndex++)
				{
					Assert::IsTrue(tree.HasElement(vec[elementIndex]), L"Element is missing after insertion!");
				}

				for (int removalCount = rand() % (vec.size()); removalCount > 0; removalCount--)
				{
					int removalIndex = rand() % (vec.size());
					tree.Remove(vec[removalIndex]);
					vec.erase(vec.begin() + removalIndex);
					for (int elementIndex = 0; elementIndex < vec.size(); elementIndex++)
					{
						if (tree.HasElement(vec[elementIndex]) == false)
							int a = 10;
						Assert::IsTrue(tree.HasElement(vec[elementIndex]), L"Element is missing after deletion!");
					}
				}

			}
		}
	};
}