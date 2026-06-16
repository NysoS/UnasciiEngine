#ifndef EXAR_PAGE_MEMORY_HPP
#define EXAR_PAGE_MEMORY_HPP

namespace Exar
{
	struct PageMemory 
	{
		size_t totalSize;
		size_t size;
		size_t startOffset;
		size_t offset;
	};
}

#endif // !EXAR_PAGE_MEMORY_HPP
