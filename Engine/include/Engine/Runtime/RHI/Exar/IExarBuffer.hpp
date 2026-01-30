#ifndef IEXAR_BUFFER_HPP
#define IEXAR_BUFFER_HPP

#include "Exar.hpp"
#include "IExarRessource.hpp"
#include "ExarBufferDesc.hpp"

namespace UnasciiEngine::RHI::EXAR {
	struct IExarBuffer : IExarRessource
	{
		virtual ExarBufferDesc getDesc() const noexcept = 0;
	};
}

#endif // !IEXAR_BUFFER_HPP
