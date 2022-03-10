#pragma once


namespace CommonUtilities
{
	template <class T>
	struct Node
	{
		T myData;
		Node* myLeftNode;
		Node* myLeftRight;
		Node(T val)
		{
			myData = val;
			myLeftNode = { nullptr };
			myLeftRight = { nullptr };
		}

		const bool operator==(const T& aRhs)
		{
			return !(myData < aRhs) && !(aRhs < myData);
		}
		const bool operator != (const T& aRhs)
		{
			return !(myData == aRhs);
		}
		const bool operator <(const T& aRhs)
		{
			return myData < aRhs;
		}
		const bool operator >(const T& aRhs)
		{
			return myData > aRhs;
		}
		const bool operator <(const Node*& aRhs)
		{
			return myData < aRhs->myData;
		}
		const bool operator >(const Node*& aRhs)
		{
			return myData > aRhs->myData;
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
		~BinaryTree();
		//Return true if the element exist in the tree.s
		bool HasElement(const T& aValue) const
		{
			auto foundElement;
			return HasCurrentNodeElement(myHeadNode, aValue, foundElement);
		};
		//Inserts the element into the tree if it doesn't already exist otherwise does nothing.
		void Insert(const T& aValue)
		{
			auto foundNode;
			if (!HasCurrentNodeElement(myHeadNode, aValue, foundNode))
			{
				//The fuck is a scale???? - Spyro * your mom
				//ADD
				//1. Create a new node
				Node* newCurrentNode = new Node(aValue);
				//2. Find where to place it

				//3. Connect so the node above points to new node

			}
			else
			{
				std::cout << "Value already exist \n";
			}
		};
		//If the element exist in the tree removes it.
		void Remove(const T& aValue)
		{
			auto foundNode;
			if (HasCurrentNodeElement(myHeadNode, aValue, foundNode))
			{
				//DELETE
				//Find the node and call RemoveNode()
			}
			else
			{
				std::cout << "Value doesn't exist \n";
			}
		};
	private:
#pragma region RemoveLogic
		Node* myHeadNode;

		void RemoveNode(Node* aCurrentNode)
		{
			Node* leftChild = aCurrentNode->myLeftNode;
			Node* rightChild = aCurrentNode->myRightNode;
			delete aCurrentNode;

		}
#pragma endregion
		//Rename or move logic to "Insert" where we actually insert an element

		//Check is an element exist in the tree
		bool HasCurrentNodeElement(Node* aCurrentNode, const T& aValue, Node*& aFoundNode)
		{
			if (!aCurrentNode)
			{
				return false;
			}

			if (aCurrentNode < aValue && !aFoundNode)
			{
				return HasCurrentNodeElement(aCurrentNode->myRightNode, aValue, aFoundNode);
			}
			else if (aCurrentNode > aValue && !aFoundNode)
			{
				return HasCurrentNodeElement(aCurrentNode->myLeftNode, aValue, aFoundNode);
			}

			aFoundNode = aCurrentNode;
			return true;
		}
	};
}
