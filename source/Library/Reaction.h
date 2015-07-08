#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public EventSubscriber
	{
		/**Overridden Update
		* @param worldstate reference
		*/
		virtual void Update(WorldState& worldState) override;

		/**Overridden Notify 
		* @param eventpublisher reference
		*/
		virtual void Notify(EventPublisher& publisher) override = 0;
	};
}
