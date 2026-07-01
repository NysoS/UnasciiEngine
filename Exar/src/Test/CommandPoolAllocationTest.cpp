#include "Exar/Test/CommandPoolAllocationTest.hpp"
#include "Exar/Device.hpp"
#include "Exar/Descriptor.hpp"

Exar::CommandPoolAllocationTest::CommandPoolAllocationTest()
	: mDevice(std::make_unique<Device>())
	, mCommandPool(nullptr)
{
	std::vector<AreaMemoryCreateInfo> lAreaCreateInfos;
	AreaMemoryCreateInfo lAreaMemoryInfo{};
	lAreaMemoryInfo.areaType = AreaMemoryType::COMMAND_POOL;
	lAreaMemoryInfo.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE;
	lAreaMemoryInfo.size = 100 * 1024;
	lAreaMemoryInfo.pageCount = 1;

	std::vector<PageMemoryCreateInfo> lPagesMemory;
	PageMemoryCreateInfo lPageInfo{};
	lPageInfo.size = 100 * 1024;
	lPagesMemory.push_back(lPageInfo);

	lAreaMemoryInfo.pageMemory = lPagesMemory.data();
	lAreaCreateInfos.push_back(lAreaMemoryInfo);

	DeviceMemoryCreateInfo lDeviceMemoryInfo{};
	lDeviceMemoryInfo.memoryType = MemoryType::AREA;
	lDeviceMemoryInfo.areaCount = 1;
	lDeviceMemoryInfo.areaMemory = lAreaCreateInfos.data();

	bool lMemoryCreated = mDevice->createDeviceMemory(lDeviceMemoryInfo);
	if (!lMemoryCreated)
	{
		std::cerr << "Memory can't created, maybe no space remaining" << std::endl;
		return;
	}

	CommandPoolCreateInfo lCmdPoolInfo{};
	lCmdPoolInfo.family = QueueFamily::GRAPHICS;
	lCmdPoolInfo.flags = CommandPoolFlags::RESET_COMMAND_BUUFER_BIT;
	lCmdPoolInfo.type = AreaMemoryType::COMMAND_POOL;

	AllocatorCreateInfo lCmdPoolAllocatorInfo{};
	lCmdPoolAllocatorInfo.align = AlignMemory::ALIGN_16;
	lCmdPoolAllocatorInfo.areaType = AreaMemoryType::COMMAND_POOL;
	lCmdPoolAllocatorInfo.mode = AllocationMode::ALLOC_DOUBLE_SCRATCH;
	lCmdPoolAllocatorInfo.pageIndex = 0;
	lCmdPoolAllocatorInfo.totalSize = 100 * 1024;
	mCmdPoolAllocatorInfo = lCmdPoolAllocatorInfo;

	Result lResult = mDevice->createCommandPool(&mCommandPool, lCmdPoolInfo, lCmdPoolAllocatorInfo);
	if (lResult != Result::SUCCESS)
	{
		std::cerr << "Error when create command pool " << (u32)lResult << std::endl;
		return;
	}

	// todo : make update data resouces

	// add buffer data ring O, page 1
	/*for (size_t i = 0; i < 10; ++i) {
	}*/
}

Exar::CommandPoolAllocationTest::~CommandPoolAllocationTest()
{
	if (!mDevice) return;

	Result lResult = mDevice->destroyCommandPool(mCommandPool, mCmdPoolAllocatorInfo);
	if (lResult != Result::SUCCESS) 
	{
		std::cerr << "Error when destroy command pool " << (u32)lResult << std::endl;
	}
}
