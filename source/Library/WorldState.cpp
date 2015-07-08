#include "pch.h"
#include "WorldState.h"

using namespace Library;

WorldState::WorldState(World* world) : mGameTime()
{
	mWorld = world;
}

WorldState::~WorldState()
{
}

const GameTime& WorldState::GetGameTime()
{
	return mGameTime;
}

void WorldState::SetGameTime(const GameTime& gametime)
{
	mGameTime = gametime;
}
