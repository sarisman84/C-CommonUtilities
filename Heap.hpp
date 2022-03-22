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
	template <class T, HeapType aHeapType = HeapType::Max, bool useStdInstead = false>
	class Heap
	{
	public:
		Heap() = default;
		//returnerar antal element i heapen
		int GetSize() const
		{
			return static_cast<int>(myBuffer.size());
		}
		//lägger till elementet i heapen
		void Enqueue(const T& aElement)
		{
			myBuffer.push_back(aElement);


			BubbleUp(myBuffer.size() - 1);
		}
		//returnerar det första elementet i heapen
		const T& GetTop() const
		{
			return myBuffer.front();
		}
		//tar bort det första elementet ur heapen och returnerar det
		T Dequeue()
		{
			if (myBuffer.size() == 0) return T();

			T topElement = GetTop();

			myBuffer[0] = myBuffer.back();
			myBuffer.pop_back();
			BubbleDown(0);

			return topElement;
		}
	private:
		int myCurrentSize = 0;
		std::vector<T> myBuffer;

		void BubbleDown(int aIndex)
		{
			int lhs = LEFT_CHILD(aIndex);
			int rhs = RIGHT_CHILD(aIndex);
			int currentElement = aIndex;

			switch (aHeapType)
			{
			case CommonUtilities::HeapType::Max:
				if (!useStdInstead)
				{
					if (lhs < myBuffer.size() && myBuffer[lhs] > myBuffer[currentElement])
						currentElement = lhs;
					if (rhs < myBuffer.size() && myBuffer[rhs] > myBuffer[currentElement])
						currentElement = rhs;
				}

				else
				{
					if (lhs < myBuffer.size() && std::greater<>(myBuffer[lhs], myBuffer[currentElement]))
						currentElement = lhs;

					if (rhs < myBuffer.size() && std::greater<>(myBuffer[rhs], myBuffer[currentElement]))
						currentElement = rhs;
				}

				break;
			case CommonUtilities::HeapType::Min:
				if (!useStdInstead)
				{
					if (lhs < myBuffer.size() && myBuffer[lhs] < myBuffer[currentElement])
						currentElement = lhs;
					if (rhs < myBuffer.size() && myBuffer[rhs] < myBuffer[currentElement])
						currentElement = rhs;
				}
				else
				{
					if (lhs < myBuffer.size() && std::less<>(myBuffer[lhs], myBuffer[currentElement]))
						currentElement = lhs;
					if (rhs < myBuffer.size() && std::less<>(myBuffer[rhs], myBuffer[currentElement]))
						currentElement = rhs;
				}

				break;
			}
			if (currentElement != aIndex)
			{
				std::swap(myBuffer[aIndex], myBuffer[currentElement]);
				BubbleDown(currentElement);
			}

		}
		void BubbleUp(int aIndex)
		{
			int parentIndex = PARENT(aIndex);
			int currentElement = aIndex;

			switch (aHeapType)
			{
			case CommonUtilities::HeapType::Max:
				if (!useStdInstead)
					currentElement = parentIndex >= 0 && myBuffer[aIndex] > myBuffer[parentIndex] ? parentIndex : currentElement;
				else
					currentElement = parentIndex >= 0 && std::greater<>(myBuffer[aIndex], myBuffer[parentIndex]) ? parentIndex : currentElement;
				break;
			case CommonUtilities::HeapType::Min:
				if (!useStdInstead)
					currentElement = parentIndex >= 0 && myBuffer[aIndex] < myBuffer[parentIndex] ? parentIndex : currentElement;
				else
					currentElement = parentIndex >= 0 && std::less<>(myBuffer[aIndex], myBuffer[parentIndex]) ? parentIndex : currentElement;
				break;
			}

			if (currentElement != aIndex)
			{
				std::swap(myBuffer[aIndex], myBuffer[currentElement]);
				BubbleUp(currentElement);
			}
		}
	};
}