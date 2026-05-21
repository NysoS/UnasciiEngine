#ifndef EXAR_SWAPCHAIN_HPP
#define EXAR_SWAPCHAIN_HPP

#include "Exar/exarpch.h"
#include "Exar/ISwapchain.hpp"
#include "Exar/Exar.hpp"
#include "Exar/SwapchainDesc.hpp"

namespace Exar
{
	class Device;

	class Swapchain : public ISwapchain
	{
	public:
		Swapchain();
		virtual ~Swapchain();

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;

		virtual Surface getSurfaceImageHandle() override;
		virtual void present() override;
		virtual void swap() noexcept override;

		virtual u32 getImageCount() const noexcept override;
		virtual Result getImages(Image* pImages) override;

	private:
		virtual void initImages() override;
		
		SwapchainDesc mDesc;
		Image* mImages;
		Device* mDevice;

		int index = 0;

		friend class Device;
	};
}

#endif // EXAR_SWAPCHAIN_HPP

