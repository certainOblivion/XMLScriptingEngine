#include "Stack.h"
#include <exception>
namespace Library
{
#pragma region Stack
	template<typename T>
	bool Stack<T>::IsEmpty() const
	{
		return (mStackList.Size() == 0);
	}

	template<typename T>
	std::uint32_t Stack<T>::Size() const
	{
		return mStackList.Size();
	}

	template<typename T>
	void Stack<T>::Push(const T& data)
	{
		mStackList.PushFront(data);
	}

	template<typename T>
	void Stack<T>::Pop()
	{
		mStackList.PopFront();
	}

	template<typename T>
	void Stack<T>::Clear()
	{
		mStackList.Clear();
	}

	template<typename T>
	T& Stack<T>::Top()
	{
		return mStackList.Front();
	}

	template<typename T>
	const T& Stack<T>::Top() const
	{
		return mStackList->Front();
	}

	template<typename T>
	Stack<T>::Stack() //default constructor
	{
	}

	template<typename T>
	Stack<T>::Stack(const Stack& rhs)
		:mStackList(rhs.mStackList)
	{
	}

	template<typename T>
	Stack<T>::~Stack()
	{
		mStackList.Clear();
	}

	template<typename T>
	Stack<T>& Stack<T>::operator=(const Stack& rhs)
	{
		if (mStackList != rhs.mStackList)
		{
			mStackList = rhs.mStackList;
		}
		return *this;
	}

#pragma endregion
}