#include "pch.h"
#include "TableXmlParseHelper.h"
#include "ScopeSharedData.h"
#include "Datum.h"

using namespace Library;

TableXmlParseHelper::TableXmlParseHelper():
IXmlParseHelper(), mReadData(""), mState(HelperState::END)
{
}

void TableXmlParseHelper::Initialize()
{
	mReadData = "";
}

bool TableXmlParseHelper::StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributePairs)
{
	ScopeSharedData* tableSharedData = sharedData->As<ScopeSharedData>();

	if (!tableSharedData)
	{
		return false;
	}

	if (tableSharedData->GetParsedScope() == nullptr)
	{
		throw std::exception("Shared Data class needs to have a valid pointer to a scope.");
	}

	if ((elementName != "value") && (elementName != "Integer") && (elementName != "Float") && (elementName != "Matrix") && (elementName != "Vector") && (elementName != "String") && (elementName != "Scope"))
	{
		return false;
	}

	if ((elementName != "value") && (elementName != "Scope") && (tableSharedData->mCurrentScope == nullptr))
	{
		return false;
	}

	if (mState == HelperState::END)
	{
		mState = HelperState::START;
	}

	if ((elementName == "value") && (mState == HelperState::START))
	{
		return false;
	}

	if (elementName != "value")
	{
		if (mState != HelperState::START)
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string nameOfAttribute;

		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				nameOfAttribute = (*it).second;
			}
			else
			{
				throw std::exception("Element needs a name to be parsed!");
			}
		}

		if ((nameOfAttribute.length() == 0) && (tableSharedData->mCurrentScope != nullptr) && (elementName != "Scope"))
		{
			throw std::exception("Name of element cannot be of zero length!");
		}

		mCurrentDatumName = nameOfAttribute;
	}
	else
	{
		std::uint32_t datumIndex = 0;

		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("index");
			if (it != attributePairs.end())
			{
				datumIndex = static_cast<std::uint32_t>(atoi((*it).second.c_str()));
			}
		}

		mCurrentDatumIndex = datumIndex;
	}

	if (elementName == "Integer")
	{
		mCurrentDatum = &(tableSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::INTEGER);

		mState = HelperState::INTEGER_DATA_READ;
	}

	if (elementName == "Float")
	{
		mCurrentDatum = &(tableSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::FLOAT);

		mState = HelperState::FLOAT_DATA_READ;
	}

	if (elementName == "Matrix")
	{
		mCurrentDatum = &(tableSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::MATRIX);

		mState = HelperState::MATRIX_DATA_READ;
	}

	if (elementName == "Vector")
	{
		mCurrentDatum = &(tableSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::VECTOR);

		mState = HelperState::VECTOR_DATA_READ;
	}

	if (elementName == "String")
	{
		mCurrentDatum = &(tableSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::STRING);

		mState = HelperState::STRING_DATA_READ;
	}

	if (elementName == "Scope")
	{
		tableSharedData->AddNestedScope(mCurrentDatumName);
		mState = HelperState::START;
	}

	mReadData = "";
	return true;
}

bool TableXmlParseHelper::EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName)
{
	ScopeSharedData* tableSharedData = sharedData->As<ScopeSharedData>();
	if ((tableSharedData == nullptr))
	{
		return false;
	}

	if ((elementName != "Scope") && (mCurrentDatum == nullptr))
	{
		return false;
	}

	if ((elementName != "Scope") && (elementName != "value"))
	{
		if (mState == HelperState::START)
		{
			return false;
		}
	}

	if (elementName == "value")
	{
		mCurrentDatum->SetFromString(mReadData, mCurrentDatumIndex);
	}
	else 
	{
		if (elementName == "Scope")
		{
			if (mState == HelperState::END)
			{
				tableSharedData->mCurrentScope = tableSharedData->mCurrentScope->GetParent();
			}
		}
		else
		{
			mState = HelperState::END;
		}

		mCurrentDatum = nullptr;
		mCurrentDatumName = "";
		mCurrentDatumIndex = 0;
	}

	return true;
}

bool TableXmlParseHelper::CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer)
{
	ScopeSharedData* tableSharedData = sharedData->As<ScopeSharedData>();
	if (tableSharedData)
	{
		if ((mState < HelperState::START) || (mState >= HelperState::END))
		{
			return false;
		}

		for (std::uint32_t i = 0; i < lengthOfBuffer; i++)
		{
			mReadData += stringBuffer[i];
		}

		return true;
	}
	return false;
}

IXmlParseHelper* TableXmlParseHelper::Clone() const
{
	return new TableXmlParseHelper();
}