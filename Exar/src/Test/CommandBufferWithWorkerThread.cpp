#include "Exar/Test/CommandBufferWithWorkerThread.hpp"
#include "Exar/Device.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/API.hpp"
#include "Exar/Fence.hpp"
#include "Exar/Internal/InternalApi.hpp"
#include "Exar/Internal/ResourceTypes.h"

Exar::CommandBufferWithWorkerThread::CommandBufferWithWorkerThread()
	: mDevice(std::make_unique<Device_>())
	, mCommandPools({})
	, mFences({})
{
	std::vector<AreaMemoryCreateInfo> lAreaCreateInfos;
	AreaMemoryCreateInfo lAreaMemoryInfo{};
	lAreaMemoryInfo.areaType = AreaMemoryType::COMMAND_POOL;
	lAreaMemoryInfo.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE;
	lAreaMemoryInfo.size = 250 * 1024;
	lAreaMemoryInfo.pageCount = 2;

	std::vector<PageMemoryCreateInfo> lPagesMemory;
	for (size_t i = 0; i < 2; ++i) {
		PageMemoryCreateInfo lPageInfo{};
		lPageInfo.size = 100 * 1024;
		lPagesMemory.push_back(lPageInfo);
	}

	lAreaMemoryInfo.pageMemory = lPagesMemory.data();
	lAreaCreateInfos.push_back(lAreaMemoryInfo);

	AreaMemoryCreateInfo lAreaFenceMemoryInfo{};
	lAreaFenceMemoryInfo.areaType = AreaMemoryType::FENCE;
	lAreaFenceMemoryInfo.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE;
	lAreaFenceMemoryInfo.size = 1024 * 1024;
	lAreaFenceMemoryInfo.pageCount = 2;

	std::vector<PageMemoryCreateInfo> lFencePagesMemory;
	for (size_t i = 0; i < 2; ++i) {
		PageMemoryCreateInfo lPageInfo{};
		lPageInfo.size = 100 * 1024;
		lFencePagesMemory.push_back(lPageInfo);
	}

	lAreaFenceMemoryInfo.pageMemory = lFencePagesMemory.data();
	lAreaCreateInfos.push_back(lAreaFenceMemoryInfo);

	DeviceMemoryCreateInfo lDeviceMemoryInfo{};
	lDeviceMemoryInfo.memoryType = MemoryType::AREA;
	lDeviceMemoryInfo.areaCount = lAreaCreateInfos.size();
	lDeviceMemoryInfo.areaMemory = lAreaCreateInfos.data();

	bool lMemoryCreated = mDevice->createDeviceMemory(lDeviceMemoryInfo);
	if (!lMemoryCreated)
	{
		EXAR_MEMORY_LOG(stderr, "Memory can't created, maybe no space remaining\n");
		return;
	}

	mCommandPools.resize(2);
	mCmdPoolAllocatorInfos.resize(2);
	mFences.resize(2);
	mFenceAllocatorInfos.resize(2);

	EXAR_MEMORY_LOG(stdout, "---- [Command Pool Creation] ----\n");
	for (size_t i = 0; i < 2; ++i) {
		CommandPoolCreateInfo lCmdPoolInfo{};
		lCmdPoolInfo.family = QueueFamily::GRAPHICS;
		lCmdPoolInfo.flags = CommandPoolFlags::RESET_COMMAND_BUUFER_BIT;
		lCmdPoolInfo.type = AreaMemoryType::COMMAND_POOL;

		AllocatorCreateInfo lCmdPoolAllocatorInfo{};
		lCmdPoolAllocatorInfo.align = AlignMemory::ALIGN_16;
		lCmdPoolAllocatorInfo.areaType = AreaMemoryType::COMMAND_POOL;
		lCmdPoolAllocatorInfo.mode = AllocationMode::ALLOC_SCRATCH;
		lCmdPoolAllocatorInfo.pageIndex = i;
		lCmdPoolAllocatorInfo.totalSize = 100 * 1024;
		mCmdPoolAllocatorInfos[i] = lCmdPoolAllocatorInfo;

		EXAR_MEMORY_LOG(stdout, "[Command Pool %d]\n", i);
		EXAR_MEMORY_LOG(stdout, "Command Pool family type %u\n", (u32)lCmdPoolInfo.family);
		EXAR_MEMORY_LOG(stdout, "Command Pool reset flags %u\n", (u32)lCmdPoolInfo.flags);
		EXAR_MEMORY_LOG(stdout, "Area type %u\n", (u32)lCmdPoolInfo.type);

		Result lResult = mDevice->createCommandPool(&mCommandPools[i], lCmdPoolInfo, lCmdPoolAllocatorInfo);
		if (lResult != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Error when create command pool %u`\n", (u32)lResult);
			return;
		}
	}
	EXAR_MEMORY_LOG(stdout, "---------------------------------\n");

	EXAR_MEMORY_LOG(stdout, "---- [Fences Creation] ----\n");
	for (size_t i = 0; i < 2; ++i) {
		FenceCreateInfo lFenceCreateInfo{};
		lFenceCreateInfo.flags = FenceCreateFlags::SIGNALED_BIT;

		AllocatorCreateInfo lFenceAllocatorCreateInfo{};
		lFenceAllocatorCreateInfo.align = AlignMemory::ALIGN_64;
		lFenceAllocatorCreateInfo.areaType = AreaMemoryType::FENCE;
		lFenceAllocatorCreateInfo.mode = AllocationMode::ALLOC_HEAP;
		lFenceAllocatorCreateInfo.pageIndex = i;
		lFenceAllocatorCreateInfo.totalSize = 100 * 1024;

		mFenceAllocatorInfos[i] = lFenceAllocatorCreateInfo;

		if (Result lResult = mDevice->createFence(&mFences[i], lFenceCreateInfo, lFenceAllocatorCreateInfo); lResult != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "Error when create fence %u`\n", (u32)lResult);
			return;
		}
	}
	EXAR_MEMORY_LOG(stdout, "---------------------------\n");

	mCommandBuffers.resize(mCommandPools.size());
	EXAR_MEMORY_LOG(stdout, "---- [Command Buffer Creation] ----\n");
	for (size_t i = 0; i < mCommandPools.size(); ++i) {
		CommandBufferAllocateInfo lCommandBufferAllocateInfo{};
		lCommandBufferAllocateInfo.commandPool = mCommandPools[i];
		lCommandBufferAllocateInfo.commandBufferCount = 1;

		EXAR_MEMORY_LOG(stdout, "[Command Buffer %u]\n", i);
		EXAR_MEMORY_LOG(stdout, "Command Pool address : %zu\n", (uintptr_t)mCommandPools[i]);
		EXAR_MEMORY_LOG(stdout, "Command Buffer allocation count : %u\n", lCommandBufferAllocateInfo.commandBufferCount);

		if (Result lResultCmdBuffer = mDevice->allocateCommandBuffer(lCommandBufferAllocateInfo, &mCommandBuffers[i]); lResultCmdBuffer != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "Command buffer allocation failed %u\n", (u32)lResultCmdBuffer);
			throw std::runtime_error("Error when command buffer allocation\n");
		}
	}
	EXAR_MEMORY_LOG(stdout, "-----------------------------------\n");

	size_t lMaxIteration = 250;
	size_t lIteration = 0;

	EXAR_MEMORY_LOG(stdout, "---- [Worker Thread Creation] ----\n");
	mWorkerThread = std::jthread(std::bind_front(&CommandBufferWithWorkerThread::WorkerSystemProcess, this), std::ref(mFences));

	while (lIteration < lMaxIteration) {
		EXAR_MEMORY_LOG(stdout, "---- [Main Thread - Logic updated] ----\n");
		if (Result lResult = mDevice->waitForFence(mFences[mCurrentFrame], ExarBool::B_TRUE, 0); lResult == Result::FENCE_PROCESSING) {
			++lIteration;
			mCurrentFrame = (mCurrentFrame + 1) % 2;
			continue;
		}
		EXAR_MEMORY_LOG(stdout, "---- [Main Thread - Itteration %u] ----\n", lIteration);

		// get newImageIndex
		u32 lImageIndex = (mCurrentFrame + 1) % 2;


		EXAR_MEMORY_LOG(stdout, "---- [Command Buffer Cmd] ----\n");
		// reset command buffer
		Result lResetResult = resetCommandBuffer(mCommandBuffers[mCurrentFrame]);
		if (lResetResult != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "Reset command buffer failed %u\n", (u32)lResetResult);
			throw std::runtime_error("Error to resetCommandBuffer\n");
		}

		CommandBufferBeginInfo lCmdBufferBeginInfo{};
		lCmdBufferBeginInfo.flags = 0;

		Result lResultCmdBeginBuffer = beginCommandBuffer(mCommandBuffers[mCurrentFrame], lCmdBufferBeginInfo);
		if (lResultCmdBeginBuffer != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "Begin command buffer failed %u\n", (u32)lResultCmdBeginBuffer);
			throw std::runtime_error("Error to beginCommandBuffer\n");
		}

		cmdTest(mCommandBuffers[mCurrentFrame], 6 + mCurrentFrame);
		cmdTest(mCommandBuffers[mCurrentFrame], 8 + mCurrentFrame);

		if (Result lResultEndCommandBuffer = endCommandBuffer(mCommandBuffers[mCurrentFrame]); lResultEndCommandBuffer != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "End command buffer failed %u\n", (u32)lResultEndCommandBuffer);
			throw std::runtime_error("Error to endCommandBuffer\n");
		}
		EXAR_MEMORY_LOG(stdout, "------------------------------\n");

		SubmitInfo lSubmitInfo{};
		lSubmitInfo.commandBufferCount = 1;
		lSubmitInfo.pCommandBuffers = &mCommandBuffers[lImageIndex];

		// reset fence
		if (Result lResultFenceReset = mDevice->resetFences(&mFences[mCurrentFrame], 1); lResultFenceReset != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Reset fence failed %u\n", (u32)lResultFenceReset);
			throw std::runtime_error("Reset fence failed\n");
		}

		if (Result lResultQueueSubmit = queueSubmit(QueueFamily::GRAPHICS, 1, lSubmitInfo, mFences[mCurrentFrame]); lResultQueueSubmit != Result::SUCCESS)
		{
			EXAR_MEMORY_LOG(stderr, "Queue Submit failed %u\n", (u32)lResultQueueSubmit);
			throw std::runtime_error("Error to Queue Submit\n");
		}

		++lIteration;

		mCurrentFrame = (mCurrentFrame + 1) % 2;
	}
	mWorkerThread.request_stop();
	/*mWorkerThread.request_stop();
	for (const auto& lFence : mFences) {
		lFence->notifyOne();
	}*/
}

