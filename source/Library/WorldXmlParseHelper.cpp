#include "pch.h"
#include "WorldXmlParseHelper.h"
#include "WorldSharedData.h"
#include "World.h"
#include "Datum.h"
#include "Entity.h"
#include "Sector.h"
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionEvent.h"
#include "ReactionAttributed.h"
using namespace Library;

WorldXmlParseHelper::WorldXmlParseHelper() :
IXmlParseHelper(), mReadData(""), mState(HelperState::END)
{
}

void WorldXmlParseHelper::Initialize()
{
	mReadData = "";
}

bool WorldXmlParseHelper::StartElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName, const Library::Hashmap<std::string, std::string>& attributePairs)
{
	WorldSharedData* worldSharedData = sharedData->As<WorldSharedData>();

	if (!worldSharedData)
	{
		return false;
	}

	if (worldSharedData->GetParsedWorld() == nullptr)
	{
		throw std::exception("Shared Data class needs to have a valid pointer to a world.");
	}

	if ((elementName != "value") 
		&& (elementName != "Integer") 
		&& (elementName != "Float") 
		&& (elementName != "Matrix") 
		&& (elementName != "Vector") 
		&& (elementName != "String") 
		&& (elementName != "World") 
		&& (elementName != "Entity") 
		&& (elementName != "Sector") 
		&& (elementName != "Action") 
		&& (elementName != "ActionList") 
		&& (elementName != "ActionCreateAction") 
		&& (elementName != "ActionDestroyAction"))
	{
		return false;
	}

	if ((elementName != "value") && (elementName != "World") && (elementName != "Sector") && (elementName != "Entity") && (worldSharedData->mCurrentScope == nullptr))
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

	if ((elementName == "Integer") || (elementName == "Float") || (elementName == "Vector") || (elementName == "Matrix") || (elementName == "String"))
	{
		if ((mState < HelperState::WORLD_DATA_READ) || (mState > HelperState::ENTITY_DATA_READ))
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

		if ((nameOfAttribute.length() == 0) && (worldSharedData->mCurrentScope != nullptr) && (elementName != "Scope"))
		{
			throw std::exception("Name of element cannot be of zero length!");
		}

		mCurrentDatumName = nameOfAttribute;
	}
	else if ((elementName == "World") || (elementName == "Sector") || (elementName == "Entity") || (elementName == "Action") || (elementName == "ActionList"))
	{
		if ((mState < HelperState::START) || (mState > HelperState::ENTITY_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
	}
	else if (elementName == "value")
	{
		if ((mState < HelperState::INTEGER_DATA_READ) || (mState > HelperState::STRING_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
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

	if (elementName == "ActionCreateAction")
	{
		if (mState!=HelperState::ENTITY_DATA_READ && mState!=HelperState::ACTIONLIST_DATA_READ)
		{
			return false;
		}
		std::string instanceName;
		std::string className;
		std::string actionName;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("instanceName");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from Action");
			}
			it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				actionName = (*it).second;
			}
			else
			{
				throw std::exception("Action name is missing from Action");
			}
			it = attributePairs.Find("className");
			if (it != attributePairs.end())
			{
				className = (*it).second;
			}
			else
			{
				throw std::exception("Class name is missing from Action");
			}
		}
		else
		{
			throw std::exception("Class, action and instance name is missing from Action");
		}

		ActionCreateAction* createdAction = new ActionCreateAction();
		createdAction->Instance() = instanceName;
		createdAction->Prototype() = className;

		ActionList* actionList = worldSharedData->mCurrentScope->As<ActionList>();
		if (actionList)
		{
			actionList->Actions()->Adopt(*createdAction, actionName);
		}

		Entity* entity = worldSharedData->mCurrentScope->As<Entity>();
		if (entity)
		{
			entity->Actions()->Adopt(*createdAction, actionName);
		}
	}

	if (elementName == "ActionDestroyAction")
	{
		if (mState != HelperState::ENTITY_DATA_READ && mState != HelperState::ACTIONLIST_DATA_READ)
		{
			return false;
		}
		std::string instanceName;
		std::string actionName;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("instanceName");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from Action");
			}

			it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				actionName = (*it).second;
			}
			else
			{
				throw std::exception("Action name is missing from Action");
			}
			
		}
		else
		{
			throw std::exception("Instance name is missing from Action");
		}

		ActionDestroyAction* createdAction = new ActionDestroyAction();
		createdAction->Instance() = instanceName;

		ActionList* actionList = worldSharedData->mCurrentScope->As<ActionList>();
		if (actionList)
		{
			actionList->Actions()->Adopt(*createdAction, actionName);
		}

		Entity* entity = worldSharedData->mCurrentScope->As<Entity>();
		if (entity)
		{
			entity->Actions()->Adopt(*createdAction, actionName);
		}
	}

	if (elementName == "Integer")
	{
		mCurrentDatum = &(worldSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::INTEGER);

		mState = HelperState::INTEGER_DATA_READ;
	}

	if (elementName == "Float")
	{
		mCurrentDatum = &(worldSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::FLOAT);

		mState = HelperState::FLOAT_DATA_READ;
	}

	if (elementName == "Matrix")
	{
		mCurrentDatum = &(worldSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::MATRIX);

		mState = HelperState::MATRIX_DATA_READ;
	}

	if (elementName == "Vector")
	{
		mCurrentDatum = &(worldSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::VECTOR);

		mState = HelperState::VECTOR_DATA_READ;
	}

	if (elementName == "String")
	{
		mCurrentDatum = &(worldSharedData->mCurrentScope)->Append(mCurrentDatumName, Datum::DatumType::STRING);

		mState = HelperState::STRING_DATA_READ;
	}

	if (elementName == "World")
	{
		if ((mState != HelperState::START))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string name;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				name = (*it).second;
			}
			else
			{
				throw std::exception("Name is missing from World");
			}
		}
		else
		{
			throw std::exception("Instance name is missing from Sector");
		}
		worldSharedData->Initialize();
		worldSharedData->mWorld->SetName(name);
		mState = HelperState::WORLD_DATA_READ;
	}

	if (elementName == "Sector")
	{
		if ((mState != HelperState::WORLD_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string instanceName;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from Sector");
			}
		}
		else
		{
			throw std::exception("Instance name is missing from Sector");
		}
		worldSharedData->AddSector(instanceName);
		mState = HelperState::SECTOR_DATA_READ;
	}
	if (elementName == "Entity")
	{
		if ((mState != HelperState::SECTOR_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string instanceName;
		std::string className;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from Entity");
			}
			it = attributePairs.Find("className");
			if (it != attributePairs.end())
			{
				className = (*it).second;
			}
			else
			{
				throw std::exception("Class name is missing from Entity");
			}
		}
		else
		{
			throw std::exception("Class and instance name is missing from Entity");
		}
		worldSharedData->AddEntity(className, instanceName);
		mState = HelperState::ENTITY_DATA_READ;
	}
	if (elementName == "Action")
	{
		if ((mState != HelperState::ENTITY_DATA_READ) && (mState != HelperState::ACTIONLIST_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string instanceName;
		std::string className;
		std::string subtype;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from Action");
			}
			it = attributePairs.Find("className");
			if (it != attributePairs.end())
			{
				className = (*it).second;
			}
			else
			{
				throw std::exception("Class name is missing from Action");
			}

			it = attributePairs.Find("subtype");
			if (it != attributePairs.end())
			{
				subtype = (*it).second;
			}
		}
		else
		{
			throw std::exception("Class and instance name is missing from Action");
		}
		worldSharedData->AddAction(className, instanceName);
		if (className == "ActionEvent")
		{
			ActionEvent* actionEvent = worldSharedData->mCurrentScope->As<ActionEvent>();
			if (actionEvent && subtype.size()>0)
			{
				actionEvent->SetSubType(subtype);
			}
			else
			{
				if (!actionEvent)
				{
					throw std::exception("ActionEvent class was not generated!");
				}
				else if (subtype.size() == 0)
				{
					throw std::exception("Subtype string is empty!");
				}
			}
		}
		else if (className == "ReactionAttributed")
		{
			ReactionAttributed* reactionAttr = worldSharedData->mCurrentScope->As<ReactionAttributed>();
			if (reactionAttr && subtype.size()>0)
			{
				reactionAttr->SetSubType(subtype);
			}
			else
			{
				if (!reactionAttr)
				{
					throw std::exception("ReactionAttributed class was not generated!");
				}
				else if (subtype.size() == 0)
				{
					throw std::exception("Subtype string is empty!");
				}
			}
		}

		mState = HelperState::ACTION_DATA_READ;
	}

	if (elementName == "ActionList")
	{
		if ((mState != HelperState::ENTITY_DATA_READ))
		{
			mState = HelperState::END;
			mCurrentDatum = nullptr;
			mCurrentDatumIndex = 0;
			mCurrentDatumName = "";
			return false;
		}
		std::string instanceName;
		if (attributePairs.Size() > 0)
		{
			auto it = attributePairs.Find("name");
			if (it != attributePairs.end())
			{
				instanceName = (*it).second;
			}
			else
			{
				throw std::exception("Instance name is missing from ActionList");
			}
		}
		else
		{
			throw std::exception("Instance name is missing from ActionList");
		}
		worldSharedData->AddActionList(instanceName);
		mState = HelperState::ACTIONLIST_DATA_READ;
	}

	mReadData = "";
	return true;
}

