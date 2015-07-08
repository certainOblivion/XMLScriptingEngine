#include "pch.h"
#include "EventPublisher.h"
#include <future>
#include <vector>

using namespace Library;
using namespace std;

RTTI_DEFINITIONS(EventPublisher)

EventPublisher::EventPublisher(SList<EventSubscriber*>& subscribers, std::recursive_mutex& mutex)
: mSubscriberReference(&subscribers), mTimeEnqueued(0.0), mDelayTime(0.0), mParentMutex(&mutex), mMutex()
{
}

void EventPublisher::SetTime(double currentTime, double delay)
{
	lock_guard<recursive_mutex> lock(mMutex);
	mTimeEnqueued = currentTime;
	mDelayTime = delay;
}

EventPublisher::EventPublisher(const EventPublisher& rhs):
mSubscriberReference(rhs.mSubscriberReference), mTimeEnqueued(rhs.mTimeEnqueued), mDelayTime(rhs.mDelayTime), mParentMutex(rhs.mParentMutex)
{
}

EventPublisher& EventPublisher::operator = (const EventPublisher& rhs)
{
	if (this != &rhs)
	{
		lock_guard<recursive_mutex> lock(mMutex);
		*mSubscriberReference = *rhs.mSubscriberReference;
		mTimeEnqueued = rhs.mTimeEnqueued;
		mDelayTime = rhs.mDelayTime;
	}
	return *this;
}

double EventPublisher::TimeEnqueued() const
{
	lock_guard<recursive_mutex> lock(mMutex);
	return mTimeEnqueued;
}

double EventPublisher::Delay() const
{
	lock_guard<recursive_mutex> lock(mMutex);
	return mDelayTime;
}

bool EventPublisher::IsExpired(std::double_t currentTime) const
{
	lock_guard<recursive_mutex> lock(mMutex);
	if ((mTimeEnqueued+mDelayTime) < currentTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EventPublisher::Deliver()
{
	vector <future<void>> futures;

	{
		lock_guard<recursive_mutex> lock(*mParentMutex);
		for (auto& it : *mSubscriberReference)
		{
			futures.emplace_back(async(launch::async, &EventSubscriber::Notify, &(*it), ref(*this)));
		}
	}
	for (auto& f : futures)
	{
		f.get();
	}
}