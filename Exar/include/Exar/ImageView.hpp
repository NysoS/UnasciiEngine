#ifndef EXAR_IMAGE_VIEW_HPP
#define EXAR_IMAGE_VIEW_HPP

#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"

namespace Exar
{
	class EXA_API ImageView_
	{
	public:
		ImageView_() = default;
		~ImageView_() = default;

		Image getImage() const;
		
		void setPixel(u32 pX, u32 pY, u32 pColor); // to modif
		
		u8 getPixel(u32 pX, u32 pY) const;
		
		const u8* getData() const;

	private:
		Image image;
		ImageViewType viewType = ImageViewType::VIEW_2D;
		PixelFormat format;
		ImageSubresource subresource;
		
		friend class Device_;
	};
}

#endif // !EXAR_IMAGE_VIEW_HPP
