#include "Exar/Test/CommandBufferAndSwapchainTest.hpp"
#include "Exar/Device.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/API.hpp"
#include "Exar/Internal/Queue.hpp"
#include "Exar/Internal/InternalApi.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Internal/Swapchain.hpp"
#include "Exar/Fence.hpp"
#include "Exar/ImageView.hpp"
#include "Exar/Semaphore.hpp"

Exar::CommandBufferAndSwapchainTest::CommandBufferAndSwapchainTest()
	: mDevice(std::make_unique<Device_>())
	, mCommandPools({})
	, mSwapchain(EXAR_NULL_HANDLE)
	, mGraphicQueue(EXAR_NULL_HANDLE)
	, mPresentQueue(EXAR_NULL_HANDLE)
{
#ifdef WIN32
	AllocConsole();
#endif

	if (!initMemory())
	{
		return;
	}

	if (!init()) return;

	size_t lMaxIteration = 500;
	size_t lIteration = 0;

	while (lIteration < lMaxIteration)
	{
		Result lResult;
		// check fences free, else skip 
		if (lResult = mDevice->waitForFence(mInFlightFences[mMainFrameIndex], ExarBool::B_TRUE, 0); lResult == Result::FENCE_PROCESSING)
		{
			++lIteration;
			continue;
		}

		// Peek the index of the next swapchain image (not acquired yet).
		u32 lNextImageIndex = 0;
		if (lResult = mDevice->acquireNextImage(mSwapchain, mImageAvailableSemaphores[mMainFrameIndex], &lNextImageIndex); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Acquire next image failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error try to acquire next image from swacphain\n");
		}
		
		// If a previous frame is still using this image, wait for its fence.
		// Nothing has been acquired at this point, so skipping the iteration is safe.
		// Assumes only this thread acquires images (peeked index == acquired index).
		if (mImagesInFlight[lNextImageIndex] != EXAR_NULL_HANDLE) {
			if (lResult = mDevice->waitForFence(mImagesInFlight[lNextImageIndex], ExarBool::B_TRUE, 0); lResult == Result::FENCE_PROCESSING) {
				++lIteration;
				continue;
			}
		}
		mImagesInFlight[lNextImageIndex] = mInFlightFences[mMainFrameIndex];

		// reset command buffer
		if (lResult = resetCommandBuffer(mCommandBuffers[mMainFrameIndex]); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Reset command buffer failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to reset commandBuffer\n");
		}

		CommandBufferBeginInfo lCommandBufferBeginInfo{
			.flags = 0
		};

		if (lResult = beginCommandBuffer(mCommandBuffers[mMainFrameIndex], lCommandBufferBeginInfo); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Begin commandBuffer failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to begin commandBuffer\n");
		}

		cmdTest(mCommandBuffers[mMainFrameIndex], 6 + mMainFrameIndex);
		cmdClearColorImage(mCommandBuffers[mMainFrameIndex], mImages[mMainFrameIndex], ClearColorValue{ .uint32{255,0,0,255 } });

		if (lResult = endCommandBuffer(mCommandBuffers[mMainFrameIndex]); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "End commandBuffer failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to end commandBuffer\n");
		}

		// declare submitInfo
		SubmitInfo lSubmitInfo{
			.commandBufferCount = 1,
			.pCommandBuffers = &mCommandBuffers[mMainFrameIndex],
		};

		// declare waitSemaphore
		Semaphore lWaitSemaphore[] = { mImageAvailableSemaphores[mMainFrameIndex] };
		lSubmitInfo.waitSemaphoreCount = 1;
		lSubmitInfo.pWaitSemaphores = lWaitSemaphore;

		// declare signalSemaphore
		Semaphore lSignalSemaphore[] = { mRenderFinishedSemaphores[mMainFrameIndex] };
		lSubmitInfo.signalSemaphoreCount = 1;
		lSubmitInfo.pSignalSemaphores = lSignalSemaphore;

		// reset fence
		if (lResult = mDevice->resetFences(&mInFlightFences[mMainFrameIndex], 1); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Reset fence failed: Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to reset fence\n");
		}

		//QueueSubmit
		if (lResult = queueSubmit(mGraphicQueue, 1, lSubmitInfo, mInFlightFences[mMainFrameIndex]); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Queue submit failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to queue submit\n");
		}

		// make present info
		Swapchain lSwapchain[] = { mSwapchain };
		PresentInfo lPresentInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = lSignalSemaphore,
			.swapchainCount = 1,
			.pSwapchains = lSwapchain,
			.pImageIndices = &lNextImageIndex
		};

		// queuePresent


		++lIteration;
		mMainFrameIndex = (mMainFrameIndex + 1) % FIFO_SWAPCHAIN_IMAGES;
	}
}

