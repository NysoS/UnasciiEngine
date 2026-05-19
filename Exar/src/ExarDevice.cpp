#include "Exar/ExarDevice.hpp"
#include "Exar/IExarBuffer.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/FramebufferInfo.hpp"
#include "Exar/Internal/ExarBuffer.hpp"
#include "Exar/Internal/ExarAllocator.hpp"
#include "Exar/Internal/Swapchain.hpp"
#include "Exar/Internal/ResourceTypes.h"
#include "Exar/Internal/Framebuffer.hpp"

#include <assert.h>

Exar::ExarDevice::ExarDevice()
	: mAllocator(nullptr)
{
}

Exar::ExarDevice::~ExarDevice()
{
}

bool Exar::ExarDevice::createMemory(size_t pMemorySize)
{
	mAllocator = std::make_unique<ExarAllocator>(pMemorySize);
	if (mAllocator)
	{
		return true;
	}
	return false;
}

Exar::IExarBuffer* Exar::ExarDevice::createBuffer(const ExarBufferDesc& pDesc)
{
	ExarBuffer* lBuffer = new ExarBuffer();
	lBuffer->mDesc = pDesc;
	lBuffer->mDevice = this;

	return lBuffer;
}

Exar::ExarMemoryRequirement Exar::ExarDevice::getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept
{
	ExarMemoryRequirement lMemRequirement{ 0, ExarAlignMemory::EXAR_ALIGN_16 };

	if (!mAllocator || !pBuffer) return lMemRequirement;

	size_t lSize = pBuffer->getDesc().size;
	//size_t lAling = 16;

	size_t lRemainingMem = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMem) return lMemRequirement;

	lMemRequirement.align = lMemRequirement.align;
	lMemRequirement.sizeInBytes = lSize;

	return lMemRequirement;
}

