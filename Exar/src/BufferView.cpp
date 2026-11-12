#include "Exar/BufferView.hpp"
#include "Exar/Internal/Buffer.hpp"

Exar::BufferView::BufferView(const Buffer& pBuffer)
	: mData(reinterpret_cast<const u8*>(pBuffer.getCPUAddress()), pBuffer.getDesc().size)
	, mCursor(0)
{
}
