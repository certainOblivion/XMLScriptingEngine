#pragma once
#include "XmlParseMaster.h"
#include "Vector.h"
namespace UnitTests
{
	class IntegerSharedData : public Library::XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(IntegerSharedData, Library::XmlParseMaster::SharedData)
	public:
		IntegerSharedData();
		virtual ~IntegerSharedData();
		Library::Vector<std::int32_t>& GetIntegers();
		void ParseInteger(std::string intString);
		virtual IntegerSharedData* Clone() const override;
		virtual void Initialize() override;
		void SetMaxSize(std::uint32_t size);

		enum class SharedDataState
		{
			MINIMUM,
			START,
			DATA_READ,
			END,
			MAX
		}mState;		 

	private:
		Library::Vector<std::int32_t> mParsedList;
		std::uint32_t mMaxSize;
	};
}
