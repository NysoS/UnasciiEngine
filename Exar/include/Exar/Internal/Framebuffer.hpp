#ifndef EXAR_FRAMEBUFFER_HPP
#define EXAR_FRAMEBUFFER_HPP

#include "Exar/MinimalCore.hpp"

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

		friend class Device;
	};
}

#endif // !EXAR_FRAMEBUFFER_HPP
