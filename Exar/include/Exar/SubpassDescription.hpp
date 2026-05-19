#ifndef EXAR_SUBPASS_DESCRIPTION_HPP
#define EXAR_SUBPASS_DESCRIPTION_HPP

#include "Exar/Exar.hpp"
#include "Exar/Types.hpp"
#include "Exar/ExarEnum.hpp"

namespace Exar
{
	struct AttachmentReference;

	struct EXA_API SubpassDescription
	{
		PipelineBindPoint pipelineBindPoint;
		u32 colorAttachmentCount;
		AttachmentReference* colorAttachments;
	};
}

#endif // EXAR_SUBPASS_DESCRIPTION_HPP
