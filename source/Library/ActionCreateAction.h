#pragma once
#include "Action.h"
namespace Library
{
	class ActionCreateAction :
		public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		/**Constructor*/
		ActionCreateAction();

		/**Destructor*/
		virtual ~ActionCreateAction() = default;

		/**Returns class name 
		* @return string class name
		*/
		std::string& Prototype();

		/**Returns instance name
		* @return string instance name
		*/
		std::string& Instance();

		/**Updates the world using worldstate
		* @param world state reference
		*/
		virtual void Update(WorldState& world) override;
	private:
		std::string mClassName;
		std::string mInstanceName;
	};

	ActionFactory(ActionCreateAction);
}
