#include "pch.h"

using namespace Library;
using namespace std;

#pragma region Datum

#pragma region FunctionPointerInitialization

const Datum::AllocateFunctionArray Datum::sAllocate[] =
{
	Datum::AllocateInteger,
	Datum::AllocateFloat,
	Datum::AllocateVector,
	Datum::AllocateMatrix,
	Datum::AllocateTable,
	Datum::AllocateString,
	Datum::AllocatePointer
};

const Datum::CopyFunctionArray Datum::sCopy[] =
{
	Datum::CopyInteger,
	Datum::CopyFloat,
	Datum::CopyVector,
	Datum::CopyMatrix,
	Datum::CopyTable,
	Datum::CopyString,
	Datum::CopyPointer
};

const Datum::DeallocateFunctionArray Datum::sDeallocate[] =
{
	Datum::DeallocateInteger,
	Datum::DeallocateFloat,
	Datum::DeallocateVector,
	Datum::DeallocateMatrix,
	Datum::DeallocateTable,
	Datum::DeallocateString,
	Datum::DeallocatePointer
};

const Datum::CompareFunctionArray Datum::sIsEqual[] =
{
	Datum::CompareIntegerFunc,
	Datum::CompareFloatFunc,
	Datum::CompareVectorFunc,
	Datum::CompareMatrixFunc,
	Datum::CompareTableFunc,
	Datum::CompareStringFunc,
	Datum::ComparePointerFunc
};

const Datum::SetFromStringFunctionArray Datum::sSetFromString[] =
{
	Datum::StringToInteger,
	Datum::StringToFloat,
	Datum::StringToVector,
	Datum::StringToMatrix,
	Datum::StringToTable,
	Datum::StringToString,
	Datum::StringToPointer
};

const Datum::ToStringFunctionArray Datum::sToString[] =
{
	Datum::IntToString,
	Datum::FloatToString,
	Datum::VecToString,
	Datum::MatToString,
	Datum::TableToString,
	Datum::StringToString,
	Datum::PointerToString
};

#pragma endregion

#pragma region Datum::ToString Function Overloads

std::string Datum::IntToString(const Datum& data, std::uint32_t index)
{
	return std::to_string(data.mStorageArray.i[index]);
}

std::string Datum::FloatToString(const Datum& data, std::uint32_t index)
{
	return std::to_string(data.mStorageArray.f[index]);
}

std::string Datum::VecToString(const Datum& data, std::uint32_t index)
{
	return glm::to_string(data.mStorageArray.v[index]);
}

std::string Datum::MatToString(const Datum& data, std::uint32_t index)
{
	return glm::to_string(data.mStorageArray.m[index]);
}

std::string Datum::StringToString(const Datum& data, std::uint32_t index)
{
	return data.mStorageArray.s[index];
}

std::string Datum::PointerToString(const Datum& data, std::uint32_t index)
{
	return "This points to an RTTI";
}

std::string Datum::TableToString(const Datum& data, std::uint32_t index)
{
	return "Table yet to be implemented";
}

#pragma endregion

#pragma region Datum::SetFromString Function Overloads
void Datum::StringToInteger(Datum& data, const std::string inputString, const std::uint32_t index)
{
	std::int32_t temp = std::atoi(inputString.c_str());
	data.Set(temp, index);
}

void Datum::StringToFloat(Datum& data, const std::string inputString, const std::uint32_t index)
{
	std::float_t temp = static_cast<std::float_t>(std::atof(inputString.c_str()));
	data.Set(temp, index);
}

void Datum::StringToVector(Datum& data, const std::string inputString, const std::uint32_t index)
{
	float a, b, c, d = 0.0f;
	sscanf_s(inputString.c_str(), "%f, %f, %f, %f", &a, &b, &c, &d);
	auto temp = glm::vec4(a, b, c, d);
	data.Set(temp, index);
}

void Datum::StringToMatrix(Datum& data, const std::string inputString, const std::uint32_t index)
{
	float a[16];
	sscanf_s(inputString.c_str(), "((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9], &a[10], &a[11], &a[12], &a[13], &a[14], &a[15]);
	auto temp = glm::mat4(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]);
	data.Set(temp, index);
}

