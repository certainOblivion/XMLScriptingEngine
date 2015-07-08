#pragma once
#include "XmlParseMaster.h"
#include "Stack.h"
#include "TableXmlParseHelper.h"
#include "Scope.h"
namespace Library
{
	class ScopeSharedData final : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(ScopeSharedData, Library::XmlParseMaster::SharedData)

		friend class TableXmlParseHelper;
	public:

		/**Default constructor
		* @param parsedScope the parsed Scope at the end of parsing of the XML document
		*/
		explicit ScopeSharedData(Scope& parsedScope);

		/**Destructor*/
		~ScopeSharedData();

		/**Returns the address of a ScopeSharedData object which has the same configuration state as “this”
		* except ready for a fresh file
		* @return A cloned ScopeSharedData
		*/
		ScopeSharedData* Clone() const override;

		/**Set the shared data to its initial usable state.*/
		void Initialize() override;

		/**Returns the parsed scope at the end of parsing
		* @return a Scope that contains all the parsed data.
		*/
		Scope* GetParsedScope();

	private:
		void AddNestedScope(std::string ScopeName);

		Scope* mParsedScope;
		Scope* mCurrentScope;
		bool bIsClone;
	};
}

