#ifndef EXAR_BUFFER_TYPES_HPP
#define EXAR_BUFFER_TYPES_HPP

#include "Exar/MinimalCore.hpp"

namespace Exar
{
	struct BufferDesc {
		size_t size;
		Usage usage;
		BindFlag bindFlags;
	};

	struct BufferInitData {
		const u8* data;
		size_t size;

		operator std::span<const u8> () const
		{
			return std::span<const u8>(data, size);
		}
	};
}

#endif // !EXAR_BUFFER_TYPES_HPP