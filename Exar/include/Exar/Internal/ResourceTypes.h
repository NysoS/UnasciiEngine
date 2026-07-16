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
		size_t halfSize;
		size_t fullSize;
	};

	struct CommandBuffer_
	{
		CommandBufferState state;
		CommandBufferUsageFlags flags;
		uint32_t count;
		size_t capacity;
		size_t cmdOffset;
	};

	struct alignas(16) Cmd_
	{
		CommandType type;
		union {
			uint32_t test;
			// ... other cmd values
		} value;
	};
	static_assert(sizeof(Cmd_) % 16 == 0);
	static_assert(alignof(Cmd_) == 16);
}

#endif // !EXAR_RESOURCE_TYPES_HPP
