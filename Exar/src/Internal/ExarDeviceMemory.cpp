#include "Exar/Internal/ExarDeviceMemory.hpp"

#include <stdexcept>

Exar::ExarDeviceMemory::ExarDeviceMemory(size_t pSize)
	: mHandle(nullptr)
	, mSize(pSize)
{
	void* lMemPtr = SysAloc(NULL, pSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lMemPtr) throw std::exception("Impossible to alloc size on device");

	mHandle = static_cast<MemoryHandle>(lMemPtr);
}

Exar::ExarDeviceMemory::~ExarDeviceMemory()
{
	VirtualFree(static_cast<void*>(mHandle), 0, MEM_RELEASE);
}

Exar::MemoryHandle Exar::ExarDeviceMemory::getMemoryHandle()
{
	return mHandle;
}

size_t Exar::ExarDeviceMemory::getMemorySize() const
{
	return mSize;
}
