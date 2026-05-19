#ifndef EXAR_FRAMEBUFFER_INFO_HPP
#define EXAR_FRAMEBUFFER_INFO_HPP

#include "Exar/Exar.hpp"
#include "Exar/Types.hpp"

namespace Exar
{
	class ImageView;

	struct EXA_API FramebufferInfo
	{
		void* renderPass;
		ImageView** attachments;
		u32 attachmentCount;
		u32 width, height;
		u32 layer;
	};
}

#endif // !EXAR_FRAMEBUFFER_INFO_HPP
