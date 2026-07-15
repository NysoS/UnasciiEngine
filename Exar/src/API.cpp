#include "Exar/API.hpp"
#include "Exar/Device.hpp"

namespace Exar
{
	Result createCommandPool(Device pDevice, const CommandPoolCreateInfo& pCreateInfo, const AllocatorCreateInfo& pAllocatorInfo, CommandPool* pCommandPool)
	{
		return pDevice->createCommandPool(pCommandPool, pCreateInfo, pAllocatorInfo);
	}

	Result beginCommandBuffer(CommandBuffer pCmdBuffer, CommandBufferBeginInfo pBeginInfo)
	{
		return Result::SUCCESS;
	}
}