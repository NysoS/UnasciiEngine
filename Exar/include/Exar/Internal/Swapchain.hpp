#ifndef EXAR_SWAPCHAIN_HPP
#define EXAR_SWAPCHAIN_HPP

#include "Exar/exarpch.h"
#include "Exar/ISwapchain.hpp"
#include "Exar/Exar.hpp"
#include "Exar/SwapchainDesc.hpp"

class ExarDevice;

namespace Exar
{
	class Swapchain : public ISwapchain
	{
	public:
		Swapchain();
		virtual ~Swapchain();

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;

		virtual Surface getSurfaceImageHandle() override;
		virtual void present() override;

		virtual u32 getImageCount() const noexcept override;
		virtual ExarResult getImages(Image* pImages) override;

	private:
		virtual void initImages() override;
		
		SwapchainDesc mDesc;
		Image* mImages;
		ExarDevice* mDevice;

		friend class ExarDevice;
	};
}

#endif // EXAR_SWAPCHAIN_HPP

