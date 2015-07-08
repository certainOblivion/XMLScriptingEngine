#include "EventSubscriber.h"

namespace UnitTests
{
	class FooSubscriber : public Library::EventSubscriber
	{
	public:
		FooSubscriber();
		~FooSubscriber() = default;
		void Notify(Library::EventPublisher& publisher) override;
		bool HasBeenNotified();
	private:
		bool mHasBeenNotified;
		
	};
}