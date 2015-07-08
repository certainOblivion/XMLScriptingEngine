#include <cxxtest/TestSuite.h>
#include "Event.h"
#include "EventPublisher.h"
#include "EventQueue.h"
#include "FooAsyncSubscriber.h"
#include "FooMessage.h"
#include <crtdbg.h>

using namespace Library;
using namespace UnitTests;

class AsyncEventTestSuite : public CxxTest::TestSuite
{
public:
	void tearDown()
	{
		Event<FooMessage>::UnsubscribeAll();
	}

	void TestEventNotification()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher1 = std::make_shared<Event<FooMessage>>(message);
		std::shared_ptr<EventPublisher> eventPublisher2 = std::make_shared<Event<FooMessage>>(message);
		FooAsyncSubscriber subscriber;
		Event<FooMessage>::Subscribe(subscriber);
		EventQueue eventQueue;
		eventQueue.Enqueue(eventPublisher1, 0.0);
		eventQueue.Enqueue(eventPublisher2, 0.0);

		TS_ASSERT(subscriber.HasBeenNotified() == false);
		TS_ASSERT(message.mCalls == 0);
		eventQueue.Update(1.0);
		TS_ASSERT(subscriber.HasBeenNotified() == true);
		Event<FooMessage>* event = eventPublisher1->As<Event<FooMessage>>();
		TS_ASSERT(event->Message().mCalls == 1);
	}

	void TestMultipleEventNotification()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher1 = std::make_shared<Event<FooMessage>>(message);
		std::shared_ptr<EventPublisher> eventPublisher2 = std::make_shared<Event<FooMessage>>(message);
		FooAsyncSubscriber subscriber;
		Event<FooMessage>::Subscribe(subscriber);
		EventQueue eventQueue;
		for (int i = 0; i < 100;i++)
		{
			eventQueue.Enqueue(eventPublisher1, 0.0);
			eventQueue.Enqueue(eventPublisher2, 0.0);
		}

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber.HasBeenNotified() == true);
		Event<FooMessage>* event1 = eventPublisher1->As<Event<FooMessage>>();
		

		Event<FooMessage>* event2 = eventPublisher2->As<Event<FooMessage>>();
		

		TS_ASSERT(subscriber.mCalls <= 2);


		subscriber.mCalls = 0;
		event1->Message().mCalls = 0;
		event2->Message().mCalls = 0;

		subscriber.mShouldUnsubscribe = false;
		Event<FooMessage>::Subscribe(subscriber);
		for (int i = 0; i < 200; i++)
		{
			eventQueue.Enqueue(eventPublisher1, 0.0);
			eventQueue.Enqueue(eventPublisher2, 0.0);
		}

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber.HasBeenNotified() == true);
		TS_ASSERT(event1->Message().mCalls <= 200);
		TS_ASSERT(event2->Message().mCalls <= 200);
		TS_ASSERT(subscriber.mCalls <= 400);
	}

	void TestEnqueue()
	{
		FooMessage message;
		std::shared_ptr<EventPublisher> eventPublisher1 = std::make_shared<Event<FooMessage>>(message);
		std::shared_ptr<EventPublisher> eventPublisher2 = std::make_shared<Event<FooMessage>>(message);
		EventQueue eventQueue;
		FooAsyncSubscriber subscriber(&eventQueue);
		Event<FooMessage>::Subscribe(subscriber);
		subscriber.mShouldUnsubscribe = false;
		for (int i = 0; i < 100; i++)
		{
			eventQueue.Enqueue(eventPublisher1, 0.0);
			eventQueue.Enqueue(eventPublisher2, 0.0);
		}

		eventQueue.Update(1.0);

		TS_ASSERT(subscriber.HasBeenNotified() == true);
		Event<FooMessage>* event1 = eventPublisher1->As<Event<FooMessage>>();
		TS_ASSERT(event1->Message().mCalls <= 100);

		Event<FooMessage>* event2 = eventPublisher2->As<Event<FooMessage>>();
		TS_ASSERT(event2->Message().mCalls <= 100);

		TS_ASSERT(subscriber.mCalls <= 200);

		TS_ASSERT(eventQueue.Count() == 200);
	}
};