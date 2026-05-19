#ifndef EXAR_RENDER_PASS_INFO_HPP
#define EXAR_RENDER_PASS_INFO_HPP

#include "Exar/Exar.hpp"
#include "Exar/Types.hpp"

namespace Exar
{
	struct AttachmentDescription;
	struct SubpassDescription;

	struct EXA_API RenderPassInfo
	{
		const u32 attachmentCount;
		const AttachmentDescription* attachments;
		const u32 subpassCount;
		const SubpassDescription* subpasses;
	};
}

#endif // !EXAR_RENDER_PASS_INFO_HPP

