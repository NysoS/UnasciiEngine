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
#ifdef WIN32
	AllocConsole();
#endif

	if (!initMemory())
	{
		return;
	}

	if (!init()) return;
}

Exar::CommandBufferAndSwapchainTest::~CommandBufferAndSwapchainTest()
{}

bool Exar::CommandBufferAndSwapchainTest::initMemory()
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
	lSwapchainPageInfos.push_back({ .size = 200 });
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

	// imageView memory info
	AreaMemoryCreateInfo lImageViewAreaMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::IMAGEVIEW,
		.pageCount = FIFO_SWAPCHAIN_IMAGES,
		.size = 1024 * 1024 * 1024
	};

	std::vector<PageMemoryCreateInfo> lImageViewPagesInfo{};
	lImageViewPagesInfo.reserve(FIFO_SWAPCHAIN_IMAGES);
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		lImageViewPagesInfo.push_back({ .size = 512 * 1024 * 1024 });
	}
	lImageViewAreaMemoryInfo.pageMemory = lImageViewPagesInfo.data();
	lAreaMemoryCreateInfos.push_back(lImageViewAreaMemoryInfo);

	// fence memory info
	AreaMemoryCreateInfo lFenceAreaMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::FENCE,
		.pageCount = FIFO_SWAPCHAIN_IMAGES,
		.size = 1024 * 1024
	};

	std::vector<PageMemoryCreateInfo> lFencePagesInfo{};
	lFencePagesInfo.reserve(FIFO_SWAPCHAIN_IMAGES);
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		lFencePagesInfo.push_back({ .size = 100 * 1024 });
	}
	lFenceAreaMemoryInfo.pageMemory = lFencePagesInfo.data();
	lAreaMemoryCreateInfos.push_back(lFenceAreaMemoryInfo);

	// semaphore memory info
	AreaMemoryCreateInfo lSemaphoreMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::SEMAPHORE,
		.pageCount = FIFO_SWAPCHAIN_IMAGES*2,
		.size = 1024 * 1024
	};
	std::vector<PageMemoryCreateInfo> lSemaphorePagesInfo{};
	lSemaphorePagesInfo.reserve(FIFO_SWAPCHAIN_IMAGES * 2);
	for (size_t i = 0; i < lSemaphoreMemoryInfo.pageCount; ++i)
	{
		lSemaphorePagesInfo.push_back({ .size = 100 * 1024 });
	}
	lSemaphoreMemoryInfo.pageMemory = lSemaphorePagesInfo.data();
	lAreaMemoryCreateInfos.push_back(lSemaphoreMemoryInfo);

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
		return false;
	}

	return true;
}

bool Exar::CommandBufferAndSwapchainTest::init()
{
	mCommandPools.resize(FIFO_SWAPCHAIN_IMAGES, nullptr);
	mCmdPoolAllocatorInfos.resize(FIFO_SWAPCHAIN_IMAGES);

	// command pool creation
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		CommandPoolCreateInfo lCmdPoolInfo{
			.type = AreaMemoryType::COMMAND_POOL,
			.family = QueueFamily::GRAPHICS,
			.flags = CommandPoolFlags::RESET_COMMAND_BUFFER_PAGE
		};

		AllocatorCreateInfo lCmdPoolAllocatorInfo{
			.totalSize = 100 * 1024,
			.align = AlignMemory::ALIGN_16,
			.areaType = AreaMemoryType::COMMAND_POOL,
			.pageIndex = (u32)i,
			.mode = AllocationMode::ALLOC_SCRATCH
		};
		mCmdPoolAllocatorInfos[i] = lCmdPoolAllocatorInfo;

		if (Result lResult = mDevice->createCommandPool(&mCommandPools[i], lCmdPoolInfo, lCmdPoolAllocatorInfo); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Creation command pool failed : Error code : %u", (u32)lResult);
			return false;
		}
	}

	// swapchain creation 
	SwapchainCreateInfo lSwapchainCreateInfo{};
	lSwapchainCreateInfo.extent = Extent2D<u32>{ .w = 600, .h = 700 };
	lSwapchainCreateInfo.minImageCount = FIFO_SWAPCHAIN_IMAGES;
	lSwapchainCreateInfo.imageFormat = PixelFormat::B8G8R8A8_UNORM_SRGB;
	lSwapchainCreateInfo.imageColor = ColorSpace::COLOR_SPACE_SRGB_NONLINEAR;
	lSwapchainCreateInfo.imageUsage = ImageUsageFlags::COLOR_ATTACHMENT;
	lSwapchainCreateInfo.imageSharingMode = SharingMode::EXCLUSIVE;
	lSwapchainCreateInfo.presentMode = PresentationMode::FIFO;

#ifdef WIN32
	HWND lHwnd = GetConsoleWindow();
	lSwapchainCreateInfo.surface = reinterpret_cast<Surface>(lHwnd);
