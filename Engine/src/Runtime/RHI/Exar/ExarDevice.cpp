#include "Engine/Runtime/RHI/Exar/ExarDevice.hpp"
#include "Engine/Runtime/RHI/Exar/IExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/MemoryArena.hpp"

UnasciiEngine::RHI::EXAR::ExarDevice::ExarDevice()
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

bool UnasciiEngine::RHI::EXAR::ExarDevice::GetBufferMemoryRequirements(const IExarBuffer* pBuffer, ExarMemoryRequirement*& pMemRequirement) noexcept
{
	if (!pBuffer) return false;

	size_t lSize = pBuffer->getDesc().size;
	size_t lAling = 32;

	if (!mMemoryArena.use_count() <= 0) return false;

	size_t lRemainingMem = mMemoryArena->getMemorySizeRemaining();
	if (lSize > lRemainingMem) return false;

	pMemRequirement->align = lAling;
	pMemRequirement->sizeInBytes = lSize;

	return true;
}