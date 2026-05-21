#ifndef EXAR_IMAGE_DESC_HPP
#define EXAR_IMAGE_DESC_HPP

#include "Exar/Types.hpp"
#include "Exar/Enum.hpp"

namespace Exar
{
	struct ImageDesc
	{
		u32 width;
		u32 height;
		AlignMemory align = AlignMemory::ALIGN_32;
		AllocLocation allocLocation = AllocLocation::ALLOC_HEAP;
	};
}

#endif // !EXAR_IMAGE_DESC_HPP
