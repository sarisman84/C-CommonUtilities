#pragma once
#include <cassert>


namespace CommonUtilities
{
	template <typename Type, int size, typename
		CountType = unsigned short, bool UseSafeModeFlag = true>
		class VectorOnStack
	{
	public:
		VectorOnStack()
		{
			Clear();

			myCount = CountType();
		}
		VectorOnStack(const VectorOnStack& aVectorOnStack)
		{
			for (int i = 0; i < aVectorOnStack.Size(); i++)
			{
				myBuffer[i] = aVectorOnStack.myBuffer[i];
			}
			myCount = aVectorOnStack.myCount;
		}

		~VectorOnStack()
		{

			Clear();

			myCount = CountType();
		}

		VectorOnStack& operator=(const VectorOnStack&
								 aVectorOnStack)
		{
			for (int i = 0; i < aVectorOnStack.Size(); i++)
			{
				myBuffer[i] = aVectorOnStack.myBuffer[i];
			}
			myCount = aVectorOnStack.myCount;
			return *this;
		}

		inline const Type& operator[](const CountType aIndex) const
		{

			assert(aIndex < size&& aIndex >= 0 && myCount != CountType(), "Inputed index is out of bounds!");

			return myBuffer[aIndex];
		}
		inline Type& operator[](const CountType aIndex)
		{
			assert(aIndex < size&& aIndex >= 0 && myCount != CountType(), "Inputed index is out of bounds!");

			return myBuffer[aIndex];
		}

		inline void Add(const Type& aObject)
		{
			assert(myCount < size, "Vector is full!");
			myBuffer[myCount] = aObject;
			myCount++;

		}
		inline void Insert(const CountType aIndex, const Type& aObject)
		{

			assert(aIndex < size && aIndex >= 0, "Inputed index is out of bounds!");

			assert(myCount < size, "Vector is full!");

			for (int i = myCount; i >= 0; i--)
			{
				if (i >= aIndex)
				{
					myBuffer[i] = myBuffer[i - 1];
				}
				if (i == aIndex)
				{
					myBuffer[i] = aObject;
					myCount++;
					return;
				}
			}

		}
		inline void RemoveCyclic(const Type& aObject)
		{

			assert(myCount > 0, "The vector is empty!");

			for (int i = 0; i < myCount; i++)
			{
				if (myBuffer[i] == aObject)
				{
					myBuffer[i] = myBuffer[myCount - 1];
				}
			}

			myCount--;
		}
		inline void RemoveCyclicAtIndex(const CountType aIndex)
		{

			assert(aIndex < size&& aIndex >= 0, "Inputed index is out of bounds!");
			assert(myCount > 0, "The vector is empty!");

			for (int i = 0; i < myCount; i++)
			{
				if (i == aIndex)
				{
					myBuffer[i] = myBuffer[myCount - 1];
				}
			}

			myCount--;
		}

		inline void Clear()
		{
			for (int i = 0; i < size; i++)
			{
				myBuffer[i] = Type();
			}
			myCount = CountType();
		}
		__forceinline CountType Size() const
		{
			return myCount;
		}

	private:
		CountType myCount;
		Type myBuffer[size];
	};
}
