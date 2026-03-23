#ifndef EXAR_ALLOCATOR_DESC_HPP
#define EXAR_ALLOCATOR_DESC_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/Exar.hpp"

namespace UnasciiEngine::RHI::EXAR {
	struct ExarAllocatorDesc
	{
		size_t totalSize = 0;
		ExarAlignMemory align = ExarAlignMemory::EXAR_ALIGN_32;
		uint32_t maxAllocation = 0;
		ExarAllocLocation allocLocation = ExarAllocLocation::EXAR_ALLOC_HEAP;
	};
}

#endif // !EXAR_ALLOCATOR_DESC_HPP
