#include "ActionFoo.h"

using namespace UnitTests;

RTTI_DEFINITIONS(ActionFoo)

void ActionFoo::Update(Library::WorldState& world)
{
	bHasBeenUpdated = true;
}

bool ActionFoo::HasUpdateBeenCalled() const
{
	return bHasBeenUpdated;
}
