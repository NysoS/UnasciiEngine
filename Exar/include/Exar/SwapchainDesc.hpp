#ifndef EXAR_SWAPCHAIN_DESC_HPP
#define EXAR_SWAPCHAIN_DESC_HPP

#include "Exar/Types.hpp"
#include "Exar/ExarEnum.hpp"
#include "Exar/Extent.hpp"
#include "Exar/Def.hpp"

namespace Exar
{
	struct SwapchainDesc
	{
		Surface surface;
		u32 minImageCount;

		PixelFormat imageFormat;
		ColorSpace imageColor;

		Extent2D<u32> extent;

		ImageUsageFlags imageUsage;
		SharingMode imageSharingMode;
		u8 useVSync = 0;
		u8 clipped = 0;
	};
}

#endif // !EXAR_SWAPCHAIN_DESC_HPP

