#pragma once
#include <array>
#include <algorithm>


#define LEFT_CHILD(i)  (2 * i) + 1
#define RIGHT_CHILD(i) (2 * i) + 2
#define PARENT(i) (i - 1) / 2
#define NULL T()

namespace CommonUtilities
{
	enum class HeapType
	{
		Max, Min
	};
	template <class T, unsigned short aHeapSize = 100, HeapType aHeapType = HeapType::Max>
	class Heap
	{
	public:
		Heap()
		{
			myBuffer = std::array<T, aHeapSize>();
		}


		//returnerar antal element i heapen
		int GetSize() const
		{
			return static_cast<int>(myBuffer.size());
		}

		//lägger till elementet i heapen
		void Enqueue(const T& aElement)
		{
			for (size_t i = 0; i < myBuffer.size(); i++)
			{

				if (myBuffer[i] == NULL)
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
		}

		//tar bort det första elementet ur heapen och returnerar det
		T Dequeue()
		{
			T topElement = GetTop();

			for (size_t i =  1; i < myCurrentSize; i++)
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
		std::array<T, aHeapSize> myBuffer;

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