#include "pch.h"
#include "Sector.h"
#include "Entity.h"
#include "WorldState.h"

using namespace Library;

RTTI_DEFINITIONS(Sector)

Sector::Sector() :Attributed()
{
	Entity* newEntity = nullptr;
	EXTERNAL_ATTRIBUTE("Name", Datum::DatumType::STRING, &mName, 1);
	INTERNAL_ATTRIBUTE("Entities", Datum::DatumType::TABLE, &newEntity , 1);
	Populate();
}

const std::string& Sector::Name() const
{
	return mName;
}

Scope* Sector::Entities()
{
	return Find("Entities")->GetTable();
}

Entity* Sector::CreateEntity(std::string entityName, std::string instanceName)
{
	RTTI* createdRTTI = Factory<Entity>::Create(entityName);

	Entity* createdEntity = createdRTTI->As<Entity>();
	if (createdEntity)
	{
		createdEntity->mName = instanceName;
		Scope* scope = Entities();
		scope->Adopt(*createdEntity,instanceName);
		createdEntity->mParentSector = this;
	}
	else
	{
		throw std::exception("Factory did not produce correct entity");
	}
	return createdEntity;
}

void Sector::SetWorld(World& world)
{
	mWorld = &world;
}

World& Sector::GetWorld()
{
	return *mWorld;
}

void Sector::Update(WorldState& world)
{
	auto * entitiesScope = Entities();
	for (std::uint32_t i = 0; i < entitiesScope->Size(); i++)
	{
		auto* it = (*entitiesScope)[i].GetTable();
		Entity* entity = it->As<Entity>();
		if (entity)
		{
			world.mEntity = entity;
			entity->Update(world);
		}
		else
		{
			throw std::exception("Entities table has something that is not an entity");
		}
	}
}
