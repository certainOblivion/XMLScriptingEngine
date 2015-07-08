#include <cxxtest/TestSuite.h>
#include "Scope.h"
#include "Datum.h"
#include <crtdbg.h>
#include "Foo.h"

using namespace Library;
using namespace UnitTests;

class ScopeTestSuite : public CxxTest::TestSuite
{
public:

	void setUp()
	{
		mTable.Clear();
	}

	void TestInitialization()
	{
		Scope* aScope = new Scope(2);
		TS_ASSERT_DIFFERS(aScope, nullptr);
		delete aScope;
	}

	void TestAppend()
	{
		Datum& data = mTable.Append("hello",Datum::DatumType::INTEGER);
		data = 20;

		TS_ASSERT_EQUALS(mTable["hello"].GetInteger(), 20);
		TS_ASSERT_EQUALS(mTable["hello"].GetType(), Datum::DatumType::INTEGER);
		TS_ASSERT_EQUALS(mTable[0].GetInteger(), 20);
		TS_ASSERT_EQUALS(mTable[0].GetType(), Datum::DatumType::INTEGER);

		Datum& data2 = mTable.Append("BTT");
		data2.SetType(Datum::DatumType::STRING);

		data2 = "PHD";

		TS_ASSERT_EQUALS(mTable["BTT"].GetString(), "PHD");
		TS_ASSERT_EQUALS(mTable["BTT"].GetType(), Datum::DatumType::STRING);
		TS_ASSERT_EQUALS(mTable[1].GetString(), "PHD");
		TS_ASSERT_EQUALS(mTable[1].GetType(), Datum::DatumType::STRING);

		Datum& data3 = mTable.Append("matrix");
		data3.SetType(Datum::DatumType::MATRIX);

		data3 = glm::mat4(2);

		TS_ASSERT_EQUALS(mTable["matrix"].GetMatrix(), glm::mat4(2));
		TS_ASSERT_EQUALS(mTable["matrix"].GetType(), Datum::DatumType::MATRIX);
		TS_ASSERT_EQUALS(mTable[2].GetMatrix(), glm::mat4(2));
		TS_ASSERT_EQUALS(mTable[2].GetType(), Datum::DatumType::MATRIX);
	}

	void TestFind()
	{
		Datum& data = mTable.Append("hello", Datum::DatumType::INTEGER);
		data = 20;

		Datum& data2 = mTable.Append("BTT");
		data2.SetType(Datum::DatumType::STRING);
		data2 = "PHD";


		Datum& data3 = mTable.Append("matrix");
		data3.SetType(Datum::DatumType::MATRIX);
		data3 = glm::mat4(2);

		Datum* foundDatum = mTable.Find("BTT");
		
		TS_ASSERT_EQUALS(foundDatum->GetString(), "PHD");
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::STRING);

		foundDatum = mTable.Find("matrix");

