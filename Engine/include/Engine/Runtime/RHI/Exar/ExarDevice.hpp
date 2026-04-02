#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

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

		virtual bool createMemory(size_t pMemorySize) override;

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) override;
		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept override;
		
		virtual MemHandle allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept override;
		virtual bool updateResourceData(MemHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept override;
	private:
		std::unique_ptr<ExarAllocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
