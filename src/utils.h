#pragma once

#include <cstddef>

namespace Utils
{
	template<typename T, size_t SIZE>
	static constexpr size_t ArraySize(const T (&)[SIZE])
	{
		return SIZE;
	}
}
