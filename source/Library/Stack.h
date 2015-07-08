#pragma once
#include "SList.h"

namespace Library
{
	/**Stack Implementation*/
	template<typename T>
	class Stack
	{
	public:

		/**Default constructor. creates an empty stack*/
		Stack();

		/**Copy constructor. copies the rhs Stack into this Stack. rhs remains unchanged
		* @param rhs The stack on the right hand side.
		*/
		Stack(const Stack& rhs);

		/**Overloaded = operator. calls copy constructor.
		* @param rhs The stack on the right hand side.
		*/
		Stack& operator=(const Stack& rhs);

		/**Destructor. calls Clear()*/
		~Stack();

		/**Returns the number of elements in the Stack
		* @return The size in 32bit integer
		*/
		std::uint32_t Size() const;

		/**Checks whether Stack has no elements
		* @return true or false
		*/
		bool IsEmpty() const;

		/**Pushes data of type T to the top of the Stack. data remains unchanged
		* @param data Data of templated datatype to be pushed in the top of the Stack
		*/
		void Push(const T& data);

		/**Deletes the top of the Stack. No data is returned*/
		void Pop();

		/**Makes the Stack empty*/
		void Clear();

		/**Returns the data at the front of the Stack. No data in the Stack is changed
		* @return Data of datatype T at the front of the Stack.
		*/
		T& Top();

		/**Returns the data at the front of the Stack. No data in the Stack is changed
		* @return Data of datatype T at the front of the Stack.
		*/
		const T& Top() const;

	private:
		SList<T> mStackList;
	};
}



#include "Stack.inl"