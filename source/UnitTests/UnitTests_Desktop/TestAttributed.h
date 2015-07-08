#include <cxxtest/TestSuite.h>
#include "AttributedFoo.h"
#include <crtdbg.h>

using namespace Library;
using namespace UnitTests;

class AttributedTestSuite : public CxxTest::TestSuite
{
public:

	void TestIsPrescribedAttribute(void)
	{
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalInteger"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalFloat"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalVector"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalMatrix"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalString"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalRTTI"));

		TS_ASSERT(mFoo.IsPrescribedAttribute("ExternalInteger"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("ExternalFloat"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("ExternalVector"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("ExternalMatrix"));
		TS_ASSERT(mFoo.IsPrescribedAttribute("ExternalString"));

		TS_ASSERT(!mFoo.IsPrescribedAttribute("BTT"));
	}

	void TestPopulate(void)
	{
		//Integer
		Datum * d = mFoo.Find("InternalInteger");
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetInteger(0), 2);
		TS_ASSERT_EQUALS(d->GetInteger(1), 2);

		d = mFoo.Find("InternalFloat");
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetFloat(0), 2.0f);
		TS_ASSERT_EQUALS(d->GetFloat(1), 2.0f);

		d = mFoo.Find("InternalVector");
		glm::vec4 v = glm::vec4(2);
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetVector(0), v);
		TS_ASSERT_EQUALS(d->GetVector(1), v);

		d = mFoo.Find("InternalMatrix");
		glm::mat4 m = glm::mat4(3);
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetMatrix(0), m);

		d = mFoo.Find("InternalString");
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetString(0), "String");
		TS_ASSERT_EQUALS(d->GetString(1), "String");

		d = mFoo.Find("InternalRTTI");
		TS_ASSERT_EQUALS(d->Size(), 3);
		TS_ASSERT_EQUALS(d->GetPointer(0), nullptr);
		TS_ASSERT_EQUALS(d->GetPointer(1), nullptr);

		//External
		d = mFoo.Find("ExternalInteger");
		TS_ASSERT_EQUALS(d->Size(), 1);
		TS_ASSERT_EQUALS(&(d->GetInteger()), &mFoo.mExternalInteger);

		d = mFoo.Find("ExternalFloat");
		TS_ASSERT_EQUALS(d->Size(), 1);
		TS_ASSERT_EQUALS(&(d->GetFloat()), &mFoo.mExternalFloat);

		d = mFoo.Find("ExternalVector");
		TS_ASSERT_EQUALS(d->Size(), 1);
		TS_ASSERT_EQUALS(&(d->GetVector()), &mFoo.mExternalVector);

		d = mFoo.Find("ExternalMatrix");
		TS_ASSERT_EQUALS(d->Size(), 1);
		TS_ASSERT_EQUALS(&(d->GetMatrix()), &mFoo.mExternalMatrix);

		d = mFoo.Find("ExternalString");
		TS_ASSERT_EQUALS(d->Size(), 1);
		TS_ASSERT_EQUALS(&(d->GetString()), &mFoo.mExternalString);
	}

	void TestInitialization()
	{
		auto* foundDatum = mFoo.Find("InternalString");
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalString"));
		TS_ASSERT_EQUALS(foundDatum->GetString(), "String");

		foundDatum = mFoo.Find("InternalInteger");
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalInteger"));
		TS_ASSERT_EQUALS(foundDatum->GetInteger(), 2);

		foundDatum = mFoo.Find("InternalFloat");
		TS_ASSERT(mFoo.IsPrescribedAttribute("InternalFloat"));
		TS_ASSERT_EQUALS(foundDatum->GetFloat(), 2.0f);
	}

	void TestAuxiliiaryAttribute(void)
	{
		Datum& d = mFoo.AppendAuxiliaryAttribute("India");
		d.SetType(Datum::DatumType::INTEGER);

		TS_ASSERT(mFoo.IsAttribute("India"));
		TS_ASSERT(mFoo.IsAuxiliaryAttribute("India"));
		TS_ASSERT(!mFoo.IsPrescribedAttribute("India"));

		TS_ASSERT_THROWS_ANYTHING(mFoo.AppendAuxiliaryAttribute("InternalInteger"));
		TS_ASSERT(!mFoo.IsAuxiliaryAttribute("InternalInteger"));

		AttributedFoo foo = AttributedFoo();
		TS_ASSERT(!foo.IsAttribute("India"));
	}

	void TestAssignmentOperator(void)
	{
		AttributedFoo foo;
		Datum * d1 = foo.Find("InternalString");
		TS_ASSERT(d1->GetString() != "Jekyl");

		Datum* d = mFoo.Find("InternalString");
		d->Set("Wonderful");

		foo = mFoo;
		Datum * d2 = foo.Find("InternalString");
		TS_ASSERT(d2->GetString() == "Wonderful");
	}

	void TestCopyConstructor(void)
	{
		AttributedFoo foo = mFoo;

		Datum* d = mFoo.Find("InternalString");
		d->Set("Hyde");

		Datum * d2 = foo.Find("InternalString");
		TS_ASSERT(d2->GetString() != "Hyde");
	}

	void TestAttribute(void)
	{
		TS_ASSERT(mFoo.IsAttribute("InternalInteger"));
		TS_ASSERT(mFoo.IsAttribute("InternalFloat"));
		TS_ASSERT(mFoo.IsAttribute("InternalVector"));
		TS_ASSERT(mFoo.IsAttribute("InternalMatrix"));
		TS_ASSERT(mFoo.IsAttribute("InternalString"));

		TS_ASSERT(mFoo.IsAttribute("ExternalInteger"));
		TS_ASSERT(mFoo.IsAttribute("ExternalFloat"));
		TS_ASSERT(mFoo.IsAttribute("ExternalVector"));
		TS_ASSERT(mFoo.IsAttribute("ExternalMatrix"));
		TS_ASSERT(mFoo.IsAttribute("ExternalString"));

		Datum& d = mFoo.AppendAuxiliaryAttribute("BTT");
		d.SetType(Datum::DatumType::INTEGER);

		TS_ASSERT(mFoo.IsAttribute("BTT"));
	}

	void TestAuxiliaryBegin(void)
	{
		TS_ASSERT(mFoo.AuxiliaryBegin() == 12);

	}




private:
	AttributedFoo mFoo;
};