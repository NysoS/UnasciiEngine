#include "Exar/Device.hpp"
#include "Exar/IBuffer.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/Internal/Buffer.hpp"
#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/Swapchain.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Internal/Framebuffer.hpp"
#include "Exar/Internal/Memory.hpp"
#include "Exar/ImageView.hpp"
#include "Exar/Fence.hpp"

#include <assert.h>

Exar::Device_::Device_()
	: mAllocator(nullptr)
{
}

Exar::Device_::~Device_()
{
}

bool Exar::Device_::createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo)
{
	mAllocator = std::make_unique<Allocator>();
	mAllocator->createDeviceMemory(pDeviceMemoryCreateInfo);
	if (mAllocator)
	{
		return true;
	}
	return false;
}

Exar::IBuffer* Exar::Device_::createBuffer(const BufferDesc& pDesc)
{
	/*Buffer* lBuffer = new Buffer();
	lBuffer->mDesc = pDesc;
	lBuffer->mDevice = this;*/

	return nullptr;
}

Exar::MemoryRequirement Exar::Device_::getBufferMemoryRequirements(const IBuffer* pBuffer) noexcept
{
	// todo : refacto buffer

	MemoryRequirement lMemRequirement{ 0, AlignMemory::ALIGN_16 };

	if (!mAllocator || !pBuffer) return lMemRequirement;

	size_t lSize = pBuffer->getDesc().size;
	//size_t lAling = 16;

	/*size_t lRemainingMem = mAllocator->getMemoryAreaSizeRemaining();
	if (lSize > lRemainingMem) return lMemRequirement;

	lMemRequirement.align = lMemRequirement.align;
	lMemRequirement.sizeInBytes = lSize;*/

	return lMemRequirement;
}

Exar::Result Exar::Device_::allocateResourceMemory(void** pMemory, const AllocatorCreateInfo& pInfo, const MemoryRequirement& pRequirement) noexcept
{
	if (!pMemory) return Result::ERROR_ALLOCATOR_NULL_POINTER;
	if (!mAllocator) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	if (pRequirement.sizeInBytes == 0) return Result::ERROR_MEMORY_MAP_FAILED;

	size_t lAlign = ((size_t)pInfo.align > 0) ? (size_t)pInfo.align : (size_t)pRequirement.align;

	MemoryRequirement lMemReq = pRequirement;
	if ((size_t)pInfo.align > 0) {
		lMemReq.align = pInfo.align;
	}

	*pMemory = mAllocator->alloc(pInfo, lMemReq);
	if (!*pMemory) return Result::NULL_POINTER;

	return Result::SUCCESS;
}

bool Exar::Device_::updateResourceData(Memory& pMemHandle, const MemoryRequirement& pRequirement, std::span<const u8> pData) noexcept
{
	void* lPtr = static_cast<void*>(pMemHandle);
	if (!lPtr || pData.empty()) return false;

	assert(lPtr != nullptr);
	assert(pData.data() != nullptr);

	if (pData.size() > pRequirement.sizeInBytes) return false;

	std::memcpy(lPtr, pData.data(), pData.size());
	
	return true;
}

