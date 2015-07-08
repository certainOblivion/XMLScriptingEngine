#pragma once
#include "XmlParseMaster.h"
#include "Stack.h"
#include "WorldXmlParseHelper.h"

namespace Library
{
	class Action;
	class World;
	class WorldSharedData final : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(WorldSharedData, Library::XmlParseMaster::SharedData)

			friend class WorldXmlParseHelper;
	public:

		/**Default constructor
		* @param parsedScope the parsed Scope at the end of parsing of the XML document
		*/
		explicit WorldSharedData(World& world);

		/**Destructor*/
		~WorldSharedData();

		/**Returns the address of a WorldSharedData object which has the same configuration state as “this”
		* except ready for a fresh file
		* @return A cloned ScopeSharedData
		*/
		WorldSharedData* Clone() const override;

		/**Set the shared data to its initial usable state.*/
		void Initialize() override;

		/**Returns the parsed scope at the end of parsing
		* @return a Scope that contains all the parsed data.
		*/
		World* GetParsedWorld();

	private:
		void AddAction(std::string name, std::string className);
		void AddActionList(std::string name);
		void AddEntity(std::string name, std::string className);
		void AddSector(std::string name);

		World* mWorld;
		Scope* mCurrentScope;
		bool bIsClone;
	};
}
