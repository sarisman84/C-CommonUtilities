#pragma once
#include <vector>
#include <assert.h>

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
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		return myBuffer.front();
	}
	//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
	//tom
	T& GetFront()
	{
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		return myBuffer.front();
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
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		T val = myBuffer.front();
		myBuffer.erase(std::remove(myBuffer.begin(), myBuffer.end(), val), myBuffer.end());
		return val;
	}

private:
	std::vector<T> myBuffer;
};