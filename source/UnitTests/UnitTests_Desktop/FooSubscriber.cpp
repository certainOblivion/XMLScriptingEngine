#include "FooSubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "FooMessage.h"
#include <thread>
#include <iostream>

using namespace UnitTests;
using namespace Library;
using namespace std;

FooSubscriber::FooSubscriber() :
mHasBeenNotified(false)
{
}

bool FooSubscriber::HasBeenNotified()
{
	return mHasBeenNotified;
}

void FooSubscriber::Notify(Library::EventPublisher& publisher)
{
	Event<FooMessage>* event = publisher.As<Event<FooMessage>>();
	if (event)
	{
		mHasBeenNotified = true;
		event->Message().mCalls++;
	}
}
