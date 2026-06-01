#ifndef IEXAR_BUFFER_HPP
#define IEXAR_BUFFER_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IRessource.hpp"
#include "Exar/BufferTypes.hpp"

namespace Exar {
	class BufferView;

	class EXA_API IBuffer : public IRessource
	{
	public:
		virtual BufferDesc getDesc() const noexcept = 0;
		[[nodiscard]] virtual BufferView getView() const = 0;
	};
}

#endif // !IEXAR_BUFFER_HPP
