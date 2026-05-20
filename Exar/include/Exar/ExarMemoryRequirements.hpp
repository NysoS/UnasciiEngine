#ifndef EXAR_MEMORY_REQUIREMENTS_HPP
#define EXAR_MEMORY_REQUIREMENTS_HPP

#include "Exar/ExarEnum.hpp"

namespace Exar
{
	struct ExarMemoryRequirement
	{
		size_t sizeInBytes;
		ExarAlignMemory align;
	};
}

#endif // !EXAR_MEMORY_REQUIREMENTS_HPP
