#pragma once
#include "Entity.h"
#include "Attributed.h"
namespace Library
{
	class WorldState;
	class Action :	public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
		friend class Entity;
		friend class ActionList;
	public:
		/**Default constructor*/
		Action() = default;

		/**Destructor*/
		virtual ~Action() = default;

		/**Returns a string with the name of the action
		* @return string with name
		*/
		std::string Name() const;

		/**Updates world state
		* @param WorldState reference*/
		virtual void Update(WorldState& world) = 0;

		/**Returns the Entity or ActionList that is containing it
		* @return Scope pointing to the entity or the ActionList
		*/
		Scope* GetContainer();

	private:
		std::string mName;
	};
	
#define ActionFactory(ActionConcrete) \
	ConcreteFactory(ActionConcrete, Action)
}
