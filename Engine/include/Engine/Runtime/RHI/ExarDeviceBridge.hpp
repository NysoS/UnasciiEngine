#ifndef EXAR_DEVICE_BRIDGE_HPP
#define EXAR_DEVICE_BRIDGE_HPP

#include "Engine/Runtime/RHI/IDevice.hpp"

namespace UnasciiEngine::RHI 
{
	class ExarDeviceBridge : public IDevice
	{
	public:
		ExarDeviceBridge(size_t pMemorySize);
		virtual ~ExarDeviceBridge();
	};
}

#endif // !EXAR_DEVICE_BRIDGE_HPP
