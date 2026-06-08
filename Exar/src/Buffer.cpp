#include "Exar/Internal/Buffer.hpp"
#include "Exar/BufferView.hpp"
#include "Exar/IDevice.hpp"

Exar::Buffer::Buffer()
	: mDevice(nullptr)
	, mData(nullptr)
{
}

Exar::Buffer::~Buffer()
{
}

Exar::BufferDesc Exar::Buffer::getDesc() const noexcept
{
	return mDesc;
}

Exar::Memory Exar::Buffer::getCPUAddress() const noexcept
{
	return mData;
}

Exar::BufferView Exar::Buffer::getView() const
{
	return Exar::BufferView(*this);
}
