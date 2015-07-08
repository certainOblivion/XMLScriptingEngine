#pragma once
#include "IXmlParseHelper.h"

namespace Library
{
	class Datum;
	class WorldXmlParseHelper final : public IXmlParseHelper
	{
	public:
		/**Default parameterless contructor*/
		WorldXmlParseHelper();

		/**Destructor*/
		~WorldXmlParseHelper() = default;

		/** Will set all of its members to their nascent stage*/
		void Initialize() override;

		/**Function called from XmlMasterParser's StartElementHandler callback function.
		* @param sharedData containing the sharedData object pointer
		* @param elementName string name of the element
		* @param attributePairs Hashmap containing the attribute value pair list in the Xml attribute section
		*/
		bool StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributePairs) override;

		/**Function called from XmlMasterParser's EndElementHandler callback function.
		* @param sharedData containing the sharedData object pointer
		* @param elementName string name of the element
		*/
		bool EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName) override;

		/**Function called from XmlMasterParser's CharDataHandler callback function.
		* @param sharedData containing the sharedData object pointer
		* @param stringBuffer Character data that was parsed from the Xml document
		* @param lengthOfBuffer unsigned int length of the stringBuffer
		*/
		bool CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer) override;

		/**Returns the address of a SharedData object which has the same internal configuration as “this”
		* except ready for a fresh file
		*/
		IXmlParseHelper* Clone() const override;

	private:
		enum class HelperState
		{
			MINIMUM,
			START,
			WORLD_DATA_READ,
			ACTION_DATA_READ,
			ACTIONLIST_DATA_READ,
			SECTOR_DATA_READ,
			ENTITY_DATA_READ,
			INTEGER_DATA_READ,
			FLOAT_DATA_READ,
			VECTOR_DATA_READ,
			MATRIX_DATA_READ,
			STRING_DATA_READ,
			END,
			MAX
		}mState;

		std::string mReadData;
		Datum* mCurrentDatum;
		std::string mCurrentDatumName;
		std::uint32_t mCurrentDatumIndex;
	};
}


