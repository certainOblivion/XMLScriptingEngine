#include <cxxtest/TestSuite.h>
#include "Hashmap.h"
#include <crtdbg.h>
#include "Foo.h"

using namespace Library;
using namespace UnitTests;

class HashmapTestSuite : public CxxTest::TestSuite
{
public:
	/** Checks whether the list is empty after the default constructor is called*/
	void setUp()
	{
		mIntHashTable.Clear();
		mIntPointerCharTable.Clear();
	}

	void tearDown()
	{
		mIntHashTable.Clear();
		mIntPointerCharTable.Clear();
	}

	void TestSizeandClear()
	{
		Hashmap<Foo, int, FooHashFunctor> fooHashMap(10);
		Foo foo1(21);

		fooHashMap.Insert(foo1,2);

		TS_ASSERT_EQUALS(fooHashMap.Size(), 1);



		TS_ASSERT_EQUALS(mIntHashTable.Size(), 0);

		int key1 = 10;
		int value1 = 30;
		int key2 = 20;
		int value2 = 40;
		int key3 = 30;
		int value3 = 50;

		auto insertedObject1 = mIntHashTable.Insert(key1, value1);
		auto insertedObject2 = mIntHashTable.Insert(key2, value2);
		auto insertedObject3 = mIntHashTable.Insert(key3, value3);

		TS_ASSERT_EQUALS(mIntHashTable.Size(), 3);
		TS_ASSERT_EQUALS(mIntHashTable[key1], value1);
		TS_ASSERT_EQUALS(mIntHashTable[key2], value2);
		TS_ASSERT_EQUALS(mIntHashTable[key3], value3);

		mIntHashTable.Clear();
		TS_ASSERT_EQUALS(mIntHashTable.Size(), 0);
		TS_ASSERT_DIFFERS(mIntHashTable[key1], value1);
		TS_ASSERT_DIFFERS(mIntHashTable[key2], value2);
		TS_ASSERT_DIFFERS(mIntHashTable[key3], value3);


		TS_ASSERT_EQUALS(mIntPointerCharTable.Size(), 0);

		int* pointerKey1 = new int(10);
		char pointerValue1 = 'a';
		int* pointerKey2 = new int(20);
		char pointerValue2 = 'x';
		int* pointerKey3 = new int(30);
		char pointerValue3 = 'i';

		mIntPointerCharTable.Insert(pointerKey1, pointerValue1);
		mIntPointerCharTable.Insert(pointerKey2, pointerValue2);
		mIntPointerCharTable.Insert(pointerKey3, pointerValue3);

		TS_ASSERT_EQUALS(mIntPointerCharTable.Size(), 3);
		TS_ASSERT_EQUALS(mIntPointerCharTable[pointerKey1], pointerValue1);
		TS_ASSERT_EQUALS(mIntPointerCharTable[pointerKey2], pointerValue2);
		TS_ASSERT_EQUALS(mIntPointerCharTable[pointerKey3], pointerValue3);

		mIntPointerCharTable.Clear();
		TS_ASSERT_EQUALS(mIntPointerCharTable.Size(), 0);
		TS_ASSERT_DIFFERS(mIntPointerCharTable[pointerKey1], pointerValue1);
		TS_ASSERT_DIFFERS(mIntPointerCharTable[pointerKey2], pointerValue2);
		TS_ASSERT_DIFFERS(mIntPointerCharTable[pointerKey3], pointerValue3);

		delete pointerKey1;
		delete pointerKey2;
		delete pointerKey3;
	}

	void  TestCopy()
	{
		mIntHashTable.Insert(10, 30);
		mIntHashTable.Insert(2, 45);
		mIntHashTable.Insert(90, 5);

		auto mNewIntHashTable = mIntHashTable;

		auto it1 = mIntHashTable.begin();
		auto it2 = mNewIntHashTable.begin();
		for (; it1 != mIntHashTable.end(); ++it1, ++it2)
		{
			TS_ASSERT_EQUALS((*it2).first, (*it1).first);
			TS_ASSERT_EQUALS((*it2).second, (*it1).second);
		}

		Hashmap<int, int*> mIntPointer;

		int a = 10;
		int b = 20;

		int* c = &a;
		int* d = &b;

		mIntPointer.Insert(1, c);
		mIntPointer.Insert(2, d);
		auto it3 = mIntPointer.Find(1);
		TS_ASSERT_EQUALS((it3->second), c);

		it3 = mIntPointer.Find(2);
		TS_ASSERT_EQUALS((it3->second), d);

		auto mIntNewPointer = mIntPointer;

		it3 = mIntNewPointer.Find(1);
		TS_ASSERT_EQUALS((it3->second), c);

		it3 = mIntNewPointer.Find(2);
		TS_ASSERT_EQUALS((it3->second), d);
	}

