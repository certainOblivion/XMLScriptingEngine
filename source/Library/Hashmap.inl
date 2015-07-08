#include "Hashmap.h"
#include "SList.h"
#include "Vector.h"
#include <cstring>

namespace Library
{
#pragma region DefaultHashFunctor

	template<typename T>
	const std::int32_t DefaultHashFunctor<T>::HashNumber = 65;

	template<typename T>
	uint32_t DefaultHashFunctor<T>::operator()(const T& key) const
	{
		uint32_t hash = 0;
		const byte* bytes = reinterpret_cast<const byte*>(&key);
		uint32_t size = sizeof(bytes);

		for (uint32_t i = 0; i < size; i++)
		{
			hash += bytes[i] * HashNumber;
		}
		return hash;
	}

	template<typename T>
	class DefaultHashFunctor<T*>
	{
	private:
		typedef unsigned char byte;
	public:
		static const std::int32_t HashNumber = 65;
		uint32_t operator()(const T* key) const
		{
			uint32_t hash = 0;
			const byte* bytes = reinterpret_cast<const byte*>(key);
			uint32_t size = sizeof(bytes);

			for (uint32_t i = 0; i < size; i++)
			{
				hash += bytes[i] * HashNumber;
			}
			return hash;
		}
	};

	template<>
	class DefaultHashFunctor<std::string>
	{
	private:
		typedef unsigned char byte;
	public:
		static const std::int32_t HashNumber = 65;
		uint32_t operator()(const std::string& key) const
		{
			uint32_t hash = 0;
			uint32_t size = key.length();

			for (uint32_t i = 0; i < size; i++)
			{
				hash += key[i] * HashNumber;
			}
			return hash;
		}
	};
#pragma endregion

#pragma region Hashmap::Iterator
	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator(Hashmap* owner, uint32_t bucketIndex,const ChainIterator& chainIterator)
		:mHashmapOwner(owner), mBucketIndex(bucketIndex), mChainIterator(chainIterator)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator()
		: mHashmapOwner(nullptr), mBucketIndex(0), mChainIterator()
	{
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator(const Iterator& rhs)
		:Iterator(rhs.mHashmapOwner,rhs.mBucketIndex,rhs.mChainIterator)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator& Hashmap<TKey, TData, HashFunctor>::Iterator::operator=(const Iterator& rhs)
	{
		if (*this != rhs)
		{
			mHashmapOwner = rhs.mHashmapOwner;
			mBucketIndex = rhs.mBucketIndex;
			mChainIterator = rhs.mChainIterator;
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::~Iterator()
	{
		mHashmapOwner = nullptr;
		mBucketIndex = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::PairType& Hashmap<TKey, TData, HashFunctor>::Iterator::operator*()
	{
		if (mHashmapOwner != nullptr)
		{
			return *mChainIterator;
		}
		else
		{
			throw std::exception("Iterator has no owner");
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::PairType* Hashmap<TKey, TData, HashFunctor>::Iterator::operator->() 
	{
		if (mHashmapOwner != nullptr)
		{
			return &(*mChainIterator);
		}
		else
		{
			throw std::exception("Iterator has no owner");
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	bool Hashmap<TKey, TData, HashFunctor>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mHashmapOwner == rhs.mHashmapOwner) && (mBucketIndex == rhs.mBucketIndex) && (mChainIterator == rhs.mChainIterator));
	}

	template<typename TKey, typename TData, typename HashFunctor>
	bool Hashmap<TKey, TData, HashFunctor>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator& Hashmap<TKey, TData, HashFunctor>::Iterator::operator++()
	{
		if (mHashmapOwner)
		{
			if (++mChainIterator != mHashmapOwner->mBucketVector[mBucketIndex].end())
			{
				return *this;
			}
			else
			{
				++mBucketIndex;
				while ((mBucketIndex < mHashmapOwner->mBucketVector.Size()) && (mHashmapOwner->mBucketVector[mBucketIndex].Size() == 0))
				{
					++mBucketIndex;
				}
				if (mBucketIndex >= mHashmapOwner->mBucketVector.Size())
				{
					ChainIterator endIterator;
					mChainIterator = endIterator;
				}
				else
				{
					mChainIterator = mHashmapOwner->mBucketVector[mBucketIndex].begin();
				}
				return *this;				
			}
		}
		else
		{
			throw std::exception("Bad owner!");
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Iterator::operator++(int)
	{
		Iterator it = this;
		operator++();
		return it;
	}
#pragma endregion

#pragma region Hashmap
	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Hashmap(std::uint32_t size)
		:mSize(0), mBucketVector(size)
	{
		mBucketVector.At(size - 1);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Hashmap(const Hashmap& rhs)
		:mBucketVector(rhs.mBucketVector), mHashFunctor(rhs.mHashFunctor), mSize(rhs.mSize)
	{
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::~Hashmap()
	{
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>& Hashmap<TKey, TData, HashFunctor>::operator=(const Hashmap& rhs)
	{
		if (this != &rhs)
		{
			BucketType newBucket(rhs.mBucketVector);
			mBucketVector = newBucket;
			mHashFunctor = rhs.mHashFunctor;
			mSize = rhs.mSize;
		}

		return *this;
	}


	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Find(const TKey& key) const
	{
		uint32_t index = mHashFunctor(key) % std::max<std::uint32_t>(mBucketVector.Size(),1);
		auto it = mBucketVector[index].begin();
		for (; it != mBucketVector[index].end(); ++it)
		{
			PairType& pair = *it;

			if (pair.first == key)
			{
				break;
			}
		}

		if (it != mBucketVector[index].end())
		{
			return Iterator(const_cast<Hashmap<TKey, TData, HashFunctor>*>(this), index, it);
		}
		else
		{
			return end();
		}

	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const TKey& entryKey, const TData& entryData, bool& isInserted)
	{
		if (ContainsKey(entryKey))
		{
			isInserted = false;
		}
		else
		{
			isInserted = true;
		}

		return Insert(entryKey, entryData);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const PairType& entry)
	{
		return Insert(entry.first, entry.second);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const TKey& entryKey, const TData& entryData)
	{
		Iterator returnValue;
		Iterator it = Find(entryKey);
		if (it != end())
		{
			returnValue = it;
		}
		else
		{
			PairType entryPair = PairType(entryKey, entryData);
			uint32_t index = mHashFunctor(entryKey) % std::max<std::uint32_t>(mBucketVector.Size(), 1);
			mBucketVector[index].PushFront(entryPair);
			returnValue = Iterator(this, index, (mBucketVector[index]).begin());
			mSize++;
		}
		return returnValue;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	TData& Hashmap<TKey, TData, HashFunctor>::operator[](const TKey& key)
	{
		TData data;
		auto it = Insert(key, data);

		return (*it).second;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	void Hashmap<TKey, TData, HashFunctor>::Remove(const TKey& key)
	{
		auto it = Find(key);
		if (it != end())
		{
			uint32_t index = mHashFunctor(key) % std::max<std::uint32_t>(mBucketVector.Size(), 1);
			mBucketVector[index].Remove(*it);
			mSize--;
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	void Hashmap<TKey, TData, HashFunctor>::Clear()
	{
		for (uint32_t i = 0; i < mBucketVector.Size();++i)
		{
			mBucketVector[i].Clear();
		}
		mSize = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	std::uint32_t Hashmap<TKey, TData, HashFunctor>::Size() const
	{
		return mSize;
	}
	
	template<typename TKey, typename TData, typename HashFunctor>
	bool Hashmap<TKey, TData, HashFunctor>::ContainsKey(const TKey key) const
	{
		return (Find(key) != end());
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::begin() const
	{
		Iterator beginIterator = end();
		uint32_t i = 0;
		for (auto it = mBucketVector.begin(); it != mBucketVector.end(); ++it)
		{
			if ((*it).Size() > 0)
			{
				beginIterator = Iterator(const_cast<Hashmap<TKey, TData, HashFunctor>*>(this), i, (*it).begin());
				break;
			}
			i++;
		}
		return beginIterator;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::end() const
	{
		ChainIterator endIterator;
		return Iterator(const_cast<Hashmap<TKey, TData, HashFunctor>*>(this), mBucketVector.Size(), endIterator);
	}

#pragma endregion
}