Exar::CommandBufferAndSwapchainTest::~CommandBufferAndSwapchainTest()
{
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		mDevice->destroySemaphore(&mImageAvailableSemaphores[i], mImageAvailableSemaphoreAllocatorInfo[i]);
		mDevice->destroySemaphore(&mRenderFinishedSemaphores[i], mRenderFinishedSemaphoreAllocatorInfo[i]);
		mDevice->destroyFence(&mImagesInFlight[i], mInFlightFencesAllocatorInfo[i]);
	
		mDevice->destroyCommandPool(&mCommandPools[i], mCmdPoolAllocatorInfos[i]);

		mDevice->destroyImageView(&mImagesViews[i], mImageViewAllocatorInfos[i]);
	}
	mDevice->destroySwapchain(&mSwapchain, mImageAllocatorInfos);
 }

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

	// queue memory info
	AreaMemoryCreateInfo lQueueMemoryInfo{
		.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE,
		.areaType = AreaMemoryType::QUEUE,
		.pageCount = 2,
		.size = 1024 * 1024
	};
	std::vector<PageMemoryCreateInfo> lQueueMemoryPageInfo{};
	for (size_t i = 0; i < lQueueMemoryInfo.pageCount; ++i)
	{
		lQueueMemoryPageInfo.push_back({ .size = 512 * 1024 });
	}
	lQueueMemoryInfo.pageMemory = lQueueMemoryPageInfo.data();
	lAreaMemoryCreateInfos.push_back(lQueueMemoryInfo);

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
			EXAR_MEMORY_LOG(stderr, "Creation command pool failed : Error code : %u\n", (u32)lResult);
			return false;
		}
	}
	// command buffer creation
	for (size_t i = 0; i < FIFO_SWAPCHAIN_IMAGES; ++i)
	{
		CommandBufferAllocateInfo lCommandBufferAllocateInfo{
			.commandPool = mCommandPools[i],
			.commandBufferCount = 1
		};

		if (Result lResult = mDevice->allocateCommandBuffer(lCommandBufferAllocateInfo, &mCommandBuffers[i]); lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Allocation command buffer failed : Error code : %u\n", (u32)lResult);
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
		EXAR_MEMORY_LOG(stderr, "Swapchain creation failed : Error code : %u\n", (u32)lResult);
		return false;
	}
	mImageAllocatorInfos = lImageAllocatorInfo;

	mImages.resize(FIFO_SWAPCHAIN_IMAGES);

	lResult = mDevice->getSwapchainImages(mSwapchain, FIFO_SWAPCHAIN_IMAGES, mImages.data());
	if (lResult != Result::SUCCESS)
	{
		EXAR_MEMORY_LOG(stderr, "Get swpachain images failed : Error code : %u\n", (u32)lResult);
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
			EXAR_MEMORY_LOG(stderr, "ImageView creation failed : Error code : %u\n", (u32)lResult);
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
			EXAR_MEMORY_LOG(stderr, "Fence creation failed : Error code : %u\n", (u32)lResult);
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
			EXAR_MEMORY_LOG(stderr, "Semaphores creation failed : Error code : %u\n", (u32)lResult);
			return false;
		}

		mImageAvailableSemaphoreAllocatorInfo[i] = lImageAvailableSemaphoreAllocatorCreateInfo;
		mRenderFinishedSemaphoreAllocatorInfo[i] = lRenderFinishedSemaphoreAllocatorCreateInfo;
	}

	// queue creation
	std::vector<QueueFamily> lFamilies = { QueueFamily::GRAPHICS, QueueFamily::PRESENT};
	mQueueAllocatorInfo.reserve(lFamilies.size());
	for (size_t i = 0; i < lFamilies.size(); ++i)
	{
		const auto& lFamily = lFamilies[i];
		AllocatorCreateInfo lMemoryInfo{
			.totalSize = 512 * 1024,
			.align = AlignMemory::ALIGN_64,
			.areaType = AreaMemoryType::QUEUE,
			.pageIndex = (u32)i,
			.mode = AllocationMode::ALLOC_HEAP
		};

		mQueueAllocatorInfo.push_back(lMemoryInfo);
	}

	QueueCreateInfo lQraphicQueueCreationInfo {
		.family = lFamilies[0],
		.proc = std::bind_front(&CommandBufferAndSwapchainTest::workerThreadProcess, this)
	};
	if (Result lResult = mDevice->createQueue(&mGraphicQueue, lQraphicQueueCreationInfo, mQueueAllocatorInfo[0]); lResult != Result::SUCCESS)
	{ 
		EXAR_MEMORY_LOG(stderr, "Qraphic queue creation failed : Error code : %u\n", (u32)lResult);
		return false;
	}

	QueueCreateInfo lPresentQueueCreateInfo{
		.family = lFamilies[1],
		.proc = std::bind_front(&CommandBufferAndSwapchainTest::displayThreadProcess, this)
	};
	if (Result lResult = mDevice->createQueue(&mPresentQueue, lPresentQueueCreateInfo, mQueueAllocatorInfo[1]); lResult != Result::SUCCESS)
	{
		EXAR_MEMORY_LOG(stderr, "Present queue creation failed : Error code : %u\n", (u32)lResult);
		return false;
	}
	
	return true;
}

