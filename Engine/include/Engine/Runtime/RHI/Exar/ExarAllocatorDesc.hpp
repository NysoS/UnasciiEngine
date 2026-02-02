#ifndef EXAR_ALLOCATOR_DESC_HPP
#define EXAR_ALLOCATOR_DESC_HPP

#include "Engine/uaepch.h"

namespace UnasciiEngine::RHI::EXAR {
	struct ExarAllocatorAreaDesc
	{
		size_t totalSize = 0;
		uint32_t maxAllocation = 0;
	};
}

#endif // !EXAR_ALLOCATOR_DESC_HPP
