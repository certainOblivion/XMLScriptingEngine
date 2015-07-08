#pragma once

#include "DefaultIncrement.h"

namespace Library
{
	/**Vector Implementation*/
	template<typename T, typename IncrementFunctor = DefaultIncrement>
	class Vector
	{
	public:
		static const uint32_t DefaultCapacity;
		class Iterator
		{
			friend class Vector;

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

			/**Overloaded operator++. Does a pre increment to the iterator object. Returns the iterator object to the next data.*/
			Iterator& operator++();

			/**Overloaded operator++. Does a post increment to the iterator object.
			* Returns the iterator object to the same node but points the current iterator to the next object.*/
			Iterator operator++(int);

		private:
			/**Parameterized constructor.
			* @param owner stores the pointer to the list which the iterator is a part of.
			* @param node stores the pointer to the node, the iterator is pointing to.
			*/
			Iterator(Vector* owner, uint32_t index);

			Vector* mOwner;
			uint32_t mIndex;
		};

		/**Default parameterless constructor*/
		Vector(uint32_t capacity = DefaultCapacity);

		/**Copy constructor
		*@param rhs The Vector to be copied*/
		Vector(const Vector& rhs);

		/**Overloaded operator=. Deep copies the data from rhs to this object
		* @param Vector object to copy from.
		*/
		Vector& operator=(const Vector& rhs);

		/**Destructor*/
		~Vector();

		/**Overloading operator[]. Should return a const reference to the item at this index.
		* @param index The index of the item
		*/
		T& operator[](uint32_t index) const;

		/**Overloading operator[]. Should return a reference to the item at this index.
		* @param index The index of the item
		*/
		T& operator[](uint32_t index);

		/**Removes the last item in the vector*/
		void PopBack();

		/**Returns true if list contains no items, otherwise false.*/
		bool IsEmpty();

		/**Returns the first item in the vector. Item cannot be changed*/
		T& Front() const;
		
		/**Returns the first item in the vector. Item can be changed*/
		T& Front();

		/**Returns the last item in the vector. Item cannot be changed*/
		T& Back() const;

		/**Returns the last item in the vector. Item can be changed*/
		T& Back();

		/**Returns the number of items in the vector.*/
		uint32_t Size() const;

		/**Returns the iterator that contains the front data of the vector
		* @return Iterator object containing the front data of the vector.
		*/
		Iterator begin() const;

		/**Returns the iterator that contains nullptr
		* @return Iterator object containing nullptr.
		*/
		Iterator end() const;

		/**Inserts the provided data at the end of the vector. May increase vector size.
		*@param data The data to be inserted.
		*/
		void PushBack(const T& data);

		/**Returns the reference to the item at index
		*@param index The index of the data to be returned.*/
		T& At(uint32_t index);

		/**Reserve size number of data chunks of the templated data in memory.
		*@param size The number of data chunks to reserve*/
		void Reserve(uint32_t size, bool fixedSize = false);

		/**Finds data in the list
		* @return iterator object copy that contains the node containing the found object
		* @param templated data to look for in the list
		*/
		Iterator Find(const T& item);

		/**Empty the vector. Makes size zero*/
		void Clear();

		/**Remove the item referred to in the iterator object
		* @param item The iterator object to remove
		*/
		void Remove(Iterator& item);

		/**Remove the items ranging from 'from' to 'to' in the vector
		* @param from The iterator object to remove from
		* @param to The end marker of the items to be removed 
		*/
		void Remove(Iterator& from,Iterator& to);

		private:
			uint32_t mCapacity;
			uint32_t mSize;
			T* mContainer;

			IncrementFunctor mIncrementFunctor;
	};
}
#include "Vector.inl"