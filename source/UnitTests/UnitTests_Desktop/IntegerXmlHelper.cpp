#include "IntegerXmlHelper.h"

using namespace Library;
using namespace UnitTests;

IntegerXmlHelper::IntegerXmlHelper():
IXmlParseHelper(), mReadData(""), mStartElementCounter(0), mEndElementCounter(0), mCharDataCounter(0), mMaxDepth(0)
{
}


IntegerXmlHelper::~IntegerXmlHelper()
{
}

void IntegerXmlHelper::Initialize()
{
	mReadData = "";
	mStartElementCounter = 0;
	mEndElementCounter = 0;
	mCharDataCounter = 0;
	mMaxDepth = 0;	
}

bool IntegerXmlHelper::StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributePairs)
{
	IntegerSharedData* intSharedData = sharedData->As<IntegerSharedData>();

	if ((intSharedData == nullptr) || (elementName != "Integer"))
	{
		return false;
	}
	mStartElementCounter++;

	if (attributePairs.Size() > 0)
	{
		auto it = attributePairs.Find("MaxSize");
		if (it != attributePairs.end())
		{
			std::string size = (*it).second;
			intSharedData->SetMaxSize(atoi(size.c_str()));
		}
	}

	if (intSharedData->mState == IntegerSharedData::SharedDataState::END)
	{
		intSharedData->mState = IntegerSharedData::SharedDataState::START;
	}
	if (mMaxDepth<intSharedData->Depth())
	{
		mMaxDepth = intSharedData->Depth();
	}
	return true;
}

bool IntegerXmlHelper::EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName)
{
	IntegerSharedData* intSharedData = sharedData->As<IntegerSharedData>();

	if ((intSharedData == nullptr) || (elementName != "Integer"))
	{
		return false;
	}
	mEndElementCounter++;
	if (mReadData == "")
	{
		mReadData = "0";
	}
	intSharedData->ParseInteger(mReadData);
	intSharedData->mState = IntegerSharedData::SharedDataState::END;
	return true;
}

bool IntegerXmlHelper::CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer)
{
	IntegerSharedData* intSharedData = sharedData->As<IntegerSharedData>();

	if (intSharedData)
	{
		if ((intSharedData->mState != IntegerSharedData::SharedDataState::START) && (intSharedData->mState != IntegerSharedData::SharedDataState::DATA_READ))
		{
			return false;
		}
		if (intSharedData->mState == IntegerSharedData::SharedDataState::START)
		{
			mReadData = "";
		}
		mCharDataCounter++;
		for (std::uint32_t i = 0; i < lengthOfBuffer; i++)
		{
			mReadData += stringBuffer[i];
		}

		intSharedData->mState = IntegerSharedData::SharedDataState::DATA_READ;
		return true;
	}
	return false;	
}

IXmlParseHelper* IntegerXmlHelper::Clone() const
{
	return new IntegerXmlHelper();
}
