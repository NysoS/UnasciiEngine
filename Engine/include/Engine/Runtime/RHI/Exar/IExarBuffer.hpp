#ifndef IEXAR_BUFFER_HPP
#define IEXAR_BUFFER_HPP

#include "Exar.hpp"
#include "IExarRessource.hpp"

namespace UnasciiEngine::RHI::EXAR {

	struct ExarBufferDesc {
		uint32_t byteWidth;
		uint32_t align;
		ExarUsage usage;
		ExarBindFlag bindFlags;
	};

	struct ExarBufferView {
		uint8_t* data;
	};

	struct IExarBuffer : IExarRessource
	{
		virtual ExarBufferDesc getDesc() const noexcept = 0;
	};
}

#endif // !IEXAR_BUFFER_HPP
