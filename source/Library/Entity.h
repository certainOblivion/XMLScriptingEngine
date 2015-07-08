#pragma once
#include "Attributed.h"
#include "Factory.h"
namespace Library
{
	class Sector;
	class WorldState;
	class Action;
	class Entity :
		public Attributed
	{
		friend class Sector;

		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		/**Default constructor
		* @param string name
		*/
		Entity();

		/**Copy constructor
		* @param Entity reference to copy
		*/
		Entity(const Entity& rhs) = delete;

		/**Overloaded assignment operator*/
		Entity& operator=(const Entity& rhs) = delete;

		/**Destructor*/
		virtual ~Entity() = default;

		/**Returns a string with the name of the entity
		* @return string with name
		*/
		const std::string& Name() const;

		/**Return the address of the Sector which contains this object
		* @return Sector pointer that contains this entity
		*/
		Sector* GetSector() const;

		/**Takes the address of a Sector, and adopts the entity into that sector
		* @param Sector reference*/
		void SetSector(Sector& parentSector);

		/**Updates world state
		* @param WorldState reference*/
		virtual void Update(WorldState& world);

		/** Returns the Scope pointer that stores the contained actions in the entity
		* @return Scope pointer containing actions
		*/
		Scope* Actions();

		/**Creates a new action using the Action factory
		* @param action name string
		* @param action instance name
		* @return pointer to the created action
		*/
		Action* CreateAction(std::string entityName, std::string instanceName);

	private:
		Sector* mParentSector;
		std::string mName;
	};

	ConcreteFactory(Entity, Entity)
}

