#ifndef EXAR_ATTACHMENT_DESCRIPTION_HPP
#define EXAR_ATTACHMENT_DESCRIPTION_HPP

#include "Exar/Exar.hpp"
#include "Exar/ExarEnum.hpp"
#include "Exar/Types.hpp"

namespace Exar
{
	struct EXA_API AttachmentDescription
	{
		PixelFormat imageFormat;
		u8 samples = 0x01;
		AttachmentLoadOp loadOp;
		AttachmentStoreOp storeOp;
		AttachmentLoadOp stencilLoadOp;
		AttachmentStoreOp stencilStoreOp;
		ImageLayout initialLayout;
		ImageLayout finalLayout;
	};
}

#endif // !EXAR_ATTACHMENT_DESCRIPTION_HPP
