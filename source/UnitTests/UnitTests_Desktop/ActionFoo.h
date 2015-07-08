#pragma once
#include "Action.h"
#include "WorldState.h"
#include "Factory.h"
#include "Entity.h"
namespace UnitTests
{
	class ActionFoo :
		public Library::Action
	{
		RTTI_DECLARATIONS(ActionFoo, Library::Action);
	public:
		ActionFoo() = default;
		virtual ~ActionFoo() = default;

		void Update(Library::WorldState& world) override;
		bool HasUpdateBeenCalled() const;
	private:
		bool bHasBeenUpdated = false;
	};
	

}
