#include "pch.h"

using namespace Library;
using namespace std;

#pragma region RTTI
RTTI_DEFINITIONS(Scope);

#pragma endregion

#pragma region Scope

const std::uint32_t Scope::DefaultSize = 5;

Scope::Scope(std::uint32_t size)
:mContainer(size), mOrderVector(size), mParent(nullptr)
{
}

Scope::Scope(const Scope& rhs) :
	mContainer(1), mOrderVector(0), mParent(nullptr)
{
	operator=(rhs);
}

Scope& Scope::operator=(const Scope& rhs)
{
	if (this != &rhs)
	{
		MakeOrphan();
		Clear();

		mOrderVector.Reserve(rhs.mOrderVector.Size());
		for (std::uint32_t i = 0; i < rhs.mOrderVector.Size(); i++)
		{
			PairType& pair = *(rhs.mOrderVector[i]);
			Datum& existingDatum = pair.second;
			Datum& createdDatum = Append(pair.first, existingDatum.GetType());

			if (existingDatum.GetType() == Datum::DatumType::TABLE)
			{
				for (std::uint32_t j = 0; j < existingDatum.Size(); j++)
				{
					Scope* appendedScope = new Scope(*(existingDatum.GetTable(j)));
					appendedScope->mParent = this;
					createdDatum.Set(appendedScope, j);
				}
			}
			else
			{
				createdDatum = existingDatum;
			}
		}

	}
	return *this;
}

Scope::~Scope()
{
	MakeOrphan();
	Clear();
}

void Scope::MakeOrphan()
{
	if (mParent)
	{
		for (std::uint32_t i = 0; i < mParent->mOrderVector.Size(); ++i)
		{
			if (mParent->mOrderVector[i]->second.GetType() == Datum::DatumType::TABLE)
			{
				for (std::uint32_t j = 0; j < mParent->mOrderVector[i]->second.Size(); ++j)
				{
					if (&(mParent->mOrderVector[i]->second[j]) == this)
					{
						Scope* nullScope = nullptr;
						mParent->mOrderVector[i]->second.Set(nullScope, j);
					}
				}
			}
		}
		mParent = nullptr;
	}
}

std::string Scope::FindName(const Scope* scope) const
{
	std::string retVal = "";
	for (std::uint32_t i = 0; i < mOrderVector.Size(); ++i)
	{
		if (mOrderVector[i]->second == scope)
		{
			retVal = mOrderVector[i]->first;
		}
	}
	return retVal;
}

std::uint32_t Scope::Size()
{
	return mOrderVector.Size();
}

bool Scope::FindName(const Scope* scope, std::string& foundName) const
{
	foundName = FindName(scope);
	return (!foundName.empty());
}

void Scope::Clear()
{
	for (std::uint32_t i = 0; i < mOrderVector.Size(); ++i)
	{
		Datum& datumToClear = mOrderVector[i]->second;
		if (datumToClear.GetType() == Datum::DatumType::TABLE)
		{
			uint32_t size = datumToClear.Size();
			for (std::uint32_t j = 0; j < size; ++j)
			{
				Scope* childScope = datumToClear.GetTable(j);
				if (childScope)
				{
					if (datumToClear.IsExternal())
					{
						childScope = nullptr;
					}
					else
					{
						delete childScope;
					}
				}
			}
		}
	}

	mContainer.Clear();
	mOrderVector.Clear();
}

Datum* Scope::Find(const std::string& key) const
{
	if (key.empty())
	{
		throw std::exception("Cannot look for an empty string as a key.");
	}
	auto it = mContainer.Find(key);
	if (it != mContainer.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}

Datum& Scope::Append(const std::string& key, const Datum::DatumType type)
{
	if ((type < Datum::DatumType::UNKNOWN) || (type >= Datum::DatumType::MAXIMUM))
	{
		throw std::exception("Invalid datum type.");
	}
	bool isInserted;
	Datum DatumToBeInserted;

	auto it = mContainer.Insert(key, DatumToBeInserted, isInserted);

	if (it->second.GetType() == Datum::DatumType::UNKNOWN)
	{
		it->second.SetType(type);
	}

	if (isInserted)
	{
		mOrderVector.PushBack(&(*it));
	}
	return it->second;
}

void Scope::Adopt(Scope& childScope, const std::string& childName, const std::uint32_t childIndex)
{
	Datum* foundDatum = Find(childName);
	if (foundDatum)
	{
		if (foundDatum->GetType() != Datum::DatumType::TABLE)
		{
			throw std::exception("Found datum is not type table.");
		}

		if (foundDatum->IsExternal())
		{
			throw std::exception("Found datum is external.");
		}

		childScope.MakeOrphan();
		childScope.mParent = this;

		if (childIndex < foundDatum->Size())
		{
			for (std::uint32_t i = foundDatum->Size() + 1; i > childIndex; i--)
			{
				foundDatum->Set(foundDatum->GetTable(i - 1), i);
			}
		}
		foundDatum->Set(&childScope, childIndex);
	}
	else
	{
		childScope.MakeOrphan();
		childScope.mParent = this;
		Datum& createdDatum = Append(childName, Datum::DatumType::TABLE);
		createdDatum.PushBack(&childScope);
	}
}

Datum& Scope::operator[](const std::string& key)
{
	return Append(key);
}

Datum& Scope::operator[](const std::uint32_t index)
{
	if (index < mOrderVector.Size())
	{
		return (mOrderVector[index]->second);
	}
	else
	{
		throw std::exception("Index out of bounds");
	}
}

Scope* Scope::GetParent()
{
	return mParent;
}

bool Scope::operator!=(const Scope& rhs) const
{
	return !(operator==(rhs));
}

bool Scope::operator==(const Scope& rhs) const
{
	bool isEqual = false;

	if (mOrderVector.Size() == rhs.mOrderVector.Size())
	{
		isEqual = true;
		auto lhsIterator = mOrderVector.begin();
		auto rhsIterator = rhs.mOrderVector.begin();

		for (; rhsIterator != rhs.mOrderVector.end(); ++rhsIterator, ++lhsIterator)
		{
			if ((*(*lhsIterator) != *(*rhsIterator)))
			{
				isEqual = false;
				break;
			}
			else
			{
				isEqual = true;
			}
		}
	}
	return isEqual;
}

Scope& Scope::AppendScope(const std::string& key)
{
	auto found = mContainer.Find(key);
	Scope* childScope = new Scope();
	childScope->mParent = this;
	if (found != mContainer.end())
	{
		Datum& foundDatum = found->second;
		if ((foundDatum.GetType()!=Datum::DatumType::TABLE)||(foundDatum.IsExternal()))
		{
			throw std::exception("Datum with the given name already exists but is not a table type, or is external.");
		}

		foundDatum.PushBack(childScope);
		return *childScope;
	}
	else
	{
		Datum& datumToBeInserted = Append(key, Datum::DatumType::TABLE);
		datumToBeInserted.Set(childScope);
		return *childScope;
	}
}

Datum* Scope::Search(const std::string& key, Scope** foundScope) const
{
	Datum* foundDatum = Find(key);
	if (foundDatum)
	{
		if (foundScope)
		{
			*foundScope = const_cast<Scope*>(this);
		}
	}
	else if (mParent)
	{
		foundDatum = mParent->Search(key, foundScope);
	}
	return foundDatum;
}

#pragma endregion