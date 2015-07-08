#include "Foo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo)

	const std::int32_t Foo::HashNumber = 33;

	Foo::Foo()
	{
		fooData = 0;
		moreFooData = new int[20];
		for (int i = 0; i < 20; i++)
		{
			moreFooData[i] = i;
		}
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		fooData = rhs.fooData;
		for (int i=0; i < 20; i++)
		{
			moreFooData[i] = rhs.moreFooData[i];
		}

		return *this;
	}

	Foo::Foo(const Foo& rhs)
		:Foo()
	{
		operator=(rhs);
	}

	Foo::~Foo()
	{
		delete[] moreFooData;
		moreFooData = nullptr;
	}
	Foo::Foo(int x)
		:Foo()
	{
		fooData = x;

	}
	int Foo::Do() const
	{
		int sum = fooData;

		for (int i = 0; i < 20; i++)
		{
			sum += moreFooData[i];
		}

		return sum;
	}

	int Foo::Get() const
	{
		return fooData;
	}



	typedef std::uint8_t byte;

	std::uint32_t FooHashFunctor::operator()(const Foo& value) const
	{
		uint32_t hash = 0;
		const byte* bytes = reinterpret_cast<const byte*>(&(value.fooData));
		uint32_t size = sizeof(*bytes);

		for (uint32_t i = 0; i < size; ++i)
		{
			hash = bytes[i] + 33 * hash;
		}

		bytes = reinterpret_cast<const byte*>(&(value).fooData);
		size = sizeof(*bytes);

		for (uint32_t i = 0; i < size; i++)
		{
			hash = bytes[i] + 33 * hash;
		}
		return hash;
	}
}