Exar::CommandBufferWithWorkerThread::~CommandBufferWithWorkerThread()
{
}

void Exar::CommandBufferWithWorkerThread::WorkerSystemProcess(std::stop_token pSt, const std::vector<Fence>& pFences)
{
	while (!pSt.stop_requested()) {
		//EXAR_MEMORY_LOG(stdout, "--- [Worker progress] ---\n", mWorkerFrameProcess);
		if (pSt.stop_requested()) return;

		if (pFences.size() == 0) return;

		Fence lFence = pFences[mWorkerFrameProcess];
		if (!lFence) continue;

		/*Result lResult = mDevice->waitForFence(lFence, ExarBool::B_TRUE, u64min);
		if (lResult != Result::SUCCESS) continue;
		*/
		
		EXAR_MEMORY_LOG(stdout, "Worker progress : frame worker %i \n", mWorkerFrameProcess);

		CommandPool lCommandPool = nullptr;

		FindRessourceInfo lInfo{};
		lInfo.ressourceType = RessourceType::COMMAND_POOL;
		lInfo.areaMemoryType = AreaMemoryType::COMMAND_POOL;
		lInfo.family = QueueFamily::GRAPHICS;
		lInfo.pageCount = 1;
		lInfo.pageStartIndex = mWorkerFrameProcess;
		lInfo.searchPageFlag = SearchPageFlag::SINGLE_RESSOURCE_PAGE;

		Result lResult = mDevice->findCommandPool(&lCommandPool, lInfo);
		if (lResult != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "FindCommandPool failed %u\n", (u32)lResult);
			throw std::runtime_error("Error when find command pool\n");
		}

		FindRessourceInfo lSearchCommandBufferInfo{};
		lSearchCommandBufferInfo.ressourceType = RessourceType::COMMAND_BUFFER;
		lSearchCommandBufferInfo.areaMemoryType = AreaMemoryType::COMMAND_POOL;
		lSearchCommandBufferInfo.family = QueueFamily::GRAPHICS;
		lSearchCommandBufferInfo.pageCount = 1;
		lSearchCommandBufferInfo.pageStartIndex = mWorkerFrameProcess;
		lSearchCommandBufferInfo.searchPageFlag = SearchPageFlag::SINGLE_RESSOURCE_PAGE;

		std::vector<CommandBuffer> lCommandBuffers;
		size_t lBufferCount = 0;
		lResult = Internal::getCommandBufferCount(&lBufferCount, lCommandPool);
		if (lResult != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "getCommandBufferCount failed %u\n", (u32)lResult);
			throw std::runtime_error("Error when fetch command buffer count\n");
		}
		
		lCommandBuffers.resize(lBufferCount);
		lResult = Internal::findCommandBuffer(lCommandBuffers.data(), lCommandPool, lSearchCommandBufferInfo);
		if (lResult != Result::SUCCESS) {
			EXAR_MEMORY_LOG(stderr, "FindCommandBuffer failed %u\n", (u32)lResult);
			throw std::runtime_error("Error when find command buffer\n");
		}

		for (CommandBuffer lCommandBuffer : lCommandBuffers) {
			std::vector<Cmd> lCmds(lCommandBuffer->count);

			FindRessourceInfo lCmdInfo{};
			lCmdInfo.ressourceType = RessourceType::CMD;

			Internal::findCommand(lCmds.data(), lCommandBuffer, lCmdInfo);

			for (size_t lIndex = 0; lIndex < lCmds.size(); ++lIndex) {
				EXAR_MEMORY_LOG(stdout, "---- [Worker Thread Cmd Read] : %i ----\n", lIndex);
				EXAR_MEMORY_LOG(stdout, "Command type %u\n", (u32)lCmds[lIndex]->type);
				EXAR_MEMORY_LOG(stdout, "Command value %u\n", lCmds[lIndex]->value);
				EXAR_MEMORY_LOG(stdout, "---------------------------------------\n");
			}
		}

		lFence->signal();
		
		mWorkerFrameProcess = (mWorkerFrameProcess + 1) % 2;
	}
}