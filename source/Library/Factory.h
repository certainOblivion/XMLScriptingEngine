#pragma once
#include <string>
#include "Hashmap.h"

namespace Library
{
	template<class T>
	class Factory
	{
	public:
		/**Given a class name (string), return the associated concrete factory.
		* @return Pointer to the found Factory class. Nullptr if not found
		* @param string class name 
		*/
		static Factory<T>* Find(const std::string& className);

		/**Given a class name (string), return a new object of that type
		* @return Pointer to the created Factory class
		* @param string class name
		*/
		static T* Create(const std::string& className);

		/**Returns an iterator for the hashmap begin
		* @return Hashmap Iterator*/
		static typename Hashmap<std::string, Factory<T>*>::Iterator begin();

		/**Return an iterator for the hashmap end
		* @return Hashmap Iterator*/
		static typename Hashmap<std::string, Factory<T>*>::Iterator end();

		/**Returns the address of a concrete product associated with this concrete factory class
		* @return The pointer to the abstract class*/
		virtual T* Create() = 0;

		/**Returns a string representing the name of the class the factory instantiates
		* @return string class name*/
		virtual const std::string& ClassName() const = 0;

	protected:
		static void Add(Factory<T>& concreteFactory);

		static void Remove(Factory<T>& concreteFactory);

	private:
		static Hashmap<std::string, Factory<T>*> mFactoryMap;
	};
}

#define ConcreteFactory(ConcreteProductType, AbstractProductType)						\
		class ConcreteProductType ## Factory : public Factory < AbstractProductType >{	\
		public:																			\
			ConcreteProductType ## Factory(){mName = # ConcreteProductType;	Add(*this);}\
			~ConcreteProductType ## Factory(){Remove(*this);}							\
			virtual const std::string& ClassName(void) const override{return mName;}		\
			virtual AbstractProductType* Create() override{return new ConcreteProductType();}\
		private:std::string mName;														\
		};																	
		

#include "Factory.inl"