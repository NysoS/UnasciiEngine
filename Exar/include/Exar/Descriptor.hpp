#ifndef EXAR_DESCRIPTOR_HPP
#define EXAR_DESCRIPTOR_HPP

#include "Exar/Exar.hpp"
#include "Exar/MinimalCore.hpp"

namespace Exar {
	struct PageMemoryCreateInfo
	{
		size_t size;
		MemoryModeFlags mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE;
	};

	struct AreaMemoryCreateInfo
	{
		MemoryModeFlags mode;
		AreaMemoryType areaType;
		u32 pageCount = 1;
		size_t size;
		const PageMemoryCreateInfo* pageMemory;
	};

	struct DeviceMemoryCreateInfo
	{
		MemoryType memoryType;
		u32 areaCount;
		const AreaMemoryCreateInfo* areaMemory;
	};

	struct EXA_API AllocatorCreateInfo
	{
		size_t totalSize = 0;
		AlignMemory align = AlignMemory::ALIGN_32;
		AreaMemoryType areaType = AreaMemoryType::NONE;
		u32	pageIndex = 0;
		AllocationMode mode = AllocationMode::ALLOC_HEAP;
	};

	struct EXA_API AttachmentDesc
	{
		PixelFormat imageFormat;
		u8 samples = 0x01;
		AttachmentLoadOp loadOp = AttachmentLoadOp::LOAD_OP_NONE;
		AttachmentStoreOp storeOp = AttachmentStoreOp::STORE_OP_NONE;
		AttachmentLoadOp stencilLoadOp = AttachmentLoadOp::LOAD_OP_NONE;
		AttachmentStoreOp stencilStoreOp = AttachmentStoreOp::STORE_OP_NONE;
		ImageLayout initialLayout = ImageLayout::UNDEFINED;
		ImageLayout finalLayout = ImageLayout::UNDEFINED;
	};

	struct EXA_API ImageCreateInfo
	{
		u32 width = 0;
		u32 height = 0;
		AlignMemory align = AlignMemory::ALIGN_32;
		AllocationMode allocLocation = AllocationMode::ALLOC_HEAP;
	};

	struct EXA_API ImageSubresource
	{
		ImageAspectMask aspectMask;
		u32 baseMipLevel = 0;
		u32 levelCount = 1;
	};

	struct EXA_API ImageViewCreateInfo
	{
		Image image;
		ImageViewType viewType = ImageViewType::VIEW_2D;
		PixelFormat format;
		ImageSubresource subresource;
	};

	struct MemoryRequirement
	{
		size_t sizeInBytes;
		AlignMemory align;
	};

	struct EXA_API AttachmentReference
	{
		u32 attachment;
		ImageLayout layout;
	};

	struct EXA_API SubpassDescription
	{
		PipelineBindPoint pipelineBindPoint;
		u32 colorAttachmentCount;
		AttachmentReference* colorAttachments;
	};

	struct EXA_API RenderPassInfo
	{
		const u32 attachmentCount;
		const AttachmentDesc* attachments;
		const u32 subpassCount;
		const SubpassDescription* subpasses;
	};

	struct EXA_API SwapchainCreateInfo
	{
		Surface surface;
		u32 minImageCount;

		PixelFormat imageFormat;
		ColorSpace imageColor;
		PresentationMode presentMode;

		struct Extent2D<u32> extent;

		ImageUsageFlags imageUsage;
		SharingMode imageSharingMode;
		u8 useVSync = 0;
		u8 clipped = 0;
	};

	struct EXA_API FramebufferCreateInfo
	{
		void* renderPass;
		class ImageView** attachments;
		u32 attachmentCount;
		u32 width, height;
		u32 layer;
	};

	struct EXA_API CommandPoolCreateInfo
	{
		AreaMemoryType type;
		QueueFamily family;
		CommandPoolFlags flags;
	};

	struct EXA_API CommandBufferAllocateInfo
	{
		CommandPool commandPool;
		// todo make level command buffer
		uint32_t commandBufferCount;
	};

	struct EXA_API SubmitInfo
	{
		u32 commandBufferCount;
		CommandBuffer* pCommandBuffers;
	};

	struct EXA_API CommandBufferBeginInfo
	{
		// TODO : add fields
		CommandBufferUsageFlags flags;
	};

	struct EXA_API FenceCreateInfo
	{
		FenceCreateFlags flags = FenceCreateFlags::UNSIGNALED_BIT;
	};

	struct FindRessourceInfo
	{
		SearchPageFlag searchPageFlag;
		AreaMemoryType areaMemoryType;
		QueueFamily family;
		uint32_t pageCount;
		uint32_t pageStartIndex;
	};
}

#endif // !EXAR_DESCRIPTOR_HPP
