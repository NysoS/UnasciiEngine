#include "Exar/API.hpp"
#include "Exar/Device.hpp"
#include "Exar/Internal/ResourceTypes.h"

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

		assert(pCmdBuffer->state == CommandBufferState::INITIAL);

		pCmdBuffer->state = CommandBufferState::RECORDING;
		pCmdBuffer->flags = pBeginInfo.flags;

#ifdef _DEBUG
		printf("BeginCommandBuffer, state -> recording\n");
#endif // _DEBUG

		return Result::SUCCESS;
	}
	
	Result endCommandBuffer(CommandBuffer pCmdBuffer) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->state = CommandBufferState::EXECUTABLE;

#ifdef _DEBUG
		printf("EndCommandBuffer, state -> executable\n");
#endif // _DEBUG

		return Result::SUCCESS;
	}

	EXA_API Result resetCommandBuffer(CommandBuffer pCmdBuffer) noexcept
	{
		if (!pCmdBuffer) return Result::ERROR_MEMORY_NULL_HANDLE;

		pCmdBuffer->count = 0;
		pCmdBuffer->cmdOffset = pCmdBuffer->cmdBaseOffset;
		pCmdBuffer->state = CommandBufferState::INITIAL;

#ifdef _DEBUG
		printf("ResetCommandBuffer, cout -> %d\n", pCmdBuffer->count);
		printf("ResetCommandBuffer, cmdOffset -> %zu\n", pCmdBuffer->cmdOffset);
		printf("ResetCommandBuffer, state -> initial\n");
#endif // _DEBUG

		return Result::SUCCESS;
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

		printf("Save cmd [%s]\n", __FUNCDNAME__);
		printf("Cmd type : %d\n", lCmd->type);
		printf("Cmd value : %d\n", lCmd->value.test);

		printf("\n CmdBuffer startCmdOffset : %zu\n", pCmdBuffer->cmdBaseOffset);
		printf("\n CmdBuffer cmdOffset : %zu\n", pCmdBuffer->cmdOffset);
		printf("\n CmdBuffer cmd count : %d, cmd capacity : %zu\n", pCmdBuffer->count, pCmdBuffer->capacity);
	}
#endif // _DEBUG
}