#pragma once
#include "SList.h"
#include "Vector.h"
#include <utility>

namespace Library
{
	template<typename T>
	class DefaultHashFunctor
	{
	private:
		typedef unsigned char byte;
	public:
		static const std::int32_t HashNumber;
		uint32_t operator()(const T& key) const;
	};

	/**Templated Hashmap Implementation*/
	template<typename TKey, typename TData, typename HashFunctor = DefaultHashFunctor<TKey>>
	class Hashmap
	{
	private:
		typedef std::pair<TKey, TData> PairType;
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;
		typedef typename ChainType::Iterator ChainIterator;


	public:
		class Iterator
		{
			friend class Hashmap;

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
			PairType& operator*();


			PairType* operator->();

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
			Iterator(Hashmap* owner, uint32_t bucketIndex, const ChainIterator& chainIterator);

			Hashmap* mHashmapOwner;
			uint32_t mBucketIndex;
			ChainIterator mChainIterator;
		};
		/**Default constructor. creates a hashmap of size size
		* @param size The stack on the right hand side.
		*/
		explicit Hashmap(std::uint32_t size = 7);

		/**Copy constructor. copies the rhs hashmap into this. rhs remains unchanged
		* @param rhs The hashmap on the right hand side.
		*/
		Hashmap(const Hashmap& rhs);

		/**Destructor. calls Clear()*/
		~Hashmap();

		/**Finds data in the hashmap
		* @return iterator object copy that contains the node containing the found object
		* @param templated key to look for in the list
		*/
		Iterator Find(const TKey& key) const;

		/**Inserts data using the key in the entry.
		* @param entry object that contains the key value pair to insert 
		* @return iterator object copy that contains the node containing the found object
		*/
		Iterator Insert(const PairType& entry);

		/**Inserts data using the key in the entry.
		* @param entryKey object that contains the key data to insert
		* @param entryData object that contains the value data to insert
		* @return iterator object copy that contains the node containing the found object
		*/
		Iterator Insert(const TKey& entryKey, const TData& entryData);

		/**Inserts data using the key in the entry.
		* @param entryKey object that contains the key data to insert
		* @param entryData object that contains the value data to insert
		* @param isInserted is set to true if the data is inserted successfully.
		* @return iterator object copy that contains the node containing the found object
		*/
		Iterator Insert(const TKey& entryKey, const TData& entryData, bool& isInserted);

		/**Overloading operator[]. Should return a reference to the item using the key.
		* @param key The key of the item
		*/
		TData& operator[](const TKey& key);


		/**Assignment operator. copies the rhs hashmap into this. rhs remains unchanged
		* @param rhs The hashmap on the right hand side.
		*/
		Hashmap& operator=(const Hashmap& rhs);

		/**Removes data associated with the key from the hashmap
		* @param key to delete
		*/
		void Remove(const TKey& key);

		/**Makes the Hashmap empty*/
		void Clear();

		/**Returns the number of elements in the Stack
		* @return The size in 32bit integer
		*/
		std::uint32_t Size() const;

		bool ContainsKey(const TKey key) const;

		/**Returns the iterator that contains the front data of the vector. Walk the vector until a SList is found where size > 0
		* @return Iterator object containing the front data of the vector.
		*/
		Iterator begin() const; 

		/**Returns the iterator that contains nullptr
		* @return Iterator object containing nullptr.
		*/
		Iterator end() const;

	private:
		uint32_t mSize;
		BucketType mBucketVector;
		HashFunctor mHashFunctor;
	};
}

#include "Hashmap.inl"