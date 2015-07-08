#pragma once
#include "GameTime.h"
namespace Library
{
	class World;
	class Sector;
	class Entity;
	class Action;

	class WorldState
	{
	public:
		/**Default parameterless constructor*/
		explicit WorldState(World* world);

		/**Destructor*/
		virtual ~WorldState();

		/**Gets GameTime with the updated GameTime class object
		* @return reference to GameTime object
		*/
		const GameTime& GetGameTime();

		/**Set the GameTime pointer
		* @param gametime object
		*/
		void SetGameTime(const GameTime& gameTime);

		GameTime mGameTime;
		World* mWorld;
		Sector* mSector;
		Entity* mEntity;
		Action* mAction;
	};
}

