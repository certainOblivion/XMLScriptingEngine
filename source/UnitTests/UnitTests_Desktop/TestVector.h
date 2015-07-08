#include <cxxtest/TestSuite.h>
#include "Vector.h"
#include <crtdbg.h>
#include "Foo.h"

using namespace Library;
using namespace UnitTests;

class VectorTestSuite : public CxxTest::TestSuite
{
public:
	/** Checks whether the list is empty after the default constructor is called*/
	void setUp()
	{
		TS_ASSERT(mIntVector.IsEmpty());
	}

	void tearDown()
	{
		mIntVector.Clear();
		TS_ASSERT(mIntVector.IsEmpty());
	}

	void TestPush()
	{
		int a = 10;
		int b = 20;
		mIntVector.PushBack(a);

		TS_ASSERT_EQUALS(mIntVector[0], a);
		TS_ASSERT_EQUALS(mIntVector.Size(), 1);
		TS_ASSERT_EQUALS(mIntVector.Front(), a);
		TS_ASSERT_EQUALS(mIntVector.Front(), mIntVector.Back());
		TS_ASSERT_EQUALS(mIntVector.Back(), a);

		mIntVector.PushBack(b);
		TS_ASSERT_EQUALS(mIntVector[0], a);
		TS_ASSERT_EQUALS(mIntVector[1], b);
		TS_ASSERT_EQUALS(mIntVector.Size(), 2);
		TS_ASSERT_EQUALS(mIntVector.Front(), a);
		TS_ASSERT_DIFFERS(mIntVector.Front(), mIntVector.Back());
		TS_ASSERT_EQUALS(mIntVector.Back(), b);

		mIntVector.Front() = b;
		TS_ASSERT_EQUALS(mIntVector[0], b);
		TS_ASSERT_EQUALS(mIntVector[1], b);
		TS_ASSERT_EQUALS(mIntVector.Size(), 2);
		TS_ASSERT_EQUALS(mIntVector.Front(), b);
		TS_ASSERT_EQUALS(mIntVector.Front(), mIntVector.Back());
		TS_ASSERT_EQUALS(mIntVector.Back(), b);

		TS_ASSERT(!mIntVector.IsEmpty());
		mIntVector.Clear();
		TS_ASSERT(mIntVector.IsEmpty());
	}

	void TestPop()
	{
		int a = 10;
		int b = 20;
		mIntVector.PushBack(a);
		mIntVector.PushBack(b);

		TS_ASSERT_EQUALS(mIntVector[0], a);
		TS_ASSERT_EQUALS(mIntVector[1], b);
		TS_ASSERT_EQUALS(mIntVector.Size(), 2);
		TS_ASSERT_EQUALS(mIntVector.Front(), a);
		TS_ASSERT_DIFFERS(mIntVector.Front(), mIntVector.Back());
		TS_ASSERT_EQUALS(mIntVector.Back(), b);

		mIntVector.PopBack();
		TS_ASSERT_EQUALS(mIntVector[0], a);
		TS_ASSERT_EQUALS(mIntVector.Size(), 1);
		TS_ASSERT_EQUALS(mIntVector.Front(), a);
		TS_ASSERT_EQUALS(mIntVector.Front(), mIntVector.Back());
		TS_ASSERT_EQUALS(mIntVector.Back(), a);
	}

	void TestAt()
	{
		Vector<Foo> fooPointerVector;
		Foo foo1(1);
		Foo foo2(2);
		Foo foo3(3);
		Foo foo4(4);

		fooPointerVector.PushBack(foo1);
		fooPointerVector.PushBack(foo2);
		fooPointerVector.PushBack(foo3);
		fooPointerVector.PushBack(foo4);

		fooPointerVector.Reserve(7);

		TS_ASSERT_EQUALS(fooPointerVector.Back().Get(), 4);
		TS_ASSERT_EQUALS(fooPointerVector.Size(), 4);
		TS_ASSERT_EQUALS(fooPointerVector.At(2).Get(), foo3.Get());
		TS_ASSERT_EQUALS(fooPointerVector.Size(), 4);
		TS_ASSERT_EQUALS(fooPointerVector.At(5).Get(), 0);
		TS_ASSERT_EQUALS(fooPointerVector.Size(), 6);	
	}

	void TestCopy()
	{
		mIntVector.PushBack(1);
		mIntVector.PushBack(2);
		mIntVector.PushBack(3);
		mIntVector.PushBack(4);

		Vector<int> newIntVector = mIntVector; //calling copy contructor
		Vector<int> anotherIntVector;

		anotherIntVector = mIntVector; //calling overloaded assignment operator

		TS_ASSERT_EQUALS(newIntVector.Back(), 4);
		TS_ASSERT_EQUALS(newIntVector.Front(), 1);
		TS_ASSERT_EQUALS(anotherIntVector.Back(), 4);
		TS_ASSERT_EQUALS(anotherIntVector.Front(), 1);
		TS_ASSERT_EQUALS(anotherIntVector.Size(), newIntVector.Size());
	}

	void TestIteratorAndFind()
	{
		for (int i = 0; i < 5; i++)
		{
			mIntVector.PushBack(i);
		}
		TS_ASSERT_EQUALS(*mIntVector.Find(2), 2);

		auto it = mIntVector.Find(3);

		TS_ASSERT_EQUALS(*it, 3);
		TS_ASSERT_EQUALS(*(++it), 4);
		int i = 0;
		for (it = mIntVector.begin(); it != mIntVector.end(); ++it,++i)
		{
			TS_ASSERT_EQUALS(*it, i);
		}
	}
private:
	Vector<int> mIntVector;
};