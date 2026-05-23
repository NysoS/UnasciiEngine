#ifndef EXAR_ALLOCATOR_DESC_HPP
#define EXAR_ALLOCATOR_DESC_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar {
	struct AllocatorDesc
	{
		size_t totalSize = 0;
		AlignMemory align = AlignMemory::ALIGN_32;
		uint32_t maxAllocation = 0;
		AllocLocation allocLocation = AllocLocation::ALLOC_HEAP;
	};
}

#endif // !EXAR_ALLOCATOR_DESC_HPP
