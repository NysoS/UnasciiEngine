#ifndef EXAR_HPP
#define EXAR_HPP

#include "Engine/uaepch.h"

namespace UnasciiEngine::RHI::EXAR {
	enum class ExarUsage : uint32_t
	{
		EXAR_USAGE_DEFAULT = 0,
		EXAR_USAGE_IMMUTABLE = 1,
		EXAR_USAGE_DYNAMIC = 2
	};

	enum class ExarBindFlag : uint32_t
	{
		EXAR_BIND_VERTEX_BUFFER = 0x1L,
		EXAR_BIND_INDEX_BUFFER = 0x2L,
		EXAR_BIND_CONSTANT_BUFFER = 0x4L
	};

	enum class ExarAllocLocation : uint32_t
	{
		EXAR_ALLOC_HEAP = 0,
		EXAR_ALLOC_STACK = 1,
		EXAR_ALLOC_SIMD = 2
	};

	enum class ExarAlignMemory : uint32_t
	{
		EXAR_ALIGN_DEFAULT = 0,
		EXAR_ALIGN_SIMD = 1
	};
};

#endif // !EXAR_HPP
