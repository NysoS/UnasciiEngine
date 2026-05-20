#include "Exar/Internal/ExarBuffer.hpp"
#include "Exar/ExarBufferView.hpp"
#include "Exar/IExarDevice.hpp"

Exar::ExarBuffer::ExarBuffer()
	: mDevice(nullptr)
	, mData(nullptr)
{
}

Exar::ExarBuffer::~ExarBuffer()
{
}

Exar::ExarBufferDesc Exar::ExarBuffer::getDesc() const noexcept
{
	return mDesc;
}

Exar::MemoryHandle Exar::ExarBuffer::getCPUAddress() const noexcept
{
	return mData;
}

Exar::ExarBufferView Exar::ExarBuffer::getView() const
{
	return Exar::ExarBufferView(*this);
}
