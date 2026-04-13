#ifndef DEVICE_FACTORY_HPP
#define DEVICE_FACTORY_HPP

#include "Engine/Runtime/RHI/RHIDef.hpp"
#include "Engine/Runtime/RHI/IDevice.hpp"

namespace UnasciiEngine::RHI
{
	class IDeviceFactory
	{
	public:
		virtual ~IDeviceFactory() = default;
		virtual IDevice* CreateDevice(size_t pMemorySize = (1024 * 1024 * 1024)) = 0;
		virtual bool DestroyDevice(IDevice* pDevice) = 0;
	};
}

#endif // !DEVICE_FACTORY_HPP
