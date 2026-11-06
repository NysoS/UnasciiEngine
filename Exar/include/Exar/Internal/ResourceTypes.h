#ifndef EXAR_RESOURCE_TYPES_HPP
#define EXAR_RESOURCE_TYPES_HPP

#include <vector>

namespace Exar
{
	struct Image_
	{
		size_t size;
	};

	class CommandPool_
	{
	public:
		std::vector<size_t> offsets;
		uint32_t resetMode;
		QueueFamily queueFamily;
		size_t halfSize;
		size_t fullSize;
	};

	class CommandBuffer_
	{
	public:
		CommandBufferState state;
		CommandBufferUsageFlags flags;
		QueueFamily pQueueFamily;
		uint32_t count;
		size_t capacity;
		size_t cmdOffset;
		size_t cmdBaseOffset;
	};

	class alignas(16) Cmd_
	{
	public:
		CommandType type;
		CmdValue value;
	};
	static_assert(sizeof(Cmd_) % 16 == 0);
	static_assert(alignof(Cmd_) == 16);

	struct SubmitHeader
	{
		u32 commandBufferCount;
		u32 waitSemaphoreCount;
		u32 signalSemaphoreCount;
		u32 hasFences;
	};

	struct PresentHeader
	{
		u32 waitSemaphoreCount;
		u32 swapchainCount;
	};
}

#endif // !EXAR_RESOURCE_TYPES_HPP
