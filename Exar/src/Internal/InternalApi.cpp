#include "Exar/Internal/InternalApi.hpp"
#include "Exar/Device.hpp"
#include "Exar/Internal/ResourceTypes.h"

Exar::Result Exar::Internal::findCommandPool(Device pDevice, const FindRessourceInfo& pFinfo, CommandPool* pCommandPool)
{
	if (!pDevice) return Result::ERROR_MEMORY_NULL_HANDLE;

	return pDevice->findCommandPool(pCommandPool, pFinfo);
}

Exar::Result Exar::Internal::getCommandBufferCount(size_t* pCount, CommandPool pCommandPool)
{
	if (!pCommandPool) return Result::NULL_POINTER;

	*pCount = pCommandPool->offsets.size();

	return Result::SUCCESS;
}

Exar::Result Exar::Internal::findCommandBuffer(CommandBuffer* pCommandBuffer, CommandPool pCommandPool, const FindRessourceInfo& pSearchInfo)
{
	if (pSearchInfo.ressourceType != RessourceType::COMMAND_BUFFER);
	if (!pCommandBuffer || !pCommandPool) return Result::NULL_POINTER;

	u8* lCommandPoolBaseOffset = reinterpret_cast<u8*>(pCommandPool);

	for (size_t i = 0; i < pCommandPool->offsets.size(); ++i) {
		CommandBuffer lCommandBuffer = reinterpret_cast<CommandBuffer>(lCommandPoolBaseOffset + pCommandPool->offsets.at(i));

		pCommandBuffer[i] = lCommandBuffer;
	}

	return Result::SUCCESS;
}

Exar::Result Exar::Internal::findCommands(Cmd* pCmd, CommandBuffer pCommandBuffer, const FindRessourceInfo& pSearchInfo)
{
	if (!pCmd || !pCommandBuffer) return Result::NULL_POINTER;
	if (pSearchInfo.ressourceType != RessourceType::CMD) return Result::ERROR_RESSOURCE_TYPE;

	u8* lCommandBufferBaseOffset = reinterpret_cast<u8*>(pCommandBuffer);

	for (size_t i = 0; i < pCommandBuffer->count; ++i) {
		size_t lOffset = pCommandBuffer->cmdBaseOffset + (i * sizeof(Cmd_));
		Cmd lCmd = reinterpret_cast<Cmd>(lCommandBufferBaseOffset + lOffset);

		pCmd[i] = lCmd;
	}

	return Result::SUCCESS;
}

Exar::Result Exar::Internal::assignCommandBufferCmd(CommandBuffer pCommandBuffer, CmdValue pValue)
{
	if (!pCommandBuffer) return Result::NULL_POINTER;

	u8* lBase = reinterpret_cast<u8*>(pCommandBuffer);

	Cmd_* lCmd = reinterpret_cast<Cmd_*>(lBase + pCommandBuffer->cmdOffset);
	lCmd->type = CommandType::NONE;
	lCmd->value = pValue;

	pCommandBuffer->count++;
	pCommandBuffer->cmdOffset += sizeof(Cmd_);
	return Result::SUCCESS;
}
