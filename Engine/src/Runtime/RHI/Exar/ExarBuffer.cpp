#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"
#include "Engine/Runtime/RHI/Exar/ExarBufferView.hpp"
#include "Engine/Runtime/RHI/Exar/IExarDevice.hpp"

UnasciiEngine::RHI::EXAR::ExarBuffer::ExarBuffer()
	: mDevice(nullptr)
	, mData(nullptr)
{
}

UnasciiEngine::RHI::EXAR::ExarBuffer::~ExarBuffer()
{
}

UnasciiEngine::RHI::EXAR::ExarBufferDesc UnasciiEngine::RHI::EXAR::ExarBuffer::getDesc() const noexcept
{
	return mDesc;
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarBuffer::getData() const noexcept
{
	return mData;
}

UnasciiEngine::RHI::EXAR::ExarBufferView UnasciiEngine::RHI::EXAR::ExarBuffer::getView() const
{
	return UnasciiEngine::RHI::EXAR::ExarBufferView(*this);
}