void Exar::CommandBufferAndSwapchainTest::workerThreadProcess(std::stop_token pSt)
{
	while (!pSt.stop_requested())
	{
		if (pSt.stop_requested()) return;

		assert(mGraphicQueue != EXAR_NULL_HANDLE);
		SubmitHeader* lSumbitHeader = reinterpret_cast<SubmitHeader*>(mGraphicQueue + 1);

		EXAR_HANDLE* lQueueData = reinterpret_cast<EXAR_HANDLE*>(lSumbitHeader + 1);

		Fence lFence = EXAR_NULL_HANDLE;
		std::span<const CommandBuffer> lCommandBuffers(reinterpret_cast<const CommandBuffer*>(lQueueData), lSumbitHeader->commandBufferCount); lQueueData += lSumbitHeader->commandBufferCount;
		std::span<const Semaphore> lWaitSemaphores(reinterpret_cast<const Semaphore*>(lQueueData), lSumbitHeader->waitSemaphoreCount); lQueueData += lSumbitHeader->waitSemaphoreCount;
		std::span<const Semaphore> lSignalSemaphores(reinterpret_cast<const Semaphore*>(lQueueData), lSumbitHeader->signalSemaphoreCount); lQueueData += lSumbitHeader->signalSemaphoreCount;

		if (lSumbitHeader->hasFences) lFence = reinterpret_cast<Fence>(*lQueueData);

		// acquire imageAvailableSemaphore
		for (const auto& lWaitSemaphore : lWaitSemaphores)
		{
			lWaitSemaphore->acquire();
		}

		if (!lFence) continue;

		CommandPool lCommandPool = nullptr;

		FindRessourceInfo lFindInfo{
			.ressourceType = RessourceType::COMMAND_POOL,
			.searchPageFlag = SearchPageFlag::SINGLE_RESSOURCE_PAGE,
			.areaMemoryType = AreaMemoryType::COMMAND_POOL,
			.family = QueueFamily::GRAPHICS,
			.pageCount = 1,
			.pageStartIndex = mWorkerFrameIndex
		};

		Result lResult = mDevice->findCommandPool(&lCommandPool, lFindInfo);
		if (lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Find commandPool failed : Error code : %u\n", (u32)lResult);
			throw std::runtime_error("Error to find commandPool\n");
		}

		FindRessourceInfo lSearchCommandBufferInfo{
			.ressourceType = RessourceType::COMMAND_BUFFER,
			.searchPageFlag = SearchPageFlag::SINGLE_RESSOURCE_PAGE,
			.areaMemoryType = AreaMemoryType::COMMAND_POOL,
			.family = QueueFamily::GRAPHICS,
			.pageCount = 1,
			.pageStartIndex = mWorkerFrameIndex
		};

		for (CommandBuffer lCommandBuffer : lCommandBuffers)
		{
			std::vector<Cmd> lCmds(lCommandBuffer->count);

			FindRessourceInfo lSearchCmdInfo{
				.ressourceType = RessourceType::CMD
			};

			if (lResult = Internal::findCommands(lCmds.data(), lCommandBuffer, lSearchCmdInfo); lResult != Result::SUCCESS)
			{
				EXAR_MEMORY_LOG(stderr, "find commands failed : Error code : %u\n", (u32)lResult);
				throw std::runtime_error("Error to find commands\n");
			}

			for (Cmd lCmd : lCmds)
			{
				if (lCmd->type == CommandType::CLEAR_COLOR_IMAGE)
				{
					// copy color value into swapchain image
					EXAR_MEMORY_LOG(stdout, "Copy color values into swapchain images : Color %u %u %u\n", lCmd->value.color.uint32[0], lCmd->value.color.uint32[1], lCmd->value.color.uint32[2]);
				}
			}
		}

		lFence->signal();
		
		// signal renderFinished - signal semaphore
		for (const auto& lSignalSemaphore : lSignalSemaphores)
		{
			lSignalSemaphore->release();
		}


		mWorkerFrameIndex = (mWorkerFrameIndex + 1) % 2;
	}
}

