#include "SList.h"
#include <exception>
namespace Library
{

#pragma region SList::Node
	template<typename T>
	SList<T>::Node::Node(const T& data, Node* next)
		:mData(data), mNext(next)
	{
	}

	template<typename T>
	typename SList<T>::Node* SList<T>::Node::GetNext()
	{
		return mNext;
	}

	template<typename T>
	void SList<T>::Node::SetNext(Node* nextNode)
	{
		mNext = nextNode;
	}

	template<typename T>
	typename T& SList<T>::Node::GetData()
	{
		return mData;
	}


#pragma endregion

#pragma region SList::Iterator


	template<typename T>
	SList<T>::Iterator::Iterator(const SList* owner, Node* node)
		: mOwner(owner), mNode(node)
	{
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNode != nullptr)
		{
			mNode = mNode->GetNext();
		}
		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename T>
	SList<T>::Iterator::~Iterator()
	{
		mOwner = nullptr;
		mNode = nullptr;
	}

	template<typename T>
	typename T& SList<T>::Iterator::operator*()
	{
		if (mNode)
		{
			return mNode->GetData();
		}
		else
			throw std::overflow_error("Bad pointer!");
	}

	template<typename T>
	typename bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return((mOwner == rhs.mOwner) && (mNode == rhs.mNode));
	}

	template<typename T>
	typename bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<typename T>
	SList<T>::Iterator::Iterator()
		:Iterator(nullptr, nullptr)
	{
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const Iterator& rhs)
		: Iterator()
	{
		operator=(rhs);
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& rhs)
	{
		if (*this != rhs)
		{
			mOwner = rhs.mOwner;
			mNode = rhs.mNode;
		}
		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(this, mFront);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(this, nullptr);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value) const
	{
		auto it = begin();
		for (; it != end(); ++it)
		{
			if (*it == value)
				break;
		}
		return it;
	}

	template<typename T>
	void SList<T>::InsertAfter(const Iterator& it, const T& value)
	{
		if (it.mOwner == this)
		{
			if ((mSize > 0) || (it.mNode->GetNext() != nullptr))
			{
				Node* nextNode = it.mNode->GetNext();
				Node* currentNode = it.mNode;

				Node* newNode = new Node(value);

				currentNode->SetNext(newNode);
				newNode->SetNext(nextNode);
				mSize++;
			}
			else
			{
				PushBack(value);
			}
		}
		else
		{
			throw std::exception("Invalid Owner!");
		}
	}

	template<typename T>
	void SList<T>::Remove(const T& value)
	{
		Node* prevNode;
		Node* currentNode;

		prevNode = mFront;
		currentNode = mFront;

		while (currentNode!=nullptr)
		{
			if (currentNode->GetData() == value)
			{
				if (currentNode == mFront)
				{
					PopFront();
					break;
				}
				else
				{

					if (currentNode == mBack)
					{
						mBack = prevNode;
					}

					prevNode->SetNext(currentNode->GetNext());
					mSize--;
					delete currentNode;
					break;
				}
			}
			else
			{
				prevNode = currentNode;
				currentNode = currentNode->GetNext();
			}
		}
		return;
	}

#pragma endregion

#pragma region SList
	template<typename T>
	bool SList<T>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template<typename T>
	std::uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	void SList<T>::PushFront(const T& data)
	{
		Node* newFrontNode = new Node(data,mFront);
		mFront = newFrontNode;
		if (mSize == 0)
		{
			mBack = newFrontNode;
		}
		mSize++;
	}

	template<typename T>
	void SList<T>::PushBack(const T& data)
	{
		Node* newBackNode = new Node(data);
		if (mSize > 0)
		{
			mBack->SetNext(newBackNode);
		}
		mBack = newBackNode;
		if (mSize == 0)
		{
			mFront = mBack;
		}
		mSize++;

	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (mFront)
		{
			Node* newFront = mFront->GetNext();
			delete mFront;
			mSize--;
			mFront = newFront;
			if (mSize == 0)
			{
				mBack = nullptr;
			}
		}
	}

	template<typename T>
	void SList<T>::Clear()
	{
		while (mSize>0)
		{
			PopFront();
		}
	}

	template<typename T>
	T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::overflow_error("List empty");
		}
		return mFront->GetData();
	}

	template<typename T>
	T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::overflow_error("List empty");
		}
		return mBack->GetData();
	}

	template<typename T>
	SList<T>::SList() //default constructor
		:mFront(nullptr), mBack(nullptr), mSize(0)
	{
	}

	template<typename T>
	SList<T>::SList(const SList& rhs)
		: SList()
	{
		operator=(rhs);
	}

	template<typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Node * currentNode = rhs.mFront;
			while(mSize<rhs.Size())
			{
				PushBack(currentNode->GetData());
				currentNode = currentNode->GetNext();
			}
		}
		return *this;
	}

#pragma endregion
}