#ifndef EXAR_INTERNAL_API_HPP
#define EXAR_INTERNAL_API_HPP

#include "Exar/Core.hpp"

namespace Exar::Internal
{
	Result findCommandPool(Device pDevice, const FindRessourceInfo& pFinfo, CommandPool* pCommandPool);
	Result getCommandBufferCount(size_t* pCount, CommandPool pCommandPool);
	Result findCommandBuffer(CommandBuffer* pCommandBuffer, CommandPool pCommandPool, const FindRessourceInfo& pSearchInfo);
	Result findCommands(Cmd* pCmd, CommandBuffer pCommandBuffer, const FindRessourceInfo& pSearchInfo);

	Result assignCommandBufferCmd(CommandBuffer pCommandBuffer, CmdValue pValue);
}

#endif // !EXAR_INTERNAL_API_HPP
