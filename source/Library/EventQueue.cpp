#include "pch.h"
#include "EventQueue.h"
#include "EventPublisher.h"
#include "GameTime.h"
#include <vector>
#include <future>

using namespace Library;
using namespace std;

EventQueue::EventQueue():
mIsUpdateRunning(false)
{
}

void EventQueue::Enqueue(const std::shared_ptr<EventPublisher> publisher, std::double_t currentTime, std::double_t delay)
{
	lock_guard<recursive_mutex> lock(mMutex);
	publisher->SetTime(currentTime, delay);
	mEventQueue.PushBack(publisher);
}

void EventQueue::Send(const std::shared_ptr<EventPublisher> publisher)
{
	lock_guard<recursive_mutex> lock(mMutex);
	if (publisher.get() == nullptr)
	{
		throw std::exception("Event publisher is null");
	}
	publisher->Deliver();
}

void EventQueue::Update(std::double_t currentTime)
{
	if (!mIsUpdateRunning.exchange(true))
	{
		SList<std::shared_ptr<EventPublisher>> eventsToBeCleared;
		vector <future<void>> futures;
		{
			lock_guard<recursive_mutex> lock(mMutex);
			for (auto& it : mEventQueue)
			{
				if (it->IsExpired(currentTime))
				{
					futures.emplace_back((async(launch::async, &EventPublisher::Deliver, &(*it))));
					eventsToBeCleared.PushBack(it);
				}
			}
		}

		for (auto& f : futures)
		{
			f.get();
		}

		for (auto& it : eventsToBeCleared)
		{
			mEventQueue.Remove(it);
		}

		mIsUpdateRunning.store(false);
	}
}

void EventQueue::Clear(std::double_t currentTime)
{
	lock_guard<recursive_mutex> lock(mMutex);
	Update(currentTime);
	mEventQueue.Clear();
}

bool EventQueue::IsEmpty()
{
	lock_guard<recursive_mutex> lock(mMutex);
	return mEventQueue.IsEmpty();
}

std::uint32_t EventQueue::Count()
{
	lock_guard<recursive_mutex> lock(mMutex);
	return mEventQueue.Size();
}