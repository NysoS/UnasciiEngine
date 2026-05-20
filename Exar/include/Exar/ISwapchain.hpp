#ifndef EXAR_ISWAPCHAIN_HPP
#define EXAR_ISWAPCHAIN_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"
#include "Exar/Types.hpp"

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
		virtual ExarResult getImages(Image* pImages) = 0;

	private:
		virtual void initImages() = 0;
	};
}

#endif // !EXAR_ISWAPCHAIN_HPP

