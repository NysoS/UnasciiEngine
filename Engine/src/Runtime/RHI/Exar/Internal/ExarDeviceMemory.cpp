#include "Engine/Runtime/RHI/Exar/Internal/ExarDeviceMemory.hpp"

#include <stdexcept>

UnasciiEngine::RHI::EXAR::ExarDeviceMemory::ExarDeviceMemory(size_t pSize)
	: mHandle(nullptr)
	, mSize(pSize)
{
	void* lMemPtr = VirtualAlloc(NULL, pSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lMemPtr) throw std::exception("Impossible to alloc size on device");

	mHandle = static_cast<MemHandle>(lMemPtr);
}

UnasciiEngine::RHI::EXAR::ExarDeviceMemory::~ExarDeviceMemory()
{
	VirtualFree(static_cast<void*>(mHandle), 0, MEM_RELEASE);
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::ExarDeviceMemory::getMemoryHandle()
{
	return mHandle;
}

size_t UnasciiEngine::RHI::EXAR::ExarDeviceMemory::getMemorySize() const
{
	return mSize;
}
