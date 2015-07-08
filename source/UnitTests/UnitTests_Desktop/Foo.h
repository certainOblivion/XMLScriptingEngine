#pragma once

#include "RTTI.H"
#include "Factory.h"
#include <cstdint>

namespace UnitTests
{
	/** class Foo to test SList with Class pointers*/
	class Foo : public Library::RTTI
	{
		RTTI_DECLARATIONS(Foo, Library::RTTI)
	private:
		int* moreFooData;

	public:
		static const std::int32_t HashNumber;

		int fooData;
		/** Default constructor*/
		Foo();

		/** Default parameterized constructor. Sets the mData member variable*/
		Foo(int x);

		/**Destructor*/
		~Foo();

		/**Returns the mData member*/
		int Do() const;

		Foo& operator=(const Foo& rhs);

		int Get() const;

		Foo(const Foo& rhs);

		bool Foo::operator==(const Foo& rhs)
		{
			return (rhs.fooData == fooData);
		}

	};

	class FooHashFunctor
	{
	public:
		std::uint32_t operator()(const Foo& value) const;
	};
}