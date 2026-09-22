#ifndef EXAR_COMMAND_POOL_ALLOCATION_TEST_HPP
#define EXAR_COMMAND_POOL_ALLOCATION_TEST_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"

class Device_;

namespace Exar {
	class EXA_API CommandPoolAllocationTest 
	{
	public:
		CommandPoolAllocationTest();
		~CommandPoolAllocationTest();
	private:
		std::unique_ptr<Device_> mDevice;
		std::vector<CommandPool> mCommandPools;
		std::vector<AllocatorCreateInfo> mCmdPoolAllocatorInfos;
		std::vector<CommandBuffer> mCommandBuffers;
	};
}

#endif // !EXAR_COMMAND_POOL_ALLOCATION_TEST_HPP
