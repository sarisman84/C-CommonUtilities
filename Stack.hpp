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
		//Returnerar det översta elementet i stacken. Kraschar med en assert om
		//stacken är tom.
		const T& GetTop() const
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");

			return myTop;
		}
		//Returnerar det översta elementet i stacken. Kraschar med en assert om
		//stacken är tom.
		T& GetTop()
		{
			assert(!myBuffer.empty() && "[GetTop()] Stack is Empty!");
			if (myBuffer.empty()) return myTop;

			myTop = myBuffer[myBuffer.size() - 1];
			return myTop;
		}
		//Lägger in ett nytt element överst på stacken
		void Push(const T& aValue)
		{
			myBuffer.push_back(aValue);
		}
		//Tar bort det översta elementet från stacken och returnerar det. Kraschar
		//med en assert om stacken är tom.
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
