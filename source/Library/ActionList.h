#pragma once
#include "Action.h"
#include "Vector.h"
namespace Library
{
	class ActionList :
		public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);
	public:
		/**Constructor*/
		ActionList();

		/**Destructor*/
		virtual ~ActionList() = default;

		/**Updates the world using worldstate
		* @param world state reference
		*/
		virtual void Update(WorldState& world) override;

		/**Returns the scope containing all the actions
		* @return Scope pointer containing all the actions
		*/
		Scope* Actions();

		/**Creates a new action using the Action factory
		* @param action name string
		* @param action instance name
		* @return pointer to the created action
		*/
		Action* CreateAction(std::string entityName, std::string instanceName);

	private:
		Entity* mParentEntity;
	};

	ActionFactory(ActionList);
}
