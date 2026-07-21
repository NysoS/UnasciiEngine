#include "Exar/Test/CommandPoolAllocationTest.hpp"
#include "Exar/Device.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/API.hpp"

Exar::CommandPoolAllocationTest::CommandPoolAllocationTest()
	: mDevice(std::make_unique<Device_>())
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

	mCommandBuffers.resize(2);

	CommandBufferAllocateInfo lCommandBufferAllocateInfo{};
	lCommandBufferAllocateInfo.commandPool = mCommandPool;
	lCommandBufferAllocateInfo.commandBufferCount = (u32)mCommandBuffers.size();

	printf("-------------- Init Command buffers --------------\n");
	if (Result lResultCmdBuffer = mDevice->allocateCommandBuffer(lCommandBufferAllocateInfo, mCommandBuffers.data()); lResultCmdBuffer != Result::SUCCESS) {
		printf("CodeResult %d", lResultCmdBuffer);
		throw std::runtime_error("Error when command buffer allocation");
	}

	for (size_t i = 0; i < mCommandBuffers.size(); ++i) {
		CommandBufferBeginInfo lCmdBufferBeginInfo{};
		lCmdBufferBeginInfo.flags = 0;

		Result lResultCmdBeginBuffer = beginCommandBuffer(mCommandBuffers[i], lCmdBufferBeginInfo);
		if (lResultCmdBeginBuffer != Result::SUCCESS) {
			printf("CodeResult %d", lResultCmdBeginBuffer);
			throw std::runtime_error("Error to beginCommandBuffer");
		}

		cmdTest(mCommandBuffers[i], 6);
		cmdTest(mCommandBuffers[i], 8);

		if (Result lResultEndCommandBuffer = endCommandBuffer(mCommandBuffers[i]); lResultEndCommandBuffer != Result::SUCCESS) {
			printf("CodeResult %d", lResultEndCommandBuffer);
			throw std::runtime_error("Error to endCommandBuffer");
		}
	}

	printf("-------------- Reset Command buffer 0 --------------\n");
	Result lReset = resetCommandBuffer(mCommandBuffers[0]);
	if (lReset != Result::SUCCESS) {
		printf("CodeResult %d", lReset);
		throw std::runtime_error("Error to resetCommandBuffer 0");
	}

	printf("-------------- Re create Command buffer 0 --------------\n");
	CommandBufferBeginInfo lCmdBufferBeginInfo{};
	lCmdBufferBeginInfo.flags = 0;

	Result lResultCmdBeginBuffer = beginCommandBuffer(mCommandBuffers[0], lCmdBufferBeginInfo);
	if (lResultCmdBeginBuffer != Result::SUCCESS) {
		printf("CodeResult %d", lResultCmdBeginBuffer);
		throw std::runtime_error("Error to beginCommandBuffer");
	}

	cmdTest(mCommandBuffers[0], 1);
	cmdTest(mCommandBuffers[0], 21);

	if (Result lResultEndCommandBuffer = endCommandBuffer(mCommandBuffers[0]); lResultEndCommandBuffer != Result::SUCCESS) {
		printf("CodeResult %d", lResultEndCommandBuffer);
		throw std::runtime_error("Error to endCommandBuffer");
	}

	printf("-------------- Reset Command buffer 1 --------------\n");
	lReset = resetCommandBuffer(mCommandBuffers[1]);
	if (lReset != Result::SUCCESS) {
		printf("CodeResult %d", lReset);
		throw std::runtime_error("Error to resetCommandBuffer 1");
	}

	printf("-------------- Reset Command buffer 0 --------------\n");
	lReset = resetCommandBuffer(mCommandBuffers[0]);
	if (lReset != Result::SUCCESS) {
		printf("CodeResult %d", lReset);
		throw std::runtime_error("Error to resetCommandBuffer 0");
	}
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
