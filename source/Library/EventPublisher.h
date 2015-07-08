#pragma once
#include "RTTI.H"
#include "SList.h"
#include "EventSubscriber.h"
#include <mutex>
namespace Library
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		/**Constructor.
		@ param Takes a list of subscribers and a boolean indicating whether to delete the event after publishing it*/
		EventPublisher(SList<EventSubscriber*>& subscribers, std::recursive_mutex& mutex);

		/**Copy constructor*/
		EventPublisher(const EventPublisher& rhs);

		/**Overloaded operator=*/
		EventPublisher& operator=(const EventPublisher& rhs);

		/**Destructor*/
		virtual ~EventPublisher() = default;

		/**Sets the current time and the delay times for the current publisher
		* @param currentTime the current time the event is created
		* @param delay for the subscriber to be notified of this event
		*/
		void SetTime(double currentTime, double delay = 0.0);

		/**Return the time that the event was enqueued
		* @return double representing the time the event was enqueued
		*/
		double TimeEnqueued() const;

		/**Return the duration after being enqueued that this event expires
		* @return double representing the duration after being enqueued that this event expires
		*/
		double Delay() const;

		/** Takes the current time and returns true if the event has expired
		* @param double current time
		* @return boolean whether the event is expired
		*/
		bool IsExpired(double currentTime) const;

		/**Notifies all the subscribers of the event*/
		void Deliver();

	private:

		SList<EventSubscriber*>* mSubscriberReference;

		std::double_t mTimeEnqueued;

		std::double_t mDelayTime;

		std::recursive_mutex* mParentMutex;
		mutable std::recursive_mutex mMutex;


	};
}