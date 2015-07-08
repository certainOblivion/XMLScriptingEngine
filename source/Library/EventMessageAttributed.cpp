#include "pch.h"
#include "EventMessageAttributed.h"
#include "World.h"

using namespace Library;

RTTI_DEFINITIONS(EventMessageAttributed);

std::string EventMessageAttributed::GetType()
{
	return mSubType;
}

EventMessageAttributed::EventMessageAttributed(const EventMessageAttributed& rhs):
Attributed(rhs), mSubType(rhs.mSubType), mWorld(rhs.mWorld)
{
	EXTERNAL_ATTRIBUTE("SubType", Datum::DatumType::STRING, &mSubType, 1);
}

EventMessageAttributed& EventMessageAttributed::operator=(const EventMessageAttributed& rhs)
{
	if (this != &rhs)
	{
		mSubType = rhs.mSubType;
		mWorld = rhs.mWorld;
		Attributed::operator=(rhs);
	}

	return *this;
}

EventMessageAttributed::EventMessageAttributed()
{
	EXTERNAL_ATTRIBUTE("SubType", Datum::DatumType::STRING, &mSubType, 1);
	Populate();
}

void EventMessageAttributed::SetType(std::string type)
{
	mSubType = type;
}

World* EventMessageAttributed::GetWorld()
{
	return mWorld;
}

void EventMessageAttributed::SetWorld(World& world)
{
	mWorld = &world;
}
