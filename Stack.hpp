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
		Stack() 
		{
			myBuffer = std::vector<T>();
		}
		//Returnerar antal element i stacken
		int GetSize() const 
		{
			return static_cast<int>(myBuffer.size());
		}
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		const T& GetTop() const
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");
			return myBuffer.back();
		}
		//Returnerar det �versta elementet i stacken. Kraschar med en assert om
		//stacken �r tom.
		T& GetTop()
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");
			return  myBuffer.back();
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
			T val = myBuffer.back();
			myBuffer.pop_back();
			return val;
		}


	private:
		std::vector<T> myBuffer;
	};
}
