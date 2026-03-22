#include "Engine/Runtime/RHI/Exar/ExarDevice.hpp"
#include "Engine/Runtime/RHI/Exar/IExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"

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
