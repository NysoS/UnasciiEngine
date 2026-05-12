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
		
		virtual ExarResult allocateResourceMemory(void** pMemory, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept override;
		virtual bool updateResourceData(MemoryHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept override;

		virtual ExarResult getImageMemoryRequirements(ExarMemoryRequirement* pRequirement, const ImageDesc& pDesc) noexcept override;
		virtual ExarResult createImage(Image* pImage, const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) override;
		virtual ExarResult destroyImage(Image pImage, const ExarAllocatorDesc& pDesc) override;

		virtual ExarResult createImageView(const ImageViewDesc& pDesc, ImageView** pImageView) override;
		virtual ExarResult destroyImageView(ImageView* pImageView) override;

		virtual ExarResult createSwapchain(const SwapchainDesc& pDesc, ISwapchain** pSwapchain) override;
		virtual ExarResult destroySwapchain(ISwapchain* pSwapchain) override;

		// create image
		// destroy image
		// check memory resources valid
		// add exar_result

	private:
		std::unique_ptr<ExarAllocator> mAllocator;
	};
}

#endif // !EXAR_DEVICE_HPP
