#include "Exar/Internal/DeviceMemory.hpp"

#include <stdexcept>

Exar::DeviceMemory::DeviceMemory(size_t pSize)
	: mHandle(nullptr)
	, mSize(pSize)
{
	void* lMemPtr = SysAloc(NULL, pSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lMemPtr) throw std::exception("Impossible to alloc size on device");

	mHandle = static_cast<Memory>(lMemPtr);
}

Exar::DeviceMemory::~DeviceMemory()
{
	VirtualFree(static_cast<void*>(mHandle), 0, MEM_RELEASE);
}

Exar::Memory Exar::DeviceMemory::getMemory()
{
	return mHandle;
}

size_t Exar::DeviceMemory::getMemorySize() const
{
	return mSize;
}
