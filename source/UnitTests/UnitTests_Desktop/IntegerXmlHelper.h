#pragma once
#include "IXmlParseHelper.h"
#include "IntegerSharedData.h"
#include "XmlParseMaster.h"

namespace UnitTests
{
	class IntegerXmlHelper : public Library::IXmlParseHelper
	{
	public:
		IntegerXmlHelper();
		virtual ~IntegerXmlHelper();

		virtual void Initialize() override;

		virtual bool StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributePairs) override;

		virtual bool EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName) override;

		virtual bool CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer) override;

		virtual IXmlParseHelper* Clone() const override;

		std::uint32_t mStartElementCounter;
		std::uint32_t mEndElementCounter;
		std::uint32_t mCharDataCounter;
		std::uint32_t mMaxDepth;
	private:
		std::string mReadData;

	};
}

