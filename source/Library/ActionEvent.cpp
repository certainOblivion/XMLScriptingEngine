#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "WorldState.h"
#include "World.h"
#include <memory>

using namespace Library;

RTTI_DEFINITIONS(ActionEvent);

ActionEvent::ActionEvent()
{
	float delay = 0.0f;
	std::string subtype = "";
	INTERNAL_ATTRIBUTE("SubType", Datum::DatumType::STRING,&subtype , 1);
	INTERNAL_ATTRIBUTE("Delay", Datum::DatumType::FLOAT, &delay, 1);
	Populate();
}


std::string ActionEvent::GetSubType()
{
	return (*this)["SubType"].GetString();
}

void ActionEvent::SetSubType(std::string subtype)
{
	(*this)["SubType"] = subtype;
}

float ActionEvent::GetDelay()
{
	return (*this)["Delay"].GetFloat();
}

void ActionEvent::SetDelay(float delay)
{
	(*this)["Delay"] = delay;
}

ActionEvent::~ActionEvent()
{
}

void ActionEvent::Update(WorldState& worldState)
{
	EventMessageAttributed message;
	message.SetType(GetSubType());
	message.SetWorld(*(worldState.mWorld));
	
	for (uint32_t i = AuxiliaryBegin(); i < Size(); i++)
	{
		auto& it = GetOrderVector()[i];
		auto& datum = message.AppendAuxiliaryAttribute((*it).first);
		datum = (*it).second;
	}

	std::shared_ptr<Event<EventMessageAttributed>> event = std::make_shared<Event<EventMessageAttributed>>(message);

	worldState.mWorld->GetEventQueue().Enqueue(event, worldState.mGameTime.ElapsedGameTime(), GetDelay());
}