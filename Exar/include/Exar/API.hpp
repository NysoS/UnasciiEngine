#ifndef EXAR_API_HPP
#define EXAR_API_HPP

#include "Exar/Core.hpp"

namespace Exar
{
	EXA_API Result createCommandPool(Device pDevice, const CommandPoolCreateInfo& pCreateInfo, const AllocatorCreateInfo& pAllocatorInfo, CommandPool* pCommandPool) noexcept;
	EXA_API Result destroyCommandPool(Device pDevice, CommandPool pCommandPool, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
	EXA_API Result allocateCommandBuffer(Device pDevice, const CommandBufferAllocateInfo& pInfo, CommandBuffer* pCommandBuffers) noexcept;

	EXA_API Result beginCommandBuffer(CommandBuffer pCmdBuffer, const CommandBufferBeginInfo& pBeginInfo) noexcept;
	EXA_API Result endCommandBuffer(CommandBuffer pCmdBuffer) noexcept;
	EXA_API Result resetCommandBuffer(CommandBuffer pCmdBuffer) noexcept;
	EXA_API Result resetCommandPool(CommandPool pCommandPool) noexcept;

	EXA_API Result waitForFences(Device pDevice, u32 pFenceCount, Fence pFence, ExarBool pWaitAll, u64 pTimeout) noexcept;

#ifdef _DEBUG
	EXA_API void cmdTest(CommandBuffer pCmdBuffer, uint32_t pValue) noexcept;
#endif
}

#endif // !EXAR_API_HPP

