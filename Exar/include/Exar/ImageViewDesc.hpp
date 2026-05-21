#ifndef EXAR_IMAGE_VIEW_DESC_HPP
#define EXAR_IMAGE_VIEW_DESC_HPP

#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"
#include "Exar/Types.hpp"
#include "Exar/Enum.hpp"

namespace Exar 
{
	struct EXA_API ImageSubresource
	{
		ImageAspectMask aspectMask;
		u32 baseMipLevel = 0;
		u32 levelCount = 1;
	};

	struct EXA_API ImageViewDesc
	{
		Image image;
		ImageViewType viewType = ImageViewType::VIEW_2D;
		PixelFormat format;
		ImageSubresource subresource;
	};
}

#endif // !EXAR_IMAGE_VIEW_DESC_HPP
