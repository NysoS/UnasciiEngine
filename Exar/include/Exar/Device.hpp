#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IDevice.hpp"

namespace Exar
{
	class Allocator;
	class IBuffer;

	class EXA_API Device : public IDevice
	{
	public:
		Device();
		virtual ~Device();

		virtual bool createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo) override;

		virtual IBuffer* createBuffer(const BufferDesc& pDesc) override;
		virtual MemoryRequirement getBufferMemoryRequirements(const IBuffer* pBuffer) noexcept override;
		
		virtual Result allocateResourceMemory(void** pMemory, const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept override;
		virtual bool updateResourceData(Memory& pMemHandle, const MemoryRequirement& pRequirement, std::span<const u8> pData) noexcept override;

		virtual Result getImageMemoryRequirements(MemoryRequirement* pRequirement, const AllocatorCreateInfo& pAlloctorInfo, const ImageCreateInfo& pDesc) noexcept override;
		virtual Result createImage(Image* pImage, const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) override;
		virtual Result destroyImage(Image pImage, const AllocatorCreateInfo& pDesc) override;

		virtual Result createCommandPool(CommandPool* pCommandPool, const CommandPoolCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo) override;
		virtual Result destroyCommandPool(CommandPool pCommandPool, const AllocatorCreateInfo& pAllocatorInfo) override;

		virtual Result createImageView(const ImageViewCreateInfo& pDesc, ImageView** pImageView) override;
		virtual Result destroyImageView(ImageView* pImageView) override;

		virtual Result createSwapchain(const SwapchainCreateInfo& pDesc, ISwapchain** pSwapchain) override;
		virtual Result destroySwapchain(ISwapchain* pSwapchain) override;

		virtual Result createFramebuffer(const FramebufferCreateInfo& pInfo, Framebuffer** pFramebuffer) override;
		virtual Result destroyFramebuffer(Framebuffer* pFramebuffer) override;

		// create image
		// destroy image
		// check memory resources valid
		// add exar_result

	private:
		std::unique_ptr<Allocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
