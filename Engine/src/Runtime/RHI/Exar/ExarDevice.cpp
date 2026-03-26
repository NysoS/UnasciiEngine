#include "Engine/Runtime/RHI/Exar/ExarDevice.hpp"
#include "Engine/Runtime/RHI/Exar/IExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarAllocator.hpp"

UnasciiEngine::RHI::EXAR::ExarDevice::ExarDevice()
	: mAllocator(nullptr)
{
}

UnasciiEngine::RHI::EXAR::ExarDevice::~ExarDevice()
{
}

UnasciiEngine::RHI::EXAR::IExarBuffer* UnasciiEngine::RHI::EXAR::ExarDevice::createBuffer(const ExarBufferDesc& pDesc)
{
	ExarBuffer* lBuffer = new ExarBuffer();
	lBuffer->mDesc = pDesc;
	lBuffer->mDevice = this;

	return lBuffer;
}

UnasciiEngine::RHI::EXAR::ExarMemoryRequirement UnasciiEngine::RHI::EXAR::ExarDevice::getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept
{
	ExarMemoryRequirement lMemRequirement{ 0, 0 };

	if (!mAllocator || !pBuffer) return lMemRequirement;

	size_t lSize = pBuffer->getDesc().size;
	size_t lAling = 32;

	size_t lRemainingMem = mAllocator->getMemorySizeRemaining();
	if (lSize > lRemainingMem) return lMemRequirement;

	lMemRequirement.align = lAling;
	lMemRequirement.sizeInBytes = lSize;

	return lMemRequirement;
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarDevice::allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept
{
	if (!mAllocator) return nullptr;

	if (pRequirement.align <= 0 && pRequirement.sizeInBytes <= 0) return nullptr;

	return mAllocator->alloc(pDesc, pRequirement);
}
