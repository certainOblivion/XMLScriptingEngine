#include <cxxtest/TestSuite.h>
#include "Action.h"
#include "ActionList.h"
#include "ActionFoo.h"
#include "World.h"
#include "WorldSharedData.h"
#include "WorldXmlParseHelper.h"
#include "XmlParseMaster.h"
#include "Entity.h"
#include "Sector.h"
#include <memory>
#include <crtdbg.h>

using namespace Library;
using namespace UnitTests;

ActionFactory(ActionFoo)

class ActionTestSuite : public CxxTest::TestSuite
{
public:
	void TestInstantiation()
	{
		ActionFooFactory mActionFooFactory;
		Action* newAction = Factory<Action>::Create("ActionFoo");
		TS_ASSERT(newAction != nullptr);
		ActionFoo* newActionFoo = newAction->As<ActionFoo>();
		TS_ASSERT(newActionFoo != nullptr);

		delete newAction;
	}
	void TestUpdate()
	{
		ActionFooFactory mActionFooFactory;
		Action* newAction = Factory<Action>::Create("ActionFoo");
		
		TS_ASSERT(newAction != nullptr);
		
		World world;
		WorldState worldState(&world);
		newAction->Update(worldState);
		ActionFoo* newActionFoo = newAction->As<ActionFoo>();
		TS_ASSERT(newActionFoo != nullptr);
		TS_ASSERT(newActionFoo->HasUpdateBeenCalled());

		delete newAction;
	}

	void TestActionList()
	{
		ActionFooFactory mActionFooFactory;
		Action* newAction1 = Factory<Action>::Create("ActionFoo");
		Action* newAction2 = Factory<Action>::Create("ActionFoo");
		Action* newAction3 = Factory<Action>::Create("ActionFoo");
		World world;
		WorldState worldState(&world);

		ActionList actionList;
		actionList.Actions()->Adopt(*newAction1, "newAction1");
		actionList.Actions()->Adopt(*newAction2, "newAction2");
		actionList.Actions()->Adopt(*newAction3, "newAction3");

		actionList.Update(worldState);

		ActionFoo* newActionFoo1 = newAction1->As<ActionFoo>();
		TS_ASSERT(newActionFoo1->HasUpdateBeenCalled());

		ActionFoo* newActionFoo2 = newAction2->As<ActionFoo>();
		TS_ASSERT(newActionFoo2->HasUpdateBeenCalled());

		ActionFoo* newActionFoo3 = newAction3->As<ActionFoo>();
		TS_ASSERT(newActionFoo3->HasUpdateBeenCalled());

		delete newAction1;
		delete newAction2;
		delete newAction3;
	}

	void TestFromXml()
	{
		ActionFooFactory mActionFooFactory;
		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		World mWorld;

		std::unique_ptr<WorldSharedData> worldData = std::make_unique<WorldSharedData>(mWorld);
		std::unique_ptr<XmlParseMaster> masterParser = std::make_unique<XmlParseMaster>(worldData.get());
		std::unique_ptr<WorldXmlParseHelper> worldHelper = std::make_unique<WorldXmlParseHelper>();

		masterParser->AddHelper(*worldHelper);

		masterParser->ParseFromFile("WorldXML.xml");

		TS_ASSERT(mWorld.Name() == "Earth");
		auto* foundSector = mWorld.Sectors()->Find("DC");

		TS_ASSERT(foundSector != nullptr);
		TS_ASSERT(foundSector->GetTable()->As<Sector>()->Name() == "DC");
		Sector* foundSectorP = foundSector->GetTable()->As<Sector>();
		auto* foundEntity = foundSectorP->Entities()->Find("Gotham");

		TS_ASSERT(foundEntity != nullptr);

		Entity* entity = foundEntity->GetTable()->As<Entity>();
		TS_ASSERT(entity->Name() == "Gotham");

		auto* actionListDatum = entity->Actions()->Find("FightCrime");
		TS_ASSERT(actionListDatum != nullptr);

		ActionList* actionList = actionListDatum->GetTable()->As<ActionList>();
		TS_ASSERT(actionList != nullptr);
		
		TS_ASSERT(actionList->Name() == "FightCrime");

		auto* actionDatum = actionList->Actions()->Find("Find Joker");
		TS_ASSERT(actionDatum != nullptr);

		Action* action = actionDatum->GetTable()->As<Action>();
		TS_ASSERT(action->Name() == "Find Joker");

		Datum* intDatum = action->Find("TimeInMinutes");
		TS_ASSERT(intDatum != nullptr);

		TS_ASSERT(intDatum->GetInteger() == 20);
	}

	void TestCreateAction()
	{
		ActionFooFactory mActionFooFactory;
		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		World mWorld;

		std::unique_ptr<WorldSharedData> worldData = std::make_unique<WorldSharedData>(mWorld);
		std::unique_ptr<XmlParseMaster> masterParser = std::make_unique<XmlParseMaster>(worldData.get());
		std::unique_ptr<WorldXmlParseHelper> worldHelper = std::make_unique<WorldXmlParseHelper>();

		masterParser->AddHelper(*worldHelper);

		masterParser->ParseFromFile("WorldXML.xml");

		auto* foundSector = mWorld.Sectors()->Find("DC");
		Sector* foundSectorP = foundSector->GetTable()->As<Sector>();
		auto* foundEntity = foundSectorP->Entities()->Find("Gotham");		
		Entity* entity = foundEntity->GetTable()->As<Entity>();
		auto* actionListDatum = entity->Actions()->Find("FightCrime");
		ActionList* actionList = actionListDatum->GetTable()->As<ActionList>();

		auto* actionDatum = actionList->Actions()->Find("Joker");
		TS_ASSERT(actionDatum == nullptr);

		WorldState worldState(&mWorld);
		mWorld.Update(worldState);

		actionDatum = actionList->Actions()->Find("Joker");
		TS_ASSERT(actionDatum != nullptr);
	}

	void TestDestroyAction()
	{
		ActionFooFactory mActionFooFactory;
		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		World mWorld;

		std::unique_ptr<WorldSharedData> worldData = std::make_unique<WorldSharedData>(mWorld);
		std::unique_ptr<XmlParseMaster> masterParser = std::make_unique<XmlParseMaster>(worldData.get());
		std::unique_ptr<WorldXmlParseHelper> worldHelper = std::make_unique<WorldXmlParseHelper>();

		masterParser->AddHelper(*worldHelper);

		masterParser->ParseFromFile("WorldXML.xml");

		auto* foundSector = mWorld.Sectors()->Find("DC");
		Sector* foundSectorP = foundSector->GetTable()->As<Sector>();
		auto* foundEntity = foundSectorP->Entities()->Find("Gotham");
		Entity* entity = foundEntity->GetTable()->As<Entity>();
		auto* actionListDatum = entity->Actions()->Find("FightCrime");
		ActionList* actionList = actionListDatum->GetTable()->As<ActionList>();

		auto* actionDatum = actionList->Actions()->Find("SolomonGrundy");
		TS_ASSERT(actionDatum->GetTable() != nullptr);

		WorldState worldState(&mWorld);
		mWorld.Update(worldState);

		actionDatum = actionList->Actions()->Find("SolomonGrundy");
		TS_ASSERT(actionDatum->GetTable() == nullptr);
	}
};