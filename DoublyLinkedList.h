#pragma once

namespace CommonUtilities {

	template <class T>
	class DoublyLinkedList;

	template <class T>
	class DoublyLinkedListNode
	{
	public:
		// Copy-konstruktorn och assignment-operatorn är borttagna, så att det enda
		// sättet att skapa en nod är genom att stoppa in ett värde i en lista.
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) =
			delete;
		// Returnerar nodens värde
		const T& GetValue() const 
		{
			return myData;
		}
		T& GetValue() 
		{
			return myData;
		}
		// Returnerar nästa nod i listan, eller nullptr om noden är sist i listan
		DoublyLinkedListNode<T>* GetNext() const 
		{
			return myNext;
		}
		// Returnerar föregående nod i listan, eller nullptr om noden är först i
		// listan
		DoublyLinkedListNode<T>* GetPrevious() const 
		{
			return myPrev;
		}
	private:
		// Konstruktorn och destruktorn är privat, så att man inte kan skapa eller
		// ta bort noder utifrån. List-klassen är friend, så att den kan skapa
		// eller ta bort noder.
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue) {
			myData = aValue;
			myNext = nullptr;
			myPrev = nullptr;
		}
		~DoublyLinkedListNode()
		{
			myNext = nullptr;
			myPrev = nullptr;
		}

		DoublyLinkedListNode<T>* myNext, * myPrev;
		T myData;
	};



	template <class T>
	class DoublyLinkedList
	{
	public:
		// Skapar en tom lista
		DoublyLinkedList() {
			myHead = nullptr;
			myTail = nullptr;
		}
		// Frigör allt minne som listan allokerat
		~DoublyLinkedList() {
			auto head = myHead;
			while (head) 
			{
				auto nextVal = head->myNext;
				head->myNext = nullptr;
				head->myPrev = nullptr;
				delete head;
				head = nextVal;

			}

			myHead = nullptr;
			myTail = nullptr;
		}
		// Returnerar antalet element i listan
		int GetSize() const {
			int size = 0;
			auto head = myHead;
			while (head) {
				size++;
				head = head->myNext;
			}

			return size;
		}
		// Returnerar första noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetFirst() {
			return myHead;
		}
		// Returnerar sista noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetLast() {
			return myTail;
		}
		// Skjuter in ett nytt element först i listan
		void InsertFirst(const T& aValue) {
			DoublyLinkedListNode<T>* newHead = new DoublyLinkedListNode<T>(aValue);
			newHead->myData = aValue;

			if (!myHead || !myTail) {
				if (!myHead) {
					myHead = newHead;

				}

				if (!myTail) {
					myTail = newHead;

				}
				return;
			}


			myHead->myPrev = newHead;
			newHead->myNext = myHead;

			myHead = newHead;
		}
		// Skjuter in ett nytt element sist i listan
		void InsertLast(const T& aValue) {
			DoublyLinkedListNode<T>* newTail = new DoublyLinkedListNode<T>(aValue);

			if (!myHead || !myTail) {
				if (!myHead) {
					myHead = newTail;

				}

				if (!myTail) {
					myTail = newTail;

				}
				return;
			}



			myTail->myNext = newTail;
			newTail->myPrev = myTail;

			myTail = newTail;
		}
		// Skjuter in ett nytt element innan aNode
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue) {
			DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

			if (GetSize() == 0)
			{
				myHead = aNode;
				return;
			}

			newNode->myNext = aNode;
			newNode->myPrev = aNode->myPrev;
			if (aNode->myPrev)
				aNode->myPrev->myNext = newNode;
			aNode->myPrev = newNode;

			if (aNode == myHead)
				myHead = newNode;
		}
		// Skjuter in ett nytt element efter aNode
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue) {
			DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);


			if (GetSize() == 0)
			{
				myTail = aNode;
				return;
			}


			newNode->myPrev = aNode;
			newNode->myNext = aNode->myNext;
			if (aNode->myNext)
				aNode->myNext->myPrev = newNode;
			aNode->myNext = newNode;

			if (aNode == myTail)
				myTail = newNode;
		}
		// Plockar bort noden ur listan och frigör minne. (Det är ok att anta att
		// aNode är en nod i listan, och inte från en annan lista)
		void Remove(DoublyLinkedListNode<T>* aNode) {
			auto val = myHead;
			while (val) {


				if (val == aNode) {
					OnRemoveNode(val);
					break;
				}


				val = val->myNext;

			}
		}
		// Hittar första elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindFirst(const T& aValue) {
			auto val = myHead;
			while (val) {

				if (val->myData == aValue) return val;

				val = val->myNext;

			}

			return nullptr;
		}
		// Hittar sista elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindLast(const T& aValue) {
			auto val = myTail;
			while (val) {

				if (val->myData == aValue) return val;

				val = val->myPrev;

			}

			return nullptr;
		}
		// Plockar bort första elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveFirst(const T& aValue) {
			auto val = myHead;
			while (val) {

				if (val->myData == aValue) {
					OnRemoveNode(val);
					return true;
				}

				val = val->myNext;

			}

			return false;
		}
		// Plockar bort sista elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveLast(const T& aValue) {
			auto val = myTail;
			while (val) {

				if (val->myData == aValue) {
					OnRemoveNode(val);
					return true;
				}

				val = val->myPrev;

			}

			return false;
		}

	private:


		void OnRemoveNode(DoublyLinkedListNode<T>* aNode) {
			if (!aNode) return;

			auto nextVal = aNode->myNext;
			auto prevVal = aNode->myPrev;

			if (prevVal)
			{
				prevVal->myNext = nextVal;
			}

			if (nextVal)
			{
				nextVal->myPrev = prevVal;
			}


			myHead = aNode == myHead ? aNode->myNext : myHead;
			myTail = aNode == myTail ? aNode->myPrev : myTail;


			delete aNode;
		}

		DoublyLinkedListNode<T>* myHead, * myTail;
	};



}
