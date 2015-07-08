#pragma once
#include "SList.h"
#include <memory>
#include <mutex>
#include <atomic>
namespace Library
{
	class EventPublisher;
	class EventQueue
	{
	public:
		/**Default parameterless constructor*/
		EventQueue();
		
		/**Destructor*/
		~EventQueue() = default;

		/**Given the address of an EventPublisher, current time, and an optional delay time, either enqueue the event or deliver it immediately
		* @param event publisher that will be actually be the event
		* @param currentTime denoting the time the event has been logged
		* @param delay time before the message should be delivered
		*/
		void Enqueue(const std::shared_ptr<EventPublisher> publisher, std::double_t currentTime, std::double_t delay = 0.0);

		/** Given the address of an EventPublisher, send the event immediately
		* @param the event to be published
		*/
		void Send(const std::shared_ptr<EventPublisher> publisher);

		/**Given the current time, publish any queued events that have expired.
		* @param currentTime
		*/
		void Update(std::double_t currentTime);

		/**Given the current time, publish any queued events that have expired and then clear the event queue
		* @param currentTime
		*/
		void Clear(std::double_t currentTime);

		/**Returns true if there are no events in the event queue
		* @return bool true if the event queue is empty
		*/
		bool IsEmpty();

		/**Returns the number of events in the event queue
		* @return unsigned integer the number of events
		*/
		std::uint32_t Count();

	private:
		SList<std::shared_ptr<EventPublisher>> mEventQueue;
		mutable std::recursive_mutex mMutex;
		std::atomic<bool> mIsUpdateRunning;
	};
}
