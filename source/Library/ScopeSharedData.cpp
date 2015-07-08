#include "pch.h"
#include "ScopeSharedData.h"

using namespace Library;

RTTI_DEFINITIONS(ScopeSharedData)

ScopeSharedData::ScopeSharedData(Scope& parsedScope) :
mParsedScope(&parsedScope), bIsClone(false), mCurrentScope(nullptr)
{
}


ScopeSharedData::~ScopeSharedData()
{
	if (bIsClone)
	{
		delete mParsedScope;
	}
}

ScopeSharedData* ScopeSharedData::Clone() const
{
	Scope* createdScope = new Scope();
	ScopeSharedData* clonedSharedData = new ScopeSharedData(*createdScope);
	clonedSharedData->bIsClone = true;
	return clonedSharedData;
}

void ScopeSharedData::Initialize()
{
	mParsedScope->Clear();
}

Scope* ScopeSharedData::GetParsedScope()
{
	return mParsedScope;
}

void ScopeSharedData::AddNestedScope(std::string ScopeName)
{
	if (mCurrentScope)
	{
		mCurrentScope = &(mCurrentScope->AppendScope(ScopeName));
	}
	else
	{
		mCurrentScope = mParsedScope;
	}
}




