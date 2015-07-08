#pragma once

#include <cstdint>

namespace Library
{
	/**Singly Linked List Implementation*/
	template<typename T>
	class SList
	{
	private:
		/**Data structure to store data and the next node in the list*/
		class Node
		{
		public:
			/**Default parameterized constructor. data is of templated type T and the next pointer points to the next node in the list
			* @param data is the datatype T of the list. next points to the next node
			*/
			Node(const T& data, Node* next = nullptr);

			/**Getter for mNext
			* @return Pointer to the next node 
			*/
			Node* GetNext();

			/**Setter for mNext
			* @param nextNode The address of the next node.
			*/
			void SetNext(Node* nextNode);

			/**Getter for mData
			* @return data of templated datatype.
			*/
			T& GetData();

		private:
			/**No default constructor*/
			Node() = delete; 

			/**Copy constructor cannot exist*/
			Node(const Node& rhs) = delete; 

			/**Overloaded operation = cannot exist*/
			Node& operator=(const Node& rhs) = delete; 

			T mData;
			Node* mNext;
		};
	public:
		class Iterator
		{
			friend class SList;

		public:
			/**Default parameterless constructor*/
			Iterator();

			/**Copy constructor
			* @param Iterator object to copy from.
			*/
			Iterator(const Iterator& rhs);

			/**Overloaded operator=. Deep copies the data from rhs to this object
			* @param Iterator object to copy from.
			*/
			Iterator& operator=(const Iterator& rhs);

			/**Destructor. Sets the member to nullptr*/
			~Iterator();


			/**Dereference operator. Returns the data contained in the iterator object.*/
			T& operator*();

			/**Overloaded operator==. Checks whether the two iterator objects point to the same node and belong to the same owner.
			* @param Iterator object to compare with.
			*/
			bool operator==(const Iterator& rhs) const;

			/**Overloaded operator!=. Checks whether the two iterator objects do not point to the same node and do not belong to the same owner.
			* @param Iterator object to compare with.
			*/
			bool operator!=(const Iterator& rhs) const;

			/**Overloaded operator++. Does a pre increment to the iterator object. Returns the iterator object to the next node.*/
			Iterator& operator++();

			/**Overloaded operator++. Does a post increment to the iterator object. 
			* Returns the iterator object to the same node but points the current iterator to the next object.*/
			Iterator operator++(int);

		private:
			/**Parameterized constructor. 
			* @param owner stores the pointer to the list which the iterator is a part of. 
			* @param node stores the pointer to the node, the iterator is pointing to.
			*/
			Iterator(const SList* owner, Node* node);

			const SList* mOwner;
			Node* mNode;
		};

	public:

		/**Default constructor. creates an empty list*/
		SList();

		/**Copy constructor. copies the rhs list into this list. rhs remains unchanged
		* @param rhs The SList on the right hand side.
		*/
		SList(const SList& rhs);

		/**Overloaded = operator. calls copy constructor.
		* @param rhs The SList on the right hand side.
		*/
		SList& operator=(const SList& rhs);

		/**Destructor. calls Clear()*/
		~SList();

		/**Returns the number of elements in the list
		* @return The size in 32bit integer
		*/
		std::uint32_t Size() const;

		/**Checks whether list has no elements
		* @return true or false
		*/
		bool IsEmpty() const;

		/**Pushes data of type T to the front of the list. data remains unchanged
		* @param data Data of templated datatype to be pushed in the front of the list
		*/
		void PushFront(const T& data);

		/**Pushes data of type T to the back of the list. data remains unchanged
		* @param data Data of templated datatype to be pushed to the back of the list
		*/
		void PushBack(const T& data);

		/**Deletes the first node of the list. No data is returned*/
		void PopFront();

		/**Makes the list empty*/
		void Clear();

		/**Returns the data at the front of the list. No data in the list is changed
		* @return Data of datatype T at the front of the list.
		*/
		T& Front() const;

		/**Returns the data at the back of the list. No data in the list is changed
		* @return Data of datatype T at the back of the list.
		*/
		T& Back() const;

		/**Returns the iterator that contains the front node of the list
		* @return Iterator object containing the front node of the list.
		*/
		Iterator begin() const;

		/**Returns the iterator that contains nullptr
		* @return Iterator object containing nullptr.
		*/
		Iterator end() const;

		/**Finds data in the list
		* @return iterator object copy that contains the node containing the found object
		* @param templated data to look for in the list
		*/
		Iterator Find(const T& value) const;

		/**Inserts data after a given node.
		* @param iterator object that contains the node to insert after and the data to insert
		*/
		void SList<T>::InsertAfter(const Iterator& it, const T& value);

		/**Removes data from the list
		* @param data to delete
		*/
		void Remove(const T& value);

	private:
		Node* mFront;
		Node* mBack;

		std::uint32_t mSize;
	};
}



#include "SList.inl"