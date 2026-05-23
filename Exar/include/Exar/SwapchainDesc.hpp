#ifndef EXAR_SWAPCHAIN_DESC_HPP
#define EXAR_SWAPCHAIN_DESC_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Extent.hpp"

namespace Exar
{
	struct SwapchainDesc
	{
		Surface surface;
		u32 minImageCount;

		PixelFormat imageFormat;
		ColorSpace imageColor;
		PresentationMode presentMode;

		Extent2D<u32> extent;

		ImageUsageFlags imageUsage;
		SharingMode imageSharingMode;
		u8 useVSync = 0;
		u8 clipped = 0;
	};
}

#endif // !EXAR_SWAPCHAIN_DESC_HPP

