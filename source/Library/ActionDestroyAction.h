#pragma once
#include "Action.h"
namespace Library
{
	class ActionDestroyAction :
		public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction,Action)
	public:
		/**Constructor*/
		ActionDestroyAction();

		/**Returns instance name
		* @return string instance name
		*/
		std::string& Instance();

		/**Destructor*/
		virtual ~ActionDestroyAction() = default;

		/**Updates the world using worldstate
		* @param world state reference
		*/
		virtual void Update(WorldState& world) override;
	private:
		std::string mInstanceName;
	};
	ActionFactory(ActionDestroyAction);
}
