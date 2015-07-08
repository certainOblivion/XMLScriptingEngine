#include <cxxtest/TestSuite.h>
#include "SList.h"
#include <crtdbg.h>
#include "Foo.h"

using namespace Library;
using namespace UnitTests;

class SListTestSuite : public CxxTest::TestSuite
{
public:
	/** Checks whether the list is empty after the default constructor is called*/
	void setUp()
	{
		TS_ASSERT(mIntList.IsEmpty());
	}

	void tearDown()
	{
		mIntList.Clear();
		TS_ASSERT(mIntList.IsEmpty());
	}

	/**Checks whether PushFront() works*/
	void TestPushFront()
	{
		mIntList.PushFront(5);
		TS_ASSERT_EQUALS(mIntList.Front(), 5);
		mIntList.Clear();
	}

	/**Checks whether PushBack() works*/
	void TestPushBack()
	{
		mIntList.PushBack(6);
		TS_ASSERT_EQUALS(mIntList.Back(), 6);
		mIntList.Clear();
	}

	void TestFrontAndBack()
	{
		mIntList.PushBack(3);
		TS_ASSERT_THROWS_NOTHING(mIntList.Front());
		TS_ASSERT_THROWS_NOTHING(mIntList.Back());
		mIntList.PopFront();
		TS_ASSERT_THROWS(mIntList.Front(), std::overflow_error);
		TS_ASSERT_THROWS(mIntList.Back(), std::overflow_error);
	}

	/**Checks whether after a series of PushBack and PushFront without duplicate data, the expected order is achieved*/
	void TestOrder()
	{
		mIntList.PushFront(3);
		mIntList.PushBack(4);
		mIntList.PushBack(5);
		mIntList.PushFront(2);
		mIntList.PushFront(1);


		for (uint32_t i = 1; i <= mIntList.Size(); i++)
		{
			TS_ASSERT_EQUALS(mIntList.Front(), i);
			mIntList.PopFront();
		}		

		mIntList.Clear();
	}

	/**Checks whether after a series of PushBack and PushFront with duplicate data, the expected order is achieved*/
	void TestOrderWithDuplicate()
	{
		mIntList.PushBack(4);
		mIntList.PushBack(4);
		mIntList.PushFront(5);
		mIntList.PushBack(5);
		mIntList.PushBack(5);

		TS_ASSERT_EQUALS(mIntList.Front(), 5);
		mIntList.PopFront();

		TS_ASSERT_EQUALS(mIntList.Front(), 4);
		mIntList.PopFront();

		TS_ASSERT_EQUALS(mIntList.Front(), 4);
		mIntList.PopFront();

		TS_ASSERT_EQUALS(mIntList.Front(), 5);
		mIntList.PopFront();

		TS_ASSERT_EQUALS(mIntList.Front(), 5);
		mIntList.PopFront();

		mIntList.Clear();
	}

