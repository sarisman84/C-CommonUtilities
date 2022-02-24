#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom k�
		Queue() {
			myBuffer = std::vector<T>();
		}
		//Returnerar antal element i k�
		int GetSize() const {
			return static_cast<int>(myBuffer.size());
		}
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		const T& GetFront() const
		{
			assert(!myBuffer.empty() && "[GetFront()] Queue is Empty!");



			return myFront;
		}
		//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
		//tom
		T& GetFront()
		{
			assert(!myBuffer.empty() && "[GetFront()] Queue is Empty!");
			if (myBuffer.empty()) return myFront;

			myFront = myBuffer.front();
			return myFront;
		}
		//L�gger in ett nytt element l�ngst bak i k�n
		void Enqueue(const T& aValue)
		{
			myBuffer.push_back(aValue);
		}
		//Tar bort elementet l�ngst fram i k�n och returnerar det. Kraschar med en
		//assert om k�n �r tom.
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
