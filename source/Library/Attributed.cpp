#include "pch.h"
#include "Attributed.h"

using namespace Library;

RTTI_DEFINITIONS(Attributed)



Attributed::Signature::Signature(const std::string& name, const Datum::DatumType type, Storage& internalInitialValue, const std::uint32_t size, Storage& externalAddress)
:mName(name), mType(type), mInitialValue(internalInitialValue), mSize(size), mAddress(externalAddress)
{

}

Attributed::Attributed()
:mSignatureVector()
{
	Append("this") = (RTTI*)this;
}

const Vector<Scope::PairType*>& Attributed::GetOrderVector() const
{
	return mOrderVector;
}

Attributed::Attributed(const Attributed& rhs)
: Attributed()
{
	operator=(rhs);
}

Attributed& Attributed::operator = (const Attributed& rhs)
{
	if (this != &rhs)
	{
		Scope::operator=(rhs);
		(*this)["this"] = (RTTI*)this;
	}
	return *this;
}

void Attributed::Populate()
{
	Append("this") = (RTTI*)this;
	for (auto& it : mSignatureVector)
	{
		Datum& appendedDatum = Append(it.mName, it.mType);
		if (it.mAddress.i)
		{
			switch (it.mType)
			{
			case Datum::DatumType::INTEGER:
				appendedDatum.SetStorage(it.mAddress.i, it.mSize);
				break;
			case Datum::DatumType::FLOAT:
				appendedDatum.SetStorage(it.mAddress.f, it.mSize);
				break;
			case Datum::DatumType::VECTOR:
				appendedDatum.SetStorage(it.mAddress.v, it.mSize);
				break;
			case Datum::DatumType::MATRIX:
				appendedDatum.SetStorage(it.mAddress.m, it.mSize);
				break;
			case Datum::DatumType::TABLE:
				appendedDatum.SetStorage(it.mAddress.t, it.mSize);
				break;
			case Datum::DatumType::STRING:
				appendedDatum.SetStorage(it.mAddress.s, it.mSize);
				break;
			case Datum::DatumType::POINTER:
				appendedDatum.SetStorage(it.mAddress.p, it.mSize);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (it.mType)
			{
			case Datum::DatumType::INTEGER:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					appendedDatum.Set((it.mInitialValue.i[0]), i);
				}
				break;
			case Datum::DatumType::FLOAT:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					appendedDatum.Set((it.mInitialValue.f[0]), i);
				}
				break;
			case Datum::DatumType::VECTOR:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					appendedDatum.Set((it.mInitialValue.v[0]), i);
				}
				break;
			case Datum::DatumType::MATRIX:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					appendedDatum.Set((it.mInitialValue.m[0]), i);
				}
				break;
			case Datum::DatumType::TABLE:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					Scope** newScope = static_cast<Scope**>(it.mInitialValue.t);
					if (!(*newScope))
					{
						*newScope = new Scope();
					}
					Adopt(**newScope, it.mName, i);
				}
				break;
			case Datum::DatumType::STRING:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					std::string s = *it.mInitialValue.s;
					appendedDatum.Set((s), i);
				}
				break;
			case Datum::DatumType::POINTER:
				for (std::uint32_t i = 0; i < it.mSize; i++)
				{
					appendedDatum.Set((it.mInitialValue.p[0]), i);
				}
				break;
			default:
				break;
			}
		}
	}
}

bool Attributed::IsPrescribedAttribute(const std::string& attributeName) const
{
	for (auto it = mSignatureVector.begin(); it != mSignatureVector.end(); ++it)
	{
		if ((*it).mName == attributeName)
		{
			return true;
		}
	}
	return false;
}

bool Attributed::IsAuxiliaryAttribute(const std::string& attributeName) const
{
	if (IsAttribute(attributeName) && !IsPrescribedAttribute(attributeName))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Attributed::IsAttribute(const std::string& attributeName) const
{
	auto* foundDatum = Find(attributeName);
	if (foundDatum)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Datum& Attributed::AppendAuxiliaryAttribute(const std::string& attributeName)
{
	if (IsPrescribedAttribute(attributeName))
	{
		throw std::exception("Cannot append Prescribed Attribute as an Auxiliary.");
	}
	return Append(attributeName);
}


std::uint32_t Attributed::AuxiliaryBegin() const
{
	return mSignatureVector.Size() + 1;
}

void Attributed::Clear()
{
	Scope::Clear();

	Append("this") = (RTTI*) this;
}
