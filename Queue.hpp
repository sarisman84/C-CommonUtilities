#pragma once
#include <vector>
#include <assert.h>

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
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		return myBuffer.front();
	}
	//Returnerar elementet l�ngst fram i k�n. Kraschar med en assert om k�n �r
	//tom
	T& GetFront()
	{
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		return myBuffer.front();
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
		assert(!myBuffer.empty(), "Called queue is empty! To the gulags with you.");
		T val = myBuffer.front();
		myBuffer.erase(std::remove(myBuffer.begin(), myBuffer.end(), val), myBuffer.end());
		return val;
	}

private:
	std::vector<T> myBuffer;
};