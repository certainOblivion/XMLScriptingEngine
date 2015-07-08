#include "Attributed.h"
#include "glm.hpp"

namespace UnitTests
{
	/** class Foo to test SList with Class pointers*/
	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		AttributedFoo();
		~AttributedFoo();

		std::int32_t mExternalInteger;
		float mExternalFloat;
		glm::vec4 mExternalVector;
		glm::mat4 mExternalMatrix;
		std::string mExternalString;
	};
}
