#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/IExarDevice.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarAllocator;
	class IExarBuffer;

	class ExarDevice : public IExarDevice
	{
	public:
		ExarDevice();
		virtual ~ExarDevice();

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) override;
		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept override;
		
		virtual MemHandle allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept override;

	private:
		std::unique_ptr<ExarAllocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
