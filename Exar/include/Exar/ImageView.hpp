#ifndef EXAR_IMAGE_VIEW_HPP
#define EXAR_IMAGE_VIEW_HPP

#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"

namespace Exar
{
	class EXA_API ImageView
	{
	public:
		ImageView() = default;
		~ImageView() = default;

		Image getImage() const;
		
		void setPixel(u32 pX, u32 pY, u32 pColor); // to modif
		
		u8 getPixel(u32 pX, u32 pY) const;
		
		const u8* getData() const;

	private:
		ImageViewCreateInfo mInfo;
		
		friend class Device;
	};
}

#endif // !EXAR_IMAGE_VIEW_HPP
