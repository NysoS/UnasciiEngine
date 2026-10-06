#include "Exar/Test/CommandBufferAndSwapchainTest.hpp"
#include "Exar/Device.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/API.hpp"
#include "Exar/Fence.hpp"
#include "Exar/Internal/InternalApi.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Internal/Swapchain.hpp"
#include "Exar/ImageView.hpp"

Exar::CommandBufferAndSwapchainTest::CommandBufferAndSwapchainTest()
	: mDevice(std::make_unique<Device_>())
	, mCommandPools({})
	, mSwapchain(EXAR_NULL_HANDLE)
{
	std::vector<AreaMemoryCreateInfo> lAreaMemoryCreateInfos;

	// command pool memory info
	AreaMemoryCreateInfo lCommandPoolAreaMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::COMMAND_POOL,
		.pageCount = 2,
		.size = 250 * 1024
	};

	std::vector<PageMemoryCreateInfo> lCommandPoolPageInfos;
	lCommandPoolPageInfos.reserve(FIFO_SWAPCHAIN_IMAGES);
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		lCommandPoolPageInfos.push_back({ .size = 100 * 1024 });
	}
	lCommandPoolAreaMemoryInfo.pageMemory = lCommandPoolPageInfos.data();
	lAreaMemoryCreateInfos.push_back(lCommandPoolAreaMemoryInfo);

	// swapchain memory info
	AreaMemoryCreateInfo lSwapchainAreaMemoryInfo{
		.mode = MemoryModeFlagBits::READ,
		.areaType = AreaMemoryType::SWAPCHAIN,
		.pageCount = 1,
		.size = 200
	};
	std::vector<PageMemoryCreateInfo> lSwapchainPageInfos{};
	lSwapchainPageInfos.push_back({.size = 200});
	lSwapchainAreaMemoryInfo.pageMemory = lSwapchainPageInfos.data();
	lAreaMemoryCreateInfos.push_back(lSwapchainAreaMemoryInfo);

	// swapchain image memory info
	AreaMemoryCreateInfo lImageSwapchainAreaMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::IMAGE,
		.pageCount = FIFO_SWAPCHAIN_IMAGES,
		.size = 1024 * 1024 * 1024
	};

	std::vector<PageMemoryCreateInfo> lImageSwapchainPageInfo{};
	lImageSwapchainPageInfo.reserve(FIFO_SWAPCHAIN_IMAGES);
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		lImageSwapchainPageInfo.push_back({ .size = 512 * 1024 * 1024 });
	}
	lImageSwapchainAreaMemoryInfo.pageMemory = lImageSwapchainPageInfo.data();
	lAreaMemoryCreateInfos.push_back(lImageSwapchainAreaMemoryInfo);

	// device memory info
	DeviceMemoryCreateInfo lDeviceMemoryInfo{
		.memoryType = MemoryType::AREA,
		.areaCount = (u32)lAreaMemoryCreateInfos.size(),
		.areaMemory = lAreaMemoryCreateInfos.data()
	};

	bool lMemoryCreated = mDevice->createDeviceMemory(lDeviceMemoryInfo);
	if (!lMemoryCreated)
	{
		EXAR_MEMORY_LOG(stderr, "Memory can't create, maybe no space remaining\n");
		return;
	}

	mCommandPools.reserve(FIFO_SWAPCHAIN_IMAGES);
	mCmdPoolAllocatorInfos.reserve(FIFO_SWAPCHAIN_IMAGES);
}

Exar::CommandBufferAndSwapchainTest::~CommandBufferAndSwapchainTest()
{}
