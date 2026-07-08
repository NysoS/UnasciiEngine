#ifndef EXAR_RESOURCE_TYPES_HPP
#define EXAR_RESOURCE_TYPES_HPP

#include <vector>

namespace Exar
{
	struct Image_
	{
		void* data;
		size_t size;
	};

	struct CommandPool_
	{
		std::vector<size_t> offsets;
		uint32_t resetMode;
		uint32_t queueFamily;
		uint32_t commandBufferCount;
	};
}

#endif // !EXAR_RESOURCE_TYPES_HPP
