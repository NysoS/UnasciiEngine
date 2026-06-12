#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/Internal/AreaMemory.hpp"

#include <stdexcept>
#include <iostream>

Exar::DeviceMemory::DeviceMemory(size_t pSize)
	: mHandle(nullptr)
	, mSize(pSize)
{
	/*void* lMemPtr = SysAloc(NULL, pSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!lMemPtr) throw std::exception("Impossible to alloc size on device");

	mHandle = static_cast<Memory>(lMemPtr);*/
}

Exar::DeviceMemory::~DeviceMemory()
{
	VirtualFree(static_cast<void*>(mHandle), 0, MEM_RELEASE);
}

Exar::Result Exar::DeviceMemory::createAreaMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram)
{
	std::cout << "createAreaMemory" << std::endl;
	if (!pAreaCreateInfo.pageMemory) return Result::ERROR_INVALID_ARG;

	switch (pAreaCreateInfo.areaType)
	{
	case AreaMemoryType::SWAPCHAIN:
		std::cout << "Type SWAPCHAIN" << std::endl;
		mSwapchainArea = std::make_unique<AreaMemory>();
		return mSwapchainArea->createMemory(pAreaCreateInfo, lVram);
	default:
		break;
	}
}

Exar::Memory Exar::DeviceMemory::getMemory()
{
	return mHandle;
}

size_t Exar::DeviceMemory::getMemorySize() const
{
	return mSize;
}
