#pragma once
#include <cassert>
#include <vector>
#include <functional>
#include <array>
using namespace std;




namespace CommonUtilities
{
	template<typename T>
	class List
	{
	public:
		List()
		{

		}

		List(vector<T> aNewList)
		{
			myBuffer = aNewList;
		}

		List(initializer_list<T> anInitializedList)
		{
			myBuffer = anInitializedList;
		}

		T& operator[](const int& aIndex)
		{
			assert(aIndex >= 0 && aIndex < myBuffer.size() && myBuffer.size() != 0);
			return myBuffer[aIndex];
		}

		const T& operator[](const int& aIndex) const
		{
			assert(aIndex >= 0 && aIndex < myBuffer.size() && myBuffer.size() != 0);
			return myBuffer[aIndex];
		}


		_inline void Add(T anElementToAdd)
		{
			myBuffer.push_back(anElementToAdd);
		}

		_inline void AddRange(List<T> someElementsToAdd)
		{
			for (auto& element : someElementsToAdd)
			{
				Add(element);
			}

		}


		_inline void AddRange(initializer_list<T> anInitializedList)
		{
			for (auto& element : anInitializedList)
			{
				Add(element);
			}

		}

		_inline void Insert(const int anIndexToInsertTo, const T anElementToInsert)
		{
			myBuffer.insert(anIndexToInsertTo, anElementToInsert);
		}

		_inline void Remove(T anElementToRemove)
		{
			myBuffer.erase(remove(myBuffer.begin(), myBuffer.end(), anElementToRemove), myBuffer.end());
		}

		inline void RemoveIf(std::function<bool(T&)> aPredicate)
		{
			myBuffer.erase(std::remove_if(myBuffer.begin(), myBuffer.end(), aPredicate), myBuffer.end());
		}

		inline void RemoveIf(std::function<bool(T&)> aPredicate, std::function<void(T&)> anOnDeletePred)
		{
			auto copy = myBuffer;
			if (anOnDeletePred)
				for (int i = 0; i < copy.size(); i++)
				{
					if (aPredicate(copy[i]))
						anOnDeletePred(copy[i]);
				}
			myBuffer.erase(std::remove_if(myBuffer.begin(), myBuffer.end(), aPredicate), myBuffer.end());
		}


		_inline void RemoveRange(vector<T> someElementsToAdd)
		{
			for (auto& element : someElementsToAdd)
			{
				Remove(element);
			}

		}


		_inline void ForEach(std::function<void(T&)> aPredicate)
		{
			if (aPredicate == nullptr) return;
			for (auto& element : myBuffer)
			{
				aPredicate(element);
			}
		}


		_inline T* Find(function<bool(T&)>aPredicate)
		{
			for (int i = 0; i < myBuffer.size(); i++)
			{
				if (aPredicate != nullptr && aPredicate(myBuffer[i]))
				{
					return &myBuffer[i];
				}
			}

			return nullptr;
		}

		_inline const T* Find(function<bool(T&)>aPredicate) const
		{
			for (int i = 0; i < myBuffer.size(); i++)
			{
				if (aPredicate != nullptr && aPredicate(myBuffer[i]))
				{
					return myBuffer[i];
				}
			}
			return nullptr;
		}

		_inline vector<T*> FindAll(function<bool(T&)>aPredicate) const
		{
			vector<T*> result;
			for (int i = 0; i < myBuffer.size(); i++)
			{
				if (aPredicate != nullptr && aPredicate(myBuffer[i]))
				{
					result.push_back(&myBuffer[i]);
				}
			}
			return result;
		}



		auto begin()
		{
			return myBuffer.begin();
		}

		auto end()
		{
			return myBuffer.end();
		}


		__forceinline void Clear()
		{
			myBuffer.clear();
		}

		_inline const int Count()
		{
			return (int)myBuffer.size();
		}



	private:
		vector<T> myBuffer;




	};



	
}