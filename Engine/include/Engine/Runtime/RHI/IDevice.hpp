#ifndef RHI_DEVICE_HPP
#define RHI_DEVICE_HPP

#include "Engine/uaepch.h"
#include "Engine/Engine.hpp"
#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI {
	class UAE_API IDevice
	{
	public:
		virtual ~IDevice() = default;
	};
}

#endif // !RHI_DEVICE_HPP
