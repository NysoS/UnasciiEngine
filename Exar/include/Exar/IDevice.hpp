#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"
#include "Exar/Types.hpp"
#include "Exar/MemoryRequirements.hpp"
#include "Exar/ImageDesc.hpp"
#include "Exar/ImageView.hpp"

#include "Exar/SwapchainDesc.hpp"

namespace Exar {
	struct 	BufferDesc;
	class 	IBuffer;
	class 	IRessource;
	struct 	AllocatorDesc;
	class 	ISwapchain;
	struct 	FramebufferInfo;
	class 	Framebuffer;

	template <typename Type>
	concept ResourceTypeName = std::derived_from<Type, IRessource>;

	class EXA_API IDevice
	{
	public:
		virtual ~IDevice() {};
		virtual bool createMemory(size_t pMemorySize) = 0;

		virtual IBuffer* createBuffer(const BufferDesc& pDesc) = 0;

		virtual MemoryRequirement getBufferMemoryRequirements(const IBuffer* pBuffer) noexcept = 0;
		
		virtual Result allocateResourceMemory(void** pMemory, const AllocatorDesc& pDesc, const MemoryRequirement& pRequirement) noexcept = 0;
		
		virtual bool updateResourceData(MemoryHandle& pMemHandle, const MemoryRequirement& pRequirement, std::span<const u8> pData) noexcept = 0;

		template <ResourceTypeName ResourceType>
		bool bindResource(ResourceType* pBuffer, MemoryHandle pResourceMemory)
		{
			if (!pBuffer || !pResourceMemory) return false;

			pBuffer->setCPUAddress(pResourceMemory);
			return true;
		}

		virtual Result getImageMemoryRequirements(MemoryRequirement* pRequirement, const ImageDesc& pDesc) noexcept = 0;
		virtual Result createImage(Image* pImage, const AllocatorDesc& pDesc, const MemoryRequirement& pRequirement) = 0;
		virtual Result destroyImage(Image pImage, const AllocatorDesc& pDesc) = 0;

		virtual Result createImageView(const ImageViewDesc& pDesc, ImageView** pImageView) = 0;
		virtual Result destroyImageView(ImageView* pImageView) = 0;

		virtual Result createSwapchain(const SwapchainDesc& pDesc, ISwapchain** pSwapchain) = 0;
		virtual Result destroySwapchain(ISwapchain* pSwapchain) = 0;
	
		virtual Result createFramebuffer(const FramebufferInfo& pInfo, Framebuffer** pFramebuffer) = 0;
		virtual Result destroyFramebuffer(Framebuffer* pFramebuffer) = 0;
	};
}

#endif // !EXAR_HPP
