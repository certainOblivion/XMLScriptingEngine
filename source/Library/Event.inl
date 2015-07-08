#include "Event.h"
namespace Library
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>);

	template<typename T>
	std::recursive_mutex Event<T>::mMutex = std::recursive_mutex();

	template<typename T>
	SList<EventSubscriber*> Event<T>::mSubscribers;

	template<typename T>
	Event<T>::Event(const T& message) :
		mMessage(message), EventPublisher(mSubscribers,mMutex)
	{
	}

	template<typename T>
	void Event<T>::Subscribe(EventSubscriber& eventSubscriber)
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mSubscribers.PushBack(&eventSubscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& eventSubscriber)
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mSubscribers.Remove(&eventSubscriber);
	}

	template<typename T>
	void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		mSubscribers.Clear();
	}

	template<typename T>
	T& Event<T>::Message()
	{
		return mMessage;
	}
}