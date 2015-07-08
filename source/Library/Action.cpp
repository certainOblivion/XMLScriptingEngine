#include "pch.h"
#include "Action.h"

using namespace Library;

RTTI_DEFINITIONS(Action);

std::string Action::Name() const
{
	return mName;
}

Scope* Action::GetContainer()
{
	return GetParent()->GetParent();
}