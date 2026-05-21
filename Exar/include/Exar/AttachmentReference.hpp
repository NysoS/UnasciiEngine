#ifndef EXAR_ATTACHMENT_REFERENCE_HPP
#define EXAR_ATTACHMENT_REFERENCE_HPP

#include "Exar/Exar.hpp"
#include "Exar/Types.hpp"
#include "Exar/Enum.hpp"

namespace Exar
{
	struct EXA_API AttachmentReference
	{
		u32 attachment;
		ImageLayout layout;
	};
}

#endif // !EXAR_ATTACHMENT_REFERENCE_HPP
