#ifndef EXAR_MEMORY_HPP
#define EXAR_MEMORY_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar::MemoryOperation {
	inline size_t alignValue(size_t pOffset, AlignMemory pAlign) noexcept {
		size_t lAlignValue = (size_t)pAlign;
		return (pOffset + (lAlignValue - 1)) & ~(lAlignValue - 1);
	}

	inline size_t alignDown(size_t pOffset, AlignMemory pAlign) noexcept {
		size_t lAlignValue = (size_t)pAlign;
		return pOffset & ~(lAlignValue - 1);
	}
}

#endif // !EXAR_MEMORY_HPP
