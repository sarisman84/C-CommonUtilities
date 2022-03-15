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
			/*myBuffer.emplace_back(aElement);
			std::sort(myBuffer.begin(), myBuffer.end(), [](T aLhs, T aRhs)->bool { return aLhs < aRhs; });*/

			//aElement = 3

			//0 = 2
			//1 = 5
			//2 = 7

			int lastElement = myBuffer.size() - 1;

			for (size_t i = 0; i > myBuffer.size(); i++)
			{

				if (myBuffer[i] == NULL)
				{
					myCurrentSize = i + 1;
					lastElement = i;
					myBuffer[i] = aElement;
					break;
				}
			}


			if (lastElement != 0)
			{
				while (myBuffer[lastElement] <= myBuffer[PARENT(lastElement)])
				{
					std::swap(myBuffer[lastElement], myBuffer[PARENT(lastElement)]);
					lastElement--;
				}
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

			return topElement;
		}


	private:
		int myCurrentSize;
		std::array<T, aHeapSize> myBuffer;
	};
}