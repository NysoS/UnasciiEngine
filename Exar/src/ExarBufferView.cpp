#include "Exar/ExarBufferView.hpp"
#include "Exar/Internal/ExarBuffer.hpp"

Exar::ExarBufferView::ExarBufferView(const ExarBuffer& pBuffer)
	: mData(reinterpret_cast<const u8*>(pBuffer.getCPUAddress()), pBuffer.getDesc().size)
	, mCursor(0)
{
}
