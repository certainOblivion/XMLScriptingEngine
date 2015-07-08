#include "pch.h"
#include "ActionCreateAction.h"
#include "ActionList.h"
#include "WorldState.h"

using namespace Library;
RTTI_DEFINITIONS(ActionCreateAction);


ActionCreateAction::ActionCreateAction():
mClassName(""), mInstanceName("")
{
}


void ActionCreateAction::Update(WorldState& world)
{
	if (mClassName == "" || mInstanceName == "")
	{
		throw std::exception("Class and instance name cannot be blank");
	}
	if (!Search(mInstanceName))
	{
		Entity* entity = GetContainer()->As<Entity>();
		if (entity)
		{
			entity->CreateAction(mClassName, mInstanceName);
			return;
		}

		ActionList* actionList = GetContainer()->As<ActionList>();
		if (actionList)
		{
			actionList->CreateAction(mClassName, mInstanceName);
			return;
		}
		throw std::exception("Current scope is not an action or an entity");
	}
}

std::string& ActionCreateAction::Prototype()
{
	return mClassName;
}
std::string& ActionCreateAction::Instance()
{
	return mInstanceName;
}
