#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/IExarDevice.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarDevice : public IExarDevice
	{
	public:
		ExarDevice();
		virtual ~ExarDevice();

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) override;
	};
}

#endif // !EXAR_DEVICE_HPP
