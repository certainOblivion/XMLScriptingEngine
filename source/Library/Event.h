#pragma once 
#include "EventPublisher.h"

namespace Library
{
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher);
	public:
		/**Constructor.
		* @param const reference to a message object
		* @param boolean indicating whether to delete the event after publishing it
		*/
		Event(const T& message);

		/**Given the address of an EventSubscriber, add it to the list of subscribers for this event type
		@param subscriber reference
		*/
		static void Subscribe(EventSubscriber& eventSubscriber);

		/**Given the address of an EventSubscriber, remove it from the list of subscribers for this event type
		@param subscriber reference
		*/
		static void Unsubscribe(EventSubscriber& eventSubscriber);

		/**Remove all the subscribers from the list of subscribers for this event type*/
		static void UnsubscribeAll();

		/**Returns message object.
		* @return message reference
		*/
		T& Message();

		Event<T>& operator=(const Event<T>& rhs) = delete;
		Event(Event& rhs) = delete;
	private:
		T mMessage;
		static SList<EventSubscriber*> mSubscribers;
		static std::recursive_mutex mMutex;
	};
}

#include "Event.inl"