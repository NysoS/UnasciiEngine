#ifndef EXAR_BUFFER_DESC_HPP
#define EXAR_BUFFER_DESC_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/Exar/Exar.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	struct ExarBufferDesc {
		uint32_t byteWidth;
		uint32_t align;
		ExarUsage usage;
		ExarBindFlag bindFlags;
	};
}

#endif // !EXAR_BUFFER_DESC_HPP