#ifndef EXAR_API_HPP
#define EXAR_API_HPP

#include "Exar/Core.hpp"

namespace Exar
{
	EXA_API Result createCommandPool(Device pDevice, const CommandPoolCreateInfo& pCreateInfo, const AllocatorCreateInfo& pAllocatorInfo, CommandPool* pCommandPool);
	EXA_API Result beginCommandBuffer(CommandBuffer pCmdBuffer, CommandBufferBeginInfo pBeginInfo);
}

#endif // !EXAR_API_HPP

