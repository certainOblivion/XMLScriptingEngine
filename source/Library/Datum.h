#pragma once
#include <cstdint>
#include "glm.hpp"
#include "gtx/string_cast.hpp"

namespace Library
{
	class Scope;
	class RTTI;

	class Datum
	{
		typedef void(*AllocateFunctionArray)(Datum& data, std::uint32_t allocSize);
		typedef void(*CopyFunctionArray)(Datum& lhs, const Datum& rhs);
		typedef void(*DeallocateFunctionArray)(Datum& data);
		typedef bool(*CompareFunctionArray)(const Datum& lhs, const Datum& rhs);
		typedef std::string(*ToStringFunctionArray)(const Datum& data, std::uint32_t index);
		typedef void(*SetFromStringFunctionArray)(Datum& data, const std::string inputString, const std::uint32_t index);

	public:
		/**Enum defining Datum types*/
		enum class DatumType
		{
			UNKNOWN = -1,
			INTEGER,
			FLOAT,
			VECTOR,
			MATRIX,
			TABLE,
			STRING,
			POINTER,
			MAXIMUM
		};

		/**Parameterized constructor.
		* @param type Data type of datum
		* @param size Size of Datum
		*/
		explicit Datum(DatumType type = DatumType::UNKNOWN, std::uint32_t size = 0);


		/**Copy constructor.
		* @param rhs Datum to be copied
		*/
		Datum(const Datum& rhs);

		/**Overloaded operator=. Copies the contents of the passed datum to this.
		* @param rhs Datum to be copied
		*/
		Datum& operator=(const Datum& rhs);

		/**Overloaded operator= by input type. Copies the contents of the passed datum to this. Considers the rhs as scalars
		* @param rhs Datum to be copied
		*/
		Datum& operator=(const std::int32_t& rhs);
		Datum& operator=(const std::float_t& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(RTTI* rhs);
		Datum& operator=(Scope* rhs);

		/**Destructor*/
		~Datum();

		/**Returns the type of the datum
		*@return DatumType type of Datum
		*/
		DatumType GetType() const;
		
		/**Set type of the datum. If type is already set, throws an exception.
		* @param Datum's type
		*/
		void SetType(DatumType type);
		
		/**Returns Datum's size
		* @return unsigned integer size of datum
		*/
		std::uint32_t Size() const;

		/**Set the size of the datum. Increase memory capacity if necessary. Will throw error if data is external.
		* @param unsigned integer size of datum
		*/
		void SetSize(std::uint32_t size);

		/**Clear datum contents and reset the size.	*/
		void Clear();

		/**Returns whether the datum is external
		* @return a boolean whether a datum is external
		*/
		bool IsExternal();

		/**Set the storage of the datum to an external variable of given size.
		* @externalData the different types of data
		* @param unsigned integer size of datum
		*/
		void SetStorage(std::int32_t* externalData, const std::uint32_t& size);
		void SetStorage(std::float_t* externalData, const std::uint32_t& size);
		void SetStorage(glm::vec4* externalData, const std::uint32_t& size);
		void SetStorage(glm::mat4* externalData, const std::uint32_t& size);
		void SetStorage(std::string* externalData, const std::uint32_t& size);
		void SetStorage(RTTI** externalData, const std::uint32_t& size);
		void SetStorage(Scope** externalData, const std::uint32_t& size);


		/**Compare the two datum or scalars provided and return true if the values are same.
		* @param value to be compared with
		*/
		bool operator==(const Datum& rhs) const;
		bool operator==(const std::int32_t& rhs) const;
		bool operator==(const std::float_t& rhs) const;
		bool operator==(const glm::vec4& rhs) const;
		bool operator==(const glm::mat4& rhs) const;
		bool operator==(const std::string& rhs) const;
		bool operator==(const RTTI* rhs) const;
		bool operator==(const Scope* rhs) const;

		/**Compare the two datum or scalars provided and return false if the values are same.
		* @param value to be compared with
		*/
		bool operator!=(const Datum& rhs) const;
		bool operator!=(const std::int32_t& rhs) const;
		bool operator!=(const std::float_t& rhs) const;
		bool operator!=(const glm::vec4& rhs) const;
		bool operator!=(const glm::mat4& rhs) const;
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const RTTI* rhs) const;
		bool operator!=(const Scope* rhs) const;

