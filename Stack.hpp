#pragma once
#include <vector>
#include <assert.h>

namespace CommonUtilities 
{
	template <class T>
	class Stack
	{
	public:
		//Skapar en tom stack
		Stack() {
			myTop = T();
			myBuffer = std::vector<T>();
		}
		//Returnerar antal element i stacken
		int GetSize() const {
			return static_cast<int>(myBuffer.size());
		}
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		const T& GetTop() const
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");

			return myTop;
		}
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		T& GetTop()
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");
			if (myBuffer.empty()) return myTop;

			myTop = myBuffer[myBuffer.size() - 1];
			return myTop;
		}
		//L�gger in ett nytt element �verst p� stacken
		void Push(const T& aValue)
		{
			myBuffer.push_back(aValue);
		}
		//Tar bort det �versta elementet fr�n stacken och returnerar det. Kraschar
		//med en assert om stacken �r tom.
		T Pop()
		{
			assert(!myBuffer.empty() && "[Pop()] Stack is Empty!");
			if (myBuffer.empty()) return T();
			T val = myBuffer.back();
			myBuffer.pop_back();
			return val;
		}


	private:
		T myTop;
		std::vector<T> myBuffer;
	};
}