#endif // !WIN32

	AllocatorCreateInfo lImageAllocatorInfo{
		.totalSize = 512 * 1024 * 1024,
		.align = AlignMemory::ALIGN_32,
		.areaType = AreaMemoryType::IMAGE,
		.pageIndex = 0,
		.mode = AllocationMode::ALLOC_HEAP
	};

	Result lResult = mDevice->createSwapchain(&mSwapchain, lSwapchainCreateInfo, lImageAllocatorInfo);
	if (lResult != Result::SUCCESS)
	{
		EXAR_MEMORY_LOG(stderr, "Swapchain creation failed : Error code : %u", (u32)lResult);
		return false;
	}
	mImageAllocatorInfos = lImageAllocatorInfo;

	mImages.resize(FIFO_SWAPCHAIN_IMAGES);

	lResult = mDevice->getSwapchainImages(mSwapchain, FIFO_SWAPCHAIN_IMAGES, mImages.data());
	if (lResult != Result::SUCCESS)
	{
		EXAR_MEMORY_LOG(stderr, "Get swpachain images failed : Error code : %u", (u32)lResult);
		return false;
	}

	// image view creation
	mImagesViews.resize(FIFO_SWAPCHAIN_IMAGES);
	mImageViewAllocatorInfos.resize(FIFO_SWAPCHAIN_IMAGES);
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		ImageViewCreateInfo lImageViewCreateInfo{
			.image = mImages[i],
			.viewType = ImageViewType::VIEW_2D,
			.format = PixelFormat::B8G8R8A8_UNORM_SRGB,
			.subresource = ImageSubresource {
				.aspectMask = ImageAspectMask::COLOR_BIT
			},
		};

		AllocatorCreateInfo lImageViewAllocatorCreateInfo{
			.totalSize = 512 * 1024 * 1024,
			.align = AlignMemory::ALIGN_32,
			.areaType = AreaMemoryType::IMAGEVIEW,
			.pageIndex = (u32)i,
			.mode = AllocationMode::ALLOC_HEAP
		};

		Result lResult = mDevice->createImageView(&mImagesViews[i], lImageViewCreateInfo, lImageViewAllocatorCreateInfo);
		if (lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "ImageView creation failed : Error code : %u", (u32)lResult);
			return false;
		}
		mImageViewAllocatorInfos[i] = lImageViewAllocatorCreateInfo;
	}

	// fences creation
	mInFlightFences.resize(FIFO_SWAPCHAIN_IMAGES, nullptr);
	mImagesInFlight.reserve(FIFO_SWAPCHAIN_IMAGES);
	mInFlightFencesAllocatorInfo.resize(FIFO_SWAPCHAIN_IMAGES);
	
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		FenceCreateInfo lFenceCreateInfo{
			.flags = FenceCreateFlags::SIGNALED_BIT
		};

		AllocatorCreateInfo lFenceAllocatorInfo{
			.totalSize = 100 * 1024,
			.align = AlignMemory::ALIGN_64,
			.areaType = AreaMemoryType::FENCE,
			.pageIndex = (u32)i,
			.mode = AllocationMode::ALLOC_HEAP
		};

		Result lResult = mDevice->createFence(&mInFlightFences[i], lFenceCreateInfo, lFenceAllocatorInfo);
		if (lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Fence creation failed : Error code : %u", (u32)lResult);
			return false;
		}
		mInFlightFencesAllocatorInfo[i] = lFenceAllocatorInfo;
	}

	// semaphore creation
	mImageAvailableSemaphores.resize(FIFO_SWAPCHAIN_IMAGES, nullptr);
	mRenderFinishedSemaphores.resize(FIFO_SWAPCHAIN_IMAGES, nullptr);
	mImageAvailableSemaphoreAllocatorInfo.resize(FIFO_SWAPCHAIN_IMAGES);
	mRenderFinishedSemaphoreAllocatorInfo.resize(FIFO_SWAPCHAIN_IMAGES);

	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		AllocatorCreateInfo lImageAvailableSemaphoreAllocatorCreateInfo {
			.totalSize = 100 * 1024,
			.align = AlignMemory::ALIGN_64,
			.areaType = AreaMemoryType::SEMAPHORE,
			.pageIndex = (u32)i,
			.mode = AllocationMode::ALLOC_HEAP
		};

		AllocatorCreateInfo lRenderFinishedSemaphoreAllocatorCreateInfo = lImageAvailableSemaphoreAllocatorCreateInfo;
		lRenderFinishedSemaphoreAllocatorCreateInfo.pageIndex = FIFO_SWAPCHAIN_IMAGES + i;

		if (mDevice->createSemaphore(&mImageAvailableSemaphores[i], lImageAvailableSemaphoreAllocatorCreateInfo) != Result::SUCCESS ||
			mDevice->createSemaphore(&mRenderFinishedSemaphores[i], lRenderFinishedSemaphoreAllocatorCreateInfo) != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Semaphores creation failed : Error code : %u", (u32)lResult);
			return false;
		}

		mImageAvailableSemaphoreAllocatorInfo[i] = lImageAvailableSemaphoreAllocatorCreateInfo;
		mRenderFinishedSemaphoreAllocatorInfo[i] = lRenderFinishedSemaphoreAllocatorCreateInfo;
	}

	return true;
}