	/**Tests whether the Copy constructor is working*/
	void TestCopy()
	{
		mIntList.PushBack(4);
		mIntList.PushBack(4);
		mIntList.PushFront(5);
		mIntList.PushBack(5);
		mIntList.PushBack(5);

		//call copy constructor
		SList<int>* newIntList = new SList<int>(mIntList);

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 4);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 4);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		newIntList->Clear();
		mIntList.Clear();

		mIntList.PushBack(4);
		mIntList.PushBack(4);
		mIntList.PushFront(5);
		mIntList.PushBack(5);
		mIntList.PushBack(5);
		
		//test operator= overload
		*newIntList = mIntList;

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 4);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 4);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		TS_ASSERT_EQUALS(newIntList->Front(), 5);
		newIntList->PopFront();

		newIntList->Clear();
		mIntList.Clear();

		TS_ASSERT_EQUALS(mIntList.Size(), 0);

		int a = 6;
		int b = 10;

		mIntList.PushFront(a);
		TS_ASSERT_EQUALS(mIntList.Front(), a);
		TS_ASSERT_EQUALS(mIntList.Back(), a);
		TS_ASSERT_EQUALS(mIntList.Front(), mIntList.Back());
		TS_ASSERT_EQUALS(mIntList.Size(), 1);

		mIntList.PushFront(b);
		TS_ASSERT_EQUALS(mIntList.Front(), b);
		TS_ASSERT_EQUALS(mIntList.Back(), a);
		TS_ASSERT_DIFFERS(mIntList.Front(), mIntList.Back());
		TS_ASSERT_EQUALS(mIntList.Size(), 2);

		delete newIntList;
	}

	/**Checks whether the list can work with object pointers of different classes*/
	void TestWithOtherDataType()
	{
		SList<Foo*> fooList;
		TS_ASSERT_EQUALS(fooList.Size(), 0);
		Foo* foo1 = new Foo(1);
		Foo* foo2 = new Foo(2);
		Foo* foo3 = new Foo(3);
		Foo* foo4 = new Foo(4);

		fooList.PushBack(foo1);
		fooList.PushBack(foo2);
		fooList.PushBack(foo3);
		fooList.PushBack(foo4);

		TS_ASSERT_EQUALS(fooList.Front()->Get(), 1);
		fooList.PopFront();
		TS_ASSERT_EQUALS(fooList.Front()->Get(), 2);
		fooList.PopFront();
		TS_ASSERT_EQUALS(fooList.Front()->Get(), 3);
		fooList.PopFront();
		TS_ASSERT_EQUALS(fooList.Front()->Get(), 4);
		
		fooList.Clear();

		delete foo1;
		delete foo2;
		delete foo3;
		delete foo4;

	}

	/**Checks whether the copy constructor and the assignment operator overload is working. */
	void TestIteratorInstantiation()
	{
		mIntList.PushFront(3);
		mIntList.PushBack(4);
		mIntList.PushBack(5);
		mIntList.PushFront(2);
		mIntList.PushFront(1);

		SList<int>::Iterator it = mIntList.begin(); //copy constructor
		SList<int>::Iterator it2;
		
		it2 = it;//assignment operator

		for (auto itCounter = it; it != mIntList.end(); ++it, ++it2)
		{
			TS_ASSERT_EQUALS(*it, *it2);
		}
	}

	/**Checks whether the iterator traversal is working*/
	void TestIteratorTraversal()
	{
		int i;
		for (i = 0; i < 5; i++)
		{
			mIntList.PushBack(i);
		}
		i = 0;
		for (auto it = mIntList.begin(); it != mIntList.end(); ++it)
		{
			TS_ASSERT_EQUALS(*it, i);
			i++;
		}

		mIntList.Clear();

		SList<Foo*>* fooList = new SList<Foo*>();
		Foo* foo1 = new Foo(1);
		Foo* foo2 = new Foo(2);
		Foo* foo3 = new Foo(3);
		Foo* foo4 = new Foo(4);

		fooList->PushBack(foo1);
		fooList->PushBack(foo2);
		fooList->PushBack(foo3);
		fooList->PushBack(foo4);

		SList<Foo*>* fooList2 = new SList<Foo*>(*fooList);
		auto fooIt2 = fooList2->begin();
		for (auto fooIt = fooList->begin(); fooIt != fooList->end();++fooIt,++fooIt2)
		{
			auto temp1 = *fooIt;
			auto temp2 = *fooIt2;
			TS_ASSERT_EQUALS(temp1->Get(), temp2->Get());
		}

		fooList->Clear();
		fooList2->Clear();

		delete foo1;
		delete foo3;
		delete foo2;
		delete foo4;
		delete fooList;
		delete fooList2;

		auto it = mIntList.begin();

		TS_ASSERT_THROWS(*it, std::overflow_error);
	}

	/**Checks whether the Find successfully returns the correct iterator node and if not, dereferencing an iterator node from a failed search throws an error*/
	void TestFind()
	{
		int i;
		for (i = 0; i < 5; i++)
		{
			mIntList.PushBack(i);
		}
		TS_ASSERT_THROWS(*mIntList.Find(5), std::overflow_error);
		TS_ASSERT_EQUALS(mIntList.Find(5), mIntList.end());
		TS_ASSERT_EQUALS(*mIntList.Find(2), 2);
	}

	/**Checks whether the user can insert an item after another in the list.*/
	void TestInsertAfter()
	{
		mIntList.PushBack(1);
		mIntList.PushBack(2);
		mIntList.PushBack(4);

		auto it = mIntList.Find(2);

		mIntList.InsertAfter(it, 3);

		int i = 1;
		for (it = mIntList.begin(); it != mIntList.end(); ++it,++i)
		{
			TS_ASSERT_EQUALS(*it, i);
		}

		mIntList.Clear();
		mIntList.PushBack(1);
		mIntList.PushBack(2);
		mIntList.PushBack(3);
		mIntList.PushBack(4);

		it = mIntList.Find(1);
		TS_ASSERT_EQUALS(mIntList.Front(),*it);
		mIntList.PopFront();
	}

	/**Checks to see whether a user can delete an object from the list. If the object is not found in the list, the list is unaltered.*/
	void TestRemove()
	{
		mIntList.PushBack(1);
		mIntList.PushBack(4);
		mIntList.PushBack(2);
		mIntList.PushBack(3);

		mIntList.Remove(4);

		int i = 1;
		for (auto it = mIntList.begin(); it != mIntList.end(); ++it, ++i)
		{
			TS_ASSERT_EQUALS(*it, i);
		}
		TS_ASSERT_EQUALS(mIntList.Size(), 3);

		mIntList.Clear();
		mIntList.PushBack(1);
		mIntList.PushBack(2);
		mIntList.PushBack(3);
		mIntList.PushBack(4);

		mIntList.Remove(4);

		i = 1;
		for (auto it = mIntList.begin(); it != mIntList.end(); ++it, ++i)
		{
			TS_ASSERT_EQUALS(*it, i);
		}
		TS_ASSERT_EQUALS(mIntList.Back(), 3);

		mIntList.Clear();
		mIntList.PushBack(1);
		mIntList.PushBack(2);
		mIntList.PushBack(3);
		mIntList.PushBack(4);

		mIntList.Remove(1);

		i = 2;
		for (auto it = mIntList.begin(); it != mIntList.end(); ++it, ++i)
		{
			TS_ASSERT_EQUALS(*it, i);
		}
		TS_ASSERT_EQUALS(mIntList.Front(), 2);
		TS_ASSERT_EQUALS(mIntList.Back(), 4);

		TS_ASSERT_THROWS_NOTHING(mIntList.Remove(6));
	}

private:
	SList<int> mIntList;
};