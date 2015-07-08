#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "EventQueue.h"
namespace Library
{
	class Sector;
	class WorldState;
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World,Attributed)
	public:
		/**Default parameterless constructor*/
		World();

		/**Destructor*/
		~World() = default;

		/**Returns a string with the name of the World
		* @return string with name
		*/
		const std::string& Name() const;

		/**Sets the name of the world
		* @param string name of the world*/
		void SetName(std::string name);

		/** Returns the Scope pointer that stores the contained sectors in the world
		* @return Scope pointer containing entities
		*/
		Scope* Sectors();

		/**Creates a new sector using the Sector factory
		* @param sector name string
		* @param sector instance name
		* @return pointer to the created sector
		*/
		Sector* CreateSector(std::string instanceName);

		/**Updates world state
		* @param WorldState reference*/
		void Update(WorldState& world);

		/**Returns the world state object reference
		* @return world state reference*/
		WorldState& GetWorldState();

		/**Returns the event queue for this world
		* @return event queue reference*/
		EventQueue& GetEventQueue();

	private:
		std::string mName;
		WorldState mWorldState;
		EventQueue mEventQueue;
	};
}

