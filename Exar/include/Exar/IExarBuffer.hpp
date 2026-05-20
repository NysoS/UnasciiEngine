#ifndef IEXAR_BUFFER_HPP
#define IEXAR_BUFFER_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IExarRessource.hpp"
#include "Exar/ExarBufferTypes.hpp"

namespace Exar {
	class ExarBufferView;

	class EXA_API IExarBuffer : public IExarRessource
	{
	public:
		virtual ExarBufferDesc getDesc() const noexcept = 0;
		[[nodiscard]] virtual ExarBufferView getView() const = 0;
	};
}

#endif // !IEXAR_BUFFER_HPP
