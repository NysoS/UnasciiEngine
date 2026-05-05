#include "Exar/ExarDevice.hpp"
#include "Exar/IExarBuffer.hpp"
#include "Exar/Internal/ExarBuffer.hpp"
#include "Exar/Internal/ExarAllocator.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/Internal/Swapchain.hpp"

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
	ExarMemoryRequirement lMemRequirement{ 0, 0 };

	if (!mAllocator || !pBuffer) return lMemRequirement;

	size_t lSize = pBuffer->getDesc().size;
	size_t lAling = 16;

	size_t lRemainingMem = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMem) return lMemRequirement;

	lMemRequirement.align = lAling;
	lMemRequirement.sizeInBytes = lSize;

	return lMemRequirement;
}

Exar::ExarResult Exar::ExarDevice::allocateResourceMemory(void** pMemory, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept
{
	if (!pMemory) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;
	if (!mAllocator) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;

	if (pRequirement.sizeInBytes == 0) return ExarResult::EXAR_ERROR_MEMORY_MAP_FAILED;

	size_t lAlign = ((size_t)pDesc.align > 0) ? (size_t)pDesc.align : pRequirement.align;
	
	ExarMemoryRequirement lMemReq = pRequirement;
	lMemReq.align = lAlign;

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

Exar::ExarResult Exar::ExarDevice::getImageMemoryRequirements(const ImageDesc& pDesc, ExarMemoryRequirement* pRequirement) noexcept
{
	if (!pRequirement) return ExarResult::EXAR_ERROR_INVALID_ARG;
	if (!mAllocator) return ExarResult::EXAR_ERROR_ALLOCATOR_NULL_POINTER;

	if (pDesc.width == 0 || pDesc.height == 0) return ExarResult::EXAR_ERROR_INVALID_ARG;

	size_t lSize = pDesc.width * pDesc.height;
	size_t lAling = (size_t)pDesc.align;
		 
	size_t lRemainingMemory = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMemory) return ExarResult::EXAR_ERROR_MEMORY_OVER_FLOW;

	pRequirement->sizeInBytes = lSize;
	pRequirement->align = lAling;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createImage(Image* pImage, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement)
{
	if (!pImage) return ExarResult::EXAR_NULL_POINTER;

	void* lImagePtr = nullptr;
	ExarResult lResult = allocateResourceMemory(&lImagePtr, pDesc, pRequirement);
	if (lResult != ExarResult::EXAR_SUCCESS) return lResult;

	*pImage = (Image)lImagePtr;

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::createSwapchain(const SwapchainDesc& pDesc, ISwapchain** pSwapchain)
{
	if (!pSwapchain) return ExarResult::EXAR_NULL_POINTER;

	Swapchain* lSwapchain = new Swapchain();
	lSwapchain->mDesc = pDesc;
	lSwapchain->mDevice = this;

	*pSwapchain = lSwapchain;
	
	

	return ExarResult::EXAR_SUCCESS;
}

Exar::ExarResult Exar::ExarDevice::destroySwapchain(ISwapchain* pSwapchain)
{
	if (!pSwapchain) return ExarResult::EXAR_NULL_POINTER;

	delete pSwapchain;

	return ExarResult::EXAR_SUCCESS;
}
