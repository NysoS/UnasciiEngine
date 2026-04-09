#ifndef EXAR_ENUM_HPP
#define EXAR_ENUM_HPP

#include "Exar/exarpch.h"

namespace Exar {
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
		EXAR_ALLOC_SCRATCH = 1, // Ring buffer
		EXAR_ALLOC_SIMD = 2
	};

	enum class ExarAlignMemory : size_t
	{
		EXAR_ALIGN_16 = 16,
		EXAR_ALIGN_32 = 32,
		EXAR_ALIGN_64 = 64
	};

	enum class ExarAllocatorType : uint32_t
	{
		EXAR_ALLOCATOR_AREA = 0,
		EXAR_ALLOCATOR_MANUALY = 1
	};
};

#endif // !EXAR_ENUM_HPP
