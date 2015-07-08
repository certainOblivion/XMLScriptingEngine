#pragma once
#include "RTTI.h"
#include "Datum.h"
#include "Hashmap.h"

namespace Library
{
	class Datum;
	template<typename TKey, typename TData, typename HashFunctor = DefaultHashFunctor<TKey>>
	class Hashmap;

	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		typedef std::pair<std::string, Datum> PairType;
		static const std::uint32_t DefaultSize;

		/**Default constructor. creates a hashmap and Vector of size size
		* @param size The size.
		*/
		explicit Scope(std::uint32_t size = DefaultSize);

		/**Destructor. calls Clear()*/
		virtual ~Scope();

		/**Makes the Hashmap empty*/
		virtual void Clear();

		/**Copy constructor. copies the rhs scope into this. rhs remains unchanged
		* @param rhs The scope on the right hand side.
		*/
		Scope(const Scope& rhs);

		/**Assignment operator. copies the rhs scope into this. rhs remains unchanged
		* @param rhs The scope on the right hand side.
		*/
		Scope& operator=(const Scope& rhs);

		/**Finds data in the table
		* @return datum pointer that contains the data containing the found object
		* @param string key to look for in the list
		*/
		Datum* Find(const std::string& key) const;

		/**Returns the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		* @return datum pointer that contains the data containing the found object
		* @param string key to look for in the list
		* @param scope to look in.
		*/
		Datum* Search(const std::string& key, Scope** foundScope = nullptr) const;

		/**Returns a reference to a Datum with the associated name
		* @return reference to a Datum with the associated name
		* @return type of the datum default to unknown
		* @param string key to add the datum
		*/
		Datum& Append(const std::string& key, const Datum::DatumType type = Datum::DatumType::UNKNOWN);

		/**Returns a reference to a Scope with the associated name
		* @return reference to a Scope with the associated name
		* @param string key to add the datum
		*/
		Scope& AppendScope(const std::string& key);

		/**Wraps Append, for syntactic convenience.
		* @return reference to a Datum with the associated name
		* @param string key to add the datum
		*/
		Datum& operator[](const std::string& key);

		/**Takes an unsigned integer and which returns a reference to a Datum at the given index
		* @return reference to a Datum with the associated index
		* @param int index to add the datum
		*/
		Datum& operator[](const std::uint32_t index);

		/**Returns the address of the Scope which contains this one
		* @return pointer to the parent scope
		*/
		Scope* GetParent();

		/**Takes the constant address of a Scope and returns the name of the corresponding member (i.e. the name of a table within this table)
		* @return name of the key
		* @param the constant address of a Scope
		*/
		std::string FindName(const Scope* scope) const;

		/**scan through all nested Scope objects, including all Scope members and all values of those members, 
		* to find a value that matches the address of the nested Scope being deleted and makes it nullptr
		*/
		void MakeOrphan();

		/**Places the given child into this table, with the given name, at the given index
		* @param childScope address of the child to adopt
		* @param childName name of the key
		* @param childIndex index of the child
		*/
		void Adopt(Scope& childScope, const std::string& childName, const std::uint32_t childIndex = std::numeric_limits<uint32_t>::max());

		/**Checks whether the two scopes have similar data
		* @return boolean is equal or not
		* @param rhs the scope to compare with
		*/
		bool operator==(const Scope& rhs) const;

		/**Checks whether the two scopes have dissimilar data
		* @return boolean is equal or not
		* @param rhs the scope to compare with
		*/
		bool operator!=(const Scope& rhs) const;

		/**Checks whether the two scopes have similar data
		* @return boolean name is found or not
		* @param scope the scope to compare with
		* @param foundName the output parameter with the name
		*/
		bool FindName(const Scope* scope, std::string& foundName) const;

		/**Returns the number of string datum pairs in the Scope
		* @return unsigned int size
		*/
		std::uint32_t Size();

		protected:
			Vector<PairType*> mOrderVector;
		private:

		Hashmap<std::string, Datum> mContainer;
		Scope* mParent;
		
	};
}