#include <cxxtest/TestSuite.h>
#include "Event.h"
#include "EventPublisher.h"
#include "EventQueue.h"
#include "FooSubscriber.h"
#include "FooMessage.h"
#include <crtdbg.h>

using namespace Library;
using namespace UnitTests;

class EventTestSuite : public CxxTest::TestSuite
{
public:
	void tearDown()
	{
		Event<FooMessage>::UnsubscribeAll();
	}

	void TestEventGeneration()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher = std::make_shared<Event<FooMessage>>(message);
		FooSubscriber subscriber;
		Event<FooMessage>::Subscribe(subscriber);
		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher, 0.0);

		TS_ASSERT(subscriber.HasBeenNotified() == false);
		TS_ASSERT(message.mCalls == 0);
		eventQueue.Update(1.0);
		TS_ASSERT(subscriber.HasBeenNotified() == true);
		Event<FooMessage>* event = eventPublisher->As<Event<FooMessage>>();
		TS_ASSERT(event->Message().mCalls == 1);
	}

	void TestMultipleMessages()
	{
		FooMessage message1;
		std::shared_ptr<EventPublisher> eventPublisher1 = std::make_shared<Event<FooMessage>>(message1);

		FooMessage message2;
		std::shared_ptr<EventPublisher> eventPublisher2 = std::make_shared<Event<FooMessage>>(message2);
		FooSubscriber subscriber1;
		FooSubscriber subscriber2;

		Event<FooMessage>::Subscribe(subscriber1);
		Event<FooMessage>::Subscribe(subscriber2);

		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher1, 0.0);
		eventQueue.Enqueue(eventPublisher2, 0.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == false);
		TS_ASSERT(subscriber2.HasBeenNotified() == false);
		Event<FooMessage>* event1 = eventPublisher1->As<Event<FooMessage>>();
		Event<FooMessage>* event2 = eventPublisher2->As<Event<FooMessage>>();
		TS_ASSERT(event1->Message().mCalls == 0);
		TS_ASSERT(event2->Message().mCalls == 0);

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == true);
		TS_ASSERT(subscriber2.HasBeenNotified() == true);
		
		TS_ASSERT(event1->Message().mCalls == 2);
		TS_ASSERT(event2->Message().mCalls == 2);
	}

	void TestSubscribe()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher = std::make_shared<Event<FooMessage>>(message);
		FooSubscriber subscriber1;
		FooSubscriber subscriber2;
		FooSubscriber subscriber3;
		FooSubscriber subscriber4;

		Event<FooMessage>::Subscribe(subscriber1);
		Event<FooMessage>::Subscribe(subscriber2);
		Event<FooMessage>::Subscribe(subscriber3);
		Event<FooMessage>::Subscribe(subscriber4);

		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher, 0.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == false);
		TS_ASSERT(subscriber2.HasBeenNotified() == false);
		TS_ASSERT(subscriber3.HasBeenNotified() == false);
		TS_ASSERT(subscriber4.HasBeenNotified() == false);

		Event<FooMessage>* event = eventPublisher->As<Event<FooMessage>>();

		TS_ASSERT(event->Message().mCalls == 0);

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == true);
		TS_ASSERT(subscriber2.HasBeenNotified() == true);
		TS_ASSERT(subscriber3.HasBeenNotified() == true);
		TS_ASSERT(subscriber4.HasBeenNotified() == true);
		TS_ASSERT(event->Message().mCalls == 4);
	}

	void TestUnSubscribe()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher = std::make_shared<Event<FooMessage>>(message);
		FooSubscriber subscriber1;
		FooSubscriber subscriber2;
		FooSubscriber subscriber3;
		FooSubscriber subscriber4;

		Event<FooMessage>::Subscribe(subscriber1);
		Event<FooMessage>::Subscribe(subscriber2);
		Event<FooMessage>::Subscribe(subscriber3);
		Event<FooMessage>::Subscribe(subscriber4);

		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher, 0.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == false);
		TS_ASSERT(subscriber2.HasBeenNotified() == false);
		TS_ASSERT(subscriber3.HasBeenNotified() == false);
		TS_ASSERT(subscriber4.HasBeenNotified() == false);
		Event<FooMessage>* event = eventPublisher->As<Event<FooMessage>>();
		TS_ASSERT(event->Message().mCalls == 0);

		Event<FooMessage>::Unsubscribe(subscriber1);
		Event<FooMessage>::Unsubscribe(subscriber4);

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber1.HasBeenNotified() == false);
		TS_ASSERT(subscriber2.HasBeenNotified() == true);
		TS_ASSERT(subscriber3.HasBeenNotified() == true);
		TS_ASSERT(subscriber4.HasBeenNotified() == false);
	}

	void TestSend()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher = std::make_shared<Event<FooMessage>>(message);
		
		FooSubscriber subscriber;
		Event<FooMessage>::Subscribe(subscriber);

		EventQueue eventQueue;
		TS_ASSERT(subscriber.HasBeenNotified() == false);
		Event<FooMessage>* event = eventPublisher->As<Event<FooMessage>>();
		TS_ASSERT(event->Message().mCalls == 0);
		eventQueue.Send(eventPublisher);
		TS_ASSERT(subscriber.HasBeenNotified() == true);
		TS_ASSERT(event->Message().mCalls == 1);
	}

	void TestClear()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher1 = std::make_shared<Event<FooMessage>>(message);
		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher1, 0.0);

		FooSubscriber subscriber;
		Event<FooMessage>::Subscribe(subscriber);

		TS_ASSERT(eventQueue.Count() == 1);
		TS_ASSERT(eventQueue.IsEmpty() == false);

		std::shared_ptr<EventPublisher> eventPublisher2 = std::make_shared<Event<FooMessage>>(message);
		eventQueue.Enqueue(eventPublisher2, 1.0);

		Event<FooMessage>* event1 = eventPublisher1->As<Event<FooMessage>>();
		Event<FooMessage>* event2 = eventPublisher2->As<Event<FooMessage>>();

		TS_ASSERT(eventQueue.Count() == 2);
		TS_ASSERT(eventQueue.IsEmpty() == false);

		eventQueue.Clear(1.0);
		TS_ASSERT(eventQueue.Count() == 0);
		TS_ASSERT(eventQueue.IsEmpty() == true);

		TS_ASSERT(subscriber.HasBeenNotified() == true);
		TS_ASSERT(event1->Message().mCalls == 1);

		eventQueue.Enqueue(eventPublisher1, 0.0);
		eventQueue.Enqueue(eventPublisher2, 1.0);

		TS_ASSERT(eventQueue.Count() == 2);
		TS_ASSERT(eventQueue.IsEmpty() == false);

		eventQueue.Clear(2.0);
		TS_ASSERT(eventQueue.Count() == 0);
		TS_ASSERT(eventQueue.IsEmpty() == true);

		TS_ASSERT(subscriber.HasBeenNotified() == true);
		TS_ASSERT(event2->Message().mCalls == 1);
		TS_ASSERT(event1->Message().mCalls == 2);
	}
};