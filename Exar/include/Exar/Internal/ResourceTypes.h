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
		QueueFamily queueFamily;
		size_t halfSize;
		size_t fullSize;
	};

	struct CommandBuffer_
	{
		CommandBufferState state;
		CommandBufferUsageFlags flags;
		QueueFamily pQueueFamily;
		uint32_t count;
		size_t capacity;
		size_t cmdOffset;
		size_t cmdBaseOffset;
	};

	typedef union {
		ClearColorValue color;
#ifdef _DEBUG
		uint32_t test;
#endif // !_DEBUG
		// ... other cmd values
	} CmdValue;

	struct alignas(16) Cmd_
	{
		CommandType type;
		CmdValue value;
	};
	static_assert(sizeof(Cmd_) % 16 == 0);
	static_assert(alignof(Cmd_) == 16);
}

#endif // !EXAR_RESOURCE_TYPES_HPP