		/**Set the internal memory to the given data at the index provided
		* @param data value to be assigned
		*@param index to be assigned at
		*/
		void Set(const std::int32_t& data, const std::uint32_t index = 0);
		void Set(const std::float_t& data, const std::uint32_t index = 0);
		void Set(const glm::vec4& data, const std::uint32_t index = 0);
		void Set(const glm::mat4& data, const std::uint32_t index = 0);
		void Set(const std::string& data, const std::uint32_t index = 0);
		void Set(RTTI* data, const std::uint32_t index = 0);
		void Set(Scope* data, const std::uint32_t index = 0);
		void PushBack(Scope* data);

		/**Get the value from the datum. Will throw exception if the types don't match.
		* @param index of the value to be returned.
		*/
		std::int32_t& GetInteger(std::uint32_t index = 0);
		std::float_t& GetFloat(std::uint32_t index = 0);
		glm::vec4& GetVector(std::uint32_t index = 0);
		glm::mat4& GetMatrix(std::uint32_t index = 0);
		std::string& GetString(std::uint32_t index = 0);
		RTTI* GetPointer(std::uint32_t index = 0);
		Scope* GetTable(std::uint32_t index = 0);

		/**Get the value from the scope datum. Will throw exception if the types don't match.
		* @param index of the value to be returned.
		*/
		Scope& operator[](std::uint32_t index);
		/**Set the datum values from string at the given index. Does not set pointer data.
		* @param string to be parsed
		* @param index to be assigned to
		*/
		void SetFromString(const std::string& stringToParse, std::uint32_t index = 0);

		/**Return a string equivalent of the data in the datum
		* @param index of the value to be returned
		*/
		std::string ToString(std::uint32_t index = 0) const;

	private:
		static const AllocateFunctionArray sAllocate[7];
		static void AllocateInteger(Datum& data, std::uint32_t size);
		static void AllocateFloat(Datum& data, std::uint32_t size);
		static void AllocateVector(Datum& data, std::uint32_t size);
		static void AllocateMatrix(Datum& data, std::uint32_t size);
		static void AllocateTable(Datum& data, std::uint32_t size);
		static void AllocateString(Datum& data, std::uint32_t size);
		static void AllocatePointer(Datum& data, std::uint32_t size);

		static const CopyFunctionArray sCopy[7];
		static void CopyInteger(Datum& lhs, const Datum& rhs);
		static void CopyFloat(Datum& lhs, const Datum& rhs);
		static void CopyVector(Datum& lhs, const Datum& rhs);
		static void CopyMatrix(Datum& lhs, const Datum& rhs);
		static void CopyTable(Datum& lhs, const Datum& rhs);
		static void CopyString(Datum& lhs, const Datum& rhs);
		static void CopyPointer(Datum& lhs, const Datum& rhs);

		static const DeallocateFunctionArray sDeallocate[7];
		static void DeallocateInteger(Datum& data);
		static void DeallocateFloat(Datum& data);
		static void DeallocateVector(Datum& data);
		static void DeallocateMatrix(Datum& data);
		static void DeallocateTable(Datum& data);
		static void DeallocateString(Datum& data);
		static void DeallocatePointer(Datum& data);

		static const CompareFunctionArray sIsEqual[7];
		static bool CompareIntegerFunc(const Datum& lhs, const Datum& rhs);
		static bool CompareFloatFunc(const Datum& lhs, const Datum& rhs);
		static bool CompareVectorFunc(const Datum& lhs, const Datum& rhs);
		static bool CompareMatrixFunc(const Datum& lhs, const Datum& rhs);
		static bool CompareTableFunc(const Datum& lhs, const Datum& rhs);
		static bool CompareStringFunc(const Datum& lhs, const Datum& rhs);
		static bool ComparePointerFunc(const Datum& lhs, const Datum& rhs);

		static const ToStringFunctionArray sToString[7];
		static std::string IntToString(const Datum& data, std::uint32_t index);
		static std::string FloatToString(const Datum& data, std::uint32_t index);
		static std::string VecToString(const Datum& data, std::uint32_t index);
		static std::string MatToString(const Datum& data, std::uint32_t index);
		static std::string TableToString(const Datum& data, std::uint32_t index);
		static std::string StringToString(const Datum& data, std::uint32_t index);
		static std::string PointerToString(const Datum& data, std::uint32_t index);

		static const SetFromStringFunctionArray sSetFromString[7];
		static void StringToInteger(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToFloat(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToVector(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToMatrix(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToTable(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToString(Datum& data, const std::string inputString, const std::uint32_t index);
		static void StringToPointer(Datum& data, const std::string inputString, const std::uint32_t index);

		bool mIsExternal;
		union Storage
		{
			std::int32_t* i;
			std::float_t* f;
			Scope**	 t;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			RTTI** p;

			Storage() : i(nullptr){}
		};

		DatumType mType;
		Storage mStorageArray;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
	};
}