void Datum::StringToString(Datum& data, const std::string inputString, const std::uint32_t index)
{
	data.Set(inputString, index);
}

void Datum::StringToPointer(Datum& data, const std::string inputString, const std::uint32_t index)
{
	//no implementation
}

void Datum::StringToTable(Datum& data, const std::string inputString, const std::uint32_t index)
{
	//no implementation
}
#pragma endregion

#pragma region AllocateFunctions

void Datum::AllocateInteger(Datum& data, std::uint32_t size)
{
	std::int32_t* newData = new std::int32_t[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = 0;
	}

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.i[index];
		}
	}
	data.mCapacity = size;
	delete[] data.mStorageArray.i;
	data.mStorageArray.i = newData;
}

void Datum::AllocateFloat(Datum& data, std::uint32_t size)
{
	std::float_t* newData = new std::float_t[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = 0.f;
	}

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.f[index];
		}
	}
	data.mCapacity = size;
	delete[] data.mStorageArray.f;
	data.mStorageArray.f = newData;
}

void Datum::AllocateVector(Datum& data, std::uint32_t size)
{
	glm::vec4* newData = new glm::vec4[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = glm::vec4();
	}

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.v[index];
		}
	}
	data.mCapacity = size;
	delete[] data.mStorageArray.v;
	data.mStorageArray.v = newData;
}

void Datum::AllocateMatrix(Datum& data, std::uint32_t size)
{
	glm::mat4* newData = new glm::mat4[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = glm::mat4();
	}
	for (std::uint32_t index = 0; index < data.mSize; index++)
	{
		newData[index] = data.mStorageArray.m[index];
	}
	data.mCapacity = size;

	delete[] data.mStorageArray.m;
	data.mStorageArray.m = newData;
}

void Datum::AllocateTable(Datum& data, std::uint32_t size)
{
	Scope** newData = new Scope*[size];

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.t[index];
		}
	}

	data.mCapacity = size;
	delete[] data.mStorageArray.t;
	data.mStorageArray.t = newData;
}

void Datum::AllocateString(Datum& data, std::uint32_t size)
{
	std::string* newData = new std::string[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = "";
	}

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.s[index];
		}
	}
	data.mCapacity = size;
	delete[] data.mStorageArray.s;
	data.mStorageArray.s = newData;
}

void Datum::AllocatePointer(Datum& data, std::uint32_t size)
{
	RTTI** newData = new RTTI*[size];

	for (std::uint32_t index = 0; index < size; index++)
	{
		newData[index] = nullptr;
	}

	if (data.mSize > 0)
	{
		for (std::uint32_t index = 0; index < data.mSize; index++)
		{
			newData[index] = data.mStorageArray.p[index];
		}
	}
	data.mCapacity = size;
	delete[] data.mStorageArray.p;
	data.mStorageArray.p = newData;
}

#pragma endregion

#pragma region CopyFunctions

void Datum::CopyInteger(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			lhs.mStorageArray.i[index] = rhs.mStorageArray.i[index];
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyFloat(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			lhs.mStorageArray.f[index] = rhs.mStorageArray.f[index];
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyVector(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			lhs.mStorageArray.v[index] = rhs.mStorageArray.v[index];
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyMatrix(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			lhs.mStorageArray.m[index] = rhs.mStorageArray.m[index];
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyTable(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			*(lhs.mStorageArray.t[index]) = *(rhs.mStorageArray.t[index]);
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyString(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			lhs.mStorageArray.s[index] = rhs.mStorageArray.s[index];
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

void Datum::CopyPointer(Datum& lhs, const Datum& rhs)
{
	if (lhs.mType != rhs.mType)
	{
		throw std::exception("Datum types do not match");
	}

	if (lhs.mSize >= rhs.mSize)
	{
		for (std::uint32_t index = 0; index < rhs.mSize; index++)
		{
			*(lhs.mStorageArray.p[index]) = *(rhs.mStorageArray.p[index]);
		}
	}
	else
	{
		throw std::exception("Not enough memory to copy!");
	}
}

#pragma endregion

#pragma region DeallocateFunctions

void Datum::DeallocateInteger(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[] data.mStorageArray.i;
	}
	data.mStorageArray.i = nullptr;
}

void Datum::DeallocateFloat(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[] data.mStorageArray.f;
	}
	data.mStorageArray.f = nullptr;
}

void Datum::DeallocateVector(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[] data.mStorageArray.v;
	}
	data.mStorageArray.v = nullptr;
}

void Datum::DeallocateMatrix(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[] data.mStorageArray.m;
	}
	data.mStorageArray.m = nullptr;
}

void Datum::DeallocateTable(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[](data.mStorageArray.t);
	}
	data.mStorageArray.t = nullptr;

}

void Datum::DeallocateString(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[] data.mStorageArray.s;
	}
	data.mStorageArray.s = nullptr;

}

