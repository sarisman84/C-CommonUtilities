#pragma once
#include <array>
#include <algorithm>
#define LEFT_CHILD(i)  (2 * i) + 1
#define RIGHT_CHILD(i) (2 * i) + 2
#define PARENT(i) (i - 1) / 2

namespace CommonUtilities
{
	enum class HeapType
	{
		Max, Min
	};
	template <class T, HeapType aHeapType = HeapType::Max>
	class Heap
	{
	public:
		Heap()
		{
			myBuffer = std::vector<T>();
		}
		//returnerar antal element i heapen
		int GetSize() const
		{
			if (myCurrentSize > 0)
			{
				return static_cast<int>(myBuffer.size());
			}
			else
			{
				return 0;
			}
		}

		//lägger till elementet i heapen
		void Enqueue(const T& aElement)
		{
			for (size_t i = 0; i < myBuffer.size(); i++)
			{
				if (myBuffer[i] == T())
				{
					myCurrentSize = i + 1;
					myBuffer[i] = aElement;
					break;
				}
			}
			switch (aHeapType)
			{
			case CommonUtilities::HeapType::Max:
				BubbleUp(myCurrentSize - 1);
				break;
			case CommonUtilities::HeapType::Min:
				BubbleDown(0);
				break;
			}
		}

		//returnerar det första elementet i heapen
		const T& GetTop() const
		{
			return myBuffer[0];
			//sda
		}

		//tar bort det första elementet ur heapen och returnerar det
		T Dequeue()
		{
			T topElement = GetTop();
			for (size_t i = 1; i < myCurrentSize; i++)
			{
				myBuffer[i - 1] = myBuffer[i];
			}
			myCurrentSize--;
			switch (aHeapType)
			{
			case CommonUtilities::HeapType::Max:
				BubbleDown(0);
				break;
			case CommonUtilities::HeapType::Min:
				BubbleUp(myCurrentSize - 1);
				break;
			}
			return topElement;
		}
	private:
		int myCurrentSize;
		std::vector<T> myBuffer;

		void BubbleDown(int aIndex)
		{
			int lhs = LEFT_CHILD(aIndex);
			int rhs = RIGHT_CHILD(aIndex);
			int biggest = aIndex;

			biggest = lhs < myCurrentSize&& myBuffer[lhs] > myBuffer[aIndex] ? lhs : rhs < myCurrentSize&& myBuffer[rhs] > myBuffer[biggest] ? rhs : biggest;

			if (biggest != aIndex)
			{
				std::swap(myBuffer[aIndex], myBuffer[biggest]);
				BubbleDown(biggest);
			}
		}
		void BubbleUp(int aIndex)
		{
			int parentIndex = PARENT(aIndex);
			int biggest = aIndex;

			biggest = parentIndex >= 0 && myBuffer[aIndex] > myBuffer[parentIndex];

			if (biggest != aIndex)
			{
				std::swap(myBuffer[aIndex], myBuffer[biggest]);
				BubbleUp(biggest);
			}
		}
	};
}