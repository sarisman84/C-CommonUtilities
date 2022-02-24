#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom kö
		Queue() {
			myBuffer = std::vector<T>();
		}
		//Returnerar antal element i kö
		int GetSize() const {
			return static_cast<int>(myBuffer.size());
		}
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		const T& GetFront() const
		{
			assert(!myBuffer.empty() && "[GetFront()] Queue is Empty!");



			return myFront;
		}
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		T& GetFront()
		{
			assert(!myBuffer.empty() && "[GetFront()] Queue is Empty!");
			if (myBuffer.empty()) return myFront;

			myFront = myBuffer.front();
			return myFront;
		}
		//Lägger in ett nytt element längst bak i kön
		void Enqueue(const T& aValue)
		{
			myBuffer.push_back(aValue);
		}
		//Tar bort elementet längst fram i kön och returnerar det. Kraschar med en
		//assert om kön är tom.
		T Dequeue()
		{
			assert(!myBuffer.empty() && "[Dequeue()] Queue is Empty!");
			if (myBuffer.empty()) return T();
			T val = myBuffer.front();
			myBuffer.erase(std::remove(myBuffer.begin(), myBuffer.end(), val), myBuffer.end());
			return val;
		}

	private:
		T myFront;
		std::vector<T> myBuffer;
	};
}
