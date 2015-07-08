#include "Vector.h"
#include <algorithm>
#include <exception>

namespace Library
{

#pragma region Vector

	template<typename T, typename IncrementFunctor>
	const uint32_t Vector<T, IncrementFunctor>::DefaultCapacity = 3;

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Vector(uint32_t capacity)
		:mCapacity(0), mSize(0), mContainer(nullptr)
	{
		Reserve(capacity);
	}

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Vector(const Vector& rhs)
		: Vector()
	{
		operator=(rhs);
	}

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>& Vector<T, IncrementFunctor>::operator=(const Vector& rhs)
 	{
		if (this != &rhs)
		{
			Clear();
			auto it = rhs.begin();
			for (; it != rhs.end(); ++it)
			{
				PushBack(*it);
			}
		}
		return *this;
	}

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::~Vector()
	{
		Clear();
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::operator[](uint32_t index) const
	{
		if (index < mSize)
		{
			return mContainer[index];
		}
		else
		{
			throw std::overflow_error("Vector Index Out Of Bounds.");
		}
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::operator[](uint32_t index)
	{
		if (index < mSize)
		{
			return mContainer[index];
		}
		else
		{
			throw std::overflow_error("Vector Index Out Of Bounds.");
		}
	}

	template<typename T,typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::PopBack()
	{
		if (mSize > 0)
		{
			mSize--;
		}
	}

	template<typename T,typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::IsEmpty()
	{
		return (mSize == 0);
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Front() const
	{
		if (mSize > 0)
		{
			return operator[](0);
		}
		else
		{
			throw std::exception("Vector has no elements");
		}
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Front()
	{
		if (mSize > 0)
		{
			return operator[](0);
		}
		else
		{
			throw std::exception("Vector has no elements");
		}
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Back() const
	{
		if (mSize > 0)
		{
			return operator[](mSize - 1);
		}
		else
		{
			throw std::exception("Vector has no elements");
		}
	}


	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Back()
	{
		if (mSize > 0)
		{
			return operator[](mSize - 1);
		}
		else
		{
			throw std::exception("Vector has no elements");
		}
	}

	template<typename T,typename IncrementFunctor>
	uint32_t Vector<T, IncrementFunctor>::Size() const
	{
		return mSize;
	}

	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::begin() const
	{
		return Iterator(const_cast<Vector<T, IncrementFunctor>*>(this), 0);
	}


	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::end() const
	{
		return Iterator(const_cast<Vector<T, IncrementFunctor>*>(this), mSize);
	}

	template<typename T,typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::PushBack(const T& data)
	{
		if (mCapacity == 0)
		{
			Reserve(DefaultCapacity);
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = mCapacity + std::max<uint32_t>(1, mIncrementFunctor(mSize, mCapacity));
			Reserve(capacity);
		}
		mContainer[mSize] = data;
		mSize++;
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::At(uint32_t index)
	{
		if ((index < mCapacity) && (index >= mSize))
		{
			mSize = index + 1;
		}
		else if (index >= mCapacity)
		{
			throw std::overflow_error("Index is greater than vector capacity.");
		}
		return operator[](index);
	}

	template<typename T,typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Reserve(uint32_t sizeToReserve, bool fixedSize)
	{
		if (sizeToReserve > mCapacity)
		{
			T* newMemory = new T[sizeToReserve];
			if (mSize > 0)
			{
				int i = 0;
				for (auto it = begin(); it != end(); ++it, ++i)
				{
					newMemory[i] = *it;
				}
			}
			delete[] mContainer;
			mContainer = newMemory;
			mCapacity = sizeToReserve;

			if (fixedSize)
			{
				At(sizeToReserve - 1);
			}
		}
	}

	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::Find(const T& item)
	{
		auto it = begin();
		for (; it != end(); ++it)
		{
			if (*it == item)
			{
				break;
			}
		}
		return it;
	}

	template<typename T,typename IncrementFunctor>
	void Vector<T, IncrementFunctor>::Clear()
	{
		delete[] mContainer;
		mSize = 0;
		mContainer = nullptr;
		mCapacity = 0;
	}

#pragma endregion

#pragma region Vector::Iterator

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::Iterator(Vector* owner, uint32_t index)
		: mOwner(owner), mIndex(index)
	{
	}
	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::Iterator()
		: mOwner(nullptr), mIndex(0)
	{
	}

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::Iterator(const Iterator& rhs)
		: Iterator()
	{
		operator=(rhs);
	}

	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator& Vector<T, IncrementFunctor>::Iterator::operator=(const Iterator& rhs)
	{
		if (*this != rhs)
		{
			mOwner = rhs.mOwner;
			mIndex = rhs.mIndex;
		}
		return *this;
	}

	template<typename T,typename IncrementFunctor>
	Vector<T, IncrementFunctor>::Iterator::~Iterator()
	{
		mOwner = nullptr;
		mIndex = 0;
	}

	template<typename T,typename IncrementFunctor>
	T& Vector<T, IncrementFunctor>::Iterator::operator *()
	{
		if (mOwner)
		{
			return (*mOwner)[mIndex];
		}
		else
		{
			throw std::exception("Iterator has no owner.");
		}
	}

	template<typename T,typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mOwner == rhs.mOwner) && (mIndex == rhs.mIndex));
	}

	template<typename T,typename IncrementFunctor>
	bool Vector<T, IncrementFunctor>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !operator==(rhs);
	}

	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator& Vector<T, IncrementFunctor>::Iterator::operator ++ ()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Bad owner!");
		}

		if (mIndex >= mOwner->mSize)
		{
			throw std::exception("Size overflow");
		}
		mIndex++;
		return *this;
	}

	template<typename T,typename IncrementFunctor>
	typename Vector<T, IncrementFunctor>::Iterator Vector<T, IncrementFunctor>::Iterator::operator ++ (int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}
#pragma endregion


}