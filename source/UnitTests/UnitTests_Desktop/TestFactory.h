#include <cxxtest/TestSuite.h>
#include "Factory.h"
#include "Foo.h"
#include "Attributed.h"

using namespace UnitTests;
using namespace Library;

ConcreteFactory(Foo, RTTI)
ConcreteFactory(Attributed, RTTI)

class FactoryTestSuite : public CxxTest::TestSuite
{
public:
	void TestInstantiation()
	{
		FooFactory mFooFactory;
		RTTI* fooRTTI = Factory<RTTI>::Create("Foo");
		TS_ASSERT(fooRTTI != nullptr);

		Foo* foo = fooRTTI->As<Foo>();
		TS_ASSERT(foo != nullptr);


		AttributedFactory mAttrFactory;
		RTTI* attr = Factory<RTTI>::Create("Attributed");
		TS_ASSERT(attr != nullptr);

		Attributed* attributed = attr->As<Attributed>();
		TS_ASSERT(attributed != nullptr);

		delete fooRTTI;
		delete attributed;
	}

	void TestClassName()
	{
		FooFactory mFooFactory;
		TS_ASSERT(mFooFactory.ClassName() == "Foo");

		AttributedFactory mAttrFactory;
		TS_ASSERT(mAttrFactory.ClassName() == "Attributed");
	}

	void TestFind()
	{
		FooFactory mFooFactory;
		auto fooFactory = Factory<RTTI>::Find("Foo");
		TS_ASSERT(fooFactory->ClassName() == "Foo");

		AttributedFactory mAttrFactory;
		auto attrFactory = Factory<RTTI>::Find("Attributed");
		TS_ASSERT(attrFactory->ClassName() == "Attributed");
	}

	void TestIterator()
	{
		FooFactory mFooFactory;
		AttributedFactory mAttrFactory;

		auto it = FooFactory::begin();

		TS_ASSERT((*it).second->ClassName() == "Attributed");

		++it;

		TS_ASSERT((*it).second->ClassName() == "Foo");

		++it;

		TS_ASSERT(it == FooFactory::end());
	}
};