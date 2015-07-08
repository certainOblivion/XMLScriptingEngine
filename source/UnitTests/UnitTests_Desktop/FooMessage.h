#pragma once
#include "RTTI.H"
#include <mutex>
namespace UnitTests
{
	class FooMessage : public Library::RTTI
	{
		RTTI_DECLARATIONS(FooMessage, RTTI)
	public:
		FooMessage();
		virtual ~FooMessage() = default;

		int mCalls;
	};
}