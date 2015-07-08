#include "pch.h"
#include "ActionDestroyAction.h"

using namespace Library;
RTTI_DEFINITIONS(ActionDestroyAction);

ActionDestroyAction::ActionDestroyAction():
mInstanceName("")
{
}

void ActionDestroyAction::Update(WorldState& world)
{
	Datum* foundDatum = Search(mInstanceName);
	if (foundDatum && (foundDatum->GetType() == Datum::DatumType::TABLE))
	{
		Scope* foundScope = foundDatum->GetTable();
		foundScope->Clear();
		delete foundScope;
	}
}

std::string& ActionDestroyAction::Instance()
{
	return mInstanceName;
}
