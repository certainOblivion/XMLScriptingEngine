#include <cxxtest/TestSuite.h>
#include "Datum.h"
#include <crtdbg.h>
#include "Foo.h"

using namespace Library;
using namespace UnitTests;

class DatumTestSuite : public CxxTest::TestSuite
{
public:

	void TestStrings()
	{
		Datum mDatum;
		mDatum = "BTT";
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::STRING);
		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetString(), "BTT");

		mDatum.Set("Mr. Hyde", 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);
		TS_ASSERT_EQUALS(mDatum.GetString(), "BTT");
		TS_ASSERT_EQUALS(mDatum.GetString(1), "Mr. Hyde");

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);
		TS_ASSERT_EQUALS(mNewDatum.GetString(), "BTT");
		TS_ASSERT_EQUALS(mNewDatum.GetString(1), "Mr. Hyde");

		TS_ASSERT(mDatum == mNewDatum);

		mNewDatum.Set("BTT", 1);

		TS_ASSERT(mDatum != mNewDatum);
		mNewDatum.Set("BTT", 5);

		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);
		TS_ASSERT_EQUALS(mNewDatum.GetString(), "BTT");
		TS_ASSERT_EQUALS(mNewDatum.GetString(1), "BTT");
		TS_ASSERT_EQUALS(mNewDatum.GetString(5), "BTT");

		mDatum.SetSize(6);

		TS_ASSERT_EQUALS(mNewDatum.Size(), mDatum.Size());
		TS_ASSERT(mNewDatum != mDatum);

		TS_ASSERT_THROWS(mNewDatum.GetInteger(), std::exception);
		TS_ASSERT_THROWS(mNewDatum.GetString(6), std::exception);

		std::string stringArray[] = { "A", "B", "C"};

		TS_ASSERT_THROWS(mNewDatum.SetStorage(stringArray, 3), std::exception);

		Datum externalDatum;
		externalDatum.SetStorage(stringArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(externalDatum.GetString(), "A");
		TS_ASSERT_EQUALS(externalDatum.GetString(1), "B");
		TS_ASSERT_EQUALS(externalDatum.GetString(2), "C");

		TS_ASSERT_THROWS(externalDatum.SetSize(4), std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);
		TS_ASSERT_EQUALS(newExternalDatum.GetString(),"A");
		TS_ASSERT_EQUALS(newExternalDatum.GetString(1), "B");
		TS_ASSERT_EQUALS(newExternalDatum.GetString(2), "C");

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		newExternalDatum.Set("BTT", 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetString(1), "BTT");
		TS_ASSERT_EQUALS(externalDatum.GetString(1), "BTT");

		TS_ASSERT_EQUALS(newExternalDatum.ToString(1), "BTT");
		TS_ASSERT_EQUALS(newExternalDatum.ToString(), "A");

		newExternalDatum.SetFromString("PHD", 1);
		TS_ASSERT_EQUALS(newExternalDatum.ToString(1), "PHD");

		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);
	}

	void TestMatrix()
	{
		Datum mDatum;
		glm::mat4 matrix1 = glm::mat4(1.f);
		mDatum = matrix1;
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::MATRIX);
		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetMatrix(), matrix1);

		glm::mat4 matrix2 = glm::mat4(2.f);
		mDatum.Set(matrix2, 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);
		TS_ASSERT_EQUALS(mDatum.GetMatrix(), matrix1);
		TS_ASSERT_EQUALS(mDatum.GetMatrix(1), matrix2);

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);
		TS_ASSERT_EQUALS(mNewDatum.GetMatrix(), matrix1);
		TS_ASSERT_EQUALS(mNewDatum.GetMatrix(1), matrix2);

		TS_ASSERT(mDatum == mNewDatum);

		glm::mat4 matrix3 = glm::mat4(3.f);

		mNewDatum.Set(matrix3, 1);

		TS_ASSERT(mDatum != mNewDatum);

		mNewDatum.SetSize(6);
		mNewDatum.Set(matrix3, 5);

		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);
		TS_ASSERT_EQUALS(mNewDatum.GetMatrix(), matrix1);
		TS_ASSERT_EQUALS(mNewDatum.GetMatrix(1), matrix3);
		TS_ASSERT_EQUALS(mNewDatum.GetMatrix(5), matrix3);

		mDatum.SetSize(6);

		TS_ASSERT_EQUALS(mNewDatum.Size(), mDatum.Size());
		TS_ASSERT(mNewDatum != mDatum);

		TS_ASSERT_THROWS(mNewDatum.GetInteger(), std::exception);
		TS_ASSERT_THROWS(mNewDatum.GetMatrix(6), std::exception);

		glm::mat4 matArray[] = { matrix1, matrix2, matrix3 };

		TS_ASSERT_THROWS(mNewDatum.SetStorage(matArray, 3), std::exception);
		TS_ASSERT_EQUALS(mNewDatum.ToString(), "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))");

		Datum externalDatum;
		externalDatum.SetStorage(matArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(externalDatum.GetMatrix(), matrix1);
		TS_ASSERT_EQUALS(externalDatum.GetMatrix(1), matrix2);
		TS_ASSERT_EQUALS(externalDatum.GetMatrix(2), matrix3);

		TS_ASSERT_THROWS(externalDatum.SetSize(4), std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);
		TS_ASSERT_EQUALS(newExternalDatum.GetMatrix(), matrix1);
		TS_ASSERT_EQUALS(newExternalDatum.GetMatrix(1), matrix2);
		TS_ASSERT_EQUALS(newExternalDatum.GetMatrix(2), matrix3);

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		newExternalDatum.Set(matrix3, 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetMatrix(1), matrix3);
		TS_ASSERT_EQUALS(externalDatum.GetMatrix(1), matrix3);

		newExternalDatum.SetFromString("((1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 1, 0), (0, 0, 0, 1))", 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetMatrix(1), matrix1);
		TS_ASSERT_EQUALS(externalDatum.GetMatrix(1), matrix1);

		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);
	}

	void TestVector()
	{
		Datum mDatum;
		glm::vec4 vector = glm::vec4(1.f, 2.f, 3.f, 4.f);
		mDatum = vector;
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::VECTOR);
		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetVector(), vector);

		glm::vec4 vector2 = glm::vec4(5.f, 6.f, 7.f, 8.f);
		mDatum.Set(vector2, 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);
		TS_ASSERT_EQUALS(mDatum.GetVector(), vector);
		TS_ASSERT_EQUALS(mDatum.GetVector(1), vector2);

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);
		TS_ASSERT_EQUALS(mNewDatum.GetVector(), vector);
		TS_ASSERT_EQUALS(mNewDatum.GetVector(1), vector2);

		TS_ASSERT(mDatum == mNewDatum);

		glm::vec4 vector3 = glm::vec4(10.f, 20.f, 30.f, 40.f);

		mNewDatum.Set(vector3, 1);

		TS_ASSERT(mDatum != mNewDatum);
		mNewDatum.Set(vector3, 5);

		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);
		TS_ASSERT_EQUALS(mNewDatum.GetVector(), vector);
		TS_ASSERT_EQUALS(mNewDatum.GetVector(1), vector3);
		TS_ASSERT_EQUALS(mNewDatum.GetVector(5), vector3);

		mDatum.SetSize(6);

		TS_ASSERT_EQUALS(mNewDatum.Size(), mDatum.Size());
		TS_ASSERT(mNewDatum != mDatum);

		TS_ASSERT_THROWS(mNewDatum.GetInteger(), std::exception);
		TS_ASSERT_THROWS(mNewDatum.GetVector(6), std::exception);

		glm::vec4 vecArray[] = { vector,vector2,vector3 };

		TS_ASSERT_THROWS(mNewDatum.SetStorage(vecArray, 3), std::exception);

		Datum externalDatum;
		externalDatum.SetStorage(vecArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(externalDatum.GetVector(), vector);
		TS_ASSERT_EQUALS(externalDatum.GetVector(1), vector2);
		TS_ASSERT_EQUALS(externalDatum.GetVector(2), vector3);

		TS_ASSERT_THROWS(externalDatum.SetSize(4), std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);
		TS_ASSERT_EQUALS(newExternalDatum.GetVector(), vector);
		TS_ASSERT_EQUALS(newExternalDatum.GetVector(1), vector2);
		TS_ASSERT_EQUALS(newExternalDatum.GetVector(2), vector3);

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		newExternalDatum.Set(vector3, 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetVector(1), vector3);
		TS_ASSERT_EQUALS(externalDatum.GetVector(1), vector3);

		TS_ASSERT_EQUALS(mDatum.ToString(), "fvec4(1.000000, 2.000000, 3.000000, 4.000000)");
		
		newExternalDatum.SetFromString("1.0, 2.0, 3.0, 4.0", 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetVector(1), vector);
		TS_ASSERT_EQUALS(externalDatum.GetVector(1), vector);
		
		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);
	}

	void TestFloats()
	{
		Datum mDatum;
		mDatum = 10.f;
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::FLOAT);
		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetFloat(), 10.f);

		mDatum.Set(20.f, 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);
		TS_ASSERT_EQUALS(mDatum.GetFloat(), 10.f);
		TS_ASSERT_EQUALS(mDatum.GetFloat(1), 20.f);

		TS_ASSERT_EQUALS(mDatum.ToString(), "10.000000");

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);
		TS_ASSERT_EQUALS(mNewDatum.GetFloat(), 10.f);
		TS_ASSERT_EQUALS(mNewDatum.GetFloat(1), 20.f);

		TS_ASSERT(mDatum == mNewDatum);

		mNewDatum.Set(10.f, 1);

		TS_ASSERT(mDatum != mNewDatum);
		mNewDatum.Set(10.f, 5);

		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);
		TS_ASSERT_EQUALS(mNewDatum.GetFloat(), 10.f);
		TS_ASSERT_EQUALS(mNewDatum.GetFloat(1), 10.f);
		TS_ASSERT_EQUALS(mNewDatum.GetFloat(5), 10.f);

		mDatum.SetSize(6);

		TS_ASSERT_EQUALS(mNewDatum.Size(), mDatum.Size());
		TS_ASSERT(mNewDatum != mDatum);

		TS_ASSERT_THROWS(mNewDatum.GetInteger(), std::exception);
		TS_ASSERT_THROWS(mNewDatum.GetFloat(6), std::exception);

		float floatArray[] = { 1.f, 2.f, 3.f };

		TS_ASSERT_THROWS(mNewDatum.SetStorage(floatArray, 3), std::exception);

		Datum externalDatum;
		externalDatum.SetStorage(floatArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(externalDatum.GetFloat(), 1.f);
		TS_ASSERT_EQUALS(externalDatum.GetFloat(1), 2.f);
		TS_ASSERT_EQUALS(externalDatum.GetFloat(2), 3.f);

		TS_ASSERT_THROWS(externalDatum.SetSize(4), std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);
		TS_ASSERT_EQUALS(newExternalDatum.GetFloat(), 1.f);
		TS_ASSERT_EQUALS(newExternalDatum.GetFloat(1), 2.f);
		TS_ASSERT_EQUALS(newExternalDatum.GetFloat(2), 3.f);

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		newExternalDatum.Set(3.f, 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetFloat(1), 3.f);
		TS_ASSERT_EQUALS(externalDatum.GetFloat(1), 3.f);

		newExternalDatum.SetFromString("2.0", 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetFloat(1), 2.0f);
		TS_ASSERT_EQUALS(externalDatum.GetFloat(1), 2.0f);

		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);
	}

	void TestIntegers()
	{
		Datum mDatum;
		mDatum = 10;
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::INTEGER);
		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetInteger(),10);

		TS_ASSERT_EQUALS(mDatum.ToString(), "10");

		mDatum.Set(20, 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);
		TS_ASSERT_EQUALS(mDatum.GetInteger(), 10);
		TS_ASSERT_EQUALS(mDatum.GetInteger(1), 20);

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);
		TS_ASSERT_EQUALS(mNewDatum.GetInteger(), 10);
		TS_ASSERT_EQUALS(mNewDatum.GetInteger(1), 20);

		TS_ASSERT(mDatum == mNewDatum);

		mNewDatum.Set(10, 1);

		TS_ASSERT(mDatum != mNewDatum);
		mNewDatum.Set(10, 5);

		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);
		TS_ASSERT_EQUALS(mNewDatum.GetInteger(), 10);
		TS_ASSERT_EQUALS(mNewDatum.GetInteger(1), 10);
		TS_ASSERT_EQUALS(mNewDatum.GetInteger(5), 10);

		mDatum.SetSize(6);

		TS_ASSERT_EQUALS(mNewDatum.Size(), mDatum.Size());
		TS_ASSERT(mNewDatum != mDatum);

		TS_ASSERT_THROWS(mNewDatum.GetFloat(), std::exception);
		TS_ASSERT_THROWS(mNewDatum.GetInteger(6), std::exception);

		int integerArray[] = { 1, 2, 3 };

		TS_ASSERT_THROWS(mNewDatum.SetStorage(integerArray, 3),std::exception);

		Datum externalDatum;
		externalDatum.SetStorage(integerArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(externalDatum.GetInteger(), 1);
		TS_ASSERT_EQUALS(externalDatum.GetInteger(1), 2);
		TS_ASSERT_EQUALS(externalDatum.GetInteger(2), 3);

		TS_ASSERT_THROWS(externalDatum.SetSize(4),std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);
		TS_ASSERT_EQUALS(newExternalDatum.GetInteger(), 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetInteger(1), 2);
		TS_ASSERT_EQUALS(newExternalDatum.GetInteger(2), 3);

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		newExternalDatum.Set(3, 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetInteger(1), 3);
		TS_ASSERT_EQUALS(externalDatum.GetInteger(1),3);

		newExternalDatum.SetFromString("2", 1);
		TS_ASSERT_EQUALS(newExternalDatum.GetInteger(1), 2);
		TS_ASSERT_EQUALS(externalDatum.GetInteger(1), 2);

		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);
	}

	void TestPointers()
	{
		RTTI* f1 = new Foo(1);
		RTTI* f2 = new Foo(2);

		Datum mDatum;
		mDatum = f1;

		TS_ASSERT_EQUALS(mDatum.Size(), 1);
		TS_ASSERT_EQUALS(mDatum.GetType(), Datum::DatumType::POINTER);

		mDatum.Set(f2, 1);
		TS_ASSERT_EQUALS(mDatum.Size(), 2);

		Datum mNewDatum = mDatum;
		TS_ASSERT_EQUALS(mNewDatum.Size(), 2);

		mNewDatum.SetSize(5);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 5);

		mNewDatum.Set(f1, 5);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 6);

		mDatum = f2;
		TS_ASSERT(mDatum == f2);

		mNewDatum.Clear();
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);

		RTTI* pointerArray[] = { f1, f2, f1 };

		//mNewDatum.SetStorage(*pointerArray, 3);

		Datum externalDatum;
		externalDatum.SetStorage(pointerArray, 3);

		TS_ASSERT_EQUALS(externalDatum.Size(), 3);

		TS_ASSERT_THROWS(externalDatum.SetSize(4), std::exception);

		Datum newExternalDatum = externalDatum;

		TS_ASSERT_EQUALS(newExternalDatum.Size(), 3);

		TS_ASSERT_THROWS(newExternalDatum.SetSize(4), std::exception);

		TS_ASSERT_THROWS_NOTHING(newExternalDatum.Set(f1, 1));

		newExternalDatum.Clear();
		mNewDatum.Clear();
		mDatum.SetSize(3);
		TS_ASSERT_EQUALS(newExternalDatum.Size(), 0);
		TS_ASSERT_EQUALS(mNewDatum.Size(), 0);
		TS_ASSERT_EQUALS(externalDatum.Size(), 3);
		TS_ASSERT_EQUALS(mDatum.Size(), 3);

		delete (Foo*)f1;
		delete (Foo*)f2;
	}

};