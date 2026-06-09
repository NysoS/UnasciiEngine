#include "Exar/Device.hpp"
#include "Exar/IBuffer.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/Internal/Buffer.hpp"
#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/Swapchain.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Internal/Framebuffer.hpp"
#include "Exar/ImageView.hpp"

#include <assert.h>

Exar::Device::Device()
	: mAllocator(nullptr)
{
}

Exar::Device::~Device()
{
}

bool Exar::Device::createMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo)
{
	/*mAllocator = std::make_unique<Allocator>(pMemorySize);
	if (mAllocator)
	{
		return true;
	}
	return false;*/
}

Exar::IBuffer* Exar::Device::createBuffer(const BufferDesc& pDesc)
{
	Buffer* lBuffer = new Buffer();
	lBuffer->mDesc = pDesc;
	lBuffer->mDevice = this;

	return lBuffer;
}

Exar::MemoryRequirement Exar::Device::getBufferMemoryRequirements(const IBuffer* pBuffer) noexcept
{
	MemoryRequirement lMemRequirement{ 0, AlignMemory::ALIGN_16 };

	if (!mAllocator || !pBuffer) return lMemRequirement;

	size_t lSize = pBuffer->getDesc().size;
	//size_t lAling = 16;

	size_t lRemainingMem = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMem) return lMemRequirement;

	lMemRequirement.align = lMemRequirement.align;
	lMemRequirement.sizeInBytes = lSize;

	return lMemRequirement;
}

Exar::Result Exar::Device::allocateResourceMemory(void** pMemory, const AllocatorCreateInfo& pInfo, const MemoryRequirement& pRequirement) noexcept
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

bool Exar::Device::updateResourceData(Memory& pMemHandle, const MemoryRequirement& pRequirement, std::span<const u8> pData) noexcept
{
	void* lPtr = static_cast<void*>(pMemHandle);
	if (!lPtr || pData.empty()) return false;

	assert(lPtr != nullptr);
	assert(pData.data() != nullptr);

	if (pData.size() > pRequirement.sizeInBytes) return false;

	std::memcpy(lPtr, pData.data(), pData.size());
	
	return true;
}

Exar::Result Exar::Device::getImageMemoryRequirements(MemoryRequirement* pRequirement, const ImageCreateInfo& pInfo) noexcept
{
	if (!pRequirement) return Result::ERROR_INVALID_ARG;
	if (!mAllocator) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	if (pInfo.width == 0 || pInfo.height == 0) return Result::ERROR_INVALID_ARG;

	size_t lAling = (size_t)pInfo.align;
	size_t lSize = (size_t)pInfo.width * (size_t)pInfo.height * 4;; // taille r�elle
		 
	size_t lRemainingMemory = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMemory) return Result::ERROR_OUT_OF_MEMORY;

	pRequirement->sizeInBytes = lSize;
	pRequirement->align = pInfo.align;

	return Result::SUCCESS;
}

Exar::Result Exar::Device::createImage(Image* pImage, const AllocatorCreateInfo& pInfo, const MemoryRequirement& pRequirement)
{
	if (!pImage) return Result::NULL_POINTER;

	if (pInfo.totalSize == 0 || pRequirement.sizeInBytes == 0) return Result::ERROR_INVALID_SIZE;

	void* lImagePtr = nullptr;
	Result lResult = allocateResourceMemory(&lImagePtr, pInfo, pRequirement);
	if (lResult != Result::SUCCESS) return lResult;

	Image_* lImage = new Image_();
	lImage->data = lImagePtr;
	lImage->size = pInfo.totalSize;

	*pImage = lImage;

	return Result::SUCCESS;
}

Exar::Result Exar::Device::destroyImage(Image pImage, const AllocatorCreateInfo& pInfo)
{
	// revome in memory area selected
	return Result::SUCCESS;
}

Exar::Result Exar::Device::createImageView(const ImageViewCreateInfo& pInfo, ImageView** pImageView)
{
	if (!pInfo.image) return Result::NULL_POINTER;

	std::unique_ptr<ImageView> lImageView = std::make_unique<ImageView>();
	lImageView->mInfo = pInfo;

	*pImageView = lImageView.release();

	return Result::SUCCESS;
}

Exar::Result Exar::Device::destroyImageView(ImageView* pImageView)
{
	if (!pImageView) return Result::NULL_POINTER;

	delete pImageView;

	return Result::SUCCESS;
}

Exar::Result Exar::Device::createSwapchain(const SwapchainCreateInfo& pInfo, ISwapchain** pSwapchain)
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

		MemoryRequirement lMemImageRequired;
		Result lImageMemReqResult = getImageMemoryRequirements(&lMemImageRequired, lImageInfo);
		if (lImageMemReqResult != Result::SUCCESS) return lImageMemReqResult;

		AllocatorCreateInfo lImageAlloc;
		lImageAlloc.align = (AlignMemory)lMemImageRequired.align;
		lImageAlloc.allocLocation = AllocLocation::ALLOC_HEAP;
		lImageAlloc.totalSize = lMemImageRequired.sizeInBytes;

		Image lImage = nullptr;
		Result lImageResult = createImage(&lImage, lImageAlloc, lMemImageRequired);
		if (lImageResult != Result::SUCCESS) return lImageResult;

		lSwapchain->mImages[i] = lImage;
	}

	*pSwapchain = lSwapchain.release();

	return Result::SUCCESS;
}

Exar::Result Exar::Device::destroySwapchain(ISwapchain* pSwapchain)
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

Exar::Result Exar::Device::createFramebuffer(const FramebufferCreateInfo& pInfo, Framebuffer** pFramebuffer)
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

Exar::Result Exar::Device::destroyFramebuffer(Framebuffer* pFramebuffer)
{
	if (!pFramebuffer) return Result::NULL_POINTER;

	delete pFramebuffer;

	return Result::SUCCESS;
}
