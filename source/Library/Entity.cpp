#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "WorldState.h"

using namespace Library;

RTTI_DEFINITIONS(Entity)

Entity::Entity() :
mParentSector(nullptr), Attributed()
{
	Action* newAction = nullptr;
	EXTERNAL_ATTRIBUTE("Name", Datum::DatumType::STRING, &mName, 1);
	INTERNAL_ATTRIBUTE("Actions", Datum::DatumType::TABLE, &newAction, 1);
	Populate();
}

const std::string& Entity::Name() const
{
	return mName;
}

Sector* Entity::GetSector() const
{
	return mParentSector;
}

void Entity::SetSector(Sector& parentSector)
{
	parentSector.Adopt(*this, mName);
	mParentSector = &parentSector;
}

void Entity::Update(WorldState& world)
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

Scope* Entity::Actions()
{
	return Find("Actions")->GetTable();
}

Action* Entity::CreateAction(std::string actionName, std::string instanceName)
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