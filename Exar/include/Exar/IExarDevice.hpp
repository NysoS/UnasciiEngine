#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"
#include "Exar/Types.hpp"
#include "Exar/ExarMemoryRequirements.hpp"

#include "Exar/SwapchainDesc.hpp"

namespace Exar {
	struct ExarBufferDesc;
	class IExarBuffer;
	class IExarRessource;
	struct ExarAllocatorDesc;
	class ISwapchain;

	template <typename Type>
	concept ResourceTypeName = std::derived_from<Type, IExarRessource>;

	class EXA_API IExarDevice
	{
	public:
		virtual ~IExarDevice() {};
		virtual bool createMemory(size_t pMemorySize) = 0;

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) = 0;

		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept = 0;
		
		virtual ExarResult allocateResourceMemory(void** pMemory, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept = 0;
		
		virtual bool updateResourceData(MemoryHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept = 0;

		template <ResourceTypeName ResourceType>
		bool bindResource(ResourceType* pBuffer, MemoryHandle pResourceMemory)
		{
			if (!pBuffer || !pResourceMemory) return false;

			pBuffer->setCPUAddress(pResourceMemory);
			return true;
		}

		virtual ExarResult getImageMemoryRequirements(const ImageDesc& pDesc, ExarMemoryRequirement* pRequirement) noexcept = 0;
		virtual ExarResult createImage(Image* pImage, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) = 0;

		virtual ExarResult createSwapchain(const SwapchainDesc& pDesc, ISwapchain** pSwapchain) = 0;
		virtual ExarResult destroySwapchain(ISwapchain* pSwapchain) = 0;
	};
}

#endif // !EXAR_HPP