void Datum::DeallocatePointer(Datum& data)
{
	if (!data.mIsExternal)
	{
		delete[](data.mStorageArray.p);
	}
	data.mStorageArray.p = nullptr;

}


#pragma endregion

#pragma region CompareFunctions
bool Datum::CompareIntegerFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.i[index] != rhs.mStorageArray.i[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::CompareFloatFunc(const Datum& lhs, const Datum& rhs){
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.f[index] != rhs.mStorageArray.f[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::CompareVectorFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.v[index] != rhs.mStorageArray.v[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::CompareMatrixFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.m[index] != rhs.mStorageArray.m[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::CompareTableFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (*(lhs.mStorageArray.t[index]) != *(rhs.mStorageArray.t[index]))
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::CompareStringFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.s[index] != rhs.mStorageArray.s[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
bool Datum::ComparePointerFunc(const Datum& lhs, const Datum& rhs)
{
	bool isEqual = true;
	for (std::uint32_t index = 0; index < lhs.mSize; index++)
	{
		if (lhs.mStorageArray.p[index] != rhs.mStorageArray.p[index])
		{
			isEqual = false;
			break;
		}
	}

	return isEqual;
}
#pragma endregion

#pragma region Constructor

Datum::Datum(DatumType type, std::uint32_t size)
	:mType(type), mIsExternal(false), mSize(0), mCapacity(0), mStorageArray()
{
	if (type >= DatumType::MAXIMUM)
	{
		throw std::exception("Invalid DatumType!");
	}
	if (type != DatumType::UNKNOWN)
	{
		SetSize(size);
	}
}

Datum::Datum(const Datum& rhs)
	:mType(DatumType::UNKNOWN)
{
	operator=(rhs);
}

#pragma endregion

#pragma region	Datum::operator=

Datum& Datum::operator=(const Datum& rhs)
{
	if (this != &rhs)
	{
		Clear();
		mType = rhs.mType;
		mIsExternal = rhs.mIsExternal;

		if (rhs.mIsExternal)
		{
			mStorageArray = rhs.mStorageArray;
			mCapacity = rhs.mCapacity;
		}
		else
		{
			if (rhs.mType != DatumType::UNKNOWN)
			{
				SetSize(rhs.mSize);
				sCopy[(int)mType](*this, rhs);
			}
		}
		mSize = rhs.mSize;
	}
	return *this;
}

Datum& Datum::operator=(const std::int32_t& rhs)
{
	if (mStorageArray.i != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(const std::float_t& rhs)
{
	if (mStorageArray.f != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(const glm::vec4& rhs)
{
	if (mStorageArray.v != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(const glm::mat4& rhs)
{
	if (mStorageArray.m != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(const std::string& rhs)
{
	if (mStorageArray.s != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(RTTI* rhs)
{
	if (mStorageArray.p != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

Datum& Datum::operator=(Scope* rhs)
{
	if (mStorageArray.t != &rhs)
	{
		Set(rhs);
		mSize = 1;
	}

	return *this;
}

#pragma endregion

#pragma region Datum::operator!=

bool Datum::operator!=(const Datum& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const std::int32_t& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const std::float_t& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const glm::vec4& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const glm::mat4& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const std::string& rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const RTTI* rhs) const
{
	return !operator==(rhs);
}

bool Datum::operator!=(const Scope* rhs) const
{
	return !operator==(rhs);
}

#pragma endregion

#pragma region Datum::operator==

bool Datum::operator==(const Datum& rhs) const
{
	if ((mSize == rhs.mSize) && (mType == rhs.mType))
	{
		if (mType != DatumType::UNKNOWN)
		{
			return sIsEqual[static_cast<std::uint32_t>(mType)](*this, rhs);
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const std::int32_t& rhs) const
{
	if ((mType == DatumType::INTEGER) && (mSize > 0))
	{
		return(mStorageArray.i[0] == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const std::float_t& rhs) const
{
	if ((mType == DatumType::FLOAT) && (mSize > 0))
	{
		return(*mStorageArray.f == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const glm::vec4& rhs) const
{
	if ((mType == DatumType::VECTOR) && (mSize > 0))
	{
		return(*mStorageArray.v == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const glm::mat4& rhs) const
{
	if ((mType == DatumType::MATRIX) && (mSize > 0))
	{
		return(*mStorageArray.m == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const std::string& rhs) const
{
	if ((mType == DatumType::STRING) && (mSize > 0))
	{
		return(*mStorageArray.s == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const RTTI* rhs) const
{
	if ((mType == DatumType::POINTER) && (mSize > 0))
	{
		return(*mStorageArray.p == rhs);
	}
	else
	{
		return false;
	}
}

bool Datum::operator==(const Scope* rhs) const
{
	if ((mType == DatumType::TABLE) && (mSize > 0))
	{
		return(*mStorageArray.t == rhs);
	}
	else
	{
		return false;
	}
}

#pragma endregion

#pragma region Datum::Get

std::int32_t& Datum::GetInteger(std::uint32_t index)
{
	if (mType != DatumType::INTEGER)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.i[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

std::float_t& Datum::GetFloat(std::uint32_t index)
{
	if (mType != DatumType::FLOAT)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.f[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

glm::vec4& Datum::GetVector(std::uint32_t index)
{
	if (mType != DatumType::VECTOR)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.v[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

glm::mat4& Datum::GetMatrix(std::uint32_t index)
{
	if (mType != DatumType::MATRIX)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.m[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

std::string& Datum::GetString(std::uint32_t index)
{
	if (mType != DatumType::STRING)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.s[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

RTTI* Datum::GetPointer(std::uint32_t index)
{
	if (mType != DatumType::POINTER)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.p[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

Scope* Datum::GetTable(std::uint32_t index)
{
	if (mType != DatumType::TABLE)
	{
		throw std::exception("Incompatible Datum type.");
	}
	if (index < mSize)
	{
		return mStorageArray.t[index];
	}
	else
	{
		throw std::exception("Index Out Of Bounds.");
	}
}

#pragma endregion

#pragma region Datum::Clear

void Datum::Clear()
{
	if (mType != DatumType::UNKNOWN)
		sDeallocate[(int)mType](*this);
	mSize = 0;
	mCapacity = 0;
	mIsExternal = false;
}

#pragma endregion

#pragma region Datum::SetType

void Datum::SetType(DatumType type)
{
	if (type >= DatumType::MAXIMUM)
	{
		throw std::exception("Invalid DatumType");
	}
	if (mType == DatumType::UNKNOWN)
	{
		mType = type;
	}
	else
	{
		throw std::exception("Datum type is already set.");
	}
}

#pragma endregion

#pragma region Datum::Size
std::uint32_t Datum::Size() const
{
	return mSize;
}
#pragma endregion

#pragma region Datum::GetType

Datum::DatumType Datum::GetType() const
{
	return mType;
}

#pragma endregion

#pragma region Destructor

Datum::~Datum()
{
	Clear();
}

#pragma endregion

#pragma region Datum::SetSize

void Datum::SetSize(const std::uint32_t newSize)
{
	if (!mIsExternal)
	{
		if (mType == DatumType::UNKNOWN)
		{
			throw std::exception("Cannot set size of an unknown datatype.");
		}
		if (newSize < mCapacity)
		{
			mSize = newSize;
		}
		else
		{
			if (mType != DatumType::MAXIMUM)
			{
				sAllocate[static_cast<int>(mType)](*this, newSize);
				mCapacity = newSize;
				mSize = mCapacity;
			}
		}
	}
	else
	{
		throw std::exception("Datum storage is external.");
	}
}

#pragma endregion

#pragma	region Datum::SetStorage

void Datum::SetStorage(std::int32_t* externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.i = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::INTEGER;
}

void Datum::SetStorage(std::float_t* externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.f = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::FLOAT;
}

void Datum::SetStorage(glm::vec4* externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.v = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::VECTOR;
}

void Datum::SetStorage(glm::mat4* externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.m = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::MATRIX;
}

void Datum::SetStorage(std::string* externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.s = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::STRING;
}

void Datum::SetStorage(RTTI** externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.p = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::POINTER;
}

void Datum::SetStorage(Scope** externalData, const std::uint32_t& size)
{
	if (!mIsExternal && (mSize > 0))
	{
		throw std::exception("Datum already contains internal data.");
	}
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
	{
		throw std::exception("Value passed in does not match Datum type.");
	}

	mIsExternal = true;
	mStorageArray.t = externalData;
	mSize = size;
	mCapacity = mSize;
	mType = DatumType::TABLE;
}

#pragma endregion

#pragma	region Datum::Set

void Datum::Set(const std::int32_t& data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::INTEGER))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::INTEGER;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.i[index] = data;
}

void Datum::Set(const std::float_t& data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::FLOAT))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::FLOAT;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.f[index] = data;
}

void Datum::Set(const glm::vec4& data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::VECTOR))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::VECTOR;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.v[index] = data;
}

void Datum::Set(const glm::mat4& data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MATRIX))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::MATRIX;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.m[index] = data;
}

void Datum::Set(const std::string& data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::STRING))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::STRING;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.s[index] = data;
}

void Datum::Set(RTTI* data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::POINTER))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::POINTER;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.p[index] = data;
}

void Datum::Set(Scope* data, const std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::TABLE))
	{
		throw std::exception("Incompatible datatype.");
	}
	mType = DatumType::TABLE;
	if (index < mCapacity)
	{
		if (index >= mSize)
		{
			mSize = index + 1;
		}
	}
	else
	{
		if (!mIsExternal)
		{
			SetSize(index + 1);
		}
		else
		{
			throw std::exception("Cannot set to index bigger than capacity in external data.");
		}
	}
	mStorageArray.t[index] = data;
}

#pragma endregion

#pragma region Datum::ToString

std::string Datum::ToString(std::uint32_t index) const
{
	if (index < mSize)
	{
		if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MAXIMUM))
		{
			return sToString[static_cast<int>(mType)](*this, index);
		}
		else
		{
			std::string returnString = "Unknown data type";
			return returnString;
		}
	}
	else
	{
		throw std::exception("Index out of bounds.");
	}
}
#pragma endregion

#pragma region Datum::SetFromString

void Datum::SetFromString(const std::string& stringToParse, std::uint32_t index)
{
	if ((mType != DatumType::UNKNOWN) && (mType != DatumType::MAXIMUM))
	{
		sSetFromString[static_cast<int>(mType)](*this, stringToParse, index);
	}
	else
	{
		throw std::exception("Datum data type has not been set.");
	}
}

#pragma endregion

Scope& Datum::operator[](std::uint32_t index)
{
	return *GetTable(index);
}

bool Datum::IsExternal()
{
	return mIsExternal;
}

void Datum::PushBack(Scope* data)
{
	if (!mIsExternal)
	{
		Set(data, Size());
	}
	else
	{
		throw std::exception("Cannot push back to external datum");
	}
}
#pragma endregion