		TS_ASSERT_EQUALS(foundDatum->GetMatrix(), glm::mat4(2));
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::MATRIX);

		foundDatum = mTable.Find("hello");
		TS_ASSERT_EQUALS(foundDatum->GetInteger(), 20);
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::INTEGER);

		foundDatum = mTable.Find("HeWhoMustNotBeNamed");
		TS_ASSERT_EQUALS(foundDatum, nullptr);
	}

	void TestAppendScope()
	{
		Scope& childTable = mTable.AppendScope("child");
		TS_ASSERT_EQUALS(childTable.GetParent(), &mTable);
	}

	void TestSearch()
	{
		Datum& data = mTable.Append("hello", Datum::DatumType::INTEGER);
		data = 20;

		Scope& childTable = mTable.AppendScope("child");
		TS_ASSERT_EQUALS(childTable.GetParent(), &mTable);

		Datum& data2 = childTable.Append("BTT");
		data2.SetType(Datum::DatumType::STRING);
		data2 = "PHD";

		Scope& childToChildTable = childTable.AppendScope("child2child");

		Datum& data3 = childToChildTable.Append("matrix");
		data3.SetType(Datum::DatumType::MATRIX);
		data3 = glm::mat4(2);
		Scope** foundScope = new Scope*();
		Datum* foundDatum = childToChildTable.Search("BTT",foundScope);

		TS_ASSERT_EQUALS(foundDatum->GetString(), "PHD");
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::STRING);
		TS_ASSERT_EQUALS(*foundScope, &childTable);

		foundDatum = childToChildTable.Search("matrix", foundScope);

		TS_ASSERT_EQUALS(foundDatum->GetMatrix(), glm::mat4(2));
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::MATRIX);
		TS_ASSERT_EQUALS(*foundScope, &childToChildTable);

		foundDatum = childToChildTable.Search("hello", foundScope);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(), 20);
		TS_ASSERT_EQUALS(foundDatum->GetType(), Datum::DatumType::INTEGER);
		TS_ASSERT_EQUALS(*foundScope, &mTable);

		foundDatum = childToChildTable.Find("HeWhoMustNotBeNamed");
		TS_ASSERT_EQUALS(foundDatum, nullptr);

		delete foundScope;
	}

	void TestEquality()
	{
		Scope newScope;
		Datum& data = mTable.Append("hello", Datum::DatumType::INTEGER);
		data = 20;

		Datum& data2 = mTable.Append("BTT");
		data2.SetType(Datum::DatumType::STRING);
		data2 = "PHD";


		Datum& data3 = mTable.Append("matrix");
		data3.SetType(Datum::DatumType::MATRIX);
		data3 = glm::mat4(2);

		Datum& data4 = newScope.Append("hello", Datum::DatumType::INTEGER);
		data4 = 20;

		Datum& data5 = newScope.Append("BTT");
		data5.SetType(Datum::DatumType::STRING);
		data5 = "PHD";


		Datum& data6 = newScope.Append("matrix");
		data6.SetType(Datum::DatumType::MATRIX);
		data6 = glm::mat4(2);

		TS_ASSERT(mTable == newScope);

		data6 = glm::mat4(3);

		TS_ASSERT(mTable != newScope);
	}

	void TestAdopt()
	{
		Scope& childTable = mTable.AppendScope("child");
		TS_ASSERT_EQUALS(childTable.GetParent(), &mTable);

		Scope anotherScope;
		anotherScope.Adopt(childTable,"child",0);

		TS_ASSERT_EQUALS(childTable.GetParent(), &anotherScope);

		Scope& anotherChildTable = mTable.AppendScope("child");
		TS_ASSERT_EQUALS(anotherChildTable.GetParent(), &mTable);

		anotherScope.Adopt(anotherChildTable, "child", 1);
		Datum * d = anotherScope.Find("child");

		TS_ASSERT_EQUALS(d->GetTable(0), &childTable);
		TS_ASSERT_EQUALS(d->GetTable(1), &anotherChildTable);
		TS_ASSERT_EQUALS(anotherChildTable.GetParent(), &anotherScope);
	}

	void TestMakeOrphan()
	{
		std::string key = "hi";
		Scope* s = &mTable.AppendScope(key);
		TS_ASSERT_EQUALS(s->GetParent(), &mTable);

		s->MakeOrphan();

		TS_ASSERT_EQUALS(s->GetParent(), nullptr);

		delete s;
	}

	void TestFindName()
	{
		std::string key = "hi";
		Scope* s = &mTable.AppendScope(key);
		TS_ASSERT_EQUALS(s->GetParent(), &mTable);

		std::string foundString = mTable.FindName(s);
		TS_ASSERT_EQUALS(foundString, key);
	}

	void TestCopy()
	{
		Datum& data = mTable.Append("hello", Datum::DatumType::INTEGER);
		data = 20;

		Datum& data2 = mTable.Append("BTT");
		data2.SetType(Datum::DatumType::STRING);
		data2 = "PHD";


		Datum& data3 = mTable.Append("matrix");
		data3.SetType(Datum::DatumType::MATRIX);
		data3 = glm::mat4(2);

		Scope newScope = mTable;

		TS_ASSERT(newScope == mTable);

		Scope& childTable = mTable.AppendScope("child");
		Datum& d = childTable.Append("childString");
		d = "hello kid!";
		Scope& anotherChildTable = mTable.AppendScope("child");

		TS_ASSERT(childTable.GetParent() == anotherChildTable.GetParent());
		newScope = mTable;

		TS_ASSERT(newScope == mTable);
	}

private:
	Scope mTable;
};