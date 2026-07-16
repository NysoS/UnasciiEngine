#include "Exar/API.hpp"
#include "Exar/Device.hpp"
#include "Exar/Internal/ResourceTypes.h"

namespace Exar
{
	Result createCommandPool(Device pDevice, const CommandPoolCreateInfo& pCreateInfo, const AllocatorCreateInfo& pAllocatorInfo, CommandPool* pCommandPool) noexcept
	{
		if (!pDevice) return Result::ERROR_MEMORY_NULL_HANDLE;
		return pDevice->createCommandPool(pCommandPool, pCreateInfo, pAllocatorInfo);
	}

	Result destroyCommandPool(Device pDevice, CommandPool pCommandPool, const AllocatorCreateInfo& pAllocatorInfo) noexcept
	{
		if (!pDevice) return Result::ERROR_MEMORY_NULL_HANDLE;
		return pDevice->destroyCommandPool(pCommandPool, pAllocatorInfo);
	}

	Result allocateCommandBuffer(Device pDevice, const CommandBufferAllocateInfo& pInfo, CommandBuffer* pCommandBuffers) noexcept
	{
		if (!pDevice) return Result::ERROR_MEMORY_NULL_HANDLE;
		return pDevice->allocateCommandBuffer(pInfo, pCommandBuffers);
	}

	Result beginCommandBuffer(CommandBuffer pCmdBuffer, const CommandBufferBeginInfo& pBeginInfo) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->state = CommandBufferState::RECORDING;
		pCmdBuffer->flags = pBeginInfo.flags;

		return Result::SUCCESS;
	}
	
	Result endCommandBuffer(CommandBuffer pCmdBuffer) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->state = CommandBufferState::EXECUTABLE;

		return Result::SUCCESS;
	}
}