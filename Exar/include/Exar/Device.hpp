#ifndef EXAR_DEVICE_HPP
#define EXAR_DEVICE_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IDevice.hpp"

namespace Exar
{
	class Allocator;
	class IBuffer;

	class EXA_API Device_
	{
	public:
		Device_();
		~Device_();

		bool createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo);

		IBuffer* createBuffer(const BufferDesc& pDesc);
		MemoryRequirement getBufferMemoryRequirements(const IBuffer* pBuffer) noexcept;
		
		Result allocateResourceMemory(void** pMemory, const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept;
		bool updateResourceData(Memory& pMemHandle, const MemoryRequirement& pRequirement, std::span<const u8> pData) noexcept;

		Result getImageMemoryRequirements(MemoryRequirement* pRequirement, const AllocatorCreateInfo& pAlloctorInfo, const ImageCreateInfo& pDesc) noexcept;
		Result createImage(Image* pImage, const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement);
		Result destroyImage(Image pImage, const AllocatorCreateInfo& pDesc);

		Result createCommandPool(CommandPool* pCommandPool, const CommandPoolCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo);
		Result destroyCommandPool(CommandPool* pCommandPool, const AllocatorCreateInfo& pAllocatorInfo);

		Result allocateCommandBuffer(const CommandBufferAllocateInfo& pInfo, CommandBuffer* pCommandBuffers);

		Result createImageView(const ImageViewCreateInfo& pDesc, ImageView** pImageView);
		Result destroyImageView(ImageView* pImageView);

		Result createSwapchain(const SwapchainCreateInfo& pDesc, ISwapchain** pSwapchain);
		Result destroySwapchain(ISwapchain* pSwapchain);

		Result createFramebuffer(const FramebufferCreateInfo& pInfo, Framebuffer** pFramebuffer);
		Result destroyFramebuffer(Framebuffer* pFramebuffer);

		Result createFence(Fence* pFence, const FenceCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo);
		Result destroyFence(Fence* pFence, const AllocatorCreateInfo& pAllocatorInfo);
		Result waitForFence(Fence pFence, ExarBool pWaitAll, u64 pTimeout);
		Result resetFences(Fence* pFences, u32 pCount);

		// create image
		// destroy image
		// check memory resources valid
		// add exar_result

	private:
		std::unique_ptr<Allocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
