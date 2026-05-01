#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IExarDevice.hpp"

namespace Exar
{
	class ExarAllocator;
	class IExarBuffer;

	class EXA_API ExarDevice : public IExarDevice
	{
	public:
		ExarDevice();
		virtual ~ExarDevice();

		virtual bool createMemory(size_t pMemorySize) override;

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) override;
		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept override;
		
		virtual MemoryHandle allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept override;
		virtual bool updateResourceData(MemoryHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept override;

		virtual ISwapchain* createSwapchain(const SwapchainDesc& pDesc) override;
		virtual ExarResult destroySwapchain(ISwapchain* pSwapchain) override;

	private:
		std::unique_ptr<ExarAllocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
