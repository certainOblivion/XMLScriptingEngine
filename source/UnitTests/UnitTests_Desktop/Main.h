#include <cxxtest/TestSuite.h>

class MyTestSuite : public CxxTest::TestSuite
{
public:
	void TestAddition(void)
	{
		TS_ASSERT_EQUALS(1 + 1, 2);
	}
};