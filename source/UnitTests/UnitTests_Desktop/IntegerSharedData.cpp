#include "IntegerSharedData.h"

using namespace Library;
using namespace UnitTests;

RTTI_DEFINITIONS(IntegerSharedData)

IntegerSharedData::IntegerSharedData():
XmlParseMaster::SharedData(), mState(SharedDataState::END), mMaxSize(1)
{
}

IntegerSharedData::~IntegerSharedData()
{
}

Library::Vector<std::int32_t>& IntegerSharedData::GetIntegers()
{
	return mParsedList;
}

void IntegerSharedData::ParseInteger(std::string intString)
{
	std::uint32_t parsedInteger = atoi(intString.c_str());
	if (mParsedList.Size() < mMaxSize)
	{
		mParsedList.PushBack(parsedInteger);
	}
	else
	{
		mParsedList[mMaxSize - 1] = parsedInteger;
	}
}

IntegerSharedData* IntegerSharedData::Clone() const
{
	return new IntegerSharedData();
}

void IntegerSharedData::Initialize()
{
	XmlParseMaster::SharedData::Initialize();
	mState = SharedDataState::END;
	mParsedList.Clear();
	mMaxSize = 1;
}
void IntegerSharedData::SetMaxSize(std::uint32_t size)
{
	mMaxSize = size;
}

