#pragma once
#include <vector>
#include <functional>

namespace CommonUtilities
{
	namespace HashAlgorithm
	{
		unsigned int MurmurHash2(const void* key, int len, unsigned int seed)
		{
			// 'm' and 'r' are mixing constants generated offline.
			// They're not really 'magic', they just happen to work well.

			const unsigned int m = 0x5bd1e995;
			const int r = 24;

			// Initialize the hash to a 'random' value

			unsigned int h = seed ^ len;

			// Mix 4 bytes at a time into the hash

			const unsigned char* data = (const unsigned char*)key;

			while (len >= 4)
			{
				unsigned int k = *(unsigned int*)data;

				k *= m;
				k ^= k >> r;
				k *= m;

				h *= m;
				h ^= k;

				data += 4;
				len -= 4;
			}

			// Handle the last few bytes of the input array

			switch (len)
			{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
				h *= m;
			};

			// Do a few final mixes of the hash to ensure the last few
			// bytes are well-incorporated.

			h ^= h >> 13;
			h *= m;
			h ^= h >> 15;

			return h;
		}
	}
}

namespace CommonUtilities
{
	template<class Key>
	struct Hash
	{
		inline static const std::size_t Get(const Key& aKey)
		{
			return static_cast<size_t>(HashAlgorithm::MurmurHash2(reinterpret_cast<void*>(const_cast<Key*>(&aKey)), sizeof(aKey), 0));
		}
	};

	template<>
	struct Hash<std::string>
	{
		inline static const std::size_t Get(const std::string& aKey)
		{
			return std::hash<std::string>{}(aKey);
		}
	};

	template <class Key, class Value, const bool aSaveDataOnHeap = false>
	class HashMap
	{
	public:
		HashMap(int aCapacity) : myValueBuffer(aCapacity), myStateBuffer(aCapacity), myHashBuffer(aCapacity)
		{
			myCapacity = aCapacity;
		}
		bool Insert(const Key& aKey, const Value& aValue)
		{
			if (myCapacity <= 0) return false;
			//Fetch the information;
			size_t hashKey = Hash<Key>::Get(aKey);
			int index = hashKey % myCapacity;
			
			//If there is a collision, increment the index by one.
			int slotCheckCount = 0;
			while (IsIndexUsed(index) && myHashBuffer[index] != hashKey)
			{
				++index;

				index = index >= myCapacity ? 0 : index;
				++slotCheckCount;

				//If we checked every single slot, return false as none of the slots are available.
				if (slotCheckCount >= myCapacity)
				{
					return false;
				}

			}

			if (myStateBuffer[index] == EState::Empty)
			{
				myHashBuffer[index] = hashKey;
			}

			myValueBuffer[index] = aValue;
			myStateBuffer[index] = EState::InUse;

			return true;
		}
		bool Remove(const Key& aKey)
		{
			//Fetch the information;
			size_t hashKey = Hash<Key>::Get(aKey);
			int index = hashKey % myCapacity;

			int slotCheckCount = 0;
			while (!IsIndexUsed(index))
			{
				++index;

				index = index >= myCapacity ? 0 : index;
				++slotCheckCount;

				//If we checked every single slot, return false as the hash map is empty.
				if (slotCheckCount >= myCapacity)
				{
					return false;
				}

			}

			myValueBuffer[index] = Value();
			myStateBuffer[index] = EState::Removed;

			return true;
		}

		const Value* Get(const Key& aKey) const
		{
			//Fetch the information;
			size_t hashKey = Hash<Key>::Get(aKey);
			int index = hashKey % myCapacity;

			int slotCheckCount = 0;
			while (myHashBuffer[index] != hashKey)
			{
				++index;

				index = index >= myCapacity ? 0 : index;
				++slotCheckCount;

				//If we checked every single slot, return false as the hash map is empty.
				if (slotCheckCount >= myCapacity)
				{
					return nullptr;
				}
			}

			if (myStateBuffer[index] == EState::Empty || myStateBuffer[index] == EState::Removed) return nullptr;
			return &myValueBuffer[index];
		}

		Value* Get(const Key& aKey)
		{
			//Fetch the information;
			size_t hashKey = Hash<Key>::Get(aKey);
			int index = hashKey % myCapacity;

			int slotCheckCount = 0;
			while (myHashBuffer[index] != hashKey)
			{
				++index;

				index = index >= myCapacity ? 0 : index;
				++slotCheckCount;

				//If we checked every single slot, return false as the hash map is empty.
				if (slotCheckCount >= myCapacity)
				{
					return nullptr;
				}
			}

			if (myStateBuffer[index] == EState::Empty || myStateBuffer[index] == EState::Removed) return nullptr;
			return &myValueBuffer[index];
		}

	private:
		enum class EState : uint8_t
		{
			Empty, InUse, Removed
		};

		const bool IsIndexUsed(const int& anIndex)
		{
			return  (myStateBuffer[anIndex] == EState::InUse);
		}

		int myCapacity;

		std::vector<Value> myValueBuffer;
		std::vector<EState> myStateBuffer;
		std::vector<size_t> myHashBuffer;
	};
}