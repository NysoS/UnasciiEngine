#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/IExarDevice.hpp"
#include "Engine/Runtime/RHI/Exar/ExarMemoryRequirements.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class MemoryArena;
	class IExarBuffer;

	class ExarDevice : public IExarDevice
	{
	public:
		ExarDevice();
		virtual ~ExarDevice();

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) override;

		virtual bool GetResourceMemoryRequirements(const IExarBuffer* pBuffer, ExarMemoryRequirement*& pMemRequirement) noexcept override;
	
	private:
		std::shared_ptr<MemoryArena> mMemoryArena;
	};
}

#endif // !EXAR_DEVICE_HPP
