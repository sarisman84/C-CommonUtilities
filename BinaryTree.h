#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>

#include <functional>

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringA(os_.str().c_str() );  \
}


namespace CommonUtilities
{
	template <class T>
	struct Node
	{
		T myData;
		Node* myLeftNode;
		Node* myRightNode;
		Node(T val)
		{
			myData = val;
			myLeftNode = { nullptr };
			myRightNode = { nullptr };
		}
	};

	template<class T>
	struct SearchResults
	{
		Node<T>* myTargetNode;
		Node<T>* myParentNode;
	};

	template <class T>
	struct ComparisonResults
	{
		SearchResults<T> mySearchResults;
		int myChildSide;
	};

	template <class T>
	class BinaryTree
	{
	public:
		BinaryTree()
		{
			myHeadNode = nullptr;
		}
		~BinaryTree()
		{
			if (myHeadNode)
			{
				myHeadNode->myLeftNode = nullptr;
				myHeadNode->myRightNode = nullptr;
				delete myHeadNode;
			}
			myHeadNode = nullptr;
		}
		//Return true if the element exist in the tree.s
		bool HasElement(const T& aValue) const
		{
			return Contains(myHeadNode, aValue);
		}
		//Inserts the element into the tree if it doesn't already exist, otherwise does nothing.
		void Insert(const T& aValue)
		{
			if (Contains(myHeadNode, aValue)) return;
			Insert(new Node<T>(aValue));
		};

		//If the element exist in the tree removes it.
		void Remove(const T& aValue)
		{
			if (!Contains(myHeadNode, aValue)) return;

			if (!myHeadNode) return;

			Node<T>* parent = nullptr;
			Node<T>* node = myHeadNode;
			bool isLeft = false;

			while (node->myData != aValue)
			{
				auto result = GetNextNode(node, aValue);
				node = result.mySearchResults.myTargetNode;
				parent = result.mySearchResults.myParentNode;
				isLeft = result.myChildSide == 1;
			}

			Node<T>* leftChild = node->myLeftNode;
			Node<T>* rightChild = node->myRightNode;
			if (parent)
				if (isLeft)
					parent->myLeftNode = nullptr;
				else
					parent->myRightNode = nullptr;
			if (node == myHeadNode)
			{
				delete node;
				myHeadNode = nullptr;
			}
			else
			{
				delete node;
			}

			if (leftChild)
				Insert(leftChild);
			if (rightChild)
				Insert(rightChild);
		};

	private:

		void Insert(Node<T>* aNode)
		{
			if (!aNode) return;
			if (!myHeadNode)
			{
				myHeadNode = aNode;
				return;
			}
			DBOUT("Value is not in tree! Inserting!");

			Node<T>* parentNode = nullptr;
			Node<T>* availableNode = myHeadNode;
			bool isLeft = false;

			while (availableNode)
			{
				ComparisonResults<T> results = GetNextNode(availableNode, aNode->myData);

				availableNode = results.mySearchResults.myTargetNode;
				parentNode = results.mySearchResults.myParentNode;

				isLeft = results.myChildSide == 1;

				DBOUT(availableNode ? "Still trying to find an empty slot" : "Found empty slot, exiting!");
			}

			if (parentNode)
			{
				if (isLeft)
					parentNode->myLeftNode = aNode;
				else
					parentNode->myRightNode = aNode;
			}
		}
		const bool Contains(Node<T>* aNode, const T& aValue) const
		{
			if (!aNode) return false;
			Node<T>* nextNode = GetNextNode(aNode, aValue).mySearchResults.myTargetNode;
			if (!nextNode) return false;
			if (nextNode == aNode) return true;
			return Contains(nextNode, aValue);
		}
		ComparisonResults<T> GetNextNode(Node<T>* aParentNode, const T& aValue) const
		{
			if (!aParentNode) return ComparisonResults<T>();

			bool isLeft = aParentNode->myData > aValue;
			bool isRight = aParentNode->myData < aValue;
			Node<T>* result = isLeft ? aParentNode->myLeftNode : isRight ? aParentNode->myRightNode : aParentNode;

			return ComparisonResults<T>{ SearchResults<T>{result, aParentNode}, isLeft ? 1 : isRight ? 2 : 0 };
		}
		Node<T>* myHeadNode;
	};
}
