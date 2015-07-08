#include <cxxtest/TestSuite.h>
#include <crtdbg.h>
#include "ScopeSharedData.h"
#include "TableXmlParseHelper.h"
#include "Scope.h"

using namespace Library;

class TableXmlParserTestSuite : public CxxTest::TestSuite
{
public:
	void TestInteger(void)
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><Integer name=\"ID\"><value>126</value></Integer></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::INTEGER);
		TS_ASSERT(foundDatum->Size() == 1);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(), 126);

		tableSharedData->Initialize();

		foundDatum = parsedScope.Find("ID");
		TS_ASSERT(foundDatum == nullptr);

		xmlData = "<Scope><Integer name=\"ID\"><value index=\"1\">526</value><value index=\"0\">126</value></Integer></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::INTEGER);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(1), 526);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(0), 126);

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestFloat(void)
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><Float name=\"ID\"><value>126.2</value></Float></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::FLOAT);
		TS_ASSERT(foundDatum->Size() == 1);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(), 126.2f);

		tableSharedData->Initialize();

		foundDatum = parsedScope.Find("ID");
		TS_ASSERT(foundDatum == nullptr);

		xmlData = "<Scope><Float name=\"ID\"><value index=\"1\">526.2</value><value index=\"0\">126.1</value></Float></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::FLOAT);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(1), 526.2f);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(0), 126.1f);

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestVector(void)
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><Vector name=\"ID\"><value>15, 16, 17, 18</value></Vector></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::VECTOR);
		TS_ASSERT(foundDatum->Size() == 1);
		TS_ASSERT_EQUALS(foundDatum->GetVector(), glm::vec4(15.f,16.f,17.f,18.f));

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestMatrix(void)
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><Matrix name=\"ID\"><value>((14, 0, 0, 0), (0, 14, 0, 0), (0, 0, 14, 0), (0, 0, 0, 14))</value></Matrix></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::MATRIX);
		TS_ASSERT(foundDatum->Size() == 1);
		TS_ASSERT_EQUALS(foundDatum->GetMatrix(), glm::mat4(14.f));

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestClone()
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);
		std::string xmlData1 = "<Scope><Integer name=\"ID\"><value>126</value></Integer></Scope>";
		std::string xmlData2 = "<Scope><Integer name=\"ID\"><value>526</value></Integer></Scope>";

		auto anotherMasterParser = masterParser->Clone();

		masterParser->Parse(xmlData1.c_str(), xmlData1.length(), true, true);
		anotherMasterParser->Parse(xmlData2.c_str(), xmlData2.length(), true, true);

		auto firstSharedData = reinterpret_cast<ScopeSharedData*>(masterParser->GetSharedData());
		auto secondSharedData = reinterpret_cast<ScopeSharedData*>(anotherMasterParser->GetSharedData());

		Datum * firstFoundDatum = firstSharedData->GetParsedScope()->Find("ID");
		Datum * secondFoundDatum = secondSharedData->GetParsedScope()->Find("ID");

		TS_ASSERT_DIFFERS(firstSharedData, secondSharedData);
		TS_ASSERT_DIFFERS(firstFoundDatum, secondFoundDatum);

		TS_ASSERT(firstFoundDatum->GetInteger() == 126);
		TS_ASSERT(secondFoundDatum->GetInteger() == 526);

		masterParser->RemoveHelper(*tableHelper);
		delete masterParser;
		delete anotherMasterParser;
		delete tableHelper;
		delete tableSharedData;
	}

	void TestScope()
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><Scope name=\"ID\"><Integer name=\"Number\"><value>126</value></Integer></Scope></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("ID");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::TABLE);
		TS_ASSERT(foundDatum->Size() == 1);

		Scope* foundScope = foundDatum->GetTable();

		TS_ASSERT(foundScope->GetParent() == &parsedScope);
		TS_ASSERT(foundScope->Find("Number")->GetInteger() == 126);

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestString(void)
	{
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);

		std::string xmlData = "<Scope><String name=\"Name\"><value>BTT</value></String></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		Datum* foundDatum = parsedScope.Find("Name");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::STRING);
		TS_ASSERT(foundDatum->Size() == 1);
		TS_ASSERT_EQUALS(foundDatum->GetString(), "BTT");

		tableSharedData->Initialize();

		foundDatum = parsedScope.Find("Name");
		TS_ASSERT(foundDatum == nullptr);

		xmlData = "<Scope><String name=\"Name\"><value index=\"1\">PHD</value><value index=\"0\">TOM</value></String></Scope>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		foundDatum = parsedScope.Find("Name");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::STRING);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetString(1), "PHD");
		TS_ASSERT_EQUALS(foundDatum->GetString(0), "TOM");

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}

	void TestParseFromFile()
	{
		std::string fileName = "ScopeXML.xml";
		Scope parsedScope;
		ScopeSharedData* tableSharedData = new ScopeSharedData(parsedScope);
		XmlParseMaster *masterParser = new XmlParseMaster(tableSharedData);
		TableXmlParseHelper* tableHelper = new TableXmlParseHelper();

		masterParser->AddHelper(*tableHelper);
		masterParser->ParseFromFile(fileName);

		Datum* foundDatum = parsedScope.Find("integers");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::INTEGER);
		TS_ASSERT(foundDatum->Size() == 3);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(), 1);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(1), 2);
		TS_ASSERT_EQUALS(foundDatum->GetInteger(2), 3);

		foundDatum = parsedScope.Find("floatingPoint");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::TABLE);

		foundDatum = foundDatum->GetTable()->Find("floating");

		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::FLOAT);
		TS_ASSERT(foundDatum->Size() == 3);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(), 1.1f);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(1), 2.2f);
		TS_ASSERT_EQUALS(foundDatum->GetFloat(2), 3.3f);

		foundDatum = parsedScope.Find("floatingPoint")->GetTable()->Find("pointParent");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::TABLE);

		foundDatum = foundDatum->GetTable()->Find("point");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::VECTOR);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetVector(), glm::vec4(1,2,3,4));
		TS_ASSERT_EQUALS(foundDatum->GetVector(1), glm::vec4(5,6,7,8));

		foundDatum = parsedScope.Find("floatingPoint")->GetTable()->Find("transformationMatrix");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::MATRIX);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetMatrix(), glm::mat4(2));
		TS_ASSERT_EQUALS(foundDatum->GetMatrix(1), glm::mat4(1));

		foundDatum = parsedScope.Find("names");
		TS_ASSERT(foundDatum != nullptr);
		TS_ASSERT(foundDatum->GetType() == Datum::DatumType::STRING);
		TS_ASSERT(foundDatum->Size() == 2);
		TS_ASSERT_EQUALS(foundDatum->GetString(), "Name");
		TS_ASSERT_EQUALS(foundDatum->GetString(1), "SomeName");

		masterParser->RemoveHelper(*tableHelper);
		delete tableSharedData;
		delete masterParser;
		delete tableHelper;
	}
};