#include "Exar/API.hpp"
#include "Exar/Device.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Fence.hpp"

#include <assert.h>

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
		return pDevice->destroyCommandPool(&pCommandPool, pAllocatorInfo);
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

		EXAR_MEMORY_LOG(stdout, "BeginCommandBuffer, state -> recording\n");

		return Result::SUCCESS;
	}
	
	Result endCommandBuffer(CommandBuffer pCmdBuffer) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->state = CommandBufferState::EXECUTABLE;

		EXAR_MEMORY_LOG(stdout, "EndCommandBuffer, state -> executable\n");

		return Result::SUCCESS;
	}

	EXA_API Result resetCommandBuffer(CommandBuffer pCmdBuffer) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->count = 0;
		pCmdBuffer->cmdOffset = pCmdBuffer->cmdBaseOffset;
		pCmdBuffer->state = CommandBufferState::INITIAL;

		EXAR_MEMORY_LOG(stdout, "---- [Reset Command Buffer] ----\n");
		EXAR_MEMORY_LOG(stdout, "CommandBuffer count %zu\n", pCmdBuffer->count);
		EXAR_MEMORY_LOG(stdout, "CommandBuffer cmdOffset %zu\n", pCmdBuffer->cmdOffset);
		EXAR_MEMORY_LOG(stdout, "CommandBuffer state -> initial\n");
		EXAR_MEMORY_LOG(stdout, "--------------------------------\n");

		return Result::SUCCESS;
	}

	EXA_API Result resetCommandPool(CommandPool pCommandPool) noexcept
	{
		if (!pCommandPool) return Result::ERROR_MEMORY_NULL_HANDLE;

		EXAR_MEMORY_LOG(stdout, "---- [Reset Command Pool] ----\n");
		
		u8* lCommandPoolBaseOffset = reinterpret_cast<u8*>(pCommandPool);

		for (const size_t lOffset : pCommandPool->offsets) {
			CommandBuffer_* lBuffer = reinterpret_cast<CommandBuffer_*>(lCommandPoolBaseOffset + lOffset);
			EXAR_MEMORY_LOG(stdout, "CommandPool offset : %zu\n", lOffset);
			resetCommandBuffer(lBuffer);
		}
		EXAR_MEMORY_LOG(stdout, "------------------------------\n");
		return Result::SUCCESS;
	}

	EXA_API Result queueSubmit(QueueFamily pFamily, u32 pCount, const SubmitInfo& pInfo, Fence pFence) noexcept
	{
		if (!pFence) return Result::ERROR_INVALID_FENCE;
		if (pCount > pInfo.commandBufferCount) Result::ERROR_INVALID_ARG;

		for (size_t i = 0; i < pCount; ++i) {
			CommandBuffer lBuffer = pInfo.pCommandBuffers[i];
			if (!lBuffer) return Result::NULL_POINTER;
			if (lBuffer->pQueueFamily != pFamily) return Result::ERROR_INVALID_FAMILIES;
			if (lBuffer->state != CommandBufferState::INITIAL) Result::ERROR_COMMAND_BUFFER_ALREADY_SUBMIT;

			lBuffer->state = CommandBufferState::PENDING;
			pFence->notifyOne();
		}

		return Result::SUCCESS;
	}

	EXA_API Result waitForFences(Device pDevice, u32 pFenceCount, Fence pFence, ExarBool pWaitAll, u64 pTimeout) noexcept
	{
		return pDevice->waitForFence(pFence, pWaitAll, pTimeout);
	}

	EXA_API Result cmdClearColorImage(CommandBuffer pCmdBuffer, Image pImage, const ClearColorValue& pColor)
	{
		/*u8* lBase = reinterpret_cast<u8*>(pCmdBuffer);

		Cmd_* lCmd = reinterpret_cast<Cmd_*>(lBase + pCmdBuffer->cmdOffset);
		lCmd->type = CommandType::NONE;
		lCmd->value.color = pColor;

		pCmdBuffer->count++;
		pCmdBuffer->cmdOffset += sizeof(Cmd_);*/

		return Internal::assignCommandBufferCmd(pCmdBuffer, CmdValue{.color = pColor});
	}

#ifdef _DEBUG
	void cmdTest(CommandBuffer pCmdBuffer, uint32_t pValue) noexcept {
		assert(pCmdBuffer);
		assert(pCmdBuffer->state == CommandBufferState::RECORDING);
		assert(pCmdBuffer->count < pCmdBuffer->capacity);

		u8* lBase = reinterpret_cast<u8*>(pCmdBuffer);

		Cmd_* lCmd = reinterpret_cast<Cmd_*>(lBase + pCmdBuffer->cmdOffset);
		lCmd->type = CommandType::TEST;
		lCmd->value.test = pValue;
		
		pCmdBuffer->count++;
		pCmdBuffer->cmdOffset += sizeof(Cmd_);

		EXAR_MEMORY_LOG(stdout, "---- [Save Cmd Test] ----\n");
		EXAR_MEMORY_LOG(stdout, "Command buffer startCmdOffset : %zu\n", pCmdBuffer->cmdBaseOffset);
		EXAR_MEMORY_LOG(stdout, "Command buffer cmdOffset : %zu\n", pCmdBuffer->cmdOffset);
		EXAR_MEMORY_LOG(stdout, "Command buffer cmd count : %d\n", pCmdBuffer->count);
		EXAR_MEMORY_LOG(stdout, "Command buffer cmd capacity : %d\n", pCmdBuffer->capacity);
		EXAR_MEMORY_LOG(stdout, "Cmd type : %d\n", lCmd->type);
		EXAR_MEMORY_LOG(stdout, "Cmd value : %d\n", lCmd->value.test);
		EXAR_MEMORY_LOG(stdout, "-------------------------\n");
	}
#endif // _DEBUG
}