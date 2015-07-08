#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"
#include "WorldState.h"

using namespace Library;

RTTI_DEFINITIONS(ReactionAttributed)

ReactionAttributed::ReactionAttributed()
{
	std::string subtype = "";
	mSignatureVector.Clear();
	INTERNAL_ATTRIBUTE("SubType", Datum::DatumType::STRING, &subtype, 1);
	Populate();
	Event<EventMessageAttributed>::Subscribe(*this);
}

ReactionAttributed::~ReactionAttributed()
{
	Event<EventMessageAttributed>::Unsubscribe(*this);
}

std::string ReactionAttributed::GetSubType()
{
	return (*this)["SubType"].GetString();
}

void ReactionAttributed::SetSubType(std::string subtype)
{
	(*this)["SubType"] = subtype;
}

void ReactionAttributed::Notify(EventPublisher& publisher)
{
	Event<EventMessageAttributed>* attributedEvent = publisher.As<Event<EventMessageAttributed>>();

	if (attributedEvent)
	{
		for (uint32_t i = attributedEvent->Message().AuxiliaryBegin(); i < attributedEvent->Message().Size(); i++)
		{
			auto& it = attributedEvent->Message().GetOrderVector()[i];
			auto& datum = AppendAuxiliaryAttribute((*it).first);
			datum = attributedEvent->Message()[i];
		}
		if (attributedEvent->Message().GetType() == GetSubType())
		{
			ActionList::Update(attributedEvent->Message().GetWorld()->GetWorldState());
		}
	}
}