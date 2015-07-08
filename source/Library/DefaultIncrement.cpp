#include "pch.h"

namespace Library
{
	uint32_t DefaultIncrement::operator()(uint32_t size, uint32_t capacity) const
	{
		return capacity / 2;
	}
}