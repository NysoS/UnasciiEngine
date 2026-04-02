#ifndef IEXAR_BUFFER_HPP
#define IEXAR_BUFFER_HPP

#include "Exar.hpp"
#include "IExarRessource.hpp"
#include "ExarBufferTypes.hpp"

namespace UnasciiEngine::RHI::EXAR {
	class ExarBufferView;

	class IExarBuffer : public IExarRessource
	{
	public:
		virtual ExarBufferDesc getDesc() const noexcept = 0;
		[[nodiscard]] virtual ExarBufferView getView() const = 0;
	};
}

#endif // !IEXAR_BUFFER_HPP
