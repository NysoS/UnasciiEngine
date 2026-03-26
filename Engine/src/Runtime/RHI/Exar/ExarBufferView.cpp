#include "Engine/Runtime/RHI/Exar/ExarBufferView.hpp"
#include "Engine/Runtime/RHI/Exar/Internal/ExarBuffer.hpp"


UnasciiEngine::RHI::EXAR::ExarBufferView::ExarBufferView(const ExarBuffer& pBuffer)
	: mData(reinterpret_cast<const u8*>(pBuffer.getCPUAddress()), pBuffer.getDesc().size)
	, mCursor(0)
{
}
