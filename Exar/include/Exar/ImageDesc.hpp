#ifndef EXAR_IMAGE_DESC_HPP
#define EXAR_IMAGE_DESC_HPP

#include "Exar/Types.hpp"
#include "Exar/ExarEnum.hpp"

namespace Exar
{
	struct ImageDesc
	{
		u32 width;
		u32 height;
		ExarAlignMemory align = ExarAlignMemory::EXAR_ALIGN_32;
		ExarAllocLocation allocLocation = ExarAllocLocation::EXAR_ALLOC_HEAP;
	};
}

#endif // !EXAR_IMAGE_DESC_HPP
