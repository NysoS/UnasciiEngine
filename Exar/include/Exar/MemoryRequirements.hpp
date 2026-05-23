#ifndef EXAR_MEMORY_REQUIREMENTS_HPP
#define EXAR_MEMORY_REQUIREMENTS_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar
{
	struct MemoryRequirement
	{
		size_t sizeInBytes;
		AlignMemory align;
	};
}

#endif // !EXAR_MEMORY_REQUIREMENTS_HPP
