#include "Exar/ExarDevice.hpp"
#include "Exar/IExarBuffer.hpp"
#include "Exar/Internal/ExarBuffer.hpp"
#include "Exar/Internal/ExarAllocator.hpp"

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

Exar::MemoryHandle Exar::ExarDevice::allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept
{
	if (!mAllocator) return nullptr;

	if (pRequirement.sizeInBytes <= 0) return nullptr;

	size_t lAlign = ((size_t)pDesc.align > 0) ? (size_t)pDesc.align : pRequirement.align;
	
	ExarMemoryRequirement lMemReq = pRequirement;
	lMemReq.align = lAlign;

	return mAllocator->alloc(pDesc, lMemReq);
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
