#pragma once

#include <cstdint>

namespace Library
{
	class DefaultIncrement
	{
	public:
		uint32_t operator()(uint32_t size, uint32_t capacity) const;
	};
}