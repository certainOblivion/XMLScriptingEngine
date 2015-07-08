#pragma once
#include "Attributed.h"
namespace Library
{
	class World;
	class EventMessageAttributed :	public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:
		/**Constructor*/
		EventMessageAttributed();

		/**Copy constructor
		* @param rhs EventMessageAttributed reference
		*/
		EventMessageAttributed(const EventMessageAttributed& rhs);

		/**Overloaded operator=
		* @param rhs EventMessageAttributed reference
		* @return EventMessageAttributed reference
		*/
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs);

		/**Destructor*/
		virtual ~EventMessageAttributed() = default;

		/**Returns subtype
		* @return string subtype
		*/
		std::string GetType();

		/**Set subtype
		* @param string subtype
		*/
		void SetType(std::string type);

		/**Returns world
		* @return world pointer
		*/
		World* GetWorld();

		/**Set world
		* @param world pointer
		*/
		void SetWorld(World& world);

	private:
		std::string mSubType;
		World* mWorld;
	};
}