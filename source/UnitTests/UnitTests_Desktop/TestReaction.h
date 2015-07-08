#include <cxxtest/TestSuite.h>
#include <crtdbg.h>
#include "ActionEvent.h"
#include "Scope.h"
#include "WorldState.h"
#include "ReactionAttributed.h"
#include "World.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"

using namespace Library;

class ReactionTestSuite : public CxxTest::TestSuite
{
public:
	void TestActionEvent()
	{
		World world;

		ReactionAttributed reaction;
		reaction.SetSubType("reaction");

		ActionEventFactory actionEventFactory;
		ActionEvent* actionEvent = actionEventFactory.Create()->As<ActionEvent>();

		TS_ASSERT(actionEvent != nullptr);

		actionEvent->SetSubType("Batman");
		actionEvent->SetDelay(2);

		Datum& powerAttribute = actionEvent->AppendAuxiliaryAttribute("Power");
		powerAttribute.SetType(Datum::DatumType::FLOAT);
		powerAttribute = 2.f;

		Datum& cityAttribute = actionEvent->AppendAuxiliaryAttribute("City");
		cityAttribute.SetType(Datum::DatumType::STRING);
		cityAttribute = "Gotham";

		TS_ASSERT((*actionEvent)["SubType"].GetString() == "Batman");
		TS_ASSERT((*actionEvent)["Delay"].GetFloat() == 2.0f);

		actionEvent->Update(world.GetWorldState());

		TS_ASSERT(world.GetEventQueue().Count() == 1);

		world.GetEventQueue().Update(3);

		TS_ASSERT(world.GetEventQueue().Count() == 0);

		Datum* reactionAuxiliaryAttribute = reaction.Find("Power");
		TS_ASSERT(reactionAuxiliaryAttribute->GetFloat() == 2.f);

		reactionAuxiliaryAttribute = reaction.Find("City");
		TS_ASSERT(reactionAuxiliaryAttribute->GetString() == "Gotham");

		delete actionEvent;
	}
	void TestXml()
	{
		ReactionAttributedFactory mReactionAttributedFactory;
		ActionEventFactory actionEventFactory;
		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		World mWorld;

		std::unique_ptr<WorldSharedData> worldData = std::make_unique<WorldSharedData>(mWorld);
		std::unique_ptr<XmlParseMaster> masterParser = std::make_unique<XmlParseMaster>(worldData.get());
		std::unique_ptr<WorldXmlParseHelper> worldHelper = std::make_unique<WorldXmlParseHelper>();

		masterParser->AddHelper(*worldHelper);

		masterParser->ParseFromFile("Reaction.xml");

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

		ActionEvent* actionEvent = action->As<ActionEvent>();
		TS_ASSERT(actionEvent->GetSubType() == "Villain");

		Datum* intDatum = action->Find("TimeInMinutes");
		TS_ASSERT(intDatum != nullptr);

		TS_ASSERT(intDatum->GetInteger() == 20);

		actionDatum = actionList->Actions()->Find("SolomonGrundy");
		TS_ASSERT(actionDatum != nullptr);

		action = actionDatum->GetTable()->As<Action>();
		TS_ASSERT(action->Name() == "SolomonGrundy");

		ReactionAttributed* reactionAttr = action->As<ReactionAttributed>();
		TS_ASSERT(reactionAttr->GetSubType() == "Villain");
	}

	void TestXMLUpdate()
	{
		ReactionAttributed* reactionAttributed;
		ActionEvent* actionEvent;
		ReactionAttributedFactory mReactionAttributedFactory;
		ActionEventFactory actionEventFactory;
		EntityFactory mEntityFactory;
		ActionListFactory mActionListFactory;
		World mWorld;

		std::unique_ptr<WorldSharedData> worldData = std::make_unique<WorldSharedData>(mWorld);
		std::unique_ptr<XmlParseMaster> masterParser = std::make_unique<XmlParseMaster>(worldData.get());
		std::unique_ptr<WorldXmlParseHelper> worldHelper = std::make_unique<WorldXmlParseHelper>();

		masterParser->AddHelper(*worldHelper);

		masterParser->ParseFromFile("Reaction.xml");
		auto* foundSector = mWorld.Sectors()->Find("DC");
		Sector* foundSectorP = foundSector->GetTable()->As<Sector>();
		auto* foundEntity = foundSectorP->Entities()->Find("Gotham");
		Entity* entity = foundEntity->GetTable()->As<Entity>();
		auto* actionListDatum = entity->Actions()->Find("FightCrime");
		ActionList* actionList = actionListDatum->GetTable()->As<ActionList>();
		auto* actionDatum = actionList->Actions()->Find("Find Joker");
		Action* action = actionDatum->GetTable()->As<Action>();
		actionEvent = action->As<ActionEvent>();
		actionDatum = actionList->Actions()->Find("SolomonGrundy");
		action = actionDatum->GetTable()->As<Action>();
		reactionAttributed = action->As<ReactionAttributed>();

		Datum& powerAttribute = actionEvent->AppendAuxiliaryAttribute("Power");
		powerAttribute.SetType(Datum::DatumType::FLOAT);
		powerAttribute = 2.f;

		Datum& cityAttribute = actionEvent->AppendAuxiliaryAttribute("City");
		cityAttribute.SetType(Datum::DatumType::STRING);
		cityAttribute = "Gotham";

		TS_ASSERT((*actionEvent)["SubType"].GetString() == "Villain");
		TS_ASSERT((*actionEvent)["Delay"].GetFloat() == 0.0f);

		actionEvent->Update(mWorld.GetWorldState());

		TS_ASSERT(mWorld.GetEventQueue().Count() == 1);

		mWorld.GetEventQueue().Update(3);

		TS_ASSERT(mWorld.GetEventQueue().Count() == 0);

		Datum* reactionAuxiliaryAttribute = reactionAttributed->Find("Power");
		TS_ASSERT(reactionAuxiliaryAttribute->GetFloat() == 2.f);

		reactionAuxiliaryAttribute = reactionAttributed->Find("City");
		TS_ASSERT(reactionAuxiliaryAttribute->GetString() == "Gotham");
	}


};