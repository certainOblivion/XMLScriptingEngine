#include "FooAsyncSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "FooMessage.h"
#include <thread>
#include <iostream>
#include <random>

using namespace UnitTests;
using namespace Library;
using namespace std;

FooAsyncSubscriber::FooAsyncSubscriber(Library::EventQueue* eventQueue) :
mHasBeenNotified(false), mCalls(0), mShouldUnsubscribe(true), mEventQueue(eventQueue)
{
}

bool FooAsyncSubscriber::HasBeenNotified()
{
	lock_guard<recursive_mutex> lock(mMutex);
	return mHasBeenNotified;
}

void FooAsyncSubscriber::Notify(Library::EventPublisher& publisher)
{
	lock_guard<recursive_mutex> lock(mMutex);
	Event<FooMessage>* event = publisher.As<Event<FooMessage>>();
	if (mShouldUnsubscribe)
	{
		Event<FooMessage>::Unsubscribe(*this);
	}
	
	if (event)
	{
		mHasBeenNotified = true;
		event->Message().mCalls++;
		mCalls++;

		if (mEventQueue)
		{
			FooMessage message;
			std::shared_ptr<EventPublisher> newEvent = std::make_shared<Event<FooMessage>>(message);

			mEventQueue->Enqueue(newEvent, 0.0);
			mEventQueue->Update(100.f);
		}

	}
}