void Exar::CommandBufferAndSwapchainTest::displayThreadProcess(std::stop_token pSt)
{
	while (!pSt.stop_requested())
	{
		if (pSt.stop_requested()) return;

		assert(mPresentQueue != EXAR_NULL_HANDLE);
		PresentHeader* lPresentHeader = reinterpret_cast<PresentHeader*>(mPresentQueue + 1);

		EXAR_HANDLE* lQueueData = reinterpret_cast<EXAR_HANDLE*>(lPresentHeader + 1);

		std::span<const Semaphore> lWaitSemaphores(reinterpret_cast<const Semaphore*>(lQueueData), lPresentHeader->waitSemaphoreCount); lQueueData += lPresentHeader->waitSemaphoreCount;
		std::span<Swapchain> lSwapchains(reinterpret_cast<Swapchain*>(lQueueData), lPresentHeader->swapchainCount); lQueueData += lPresentHeader->swapchainCount;
		std::span<const u32> lImageIndices(reinterpret_cast<const u32*>(lQueueData), lPresentHeader->swapchainCount);

		// acquire renderFinished
		for (const auto& lWaitSemaphore : lWaitSemaphores)
			lWaitSemaphore->acquire();

		// signal imageAvailable
		for (const auto& lSwapchain : lSwapchains)
		{
			// ... draw pixel

			lSwapchain->swap();
			auto lSemaphore = lSwapchain->getSemaphore();
			if (lSemaphore) lSemaphore->release();
		}
	}

}
