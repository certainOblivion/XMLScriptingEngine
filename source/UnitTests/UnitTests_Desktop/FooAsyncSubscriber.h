#pragma once
#include "EventSubscriber.h"
#include "EventQueue.h"

class FooAsyncSubscriber : public Library::EventSubscriber
{
public:
	FooAsyncSubscriber(Library::EventQueue* eventQueue = nullptr);
	virtual ~FooAsyncSubscriber() = default;

	void Notify(Library::EventPublisher& publisher) override;
	bool HasBeenNotified();
	int mCalls;
	bool mShouldUnsubscribe;
private:
	bool mHasBeenNotified;
	Library::EventQueue* mEventQueue;
	std::recursive_mutex mMutex;
};