Exar::ExarResult Exar::ExarDevice::allocateResourceMemory(void** pMemory, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept
{
	if (!pMemory) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;
	if (!mAllocator) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;

	if (pRequirement.sizeInBytes == 0) return ExarResult::EXAR_ERROR_MEMORY_MAP_FAILED;

	size_t lAlign = ((size_t)pDesc.align > 0) ? (size_t)pDesc.align : (size_t)pRequirement.align;

	ExarMemoryRequirement lMemReq = pRequirement;
	if ((size_t)pDesc.align > 0) {
		lMemReq.align = pDesc.align;
	}

	*pMemory = mAllocator->alloc(pDesc, lMemReq);
	if (!*pMemory) return ExarResult::EXAR_NULL_POINTER;

	return ExarResult::EXAR_SUCCESS;
}

bool Exar::ExarDevice::updateResourceData(MemoryHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept
{
	void* lPtr = static_cast<void*>(pMemHandle);
	if (!lPtr || pData.empty()) return false;

	assert(lPtr != nullptr);
	assert(pData.data() != nullptr);

	if (pData.size() > pRequirement.sizeInBytes) return false;

	std::memcpy(lPtr, pData.data(), pData.size());
	
	return true;
}

Exar::ExarResult Exar::ExarDevice::getImageMemoryRequirements(ExarMemoryRequirement* pRequirement, const ImageDesc& pDesc) noexcept
{
	if (!pRequirement) return ExarResult::EXAR_ERROR_INVALID_ARG;
	if (!mAllocator) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;

	if (pDesc.width == 0 || pDesc.height == 0) return ExarResult::EXAR_ERROR_INVALID_ARG;

	size_t lAling = (size_t)pDesc.align;
	size_t lSize = (size_t)pDesc.width * (size_t)pDesc.height * 4;; // taille réelle
		 
	size_t lRemainingMemory = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMemory) return ExarResult::EXAR_ERROR_OUT_OF_MEMORY;

	pRequirement->sizeInBytes = lSize;
	pRequirement->align = pDesc.align;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createImage(Image* pImage, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement)
{
	if (!pImage) return ExarResult::EXAR_NULL_POINTER;

	if (pDesc.totalSize == 0 || pRequirement.sizeInBytes == 0) return ExarResult::EXAR_ERROR_INVALID_SIZE;

	void* lImagePtr = nullptr;
	ExarResult lResult = allocateResourceMemory(&lImagePtr, pDesc, pRequirement);
	if (lResult != ExarResult::EXAR_SUCCESS) return lResult;

	Image_* lImage = new Image_();
	lImage->data = lImagePtr;
	lImage->size = pDesc.totalSize;

	*pImage = lImage;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::destroyImage(Image pImage, const ExarAllocatorDesc& pDesc)
{
	// revome in memory area selected
	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createImageView(const ImageViewDesc& pDesc, ImageView** pImageView)
{
	if (!pDesc.image) return ExarResult::EXAR_NULL_POINTER;

	std::unique_ptr<ImageView> lImageView = std::make_unique<ImageView>();
	lImageView->mDesc = pDesc;

	*pImageView = lImageView.release();

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::destroyImageView(ImageView* pImageView)
{
	if (!pImageView) return ExarResult::EXAR_NULL_POINTER;

	delete pImageView;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createSwapchain(const SwapchainDesc& pDesc, ISwapchain** pSwapchain)
{
	if (!pSwapchain) return ExarResult::EXAR_NULL_POINTER;

	std::unique_ptr<Swapchain> lSwapchain = std::make_unique<Swapchain>();
	lSwapchain->mDesc = pDesc;
	lSwapchain->mDevice = this;
	lSwapchain->initImages();

	for (size_t i = 0; i < pDesc.minImageCount; ++i)
	{
		// create images
		ImageDesc lImageDesc;
		lImageDesc.height = pDesc.extent.h;
		lImageDesc.width = pDesc.extent.w;

		ExarMemoryRequirement lMemImageRequired;
		ExarResult lImageMemReqResult = getImageMemoryRequirements(&lMemImageRequired, lImageDesc);
		if (lImageMemReqResult != ExarResult::EXAR_SUCCESS) return lImageMemReqResult;

		ExarAllocatorDesc lImageAlloc;
		lImageAlloc.align = (ExarAlignMemory)lMemImageRequired.align;
		lImageAlloc.allocLocation = ExarAllocLocation::EXAR_ALLOC_HEAP;
		lImageAlloc.totalSize = lMemImageRequired.sizeInBytes;

		Image lImage = nullptr;
		ExarResult lImageResult = createImage(&lImage, lImageAlloc, lMemImageRequired);
		if (lImageResult != ExarResult::EXAR_SUCCESS) return lImageResult;

		lSwapchain->mImages[i] = lImage;
	}

	*pSwapchain = lSwapchain.release();

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::destroySwapchain(ISwapchain* pSwapchain)
{
	if (!pSwapchain) return ExarResult::EXAR_NULL_POINTER;

	std::vector<Image> mImages(pSwapchain->getImageCount());

	ExarResult lImagesResult = pSwapchain->getImages(mImages.data());

	for (const auto& lImage : mImages)
	{
		//ExarResult lImageDestroyResult = 
	}

	delete pSwapchain;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createFramebuffer(const FramebufferInfo& pInfo, Framebuffer** pFramebuffer)
{
	if (pInfo.attachmentCount == 0 || pInfo.width == 0 || pInfo.height == 0) return ExarResult::EXAR_ERROR_INVALID_SIZE;
	if (!pInfo.attachments) return ExarResult::EXAR_NULL_POINTER;

	for (size_t i = 0; i < pInfo.attachmentCount; ++i)
	{
		if (!pInfo.attachments[i]) return ExarResult::EXAR_NULL_POINTER;
	}

	Framebuffer* lFramebuffer = new Framebuffer();
	lFramebuffer->mRenderPass = pInfo.renderPass;
	lFramebuffer->mAttachments= pInfo.attachments;
	lFramebuffer->mWidth = pInfo.width;
	lFramebuffer->mHeight = pInfo.height;
	lFramebuffer->mLayer = pInfo.layer;

	*pFramebuffer = lFramebuffer;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::destroyFramebuffer(Framebuffer* pFramebuffer)
{
	if (!pFramebuffer) return ExarResult::EXAR_NULL_POINTER;

	delete pFramebuffer;

	return ExarResult::EXAR_SUCCESS;
}
