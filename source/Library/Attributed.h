#pragma once
#include "Scope.h"

namespace Library
{
	class Datum;

	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, RTTI)

	public:
		/**Default parameterless constructor*/
		Attributed();

		/**Destructor*/
		virtual ~Attributed() = default;

		/**Copy constructor.
		* @param rhs Attributed to be copied
		*/
		Attributed(const Attributed& rhs);

		/**Overloaded operator=. Copies the contents of the passed Attributed to this.
		* @param rhs Attributed to be copied
		*/
		Attributed& operator=(const Attributed& rhs);

		/** Given a string, returns a bool indicating whether it is the name of a prescribed attribute for this class
		* @param string attribute name
		* @return bool indicating whether it is a prescribed attribute
		*/
		bool IsPrescribedAttribute(const std::string& attributeName) const;

		/** Given a string, return a bool indicating whether it is the name of an auxiliary attribute for this object
		* @param string attribute name
		* @return bool indicating whether it is a auxillary attribute
		*/
		bool IsAuxiliaryAttribute(const std::string& attributeName) const;

		/**Given a string, return a bool indicating whether it is the name of an attribute for this object
		* @param string attribute name
		* @return bool indicating whether it is an attribute
		*/
		bool IsAttribute(const std::string& attributeName) const;

		/** Given a string, append a Datum to this Scope with the given name, and return a reference to that Datum
		* @param string attribute name
		* @return reference to a Datum
		*/
		Datum& AppendAuxiliaryAttribute(const std::string& attributeName);

		/**Return an unsigned integer which is the index of the first auxiliary attribute
		* @return index of the first auxiliary attribute
		*/
		std::uint32_t AuxiliaryBegin() const;

		/**Clears the scope*/
		virtual void Clear() override;

		const Vector<PairType*>& GetOrderVector() const;

#define INTERNAL_ATTRIBUTE(Name, Type, InitialValue, Size)			\
		{	Attributed::Signature::Storage d; \
		d.i = (int*)InitialValue; \
		Signature::Storage d2; \
		mSignatureVector.PushBack(Signature(Name, Type, d, Size, d2)); }

#define EXTERNAL_ATTRIBUTE(Name, Type, ExternalAddress, Size)		\
		{	Attributed::Signature::Storage d; \
		d.i = (int*)ExternalAddress; \
		Signature::Storage d2; \
		mSignatureVector.PushBack(Signature(Name, Type, d2, Size, d)); }

	protected:
		class Signature
		{
			friend class Attributed;
		public:
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

			std::string mName;
			Datum::DatumType mType;
			Storage mInitialValue;
			std::uint32_t mSize;
			Storage mAddress;

		public:
			Signature() = default;
			Signature(const std::string& name, const Datum::DatumType type, Storage& internalInitialValue, const std::uint32_t size, Storage& externalAddress);
			~Signature() = default;
		};

	protected:
		/**Populate the Scope with prescribed members*/
		void Populate();

		Vector<Signature> mSignatureVector;
	};
}