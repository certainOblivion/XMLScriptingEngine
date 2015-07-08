#include "pch.h"
#include "World.h"
#include "Sector.h"

using namespace Library;

RTTI_DEFINITIONS(World)

World::World() : Attributed(), mWorldState(this)
{
	Scope* newSector = nullptr;
	EXTERNAL_ATTRIBUTE("Name", Datum::DatumType::STRING, &mName, 1);
	INTERNAL_ATTRIBUTE("Sectors", Datum::DatumType::TABLE, &newSector, 1);
	Populate();
}

const std::string& World::Name() const
{
	return mName;
}

void World::SetName(std::string name)
{
	mName = name;
}

Scope* World::Sectors()
{
	return Find("Sectors")->GetTable();
}

Sector* World::CreateSector(std::string instanceName)
{
	Sector* createdSector = new Sector();
	createdSector->mName = instanceName;
	createdSector->SetWorld(*this);

	Sectors()->Adopt(*createdSector, instanceName);
	return createdSector;
}

void World::Update(WorldState& world)
{
	auto * sectorsScope = Sectors();
	for (std::uint32_t i = 0; i < sectorsScope->Size(); i++)
	{
		auto* it = (*sectorsScope)[i].GetTable();
		Sector* sector = it->As<Sector>();
		if (sector)
		{
			world.mSector = sector;
			sector->Update(world);
		}
		else
		{
			throw std::exception("Sectors table has something that is not an sector");
		}
	}
}

WorldState& World::GetWorldState()
{
	return mWorldState;
}

EventQueue& World::GetEventQueue()
{
	return mEventQueue;
}