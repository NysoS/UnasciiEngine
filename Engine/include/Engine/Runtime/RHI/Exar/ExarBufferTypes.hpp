#ifndef EXAR_BUFFER_TYPES_HPP
#define EXAR_BUFFER_TYPES_HPP

#include "Engine/uaepch.h"
#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/Exar/Exar.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	struct ExarBufferDesc {
		size_t size;
		ExarUsage usage;
		ExarBindFlag bindFlags;
	};

	struct ExarBufferInitData {
		const u8* data;
		size_t size;

		operator std::span<const u8> () const
		{
			return std::span<const u8>(data, size);
		}
	};
}

#endif // !EXAR_BUFFER_TYPES_HPP