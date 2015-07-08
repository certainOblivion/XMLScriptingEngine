#pragma once 

namespace Library
{
	class EventPublisher;
	class EventSubscriber
	{
	public:
		/**Function called when event occurs and the subscriber is being notified. 
		* Pure virtual method, subscriber must derive from this class and implement the function.
		* @param EventPublisher reference that tells the subscriber who the event is being generated from and what kind of event it is.
		*/
		virtual void Notify(EventPublisher& publisher) = 0;
	};
}