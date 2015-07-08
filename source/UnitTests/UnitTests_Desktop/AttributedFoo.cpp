#include "AttributedFoo.h"

using namespace UnitTests;
using namespace Library;

RTTI_DEFINITIONS(AttributedFoo)

AttributedFoo::AttributedFoo()
:Attributed()
{
	std::string mInternalString = "String";
	INTERNAL_ATTRIBUTE(std::string("InternalString"), Datum::DatumType::STRING, &mInternalString, 3);

	std::uint32_t mInternalInt = 2;
	INTERNAL_ATTRIBUTE(std::string("InternalInteger"), Datum::DatumType::INTEGER, &mInternalInt, 3);

	float mInternalFloat = 2.0f;
	INTERNAL_ATTRIBUTE(std::string("InternalFloat"), Datum::DatumType::FLOAT, &mInternalFloat, 3);
	
	glm::vec4 mInternalVector = glm::vec4(2);
	INTERNAL_ATTRIBUTE(std::string("InternalVector"), Datum::DatumType::VECTOR, &mInternalVector, 3);

	glm::mat4 mInternalMatrix = glm::mat4(3);
	INTERNAL_ATTRIBUTE(std::string("InternalMatrix"), Datum::DatumType::MATRIX, &mInternalMatrix, 3);

	RTTI* mInternalRTTI = nullptr;
	INTERNAL_ATTRIBUTE(std::string("InternalRTTI"), Datum::DatumType::POINTER, &mInternalRTTI, 3);

	EXTERNAL_ATTRIBUTE(std::string("ExternalString"), Datum::DatumType::STRING, &mExternalString, 1);

	EXTERNAL_ATTRIBUTE(std::string("ExternalInteger"), Datum::DatumType::INTEGER, &mExternalInteger, 1);

	EXTERNAL_ATTRIBUTE(std::string("ExternalFloat"), Datum::DatumType::FLOAT, &mExternalFloat, 1);

	EXTERNAL_ATTRIBUTE(std::string("ExternalVector"), Datum::DatumType::VECTOR, &mExternalVector, 1);

	EXTERNAL_ATTRIBUTE(std::string("ExternalMatrix"), Datum::DatumType::MATRIX, &mExternalMatrix, 1);

	Populate();
}

AttributedFoo::~AttributedFoo()
{
	Clear();
}

