#ifndef EXAR_FRAMEBUFFER_HPP
#define EXAR_FRAMEBUFFER_HPP

#include "Exar/exarpch.h"
#include "Exar/Types.hpp"

namespace Exar
{
	class ImageView;

	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer();

	private:
		void* mRenderPass;
		ImageView** mAttachments;
		u32 mWidth, mHeight;
		u32 mLayer;

		friend class ExarDevice;
	};
}

#endif // !EXAR_FRAMEBUFFER_HPP
