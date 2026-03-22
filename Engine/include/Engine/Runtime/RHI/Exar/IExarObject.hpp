#ifndef IEXAR_OBJECT_HPP
#define IEXAR_OBJECT_HPP

#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI::EXAR {

	class IExarDevice;

	class IExarObject
	{
		virtual inline IExarDevice* getDevice() const noexcept = 0;
	};
}

#endif // !IEXAR_OBJECT_HPP
