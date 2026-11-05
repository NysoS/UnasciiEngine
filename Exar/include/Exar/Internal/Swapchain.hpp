#ifndef EXAR_SWAPCHAIN_HPP
#define EXAR_SWAPCHAIN_HPP

#include "Exar/Descriptor.hpp"
#include "Exar/MinimalCore.hpp"
#include "Exar/ISwapchain.hpp"

namespace Exar
{
	class Device_;

	class Swapchain_ : public ISwapchain
	{
	public:
		Swapchain_();
		virtual ~Swapchain_();

		Swapchain_(const Swapchain_&) = delete;
		Swapchain_& operator=(const Swapchain_&) = delete;

		virtual Surface getSurfaceImageHandle() override;
		virtual void present() override;
		virtual void swap() noexcept override;

		virtual u32 getImageCount() const noexcept override;
		virtual Result getImages(Image* pImages) override;

		virtual u32 getIndex() const noexcept override;
		virtual void setSemaphore(Semaphore pSemaphore) noexcept override;
		virtual Semaphore getSemaphore() const noexcept override;

	private:
		virtual void initImages() override;
		
		SwapchainCreateInfo mInfo;
		Image* mImages;

		Semaphore mImageAvailableSemaphore = EXAR_NULL_HANDLE;

		u32 mIndex = 0;

		friend class Device_;
	};
}

#endif // EXAR_SWAPCHAIN_HPP

