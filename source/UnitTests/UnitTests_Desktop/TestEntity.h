#include <cxxtest/TestSuite.h>
#include "Entity.h"
#include "World.h"
#include "Sector.h"
#include "XmlParseMaster.h"
#include "WorldSharedData.h"
#include "WorldXmlParseHelper.h"
#include <crtdbg.h>

using namespace Library;

class EntityTestSuite : public CxxTest::TestSuite
{
public:

	void TestShufflingEntityInSector()
	{
		EntityFactory mEntityFactory;
		World mWorld;
		mWorld.SetName("DC");
		Sector* sector1 = mWorld.CreateSector("Gotham");
		Sector* sector2 = mWorld.CreateSector("Metropolis");
		Entity* hero1 = sector1->CreateEntity("Entity", "Batman");
		Entity* hero2 = sector2->CreateEntity("Entity", "Superman");

		TS_ASSERT(&(sector1->GetWorld()) == &mWorld);
		TS_ASSERT(&(sector2->GetWorld()) == &mWorld);

		TS_ASSERT(hero1->GetSector() == sector1);
		TS_ASSERT(hero2->GetSector() == sector2);
		
		hero1->SetSector(*sector2);
		TS_ASSERT(hero1->GetSector() == sector2);
	}

	void TestInstantiation()
	{
		EntityFactory mEntityFactory;
		World newWorld;
		TS_ASSERT(newWorld.Size() == 3);
		newWorld.SetName("Pandora");

		TS_ASSERT(newWorld.Name() == "Pandora");

		TS_ASSERT(newWorld.Find("Name")->GetString() == "Pandora");
		auto* sectors = newWorld.Sectors();
		
		TS_ASSERT(newWorld.Find("Sectors")->GetTable(0) == newWorld.Sectors());
		TS_ASSERT(sectors->Size() == 0);

		Sector* earthSector = newWorld.CreateSector("Earth");
		TS_ASSERT(&(earthSector->GetWorld()) == &newWorld);

		auto* entityList = earthSector->Entities();

		Entity* entityPaul = earthSector->CreateEntity("Entity", "Paul");
		TS_ASSERT_EQUALS(entityList->Size(), 1);

		Entity* entityTom = earthSector->CreateEntity("Entity", "Tom");
		TS_ASSERT_EQUALS(entityList->Size(), 2);

		TS_ASSERT(entityList->Find("Paul")->GetTable()->As<Entity>() == entityPaul);
		TS_ASSERT(entityList->Find("Tom")->GetTable()->As<Entity>() == entityTom);
	}

	void TestWorldFromXml()
	{
		EntityFactory mEntityFactory;
		std::string xml = "<World name=\"Earth\"><Sector name=\"Continent\"><Entity name=\"India\" className=\"Entity\"></Entity></Sector></World>";
		World mWorld;

		WorldSharedData* worldData = new WorldSharedData(mWorld);
		XmlParseMaster *masterParser = new XmlParseMaster(worldData);
		WorldXmlParseHelper* worldHelper = new WorldXmlParseHelper();

		masterParser->AddHelper(*worldHelper);

		masterParser->Parse(xml.c_str(), xml.length(), true, true);

		TS_ASSERT(mWorld.Name() == "Earth");
		auto* foundSector = mWorld.Sectors()->Find("Continent");

		TS_ASSERT(foundSector != nullptr);
		TS_ASSERT(foundSector->GetTable()->As<Sector>()->Name() == "Continent");
		Sector* foundSectorP = foundSector->GetTable()->As<Sector>();
		auto* foundEntity = foundSectorP->Entities()->Find("India");

		TS_ASSERT(foundEntity != nullptr);
		TS_ASSERT(foundEntity->GetTable()->As<Entity>()->Name() == "India");

		//another XML
		xml = "<World name=\"Earth\"><Sector name=\"Continent\"><Entity name=\"India\" className=\"Entity\"></Entity></Sector><Sector name=\"Ocean\"><Entity name=\"Pacific\" className=\"Entity\"><Integer name=\"Size\"><value>20</value></Integer></Entity></Sector></World>";
		masterParser->Parse(xml.c_str(), xml.length(), true, true);

		TS_ASSERT(mWorld.Name() == "Earth");
		foundSector = mWorld.Sectors()->Find("Continent");

		TS_ASSERT(foundSector != nullptr);
		TS_ASSERT(foundSector->GetTable()->As<Sector>()->Name() == "Continent");
		foundSectorP = foundSector->GetTable()->As<Sector>();
		foundEntity = foundSectorP->Entities()->Find("India");

		TS_ASSERT(foundEntity != nullptr);
		TS_ASSERT(foundEntity->GetTable()->As<Entity>()->Name() == "India");

		foundSector = mWorld.Sectors()->Find("Ocean");

		TS_ASSERT(foundSector != nullptr);
		TS_ASSERT(foundSector->GetTable()->As<Sector>()->Name() == "Ocean");
		foundSectorP = foundSector->GetTable()->As<Sector>();
		foundEntity = foundSectorP->Entities()->Find("Pacific");

		TS_ASSERT(foundEntity != nullptr);
		Entity* entity = foundEntity->GetTable()->As<Entity>();
		TS_ASSERT(entity->Name() == "Pacific");
		TS_ASSERT(entity->Find("Size")->GetInteger() == 20);

		delete worldData;
		delete masterParser;
		delete worldHelper;
	}
};