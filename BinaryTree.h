#pragma once


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





		Node* GetNextNode(const T& aValue)
		{
			return myData < aValue ? myLeftNode : myRightNode;
		}
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
				delete myHeadNode;
			myHeadNode = nullptr;
		}
		//Return true if the element exist in the tree.s
		bool HasElement(const T& aValue) const
		{
			return Contains(myHeadNode, aValue);
		}
		//Inserts the element into the tree if it doesn't already exist otherwise does nothing.
		void Insert(const T& aValue)
		{
			if (!Contains(myHeadNode, aValue))
			{
				Node<T>* parentNode;
				Node<T>* potentialSlot = GetChild(myHeadNode, aValue);
				while (potentialSlot)
				{

					Node<T>* slot = GetChild(potentialSlot, aValue);
					if (!slot)
					{
						parentNode = potentialSlot;
						potentialSlot = slot;
					}
				}

				SetChild(parentNode, new Node<T>(aValue), parentNode->myData < aValue ? 1 : 0);


			}

		};
		//If the element exist in the tree removes it.
		void Remove(const T& aValue)
		{
			if (Contains(myHeadNode, aValue))
			{
				Node<T>* parentNode;
				Node<T>* targetNodeToRemove = GetChild(myHeadNode, aValue);
				while (targetNodeToRemove->myData != aValue)
				{

					Node<T>* slot = GetChild(targetNodeToRemove, aValue);
					if (slot->myData == aValue)
					{
						parentNode = targetNodeToRemove;
						targetNodeToRemove = slot;
					}
				}

				Node<T>* leftChild = targetNodeToRemove->myLeftNode;
				Node<T>* rightChild = targetNodeToRemove->myRightNode;


				delete targetNodeToRemove;

				Insert(leftChild);
				Insert(rightChild);

			}
		};



	private:

		void Insert(Node<T>*& aNode)
		{
			if (!Contains(myHeadNode, aNode->myData))
			{
				Node<T>* parentNode;
				Node<T>* potentialSlot = GetChild(myHeadNode, aNode->myData);
				while (potentialSlot)
				{

					Node<T>* slot = GetChild(potentialSlot, aNode->myData);
					if (!slot)
					{
						parentNode = potentialSlot;
						potentialSlot = slot;
					}
				}

				SetChild(parentNode, aNode, (parentNode->myData < aNode->myData) ? 1 : 0);


			}

		};


		bool Contains(Node<T>* aNode, const T& aValue) const
		{
			if (!aNode) return false;
			if (aNode->myData == aValue) return true;

			if (Contains(GetChild(aNode, aValue), aValue))
			{
				return true;
			}

			return false;
		}

		Node<T>* GetChild(Node<T>* aParent, const T& aValue) const
		{
			return aParent->myData > aValue ? aParent->myRightNode : aParent->myLeftNode;
		}

		void SetChild(Node<T>* aParent, Node<T>* aChild, const int aSide = 0)
		{
			if (aSide <= 0)
				aParent->myLeftNode = aChild;
			else
				aParent->myRightNode = aChild;
		}





		Node<T>* myHeadNode;
	};
}
