#ifndef EXAR_ISWAPCHAIN_HPP
#define EXAR_ISWAPCHAIN_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"

namespace Exar
{
	class EXA_API ISwapchain
	{
	public:
		virtual ~ISwapchain() = default;

		virtual Surface getSurfaceImageHandle() = 0;
		virtual void present() = 0;
		virtual void swap() noexcept = 0;

		virtual u32 getImageCount() const noexcept = 0;
		virtual Result getImages(Image* pImages) = 0;

	private:
		virtual void initImages() = 0;
	};
}

#endif // !EXAR_ISWAPCHAIN_HPP

