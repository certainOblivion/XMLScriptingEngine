#include "pch.h"
#include "WorldState.h"
#include "ActionList.h"

using namespace Library;

RTTI_DEFINITIONS(ActionList);

void ActionList::Update(WorldState& world)
{
	auto * actions = Actions();
	for (std::uint32_t i = 0; i < actions->Size(); i++)
	{
		auto* it = (*actions)[i].GetTable();
		Action* action = it->As<Action>();
		if (action)
		{
			world.mAction = action;
			action->Update(world);
		}
		else
		{
			throw std::exception("Actions table has something that is not an action");
		}
	}
}


ActionList::ActionList()
{
	Action* newAction = nullptr;
	EXTERNAL_ATTRIBUTE("Name", Datum::DatumType::STRING, &mName, 1);
	INTERNAL_ATTRIBUTE("Actions", Datum::DatumType::TABLE, &newAction, 1);
	Populate();
}

Scope* ActionList::Actions()
{
	return Find("Actions")->GetTable();
}

Action* ActionList::CreateAction(std::string actionName, std::string instanceName)
{
	Action* createdAction = Factory<Action>::Create(actionName);
	if (createdAction)
	{
		createdAction->mName = instanceName;
		Scope* scope = Actions();
		scope->Adopt(*createdAction, instanceName);
	}
	else
	{
		throw std::exception("Factory did not produce correct action");
	}
	return createdAction;
}
