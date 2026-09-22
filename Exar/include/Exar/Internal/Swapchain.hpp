#ifndef EXAR_SWAPCHAIN_HPP
#define EXAR_SWAPCHAIN_HPP

#include "Exar/Descriptor.hpp"
#include "Exar/MinimalCore.hpp"
#include "Exar/ISwapchain.hpp"

namespace Exar
{
	class Device_;

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
		
		SwapchainCreateInfo mInfo;
		Image* mImages;
		Device_* mDevice;

		int index = 0;

		friend class Device_;
	};
}

#endif // EXAR_SWAPCHAIN_HPP

