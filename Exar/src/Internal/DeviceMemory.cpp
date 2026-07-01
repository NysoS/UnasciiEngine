#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/Internal/AreaMemory.hpp"

#include <stdexcept>
#include <iostream>

Exar::DeviceMemory::DeviceMemory(size_t pSize)
{
}

Exar::DeviceMemory::~DeviceMemory()
{
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
	case AreaMemoryType::COMMAND_POOL:
		std::cout << "Type COMMAND POOL" << std::endl;
		mCommandPoolArea = std::make_unique<AreaMemory>();
		return mCommandPoolArea->createMemory(pAreaCreateInfo, lVram);
	default:
		break;
	}
}

Exar::Memory Exar::DeviceMemory::clearAreaMemory()
{
	return Memory();
}

Exar::AreaMemory* Exar::DeviceMemory::getAreaMemory(const AreaMemoryType& pAreaType) const
{
	switch (pAreaType)
	{
	case AreaMemoryType::SWAPCHAIN:
		return mSwapchainArea.get();
	case AreaMemoryType::COMMAND_POOL:
		return mCommandPoolArea.get();
	default:
		break;
	}

	return nullptr;
}
