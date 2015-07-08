#include "pch.h"
#include "WorldSharedData.h"
#include "World.h"
#include "Sector.h"
#include "Action.h"
#include "ActionList.h"

using namespace Library;


RTTI_DEFINITIONS(WorldSharedData)

WorldSharedData::WorldSharedData(World& world) :
mWorld(&world), bIsClone(false), mCurrentScope(&world)
{
}


WorldSharedData::~WorldSharedData()
{
	if (bIsClone)
	{
		delete mWorld;
	}
}

WorldSharedData* WorldSharedData::Clone() const
{
	World* createdWorld = new World();
	WorldSharedData* clonedSharedData = new WorldSharedData(*createdWorld);
	clonedSharedData->bIsClone = true;
	return clonedSharedData;
}

void WorldSharedData::Initialize()
{
	mWorld->Sectors()->Clear();
	mCurrentScope = mWorld;
}

World* WorldSharedData::GetParsedWorld()
{
	return mWorld;
}

void WorldSharedData::AddEntity(std::string className, std::string instanceName)
{
	Sector* currentSector = mCurrentScope->As<Sector>();
	if (currentSector != nullptr)
	{
		Entity* createdEntity = currentSector->CreateEntity(className,instanceName);
		mCurrentScope = (Scope*)createdEntity;
	}
	else
	{
		throw std::exception("Cannot create the Entity since it is not inside a sector");
	}
}

void WorldSharedData::AddSector(std::string className)
{
	World* currentWorld = mCurrentScope->As<World>();
	if (currentWorld != nullptr)
	{
		Sector* createdSector = currentWorld->CreateSector(className);
		mCurrentScope = (Scope*)createdSector;
	}
	else
	{
		throw std::exception("Cannot create the Sector since it is not inside a World");
	}
}

void WorldSharedData::AddAction(std::string className, std::string name)
{
	Entity* entity = mCurrentScope->As<Entity>();
	if (entity)
	{
		mCurrentScope = entity->CreateAction(className, name);		
		return;
	}

	ActionList* actionList = mCurrentScope->As<ActionList>();
	if (actionList)
	{
		mCurrentScope = actionList->CreateAction(className, name);
		return;
	}
	throw std::exception("Current scope is not an action or an entity");

}

void WorldSharedData::AddActionList(std::string name)
{
	Entity* entity = mCurrentScope->As<Entity>();
	if (entity)
	{
		mCurrentScope = entity->CreateAction("ActionList", name);
	}
	else
	{
		throw std::exception("Cannot create actionList since the current scope is not an entity");
	}
}