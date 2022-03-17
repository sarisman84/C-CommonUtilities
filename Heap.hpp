#pragma once
#include <array>
#include <algorithm>


#define LEFT_CHILD(i)  (2 * i) + 1
#define RIGHT_CHILD(i) (2 * i) + 2
#define PARENT(i) (i - 1) / 2
#define NULL T()

namespace CommonUtilities
{
	template <class T, unsigned short aHeapSize = 100>
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
			int lastElement = myBuffer.size() - 1;

			for (size_t i = 0; i < myBuffer.size(); i++)
			{

				if (myBuffer[i] == NULL)
				{
					myCurrentSize = i + 1;
					lastElement = i;
					myBuffer[i] = aElement;
					break;
				}
			}

			while (lastElement != 0 && myBuffer[lastElement] <= myBuffer[PARENT(lastElement)])
			{
				std::swap(myBuffer[lastElement], myBuffer[PARENT(lastElement)]);
				lastElement = PARENT(lastElement);
			}
		}

		//returnerar det största elementet i heapen
		const T& GetTop() const
		{
			return myBuffer[myCurrentSize - 1];
		}

		//tar bort det största elementet ur heapen och returnerar det
		T Dequeue()
		{
			T topElement = GetTop();

			myBuffer[myCurrentSize - 1] = NULL;
			myCurrentSize--;
			BubbleDown();
			return topElement;
		}


	private:
		int myCurrentSize;
		std::array<T, aHeapSize> myBuffer;

		void BubbleDown(int index)
		{
			T biggest;
			if (myBuffer[LEFT_CHILD] > myBuffer[biggest])
			{
				biggest = myBuffer[LEFT_CHILD];
			}
			if (myBuffer[RIGHT_CHILD] > myBuffer[biggest])
			{
				biggest = myBuffer[RIGHT_CHILD];
			}
			if (biggest != myBuffer[index])
			{
				std::swap(myBuffer[index], myBuffer[biggest]);
				BubbleDown(biggest);
			}
		}
	};
}