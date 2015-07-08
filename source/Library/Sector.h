#pragma once
#include "Attributed.h"
#include "Factory.h"
namespace Library
{
	class Entity;
	class World;
	class WorldState;
	class Sector final : public Attributed
	{
		friend class World;

		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		/**Default parameterless constructor*/
		Sector();

		/**Destructor*/
		~Sector() = default;

		/**Returns a string with the name of the entity
		* @return string with name
		*/
		const std::string& Name() const;

		/** Returns the Scope pointer that stores the contained entities in the sector
		* @return Scope pointer containing entities
		*/
		Scope* Entities();

		/**Creates a new entity using the Entity factory
		* @param entity name string
		* @param entity instance name
		* @return pointer to the created entity
		*/
		Entity* CreateEntity(std::string entityName, std::string instanceName);

		/**Set the world variable.
		* @param Reference to the world
		*/
		void SetWorld(World& world);

		/**Return the world reference
		* @return Reference to the world
		*/
		World& GetWorld();

		/**Updates world state
		* @param WorldState reference*/
		void Update(WorldState& world);

	private:
		std::string mName;
		World* mWorld;
	};
}