bool WorldXmlParseHelper::EndElementHandler(Library::XmlParseMaster::SharedData* sharedData, const std::string& elementName)
{
	WorldSharedData* worldSharedData = sharedData->As<WorldSharedData>();
	if ((worldSharedData == nullptr))
	{
		return false;
	}

	if (((elementName != "World") && (elementName != "Sector") && (elementName != "Entity") && (elementName != "Action") && (elementName != "ActionList")) && (mCurrentDatum == nullptr))
	{
		return false;
	}

	if (((elementName != "World") && (elementName != "Sector") && (elementName != "Entity") && (elementName != "Action") && (elementName != "ActionList")) && (elementName != "value"))
	{
		if (mState == HelperState::START)
		{
			return false;
		}
	}

	if (elementName == "value")
	{
		if ((mState < HelperState::INTEGER_DATA_READ) || (mState > HelperState::STRING_DATA_READ))
		{
			return false;
		}
		mCurrentDatum->SetFromString(mReadData, mCurrentDatumIndex);
	}
	else
	{
		if (elementName == "Action")
		{
			worldSharedData->mCurrentScope = (Scope*)worldSharedData->mCurrentScope->As<Action>()->GetContainer();
			ActionList* actionList = worldSharedData->mCurrentScope->As<ActionList>();
			if (actionList)
			{
				mState = HelperState::ACTIONLIST_DATA_READ;
			}

			Entity* entity = worldSharedData->mCurrentScope->As<Entity>();
			if (entity)
			{
				mState = HelperState::ENTITY_DATA_READ;
			}
		}
		else if (elementName == "ActionList")
		{
			mState = HelperState::ENTITY_DATA_READ;
			worldSharedData->mCurrentScope = (Scope*)worldSharedData->mCurrentScope->As<ActionList>()->GetContainer();
		}
		else if (elementName == "Entity")
		{
			mState = HelperState::SECTOR_DATA_READ;
			worldSharedData->mCurrentScope = (Scope*)worldSharedData->mCurrentScope->As<Entity>()->GetSector();
		}
		else if (elementName == "Sector")
		{
			mState = HelperState::WORLD_DATA_READ;
			worldSharedData->mCurrentScope = (Scope*)&(worldSharedData->mCurrentScope->As<Sector>()->GetWorld());
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

bool WorldXmlParseHelper::CharDataHandler(Library::XmlParseMaster::SharedData* sharedData, const char* stringBuffer, std::uint32_t lengthOfBuffer)
{
	WorldSharedData* worldSharedData = sharedData->As<WorldSharedData>();
	if (worldSharedData)
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

IXmlParseHelper* WorldXmlParseHelper::Clone() const
{
	return new WorldXmlParseHelper();
}
