#include <cxxtest/TestSuite.h>
#include "Stack.h"
#include <crtdbg.h>
#include "Foo.h"


using namespace Library;
using namespace UnitTests;

class StackTestSuite : public CxxTest::TestSuite
{
public:
	/** Checks whether the list is empty after the default constructor is called*/
	void setUp()
	{
		mIntStack.Clear();
	}

	void tearDown()
	{
		mIntStack.Clear();
	}

	void TestPushPop()
	{
		mIntStack.Push(1);

		TS_ASSERT_EQUALS(mIntStack.Size(), 1);
		TS_ASSERT_EQUALS(mIntStack.Top(), 1);

		mIntStack.Pop();
		TS_ASSERT_EQUALS(mIntStack.Size(), 0);
		TS_ASSERT_THROWS(mIntStack.Top(), std::overflow_error);
	}

	void TestCopy()
	{
		mIntStack.Push(1);
		mIntStack.Push(2);

		Stack<int> newIntStack = mIntStack;

		TS_ASSERT_EQUALS(mIntStack.Size(), newIntStack.Size());
		TS_ASSERT_EQUALS(mIntStack.Top(), newIntStack.Top());

		newIntStack.Pop();
		mIntStack.Pop();

		TS_ASSERT_EQUALS(mIntStack.Size(), newIntStack.Size());
		TS_ASSERT_EQUALS(mIntStack.Top(), newIntStack.Top());

		mIntStack.Clear();
		newIntStack.Clear();
	}

	void TestClear()
	{
		mIntStack.Push(1);
		mIntStack.Push(2);

		TS_ASSERT_EQUALS(mIntStack.Size(), 2);
		TS_ASSERT_EQUALS(mIntStack.Top(), 2);

		mIntStack.Clear();

		TS_ASSERT_EQUALS(mIntStack.Size(), 0);
	}

	void TestWithDifferentDataType()
	{
		Stack<Foo*> fooStack;
		TS_ASSERT_EQUALS(fooStack.Size(), 0);
		Foo* foo1 = new Foo(1);
		Foo* foo2 = new Foo(2);
		Foo* foo3 = new Foo(3);
		Foo* foo4 = new Foo(4);

		fooStack.Push(foo1);
		fooStack.Push(foo2);
		fooStack.Push(foo3);
		fooStack.Push(foo4);

		TS_ASSERT_EQUALS(fooStack.Top()->Get(), 4);
		fooStack.Pop();
		TS_ASSERT_EQUALS(fooStack.Top()->Get(), 3);
		fooStack.Pop();
		TS_ASSERT_EQUALS(fooStack.Top()->Get(), 2);
		fooStack.Pop();
		TS_ASSERT_EQUALS(fooStack.Top()->Get(), 1);

		fooStack.Clear();

		delete foo1;
		delete foo2;
		delete foo3;
		delete foo4;
	}

private:
	Stack<int> mIntStack;
};