#pragma once
#include "Action.h"
namespace Library
{
	class ActionEvent :	public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);
	public:
		/**Constructor*/
		ActionEvent();

		/**Destructor*/
		virtual ~ActionEvent();

		/**Returns subtype
		* @return string subtype
		*/
		std::string GetSubType();

		/**Set subtype
		* @param string subtype
		*/
		void SetSubType(std::string subtype);

		/**Set delay
		* @return float delay
		*/
		float GetDelay();

		/**Set delay
		* @param float delay
		*/
		void SetDelay(float delay);

		/**Update the current ActionEvent
		* @param WorldState reference
		*/
		virtual void Update(WorldState& worldState) override;
	};
	ActionFactory(ActionEvent)
}