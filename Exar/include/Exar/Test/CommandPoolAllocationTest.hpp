#ifndef COMMAND_POOL_ALLOCATION_TEST_HPP
#define COMMAND_POOL_ALLOCATION_TEST_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/IDevice.hpp"

namespace Exar {
	class EXA_API CommandPoolAllocationTest 
	{
	public:
		CommandPoolAllocationTest();
		~CommandPoolAllocationTest();
	private:
		std::unique_ptr<IDevice> mDevice;
		CommandPool mCommandPool;
		AllocatorCreateInfo mCmdPoolAllocatorInfo;
	};
}

#endif // !COMMAND_POOL_ALLOCATION_TEST_HPP
