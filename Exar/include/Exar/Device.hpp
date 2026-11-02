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
		Result createImage(Image* pImage, const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept;
		Result destroyImage(Image* pImage, const AllocatorCreateInfo& pDesc) noexcept;

		Result createCommandPool(CommandPool* pCommandPool, const CommandPoolCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result destroyCommandPool(CommandPool* pCommandPool, const AllocatorCreateInfo& pAllocatorInfo) noexcept;

		Result allocateCommandBuffer(const CommandBufferAllocateInfo& pInfo, CommandBuffer* pCommandBuffers) noexcept;

		Result createImageView(ImageView* pImageView, const ImageViewCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result destroyImageView(ImageView* pImageView, const AllocatorCreateInfo& pAllocatorInfo) noexcept;

		Result createSwapchain(Swapchain* pSwapchain, const SwapchainCreateInfo& pDesc, const AllocatorCreateInfo& pAllocatorInfo);
		Result destroySwapchain(Swapchain* pSwapchain, const AllocatorCreateInfo& pAllocatorInfo);
		Result acquireNextImage(Swapchain pSwapchain, Semaphore pSemaphore, Fence pFence, u32* pImageIndex) noexcept;
		Result getSwapchainImages(Swapchain pSwapchain, u32 pImagesCount, Image* pImages) noexcept;

		Result createFramebuffer(const FramebufferCreateInfo& pInfo, Framebuffer** pFramebuffer) noexcept;
		Result destroyFramebuffer(Framebuffer* pFramebuffer) noexcept;

		Result createFence(Fence* pFence, const FenceCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result destroyFence(Fence* pFence, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result waitForFence(Fence pFence, ExarBool pWaitAll, u64 pTimeout) noexcept;
		Result resetFences(Fence* pFences, u32 pCount) noexcept;

		Result createSemaphore(Semaphore* pSemaphore, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result destroySemaphore(Semaphore* pSemaphore, const AllocatorCreateInfo& pAllocatorInfo) noexcept;

		Result createQueue(Queue* pQueue, const QueueCreateInfo& pInfo, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result destroyQueue(Queue* pQueue, const AllocatorCreateInfo& pAllocatorInfo) noexcept;
		Result getQueue(Queue* pQueue, u32 pQueueIndex, QueueFamily pQueueFamily) noexcept;

		// create image
		// destroy image
		// check memory resources valid
		// add exar_result

		Result findCommandPool(CommandPool* pCommandPool, const FindRessourceInfo& pSearchInfo) noexcept;
		Result findSwpachainImages(u32 pImageIndex, Image* pImages, const FindRessourceInfo& pSearchInfo) noexcept;
	private:
		Result createResourceObject(EXAR_HANDLE* pResourceHandle, const AllocatorCreateInfo& pAllocatorInfo, AreaMemoryType pCompareType) noexcept;

		std::unique_ptr<Allocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