Exar::Result Exar::Device_::getImageMemoryRequirements(MemoryRequirement* pRequirement, const AllocatorCreateInfo& pAllocatorInfo, const ImageCreateInfo& pInfo) noexcept
{
	if (!pRequirement) return Result::ERROR_INVALID_ARG;
	if (!mAllocator) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	if (pInfo.width == 0 || pInfo.height == 0) return Result::ERROR_INVALID_ARG;

	size_t lAling = (size_t)pInfo.align;
	if (lAling != (size_t)pAllocatorInfo.align) return Result::ERROR_INVALID_ALIGN_MEMORY;
	
	size_t lSize = (size_t)pInfo.width * (size_t)pInfo.height * 4; // real size
		 
	size_t lRemainingMemory = mAllocator->getMemoryAreaSizeRemaining(pAllocatorInfo.areaType);
	if (lSize > lRemainingMemory) return Result::ERROR_OUT_OF_MEMORY;

	pRequirement->sizeInBytes = lSize;
	pRequirement->align = pInfo.align;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createImage(Image* pImage, const AllocatorCreateInfo& pInfo, const MemoryRequirement& pRequirement)
{
	if (!pImage) return Result::NULL_POINTER;

	if (pRequirement.sizeInBytes == 0) return Result::ERROR_INVALID_SIZE;

	void* lImagePtr = nullptr;
	Result lResult = allocateResourceMemory(&lImagePtr, pInfo, pRequirement);
	if (lResult != Result::SUCCESS) return lResult;

	Image_* lImage = new Image_();
	lImage->data = lImagePtr;
	lImage->size = pInfo.totalSize;

	*pImage = lImage;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroyImage(Image pImage, const AllocatorCreateInfo& pInfo)
{
	if (!pImage) return Result::NULL_POINTER;
	
	Image_* lImg = static_cast<Image_*>(pImage);
	if (!lImg) return Result::NULL_POINTER;

	if (!lImg->data) return Result::ERROR_MEMORY_NULL_HANDLE;

	if (!mAllocator->dealloc(lImg->data, pInfo)) {
		return Result::ERROR_MEMORY_CLEANUP;
	}

	delete lImg;
	lImg = nullptr;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createCommandPool(CommandPool* pCommandPool, const CommandPoolCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo)
{
	if (pInfo.type != pAllocatorInfo.areaType) return Result::ERROR_INVALID_MEMORY_AREA;

	if (!pCommandPool) return Result::NULL_POINTER;
	if (pAllocatorInfo.totalSize == 0) return Result::ERROR_INVALID_SIZE;

	size_t lRemainingMemory = mAllocator->getMemoryAreaSizeRemaining(pAllocatorInfo.areaType);
	if (pAllocatorInfo.totalSize > lRemainingMemory) return Result::ERROR_OUT_OF_MEMORY;

	MemoryRequirement lRequired;
	lRequired.sizeInBytes = pAllocatorInfo.totalSize;
	lRequired.align = pAllocatorInfo.align;

	void* lPoolPtr = nullptr;
	Result lResult = allocateResourceMemory(&lPoolPtr, pAllocatorInfo, lRequired);
	if (lResult != Result::SUCCESS) return lResult;

	CommandPool_* lCmdPool = reinterpret_cast<CommandPool_*>(lPoolPtr);
	lCmdPool->resetMode = (u32)pInfo.flags;
	lCmdPool->queueFamily = pInfo.family;

	size_t lCmdPoolHeader = MemoryOperation::alignValue(sizeof(CommandPool_), AlignMemory::ALIGN_16);
	size_t lCmdPoolHalfSize = MemoryOperation::alignDown(((size_t)pAllocatorInfo.totalSize - lCmdPoolHeader) / 2, AlignMemory::ALIGN_16);
	lCmdPool->halfSize = lCmdPoolHalfSize;
	lCmdPool->fullSize = (size_t)pAllocatorInfo.totalSize;

	if (pAllocatorInfo.mode == AllocationMode::ALLOC_DOUBLE_SCRATCH) {
		lCmdPool->offsets = { lCmdPoolHeader, (lCmdPoolHeader + lCmdPoolHalfSize) };
	}
	else {
		lCmdPool->offsets = { lCmdPoolHeader };
	}

	*pCommandPool = lCmdPool;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroyCommandPool(CommandPool* pCommandPool, const AllocatorCreateInfo& pAllocatorInfo)
{
	if (!pCommandPool) return Result::ERROR_MEMORY_NULL_HANDLE;
	
	if (!mAllocator->dealloc(*pCommandPool, pAllocatorInfo)) {
		assert(false && "Command Pool dealloc failed");
		return Result::ERROR_MEMORY_CLEANUP;
	}

	*pCommandPool = nullptr;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::allocateCommandBuffer(const CommandBufferAllocateInfo& pInfo, CommandBuffer* pCommandBuffers)
{
	if (!pInfo.commandPool) return Result::ERROR_MEMORY_NULL_HANDLE;
	if (pInfo.commandBufferCount > pInfo.commandPool->offsets.size()) return Result::ERROR_INVALID_SIZE;
	
	size_t lCommandBufferAligned = MemoryOperation::alignValue(sizeof(CommandBuffer_), AlignMemory::ALIGN_16);
	size_t lCapacity = (pInfo.commandPool->halfSize - lCommandBufferAligned) / sizeof(Cmd_);
	 
	u8* lBase = reinterpret_cast<u8*>(pInfo.commandPool);

	for (size_t i = 0; i < pInfo.commandBufferCount; ++i)
	{
		CommandBuffer_* lCommandBuffer = reinterpret_cast<CommandBuffer_*>(lBase + pInfo.commandPool->offsets.at(i));
		
		lCommandBuffer->state = CommandBufferState::INITIAL;
		lCommandBuffer->count = 0;
		lCommandBuffer->capacity = lCapacity;
		lCommandBuffer->cmdOffset = lCommandBufferAligned;
		lCommandBuffer->cmdBaseOffset = lCommandBufferAligned;
		lCommandBuffer->pQueueFamily = pInfo.commandPool->queueFamily;

		pCommandBuffers[i] = lCommandBuffer;
	}

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createImageView(const ImageViewCreateInfo& pInfo, ImageView** pImageView)
{
	if (!pInfo.image) return Result::NULL_POINTER;

	std::unique_ptr<ImageView> lImageView = std::make_unique<ImageView>();
	lImageView->mInfo = pInfo;

	*pImageView = lImageView.release();

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroyImageView(ImageView* pImageView)
{
	if (!pImageView) return Result::NULL_POINTER;

	delete pImageView;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createSwapchain(const SwapchainCreateInfo& pInfo, ISwapchain** pSwapchain)
{
	if (!pSwapchain) return Result::NULL_POINTER;

	std::unique_ptr<Swapchain> lSwapchain = std::make_unique<Swapchain>();
	lSwapchain->mInfo = pInfo;
	lSwapchain->mDevice = this;
	lSwapchain->initImages();

	for (size_t i = 0; i < pInfo.minImageCount; ++i)
	{
		// create images
		ImageCreateInfo lImageInfo;
		lImageInfo.height = pInfo.extent.h;
		lImageInfo.width = pInfo.extent.w;

		AllocatorCreateInfo lImageAllocInfo;
		lImageAllocInfo.align = AlignMemory::ALIGN_32;
		lImageAllocInfo.areaType = AreaMemoryType::SWAPCHAIN;
		lImageAllocInfo.pageIndex = (u32)i;

		MemoryRequirement lMemImageRequired;
		Result lImageMemReqResult = getImageMemoryRequirements(&lMemImageRequired, lImageAllocInfo, lImageInfo);
		if (lImageMemReqResult != Result::SUCCESS) return lImageMemReqResult;

		Image lImage = nullptr;
		Result lImageResult = createImage(&lImage, lImageAllocInfo, lMemImageRequired);
		if (lImageResult != Result::SUCCESS) return lImageResult;

		lSwapchain->mImages[i] = lImage;
	}

	*pSwapchain = lSwapchain.release();

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroySwapchain(ISwapchain* pSwapchain)
{
	if (!pSwapchain) return Result::NULL_POINTER;

	std::vector<Image> mImages(pSwapchain->getImageCount());

	Result lImagesResult = pSwapchain->getImages(mImages.data());

	for (const auto& lImage : mImages)
	{
		//ExarResult lImageDestroyResult = 
	}

	delete pSwapchain;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createFramebuffer(const FramebufferCreateInfo& pInfo, Framebuffer** pFramebuffer)
{
	if (pInfo.attachmentCount == 0 || pInfo.width == 0 || pInfo.height == 0) return Result::ERROR_INVALID_SIZE;
	if (!pInfo.attachments) return Result::NULL_POINTER;

	for (size_t i = 0; i < pInfo.attachmentCount; ++i)
	{
		if (!pInfo.attachments[i]) return Result::NULL_POINTER;
	}

	Framebuffer* lFramebuffer = new Framebuffer();
	lFramebuffer->mRenderPass = pInfo.renderPass;
	lFramebuffer->mAttachments= pInfo.attachments;
	lFramebuffer->mWidth = pInfo.width;
	lFramebuffer->mHeight = pInfo.height;
	lFramebuffer->mLayer = pInfo.layer;

	*pFramebuffer = lFramebuffer;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroyFramebuffer(Framebuffer* pFramebuffer)
{
	if (!pFramebuffer) return Result::NULL_POINTER;

	delete pFramebuffer;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::createFence(Fence* pFence, const FenceCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo)
{
	// REFACTO SAME CODE //
	if (!pFence) return Result::NULL_POINTER;
	if (pAllocatorInfo.areaType != AreaMemoryType::FENCE) return Result::ERROR_INVALID_MEMORY_AREA;
	if (pAllocatorInfo.totalSize == 0) return Result::ERROR_INVALID_SIZE;

	size_t lRemainingMemory = mAllocator->getMemoryAreaSizeRemaining(pAllocatorInfo.areaType);
	if (pAllocatorInfo.totalSize > lRemainingMemory) return Result::ERROR_OUT_OF_MEMORY;

	MemoryRequirement lRequired;
	lRequired.sizeInBytes = pAllocatorInfo.totalSize;
	lRequired.align = pAllocatorInfo.align;

	void* lPoolPtr = nullptr;
	Result lResult = allocateResourceMemory(&lPoolPtr, pAllocatorInfo, lRequired);
	if (lResult != Result::SUCCESS) return lResult;
	//////////////////////

	Fence_* lFence = reinterpret_cast<Fence_*>(lPoolPtr);
	lFence->setCurrentState(pInfo.flags == FenceCreateFlags::SIGNALED_BIT ? FenceState::Signaled : FenceState::Waiting);

	*pFence = lFence;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::destroyFence(Fence* pFence, const AllocatorCreateInfo& pAllocatorInfo)
{
	if (!pFence) return Result::ERROR_MEMORY_NULL_HANDLE;

	if (!mAllocator->dealloc(*pFence, pAllocatorInfo)) {
		assert(false && "Command Pool dealloc failed");
		return Result::ERROR_MEMORY_CLEANUP;
	}

	*pFence = nullptr;

	return Result::SUCCESS;
}

// pWaitAll only more fence
Exar::Result Exar::Device_::waitForFence(Fence pFence, ExarBool pWaitAll, u64 pTimeout)
{
	if (!pFence) return Result::NULL_POINTER;

	FenceState lCurrentState = pFence->getCurrentState();
	
	if (pTimeout == u64max) {
		pFence->wait();
	}

	if (lCurrentState == FenceState::Waiting && pWaitAll == ExarBool::B_TRUE) return Result::FENCE_PROCESSING;

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::resetFences(Fence* pFences, u32 pCount)
{
	if (!pFences) return Result::NULL_POINTER;

	for (size_t i = 0; i < pCount; ++i)
	{
		Fence lFence = pFences[i];
		if (!lFence) return Result::ERROR_INVALID_FENCE;
		lFence->reset();
	}

	return Result::SUCCESS;
}

Exar::Result Exar::Device_::findCommandPool(CommandPool* pCommandPool, const FindRessourceInfo& pSearchInfo)
{
	void* lRessource = mAllocator->findRessource(pSearchInfo);
	if (!lRessource) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	CommandPool lCommandPool = reinterpret_cast<CommandPool>(lRessource);
	EXAR_MEMORY_LOG(stdout, "CommandPool family : %u", lCommandPool->queueFamily);

	*pCommandPool = lCommandPool;

	return Result::SUCCESS;
}
