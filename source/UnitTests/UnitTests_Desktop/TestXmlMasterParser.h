#pragma once
#include <cxxtest/TestSuite.h>
#include <crtdbg.h>
#include "Vector.h"
#include "XmlParseMaster.h"
#include "IntegerSharedData.h"
#include "IntegerXmlHelper.h"

using namespace Library;
using namespace UnitTests;

class XmlMasterParserTestSuite : public CxxTest::TestSuite
{
public:
	void TestInteger()
	{
		IntegerSharedData* intSharedData = new IntegerSharedData();
		XmlParseMaster *masterParser = new XmlParseMaster(intSharedData);
		IntegerXmlHelper* intHelper = new IntegerXmlHelper();

		masterParser->AddHelper(*intHelper);
		std::string xmlData = "<Integer>126</Integer>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);

		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 1);
		TS_ASSERT_EQUALS((intSharedData->GetIntegers())[0], 126);

		masterParser->RemoveHelper(*intHelper);
		delete intSharedData;
		delete masterParser;
		delete intHelper;
	}

	void TestParse()
	{
		IntegerSharedData* intSharedData = new IntegerSharedData();
		XmlParseMaster *masterParser = new XmlParseMaster(intSharedData);
		IntegerXmlHelper* intHelper = new IntegerXmlHelper();

		masterParser->AddHelper(*intHelper);
		std::string xmlData = "<Integer>126</Integer>";
		for (std::uint32_t i = 0; i < xmlData.length(); i++)
		{
			masterParser->Parse(&((xmlData.c_str())[i]), 1, ((i == 0) ? true : false), ((i == (xmlData.length() - 1)) ? true : false));
		}

		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 3);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 1);
		TS_ASSERT_EQUALS((intSharedData->GetIntegers())[0], 126);

		intSharedData->Initialize();

		xmlData = "<Integers><Integer MaxSize=\"2\">329</Integer><Integer>12</Integer></Integers>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);
		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 2);
		TS_ASSERT_EQUALS((intSharedData->GetIntegers())[0], 329);
		TS_ASSERT_EQUALS((intSharedData->GetIntegers())[1], 12);

		intSharedData->Initialize();
		xmlData = "<Integers><Integer>329</Integer><Integer>12</Integer></Integers>";
		masterParser->Parse(xmlData.c_str(), xmlData.length(), true, true);
		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 2);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 2);
		TS_ASSERT_EQUALS((intSharedData->GetIntegers())[0], 12);
		TS_ASSERT_EQUALS(intSharedData->GetIntegers().Size(), 1);

		masterParser->RemoveHelper(*intHelper);
		delete intSharedData;
		delete masterParser;
		delete intHelper;
	}

	void TestClone()
	{
		IntegerSharedData* intSharedData = new IntegerSharedData();
		XmlParseMaster *masterParser = new XmlParseMaster(intSharedData);
		IntegerXmlHelper* intHelper = new IntegerXmlHelper();

		masterParser->AddHelper(*intHelper);
		std::string xmlData1 = "<Integer>8349</Integer>";
		std::string xmlData2 = "<Integer>9342</Integer>";

		auto anotherMasterParser = masterParser->Clone();

		masterParser->Parse(xmlData1.c_str(), xmlData1.length(), true, true);
		anotherMasterParser->Parse(xmlData2.c_str(), xmlData2.length(), true, true);

		auto firstSharedData = reinterpret_cast<IntegerSharedData*>(masterParser->GetSharedData());
		auto secondSharedData = reinterpret_cast<IntegerSharedData*>(anotherMasterParser->GetSharedData());

		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 1);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 1);
		TS_ASSERT_DIFFERS(firstSharedData, secondSharedData);
		TS_ASSERT_EQUALS((firstSharedData->GetIntegers())[0], 8349);
		TS_ASSERT_EQUALS((secondSharedData->GetIntegers())[0], 9342);
		masterParser->RemoveHelper(*intHelper);
		delete intSharedData;
		delete masterParser;
		delete anotherMasterParser;
		delete intHelper;
	}

	void TestParseFromFile()
	{
		std::string fileName = "IntegerXML.xml";

		IntegerSharedData* intSharedData = new IntegerSharedData();
		XmlParseMaster *masterParser = new XmlParseMaster(intSharedData);
		IntegerXmlHelper* intHelper = new IntegerXmlHelper();

		masterParser->AddHelper(*intHelper);
		masterParser->ParseFromFile(fileName);

		TS_ASSERT_EQUALS(intHelper->mStartElementCounter, 9);
		TS_ASSERT_EQUALS(intHelper->mEndElementCounter, 9);
		TS_ASSERT_EQUALS(intHelper->mCharDataCounter, 9);
		TS_ASSERT_EQUALS(intHelper->mMaxDepth, 2);
		for (std::uint32_t i = 0; i < intSharedData->GetIntegers().Size(); i++)
		{
			TS_ASSERT_EQUALS((intSharedData->GetIntegers())[i], i + 1);
		}		
		masterParser->RemoveHelper(*intHelper);
		delete intSharedData;
		delete masterParser;
		delete intHelper;
	}
};