	void TestInsertFind()
	{
		auto insertedObject1 = mIntHashTable.Insert(10, 30);
		auto insertedObject2 = mIntHashTable.Insert(2, 45);
		auto insertedObject3 = mIntHashTable.Insert(90, 5);
		auto foundObject = mIntHashTable.Find(10);
		std::pair<int,int> foundPair = *foundObject;
		std::pair<int, int> insertedPair = *insertedObject1;

		TS_ASSERT_EQUALS(foundPair.first, 10);
		TS_ASSERT_EQUALS(foundPair.second, 30);
		TS_ASSERT_EQUALS(foundPair.first, insertedPair.first);
		TS_ASSERT_EQUALS(foundPair.second, insertedPair.second);

		foundObject = mIntHashTable.Find(2);
		foundPair = *foundObject;
		insertedPair = *insertedObject2;

		TS_ASSERT_EQUALS(foundPair.first, 2);
		TS_ASSERT_EQUALS(foundPair.second, 45);
		TS_ASSERT_EQUALS(foundPair.first, insertedPair.first);
		TS_ASSERT_EQUALS(foundPair.second, insertedPair.second);

		foundObject = mIntHashTable.Find(90);
		foundPair = *foundObject;
		insertedPair = *insertedObject3;

		TS_ASSERT_EQUALS(foundPair.first, 90);
		TS_ASSERT_EQUALS(foundPair.second, 5);
		TS_ASSERT_EQUALS(foundPair.first, insertedPair.first);
		TS_ASSERT_EQUALS(foundPair.second, insertedPair.second);
	}

	void TestRemoveAndContainsKey()
	{
		int key1 = 10;
		int value1 = 30;
		int key2 = 20;
		int value2 = 40;
		int key3 = 30;
		int value3 = 50;
		auto insertedObject1 = mIntHashTable.Insert(key1, value1);
		auto insertedObject2 = mIntHashTable.Insert(key2, value2);
		auto insertedObject3 = mIntHashTable.Insert(key3, value3);
		TS_ASSERT_EQUALS(mIntHashTable.Size(), 3);
		TS_ASSERT(mIntHashTable.ContainsKey(key2));
		TS_ASSERT_EQUALS(mIntHashTable[key2], value2);
		mIntHashTable.Remove(key2);
		TS_ASSERT_EQUALS(mIntHashTable.Size(), 2);
		TS_ASSERT(!mIntHashTable.ContainsKey(key2));
		TS_ASSERT_DIFFERS(mIntHashTable[key2], value2);
	}

	void TestIterator()
	{
		int key[] = { 7, 13, 11, 19, 52, 73, 112, 993, 23, 25, 12, 43, 75, 93, 78, 2, 87, 32224, 3, 234 };
		int value[] = { 100, 200, 300, 23, 7, 234, 123, 567, 213, 546, 23, 56, 128, 34, 1931, 5652, 5336, 24658, 32432, 123 };


		for (int i = 0; i < 20; i++)
		{
			mIntHashTable.Insert(key[i], value[i]);
		}

		int index = 0;
		for (auto it = mIntHashTable.begin(); it != mIntHashTable.end(); ++it, index++)
		{
			TS_ASSERT_EQUALS(mIntHashTable[key[index]], value[index]);
			TS_ASSERT_EQUALS(mIntHashTable.Find((*it).first), it);
		}

		std::string string1 = "hello";
		std::string string2 = "i";
		std::string string3 = "hope";
		std::string string4 = "hashmap";
		std::string string5 = "works";

		std::string stringKey[] = { string1,string2,string3,string4,string5 };
		int stringValue[] = { 0, 1, 2, 3, 4 };

		Hashmap<std::string, int> stringIntHash(15);

		for (int index1 = 0; index1 < 5; index1++)
		{
			stringIntHash.Insert(stringKey[index1], stringValue[index1]);
		}

		int counter = 0;
		for (auto it = stringIntHash.begin(); it != stringIntHash.end(); ++it, counter++)
		{
			TS_ASSERT_EQUALS(stringIntHash[stringKey[counter]], stringValue[counter]);
		}

	}

	void TestIndexOperator()
	{
		auto insertedObject1 = mIntHashTable.Insert(10, 30);
		auto insertedObject2 = mIntHashTable.Insert(2, 45);
		auto insertedObject3 = mIntHashTable.Insert(90, 5);

		int foundObject = mIntHashTable[10];
		TS_ASSERT_EQUALS(foundObject, 30);

		foundObject = mIntHashTable[2];
		TS_ASSERT_EQUALS(foundObject, 45);

		foundObject = mIntHashTable[90];
		TS_ASSERT_EQUALS(foundObject, 5);

		TS_ASSERT_THROWS_NOTHING(mIntHashTable[3]);

		Hashmap<std::string, int> stringIntHash(15);

		stringIntHash.Insert("btt", 2015);
		TS_ASSERT_EQUALS(stringIntHash["btt"], 2015);
	}


	HashmapTestSuite()
	{
		mIntHashTable = Hashmap<int, int>(23);
		mIntPointerCharTable = Hashmap<int*, char>(17);
	}

private:
	
	Hashmap<int,int> mIntHashTable;
	Hashmap<int*, char> mIntPointerCharTable;
};