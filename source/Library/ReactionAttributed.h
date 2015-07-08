#pragma once
#include "Reaction.h"
namespace Library
{
	class ReactionAttributed :
		public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	public:
		/**Constructor*/
		ReactionAttributed();

		/**Destructor*/
		virtual ~ReactionAttributed();

		/**Returns subtype
		* @return string subtype
		*/
		std::string GetSubType();

		/**Set subtype
		* @param string subtype
		*/
		void SetSubType(std::string subtype);

		/**Notify overloaded from event
		* @param eventpublisher reference
		*/
		virtual void Notify(EventPublisher& publisher) override;
	};

	ActionFactory(ReactionAttributed)
}