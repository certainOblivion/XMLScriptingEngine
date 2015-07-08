#include "pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"

using namespace Library;

RTTI_DEFINITIONS(XmlParseMaster::SharedData)

#pragma region XmlParseMaster

XmlParseMaster::XmlParseMaster(SharedData* sharedData):
mSharedData(sharedData), mIsClone(false), mFileName(std::string("")), mXmlParser(XML_ParserCreate("UTF-8")), mHelperList()
{
	Reset();
	sharedData->SetXmlParseMaster(this);
}

XmlParseMaster::~XmlParseMaster()
{
	if (mIsClone)
	{
		delete mSharedData;

		for (auto it : mHelperList)
		{
			delete it;
			it = nullptr;
		}
	}

	XML_ParserFree(mXmlParser);
}

void XmlParseMaster::Parse(const char *buffer, std::uint32_t length, bool isFirst, bool isFinal)
{
	if (isFirst)
	{
		Reset();
	}
	if (XML_Parse(mXmlParser, buffer, length, isFinal) == XML_STATUS_ERROR)
	{
		throw std::exception("Bad XML data");
	}
}

void XmlParseMaster::ParseFromFile(const std::string& fileName)
{
	mFileName = fileName;

	if (mFileName.empty())
	{
		throw std::exception("Empty file name");
	}

	std::fstream file;
	file.open(fileName.c_str(), std::fstream::in);

	if (file.is_open())
	{
		std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		Parse(fileData.c_str(), fileData.length(), true, true);
	}
}

XmlParseMaster* XmlParseMaster::Clone() const
{ 
	auto clone = new XmlParseMaster((mSharedData == nullptr) ? nullptr : (mSharedData->Clone()));

	for (auto it : mHelperList)
	{
		clone->AddHelper(*(*it).Clone());
	}
	clone->mIsClone = true;
	return clone;
}

void XmlParseMaster::AddHelper(IXmlParseHelper& helper)
{
	mHelperList.PushBack(&helper);
}

void XmlParseMaster::RemoveHelper(IXmlParseHelper& helper)
{
	mHelperList.Remove(&helper);
}

std::string XmlParseMaster::GetFileName() const
{
	return mFileName;
}

XmlParseMaster::SharedData* XmlParseMaster::GetSharedData()
{
	return mSharedData;
}

void XmlParseMaster::SetSharedData(SharedData* sharedData)
{
	mSharedData = sharedData;
}

void XmlParseMaster::StartElementHandler(void *userData, const char *name, const char **atts)
{
	if (userData)
	{
		SharedData* sharedData = reinterpret_cast<SharedData*>(userData);
		std::string elementName(name);
		sharedData->IncrementDepth();

		Hashmap<std::string, std::string> readData;
		int i = 0;
		while (atts[i])
		{
			readData.Insert(std::string(atts[i]), std::string(atts[i + 1]));
			i += 2;
		}

		for (auto it : sharedData->GetXmlParseMaster()->mHelperList)
		{
			if (it->StartElementHandler(sharedData, elementName, readData))
			{
				break;
			}
		}
	}
	else
	{
		throw std::exception("User Data received from Expat is a nullptr.");
	}
}

void XmlParseMaster::EndElementHandler(void *userData, const char *name)
{
	SharedData* sharedData = reinterpret_cast<SharedData*>(userData);
	std::string elementName(name);
	sharedData->DecrementDepth();

	for (auto it : sharedData->GetXmlParseMaster()->mHelperList)
	{
		if (it->EndElementHandler(sharedData, elementName))
		{
			break;
		}
	}
}

void XmlParseMaster::CharDataHandler(void *userData, const char *stringData, int length)
{
	SharedData* sharedData = reinterpret_cast<SharedData*>(userData);

	for (auto it : sharedData->GetXmlParseMaster()->mHelperList)
	{
		if (it->CharDataHandler(sharedData, stringData, length))
		{
			break;
		}
	}
}

void XmlParseMaster::Reset()
{
	if (mIsClone)
	{
		SharedData* newSharedData = mSharedData->Clone();
		delete mSharedData;
		SetSharedData(newSharedData);
		mSharedData->SetXmlParseMaster(this);
	}
	else
	{
		if (mSharedData)
		{
			mSharedData->Initialize();
		}
		
	}
	XML_ParserReset(mXmlParser, "UTF-8");
	XML_SetStartElementHandler(mXmlParser, StartElementHandler);
	XML_SetEndElementHandler(mXmlParser, EndElementHandler);
	XML_SetCharacterDataHandler(mXmlParser, CharDataHandler);
	XML_SetUserData(mXmlParser, reinterpret_cast<void*>(mSharedData));

	for (auto it : mHelperList)
	{
		it->Initialize();
	}
}

#pragma endregion

#pragma region XmlParseMaster::SharedData

XmlParseMaster::SharedData::SharedData():
mDepth(0), mXmlParseMaster(nullptr)
{

}

XmlParseMaster::SharedData::~SharedData()
{
	mDepth = 0;
	mXmlParseMaster = nullptr;
}

void XmlParseMaster::SharedData::Initialize()
{
	mDepth = 0;
}

XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone() const
{
	return new SharedData();
}

XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const 
{
	return mXmlParseMaster;
}

void XmlParseMaster::SharedData::IncrementDepth()
{
	mDepth++;
}

void XmlParseMaster::SharedData::DecrementDepth()
{
	if (mDepth>0)
	{
		mDepth--;
	}
	else
	{
		throw std::exception("Depth mismatch! Should not go below zero.");
	}
	
}

std::uint32_t XmlParseMaster::SharedData::Depth() const
{
	return mDepth;
}

void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* parseMaster)
{
	mXmlParseMaster = parseMaster;
}
#pragma endregion